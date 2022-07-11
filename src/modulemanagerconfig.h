#ifndef MODULEMANAGERCONFIG_H
#define MODULEMANAGERCONFIG_H

#include <QJsonDocument>
#include <mutex>

class ModulemanagerConfig
{
public:
    static ModulemanagerConfig *getInstance();
    bool isValid();
    const QString getDeviceName();
    bool getCustomerDataEnabled();
    const QStringList getAvailableSessions();
    const QString getDefaultSession();
private:
    ModulemanagerConfig();
    const QString getDevNameFromUBoot();
    QJsonObject getDeviceJson();
    static ModulemanagerConfig *m_instance;
    static std::once_flag m_onceflag;
    QJsonDocument m_jsonConfig;
    QString m_deviceName;
};

#endif // MODULEMANAGERCONFIG_H
