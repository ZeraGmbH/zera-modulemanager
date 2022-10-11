#include "test_jsonloggercontentloader.h"
#include "jsonloggercontentloader.h"
#include <QTest>
#include <QFileInfo>

void test_jsonloggercontentloader::init()
{
}

void test_jsonloggercontentloader::cleanup()
{
}

void test_jsonloggercontentloader::noSessionSetEmptyAvailableContentSets()
{
    JsonLoggerContentLoader loader;
    QVERIFY(loader.getAvailableContentSets().isEmpty());
}


void test_jsonloggercontentloader::compareSessionLogConfigFileCountEqual()
{
    QStringList sessionFileList = QString(SESSION_FILES).split(",");
    QStringList loggerFileList = QString(LOGGER_CONFIG_FILES).split(",");
    QCOMPARE(sessionFileList.count(), loggerFileList.count());
}

static void toBaseName(QStringList &fileList)
{
    for(auto& file : fileList) {
        QFileInfo fi(file);
        file = fi.baseName();
    }
}

void test_jsonloggercontentloader::compareSessionLogConfigFileBasenamesEqual()
{
    QStringList sessionFileList = QString(SESSION_FILES).split(",");
    sessionFileList.sort();
    toBaseName(sessionFileList);
    QStringList loggerFileList = QString(LOGGER_CONFIG_FILES).split(",");
    loggerFileList.sort();
    toBaseName(loggerFileList);
    QCOMPARE(sessionFileList, loggerFileList);
}

void test_jsonloggercontentloader::testSimpleContentSets()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(JSON_TEST_DIR);
    loader.setSession("simple-valid.json");
    QStringList cs = loader.getAvailableContentSets();
    cs.sort();
    QCOMPARE(cs.size(), 3);
    QCOMPARE(cs[0], "ZeraActualValues");
    QCOMPARE(cs[1], "ZeraDCReference");
    QCOMPARE(cs[2], "ZeraHarmonics");
}

QTEST_MAIN(test_jsonloggercontentloader)
