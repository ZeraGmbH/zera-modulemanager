#include "jsonloggercontentloader.h"
#include <QDir>

JsonLoggerContentLoader::JsonLoggerContentLoader()
{
}

void JsonLoggerContentLoader::setConfigFileDir(const QString &dir)
{
    m_configFileDir = dir;
}

QStringList JsonLoggerContentLoader::getAvailableContentSets(const QString &session)
{
    QStringList ret;
    QDir dir;
    QFile sessionFile;

    if(!m_configFileDir.isEmpty() && dir.exists(m_configFileDir)) {
        sessionFile.setFileName(session);
        if(sessionFile.exists()) {
            ret.append("foo");
        }
    }
    return ret;
}

QMap<int, QStringList> JsonLoggerContentLoader::getEntityComponents(const QString &session, const QString &contentSetName)
{
    QMap<int, QStringList> ret;
    QDir dir;
    QFile sessionFile;

    if(!m_configFileDir.isEmpty() && dir.exists(m_configFileDir)) {
        sessionFile.setFileName(session);
        if(sessionFile.exists()) {
            ret.insert(0, QStringList() << "foo");
        }
    }
    return ret;
}
