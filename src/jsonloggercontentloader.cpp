#include "jsonloggercontentloader.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

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
        dir.setCurrent(m_configFileDir);
        sessionFile.setFileName(session);
        if(sessionFile.exists()) {
            sessionFile.open(QIODevice::ReadOnly);
            QByteArray fileContent = sessionFile.readAll();
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(fileContent, &err);
            if(err.error == QJsonParseError::NoError){
                if(doc.isObject()){
                    QJsonObject rootObj = doc.object();
                    QJsonObject contentSetList = rootObj.value("ContentSet").toObject();
                    if(contentSetList.isEmpty()) {
                    }
                    else {
                        ret = contentSetList.keys();
                    }

                }
            }

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
