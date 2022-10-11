#include "test_jsonloggercontentloader.h"
#include "jsonloggercontentloader.h"
#include <QTest>

static const char *validContentSetJsonFile = "contentset_valid.json";
static const char *undefinedContentSetJsonFile = "contentset_undefined.json";
static const char *emptyContentSetJsonFile = "contentset_empty.json";

static const char *validContentSetName = "ZeraActualValues";

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
    QVERIFY(loader.getEntityComponents("foo", "bar").isEmpty());
}

void test_jsonloggercontentloader::invalidDirSetEmptyAvailableContentSets()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(CURRENT_SOURCE_DIR "foo");
    QVERIFY(loader.getAvailableContentSets(validContentSetJsonFile).isEmpty());
}

void test_jsonloggercontentloader::invalidDirSetEmptyEntityComponents()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(CURRENT_SOURCE_DIR "foo");
    QVERIFY(loader.getEntityComponents(validContentSetJsonFile, validContentSetName).isEmpty());
}

void test_jsonloggercontentloader::invalidSessionEmptyAvailableContentSets()
{
    JsonLoggerContentLoader loader;
    loader.setConfigFileDir(CURRENT_SOURCE_DIR);
    QVERIFY(loader.getAvailableContentSets("foo").isEmpty());
}


QTEST_MAIN(test_jsonloggercontentloader)
