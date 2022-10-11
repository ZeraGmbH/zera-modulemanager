#ifndef JSONLOGGERCONTENTLOADER_H
#define JSONLOGGERCONTENTLOADER_H

#include <vf_loggercontenthandler.h>

class JsonLoggerContentLoader : public LoggerContentHandler
{
public:
    enum class error {
        dir_not_set
    };
    JsonLoggerContentLoader();
    void setConfigFileDir(const QString &dir) override;
    QStringList getAvailableContentSets(const QString &session) override;
    QMap<int, QStringList> getEntityComponents(const QString &session, const QString &contentSetName) override;
private:
    QString m_configFileDir;
};

#endif // JSONLOGGERCONTENTLOADER_H
