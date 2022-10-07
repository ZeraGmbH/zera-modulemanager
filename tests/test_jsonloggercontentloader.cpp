#include "test_jsonloggercontentloader.h"
#include "jsonloggercontentloader.h"
#include <QTest>

static const char *validJsonFileName = "contentset_valid.json";
static const char *emptyJsonFileName = "contentset_empty.json";

void test_jsonloggercontentloader::init()
{
}

void test_jsonloggercontentloader::cleanup()
{
}

void test_jsonloggercontentloader::noDirSetEmptyAvailableContentSets()
{
    JsonLoggerContentLoader loader;
    QVERIFY(loader.getAvailableContentSets("foo").isEmpty());
}

void test_jsonloggercontentloader::noDirSetEmptyEntityComponents()
{
    JsonLoggerContentLoader loader;
    QVERIFY(loader.getEntityComponents("foo").isEmpty());
}

void test_jsonloggercontentloader::invalidDirSetEmptyAvailableContentSets()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(CURRENT_SOURCE_DIR "foo");
    QVERIFY(loader.getAvailableContentSets(validJsonFileName).isEmpty());
}

void test_jsonloggercontentloader::invalidDirSetEmptyEntityComponents()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(CURRENT_SOURCE_DIR "foo");
    QVERIFY(loader.getEntityComponents(validJsonFileName).isEmpty());
}

QTEST_MAIN(test_jsonloggercontentloader)