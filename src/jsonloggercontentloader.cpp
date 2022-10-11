#include "jsonloggercontentloader.h"
#include <zera-jsonfileloader.h>
#include <QDir>

JsonLoggerContentLoader::JsonLoggerContentLoader()
{
}

void JsonLoggerContentLoader::setConfigFileDir(const QString &dir)
{
    m_configFileDir = dir;
}

void JsonLoggerContentLoader::setSession(const QString &session)
{
    m_currentJsonContentSet = cJsonFileLoader::loadJsonFile(QDir::cleanPath(m_configFileDir + QDir::separator() + session));
}

QStringList JsonLoggerContentLoader::getAvailableContentSets()
{
    QStringList ret;
    if(!m_currentJsonContentSet.isEmpty()) {
        ret.append("foo");
    }
    return ret;
}

QMap<int, QStringList> JsonLoggerContentLoader::getEntityComponents(const QString &p_contentSetName)
{
    QMap<int, QStringList> ret;
    if(!m_currentJsonContentSet.isEmpty()) {
        ret.insert(0, QStringList() << "foo");
    }
    return ret;
}
