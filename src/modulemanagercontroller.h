#ifndef MODULEMANAGERSETTINGS_H
#define MODULEMANAGERSETTINGS_H

#include <ve_eventsystem.h>

#include <QJsonDocument>

namespace VeinEvent {
  class StorageSystem;
}

class ModuleManagerController : public VeinEvent::EventSystem
{
  Q_OBJECT
public:
  explicit ModuleManagerController(QObject *t_parent = 0);
  static constexpr int getEntityId();
  VeinEvent::StorageSystem *getStorageSystem() const;
  void setStorage(VeinEvent::StorageSystem *t_storageSystem);

  // EventSystem interface
public:
  bool processEvent(QEvent *t_event) override;

signals:
  void sigChangeSession(QVariant newSessionPath);
  void sigModulesPausedChanged(bool t_paused);

public slots:
  void initializeEntity(QString t_sessionPath);
  void initOnce();
  void setModulesPaused(bool t_paused);

private:
  void handleNotificationMessage(QJsonObject t_message);

  static constexpr int s_entityId = 0;
  static constexpr QLatin1String s_entityName = QLatin1String("_System");
  static constexpr QLatin1String s_entityNameComponentName = QLatin1String("EntityName");
  static constexpr QLatin1String s_entitiesComponentName = QLatin1String("Entities");
  static constexpr QLatin1String s_sessionComponentName = QLatin1String("Session");
  static constexpr QLatin1String s_notificationMessagesComponentName = QLatin1String("Error_Messages");
  static constexpr QLatin1String s_loggedComponentsComponentName = QLatin1String("LoggedComponents");
  static constexpr QLatin1String s_modulesPausedComponentName = QLatin1String("ModulesPaused");


  VeinEvent::StorageSystem *m_storageSystem = 0;
  QJsonDocument m_notificationMessages;
  QString m_currentSession;
  bool m_initDone=false;
  bool m_sessionReady=false;
  bool m_modulesPaused=false;
};

#endif // MODULEMANAGERSETTINGS_H
