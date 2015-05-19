#include "modulemanager.h"
#include "jsonsessionloader.h"

#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QUuid>

#include <veinhub.h>

#include <ve_eventhandler.h>
#include <ve_validator.h>
#include <ve_scriptengine.h>
#include <vn_networksystem.h>
#include <vn_tcpsystem.h>
#include <vn_introspectionsystem.h>
#include <vs_veinhash.h>
#include <vscr_componentengine.h>

#include <QDebug>
#include <QFile>

#include <QLoggingCategory>
#include <QStringList>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QStringList loggingFilters = QStringList() << QString("%1.debug=false").arg(VEIN_API_HUB().categoryName()) <<
                                                QString("%1.debug=false").arg(VEIN_EVENT().categoryName()) <<
                                                QString("%1.debug=false").arg(VEIN_NET().categoryName()) <<
                                                QString("%1.debug=false").arg(VEIN_NET_TCP().categoryName()) <<
                                                QString("%1.debug=false").arg(VEIN_STORAGE_HASH().categoryName());


  QLoggingCategory::setFilterRules(loggingFilters.join("\n"));

  VeinEvent::EventHandler *evHandler = new VeinEvent::EventHandler();

  VeinEvent::Validator *validator = new VeinEvent::Validator(&a);
//  VeinScript::ComponentEngine *scrSystem = new VeinScript::ComponentEngine(&a);
  VeinStorage::VeinHash *storSystem = new VeinStorage::VeinHash(&a);
  VeinNet::IntrospectionSystem *introspectionSystem = new VeinNet::IntrospectionSystem(&a);
  VeinNet::NetworkSystem *netSystem = new VeinNet::NetworkSystem(&a);
  VeinNet::TcpSystem *tcpSystem = new VeinNet::TcpSystem(&a);


  ZeraModules::ModuleManager *modMan = new ZeraModules::ModuleManager(&a);
  JsonSessionLoader *sessionLoader = new JsonSessionLoader(&a);


  VeinHub *localHub=new VeinHub(&a);
  localHub->setStorage(storSystem);

  netSystem->setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH);

  QList<VeinEvent::EventSystem*> subSystems;

  subSystems.append(validator);
//  subSystems.append(scrSystem);
  subSystems.append(storSystem);
  subSystems.append(localHub);
  subSystems.append(introspectionSystem);
  subSystems.append(netSystem);
  subSystems.append(tcpSystem);


  evHandler->setSubsystems(subSystems);

  introspectionSystem->setStorage(storSystem);

  QObject::connect(sessionLoader, &JsonSessionLoader::sigLoadModule, modMan, &ZeraModules::ModuleManager::startModule);
  QObject::connect(modMan, &ZeraModules::ModuleManager::sigSessionSwitched, sessionLoader, &JsonSessionLoader::loadSession);

  bool modulesFound;

  qRegisterMetaTypeStreamOperators<QList<qreal> >("QList<qreal>");
  qRegisterMetaTypeStreamOperators<QList<QString> >("QList<QString>");

  localHub->setUuid(QUuid::createUuid());
  modMan->setHub(localHub);


  modulesFound = modMan->loadModules();

  if(!modulesFound)
  {
    qDebug() << "[Zera-Module-Manager] No modules found";
    a.quit();
  }
  else
  {
    modMan->loadDefaultSession();
    tcpSystem->startServer(12000);
  }
  return a.exec();
}
