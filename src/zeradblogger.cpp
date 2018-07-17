#include "zeradblogger.h"

#include <vcmp_remoteproceduredata.h>
#include <vcmp_errordata.h>
#include <ve_commandevent.h>

#include <QStorageInfo>

class ZeraDBLoggerPrivate
{

  ZeraDBLoggerPrivate(ZeraDBLogger *t_qPtr) :
    m_qPtr(t_qPtr),
    m_remoteProcedures{VF_RPC_BIND(listStorages, std::bind(&ZeraDBLoggerPrivate::listStorages, this, std::placeholders::_1, std::placeholders::_2))}
  {

  }

  /**
   * @brief Policy is to not store the database on the OS partition due to sdcard / flash wear
   * @param t_dbFilePath
   * @return
   */
  bool checkStorageLocation(const QString &t_dbFilePath) const
  {
    bool retVal = false;
    const auto storages = QStorageInfo::mountedVolumes();
    for(const auto storDevice : storages)
    {
      if(storDevice.fileSystemType().contains("tmpfs") == false
         && storDevice.isRoot() == false
         && t_dbFilePath.contains(storDevice.rootPath()))
      {
        retVal = true;
        break;
      }
    }

    return retVal;
  }

  bool processCommandEvent(VeinEvent::CommandEvent *t_cEvent)
  {
    bool retVal = false;

    if(t_cEvent->eventData()->type() == VeinComponent::RemoteProcedureData::dataType())
    {
      VeinComponent::RemoteProcedureData *rpcData=nullptr;
      rpcData = static_cast<VeinComponent::RemoteProcedureData *>(t_cEvent->eventData());
      Q_ASSERT(rpcData != nullptr);
      if(rpcData->command() == VeinComponent::RemoteProcedureData::Command::RPCMD_CALL)
      {
        if(m_remoteProcedures.contains(rpcData->procedureName()))
        {
          retVal = true;
          const QUuid callId = rpcData->invokationData().value(VeinComponent::RemoteProcedureData::s_callIdString).toUuid();
          Q_ASSERT(callId.isNull() == false);
          Q_ASSERT(m_pendingRpcHash.contains(callId) == false);
          m_pendingRpcHash.insert(callId, t_cEvent->peerId());
          m_remoteProcedures.value(rpcData->procedureName())(callId, rpcData->invokationData()); //rpc invokation
          t_cEvent->accept();
        }
        else //unknown procedure
        {
          retVal = true;
          qWarning() << "No remote procedure with entityId:" << m_qPtr->entityId() << "name:" << rpcData->procedureName();
          VF_ASSERT(false, QStringC(QString("No remote procedure with entityId: %1 name: %2").arg(m_qPtr->entityId()).arg(rpcData->procedureName())));
          VeinComponent::ErrorData *eData = new VeinComponent::ErrorData();
          eData->setEntityId(m_qPtr->entityId());
          eData->setErrorDescription(QString("No remote procedure with name: %1").arg(rpcData->procedureName()));
          eData->setOriginalData(rpcData);
          eData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
          eData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
          VeinEvent::CommandEvent *errorEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, eData);
          errorEvent->setPeerId(t_cEvent->peerId());
          t_cEvent->accept();
          emit m_qPtr->sigSendEvent(errorEvent);
        }
      }
    }

    return retVal;
  }

  void rpcFinished(QUuid t_callId, const QString &t_procedureName, const QVariantMap &t_data)
  {
    Q_ASSERT(m_pendingRpcHash.contains(t_callId));
    VeinComponent::RemoteProcedureData *resultData = new VeinComponent::RemoteProcedureData();
    resultData->setEntityId(m_qPtr->entityId());
    resultData->setEventOrigin(VeinEvent::EventData::EventOrigin::EO_LOCAL);
    resultData->setEventTarget(VeinEvent::EventData::EventTarget::ET_ALL);
    resultData->setCommand(VeinComponent::RemoteProcedureData::Command::RPCMD_RESULT);
    resultData->setProcedureName(t_procedureName);
    resultData->setInvokationData(t_data);

    VeinEvent::CommandEvent *rpcResultEvent = new VeinEvent::CommandEvent(VeinEvent::CommandEvent::EventSubtype::NOTIFICATION, resultData);
    rpcResultEvent->setPeerId(m_pendingRpcHash.value(t_callId));
    emit m_qPtr->sigSendEvent(rpcResultEvent);
    m_pendingRpcHash.remove(t_callId);
  }

  //rpc
  VF_RPC(listStorages, "listStorages()", "returns ZeraDBLogger::storgeList: A lists of available storages for writing the database")
  void listStorages(const QUuid &t_callId, const QVariantMap &t_parameters)
  {
    QVariantMap retVal = t_parameters;

    const auto storages = QStorageInfo::mountedVolumes();
    QStringList storageList;

    for(const auto storDevice : storages)
    {
      if(storDevice.isRoot() == false && storDevice.fileSystemType().contains("tmpfs") == false)
      {
        storageList.append(storDevice.rootPath());
      }
    }
    retVal.insert(s_listStoragesReturnValueName, storageList);
    retVal.insert(VeinComponent::RemoteProcedureData::s_resultCodeString, 0); //success
    rpcFinished(t_callId, s_listStoragesProcedureName, retVal);
  }

  ZeraDBLogger *m_qPtr=nullptr;
  /**
   * @brief call id, peer id
   */
  QHash<QUuid, QUuid> m_pendingRpcHash;
  //functions need an instance so no static variable
  const VeinEvent::RoutedRemoteProcedureAtlas m_remoteProcedures;
  static constexpr QLatin1String s_listStoragesReturnValueName=QLatin1String("ZeraDBLogger::storageList");

  friend class ZeraDBLogger;
};

//constexpr definition, see: https://stackoverflow.com/questions/8016780/undefined-reference-to-static-constexpr-char
constexpr QLatin1String ZeraDBLoggerPrivate::s_listStoragesProcedureName;
constexpr QLatin1String ZeraDBLoggerPrivate::s_listStoragesProcedureDescription;
constexpr QLatin1String ZeraDBLoggerPrivate::s_listStoragesReturnValueName;



ZeraDBLogger::ZeraDBLogger(VeinLogger::DataSource *t_dataSource, VeinLogger::DBFactory t_factoryFunction, QObject *t_parent) :
  VeinLogger::DatabaseLogger(t_dataSource, t_factoryFunction, t_parent),
  m_dPtr(new ZeraDBLoggerPrivate(this))
{

}

ZeraDBLogger::~ZeraDBLogger()
{
  delete m_dPtr;
}

bool ZeraDBLogger::openDatabase(const QString &t_filePath)
{
  bool retVal = false;
  if(m_dPtr->checkStorageLocation(t_filePath))
  {
    retVal = VeinLogger::DatabaseLogger::openDatabase(t_filePath);
  }
  return retVal;
}

bool ZeraDBLogger::processEvent(QEvent *t_event)
{
  bool retVal = VeinLogger::DatabaseLogger::processEvent(t_event);
  if(t_event->type()==VeinEvent::CommandEvent::eventType())
  {
    VeinEvent::CommandEvent *cEvent = nullptr;
    VeinEvent::EventData *evData = nullptr;
    cEvent = static_cast<VeinEvent::CommandEvent *>(t_event);
    Q_ASSERT(cEvent != nullptr);

    evData = cEvent->eventData();
    Q_ASSERT(evData != nullptr);

    if(evData->entityId() == entityId())
    {
      const bool intermediate = m_dPtr->processCommandEvent(cEvent);
      retVal = retVal || intermediate;
    }
  }
  return retVal;
}
