#include <QString>
#include <QObject>
#include <CoreServices/CoreServices.h>

// https://github.com/radekp/qt/blob/master/src/corelib/io/qfilesystemwatcher_fsevents.cpp
class OSXWatcher: public QObject
{
    Q_OBJECT
public:

    explicit OSXWatcher(const QString& /*strDirectory*/){}
    ~OSXWatcher();

    void Start(QString dirToWatch);
    void Stop();

private:
    static void fileSystemEventCallback(ConstFSEventStreamRef streamRef, void *clientCallBackInfo, size_t numEvents, void *eventPaths, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]);
    FSEventStreamRef stream;
};
