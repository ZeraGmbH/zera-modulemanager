#ifndef TEST_JSONLOGGERCONTENTLOADER_H
#define TEST_JSONLOGGERCONTENTLOADER_H

#include <QObject>

class test_jsonloggercontentloader : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void noSessionSetEmptyAvailableContentSets();

    void compareSessionLogConfigFileCountEqual();
    void compareSessionLogConfigFileBasenamesEqual();

    void testSimpleContentSets();
};

#endif // TEST_JSONLOGGERCONTENTLOADER_H
