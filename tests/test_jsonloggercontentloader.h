#ifndef TEST_JSONLOGGERCONTENTLOADER_H
#define TEST_JSONLOGGERCONTENTLOADER_H

#include <QObject>

class test_jsonloggercontentloader : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void noDirSetEmptyAvailableContentSets();
    void noDirSetEmptyEntityComponents();
    void invalidDirSetEmptyAvailableContentSets();
    void invalidDirSetEmptyEntityComponents();
    void invalidSessionEmptyAvailableContentSets();

    void compareSessionLogConfigFileCountEqual();
    void compareSessionLogConfigFileBasenamesEqual();
};

#endif // TEST_JSONLOGGERCONTENTLOADER_H
