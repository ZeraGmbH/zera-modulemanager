#include "modulemanager.h"
#include "jsonsessionloader.h"

#include <QCoreApplication>
#include <veintcpcontroller.h>
#include <veinhub.h>
#include <QFile>
#include <QByteArray>


#include <QDebug>
#include <QFile>


//debug
#include <veinpeer.h>
#include <veinentity.h>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  VeinTcpController *vTC = new VeinTcpController(&a);
  ZeraModules::ModuleManager *modMan = new ZeraModules::ModuleManager(&a);
  JsonSessionLoader *sessionLoader = new JsonSessionLoader(&a);


  VeinHub *localHub=vTC->getLocalHub();
  bool modulesFound;

  qRegisterMetaTypeStreamOperators<QList<qreal> >("QList<qreal>");

  localHub->setUuid(QUuid::createUuid());
  modMan->setHub(localHub);


  modulesFound = modMan->loadModules();

  if(!modulesFound)
  {
    qDebug() << "[Zera-Module-Manager] No modules found";
  }

  QObject::connect(sessionLoader, &JsonSessionLoader::sigLoadModule, modMan, &ZeraModules::ModuleManager::startModule);
  QObject::connect(modMan, &ZeraModules::ModuleManager::sigSessionSwitched, sessionLoader, &JsonSessionLoader::loadSession);

  sessionLoader->loadSession("://default-session.json");

  vTC->startService(12000);
  return a.exec();
}
