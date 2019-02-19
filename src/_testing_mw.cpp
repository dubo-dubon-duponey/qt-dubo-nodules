// #include <errno.h>
//#include <fcntl.h>       // for O_RDONLY
/*#include <stdio.h>       // for fprintf()
#include <stdlib.h>      // for EXIT_SUCCESS
#include <string.h>      // for strerror()
#include <sys/event.h>   // for kqueue() etc.
#include <unistd.h>      // for close()*/

#include "mw.h"

#include <QDebug>

void OSXWatcher::Start(QString dirToWatch)
{
    CFStringRef pathToWatchCF = CFStringCreateWithCString(nullptr, dirToWatch.toUtf8().constData(), kCFStringEncodingUTF8);
    CFArrayRef pathsToWatch = CFArrayCreate(nullptr, reinterpret_cast<const void **>(&pathToWatchCF), 1, nullptr);

    FSEventStreamContext context;
    context.version = 0;
    context.info = this;
    context.retain = nullptr;
    context.release = nullptr;
    context.copyDescription = nullptr;

    stream = FSEventStreamCreate(nullptr, &OSXWatcher::fileSystemEventCallback, &context, pathsToWatch, kFSEventStreamEventIdSinceNow, 3.0, kFSEventStreamCreateFlagFileEvents);
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    CFRelease(pathToWatchCF);

    // Read the folder content to protect any unprotected or pending file
    // ReadFolderContent();
}

void OSXWatcher::Stop()
{
    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
}

void OSXWatcher::fileSystemEventCallback(ConstFSEventStreamRef /*streamRef*/, void * /*clientCallBackInfo*/, size_t numEvents, void * /*eventPaths*/, const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId /* eventIds*/[] )
{
    // char **paths = static_cast<char **>(eventPaths);

    for (size_t i=0; i<numEvents; i++) {
        qDebug() << "Event" << i;
        qDebug() << "Event Flags" << eventFlags[i];
        // When a file is created we receive first a kFSEventStreamEventFlagItemCreated and second a (kFSEventStreamEventFlagItemCreated & kFSEventStreamEventFlagItemModified)
        // when the file is finally copied. Catch this second event.
        if (eventFlags[i] & kFSEventStreamEventFlagItemCreated
                && eventFlags[i] & kFSEventStreamEventFlagItemModified
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsDir)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemIsSymlink)
                && !(eventFlags[i] & kFSEventStreamEventFlagItemFinderInfoMod)) {

            /* OSXWatcher *watcher = static_cast<OSXWatcher *>(clientCallBackInfo);
            if (watcher->FileValidator(paths[i]))
                emit watcher->yourSignalHere();*/
        }
    }
}
