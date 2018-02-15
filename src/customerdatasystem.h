#ifndef CUSTOMERDATASYSTEM_H
#define CUSTOMERDATASYSTEM_H

#include <ve_eventsystem.h>
#include <QJsonDocument>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QFile>
#include <QUuid>
#include <QFutureWatcher>
#include <functional>

#include "modman_util.h"

namespace VeinComponent
{
  class RemoteProcedureData;
}

class CustomerDataSystem : public VeinEvent::EventSystem
{
  Q_OBJECT
public:
  enum RPCResultCodes {
    CDS_CANCELED = -64,
    CDS_EINVAL = -EINVAL, //invalid parameters
    CDS_EEXIST = -EEXIST, //file exists (when creating files)
    CDS_ENOENT = -ENOENT, //file doesn't exist (when removing files)
    CDS_SUCCESS = 0,
    CDS_QFILEDEVICE_FILEERROR_BEGIN = QFileDevice::ReadError //if the resultCode is >= CDS_QFILEDEVICE_FILEERROR_BEGIN then it is a QFileDevice::FileError
  };

  CustomerDataSystem(QObject *t_parent = 0);

  // EventSystem interface
public:
  bool processEvent(QEvent *t_event) override;

public slots:
  void writeCustomerdata();

signals:
  void sigCustomerDataError(QString t_message);
  void sigDataValueChanged(QString t_componentName, QString t_newValue);

private:
  void intializeEntity();
  void updateCustomerDataFileList();
  void updateDataFile(QString t_componentName, QString t_newValue);
  bool parseCustomerDataFile(const QString &t_fileName);
  void rpcFinished(QUuid t_callId, const QString &t_procedureName, const QVariantMap &t_data);
  void rpcProgress(QUuid t_callId, const QString &t_procedureName, const QVariantMap &t_data);

  static constexpr int s_entityId = 200;

  VF_COMPONENT(entityName, "EntityName", "Entity name")
  static constexpr QLatin1String s_entityName = QLatin1String("CustomerData");
  VF_COMPONENT(introspection, "INF_ModuleInterface", "Introspection data")
  VF_COMPONENT(fileList, "FileList", "Customer data files available")
  QStringList m_fileList;
  VF_COMPONENT(fileSelected, "FileSelected", "Currently selected customer data file")
  QString m_currentCustomerFileName;
  QJsonDocument m_currentCustomerDocument;

  //rpc
  VF_RPC(customerDataAdd, "customerDataAdd(QString fileName)", "fileName: The file name to store the new file, does not accept names of existing files")
  void customerDataAdd(const QUuid &t_callId, const QVariantMap &t_parameters);
  VF_RPC(customerDataRemove, "customerDataRemove(QString fileName)", "fileName: the name of the file to be removed")
  void customerDataRemove(const QUuid &t_callId, const QVariantMap &t_parameters);
  VF_RPC(customerDataSearch, "customerDataSearch(QVariantMap searchMap)", "searchMap: regular expression values in the map are tested against all files")
  void customerDataSearch(const QUuid &t_callId, const QVariantMap &t_parameters);
  static constexpr QLatin1String s_cusomerDataSearchResultText = QLatin1String("CustomerDataSystem::searchResult");
  static constexpr QLatin1String s_cusomerDataRpcProgress = QLatin1String("CustomerDataSystem::progress");
  //base
  VF_COMPONENT(baseIdentifier, "PAR_DatasetIdentifier", "Unique identifier")
  VF_COMPONENT(baseComment, "PAR_DatasetComment", "Free form text comment")

  //customer
  VF_COMPONENT(customerFirstName, "PAR_CustomerFirstName", "First name")
  VF_COMPONENT(customerLastName, "PAR_CustomerLastName", "Last name")
  VF_COMPONENT(customerStreet, "PAR_CustomerStreet", "Street of the address")
  VF_COMPONENT(customerPostalCode, "PAR_CustomerPostalCode", "Postal/Zip code of the address")
  VF_COMPONENT(customerCity, "PAR_CustomerCity", "City of the address")
  VF_COMPONENT(customerCountry, "PAR_CustomerCountry", "Country of the address")
  VF_COMPONENT(customerNumber, "PAR_CustomerNumber", "Customer number")
  VF_COMPONENT(customerComment, "PAR_CustomerComment", s_baseCommentComponentDescription)

  //location
  VF_COMPONENT(locationFirstName, "PAR_LocationFirstName", s_customerFirstNameComponentDescription)
  VF_COMPONENT(locationLastName, "PAR_LocationLastName", s_customerLastNameComponentDescription)
  VF_COMPONENT(locationStreet, "PAR_LocationStreet", s_customerStreetComponentDescription)
  VF_COMPONENT(locationPostalCode, "PAR_LocationPostalCode", s_customerPostalCodeComponentDescription)
  VF_COMPONENT(locationCity, "PAR_LocationCity", s_customerCityComponentDescription)
  VF_COMPONENT(locationCountry, "PAR_LocationCountry", s_customerCountryComponentDescription)
  VF_COMPONENT(locationNumber, "PAR_LocationNumber", "Location number")
  VF_COMPONENT(locationComment, "PAR_LocationComment", s_baseCommentComponentDescription)

  //powerGrid
  VF_COMPONENT(powerGridOperator, "PAR_PowerGridOperator", "Power grid operator")
  VF_COMPONENT(powerGridSupplier, "PAR_PowerGridSupplier", "Power grid supplier")
  VF_COMPONENT(powerGridComment, "PAR_PowerGridComment", s_baseCommentComponentDescription)

  //meter
  VF_COMPONENT(meterManufacturer, "PAR_MeterManufacturer", "Meter manufacturer")
  VF_COMPONENT(meterFactoryNumber, "PAR_MeterFactoryNumber", "Meter factory number")
  VF_COMPONENT(meterOwner, "PAR_MeterOwner", "Meter owner")
  VF_COMPONENT(meterComment, "PAR_MeterComment", s_baseCommentComponentDescription)

  static const QHash<QString, QString> s_componentIntrospection;
  static const QHash<QString, QString> s_remoteProcedureIntrospection;
  static const QSet<QString> s_writeProtectedComponents;
  //functions need an instance so no static variable
  const VeinEvent::RemoteProcedureAtlas m_remoteProcedures;

  QFileSystemWatcher m_fileWatcher;
  QTimer m_dataWriteDelay;
  /**
   * @brief call id, peer id
   */
  QHash<QUuid, QUuid> m_pendingRpcHash;
  QHash<QUuid, QFutureWatcher<QString> *> m_pendingSearchResultWatchers;
};

#endif // CUSTOMERDATASYSTEM_H