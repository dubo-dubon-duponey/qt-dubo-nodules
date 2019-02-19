/*
 * Copyright (c) 2019, Dubo Dubon Duponey <dubodubonduponey+github@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DUBONODULES_FILESYSTEM_H
#define DUBONODULES_FILESYSTEM_H

#include <QObject>
#include <QList>
#include <QDirIterator>
#include <QVariant>
#include <QFileSystemWatcher>

#include "walker.h"

namespace DuboNodules{
namespace FileSystem{

class Helpers : public QObject
{
    Q_OBJECT
public:
    explicit Helpers(QObject * parent = nullptr): QObject(parent) {}

    Q_INVOKABLE const QString pick(const QString & path, const QString & label);
    Q_INVOKABLE void open(const QString & path);

    Q_INVOKABLE static bool dirExists(const QString & path);
    Q_INVOKABLE static bool fileExists(const QString & path);
    Q_INVOKABLE static qint64 fileSize(const QString & path);
    Q_INVOKABLE static const QString fileHash(const QString & path);

    Q_INVOKABLE static bool removeDir(const QString &dirName);
    Q_INVOKABLE static bool makeDir(const QString &dirName);
};

class Watcher : public QObject
{
    Q_OBJECT
public:
    explicit Watcher(QObject * parent = nullptr):
        QObject(parent)
    {
        m_watcher = new QFileSystemWatcher(this);
        connect(
                    m_watcher,
                    SIGNAL(directoryChanged(const QString &)),
                    this,
                    SIGNAL(directoryChanged(const QString &))
        );
        connect(
                    m_watcher,
                    SIGNAL(fileChanged(const QString &)),
                    this,
                    SIGNAL(fileChanged(const QString &))
        );
    }

    Q_INVOKABLE QVariant newWatcher()
    {
        return QVariant::fromValue(static_cast<QObject *>(new Watcher(this)));
    }

    Q_INVOKABLE void watch(const QString & path)
    {
        m_watcher->addPath(path);
    }

    Q_INVOKABLE void forget(const QString & path)
    {
        m_watcher->removePath(path);
    }

signals:
    // Dispatched whenever anything in the current directory changes
    void directoryChanged(const QString & a);
    // Dispatched only if you watch a single file
    void fileChanged(const QString & a);


private:
     QFileSystemWatcher * m_watcher;

};

class Walker : public QObject
{
    Q_OBJECT
public:
    explicit Walker(QObject * parent = nullptr):
        QObject(parent)
    {}

    Q_INVOKABLE QVariant newWalker()
    {
        return QVariant::fromValue(static_cast<QObject *>(new Walker(this)));
    }

    // XXX should provide a way to terminate an ongoing worker thread?

    Q_INVOKABLE void scan(const QString & directory, bool deep = false)
    {
        QThread* thread = new QThread;
        DuboNodules::Private::Walker* worker;
        if (deep){
            worker = new DuboNodules::Private::Walker(directory, QDirIterator::Subdirectories);
        }else{
            worker = new DuboNodules::Private::Walker(directory, QDirIterator::NoIteratorFlags);
        }

        worker->moveToThread(thread);
        // Thread management
        // connect(worker, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
        connect(thread, SIGNAL (started()), worker, SLOT (process()));
        connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
        connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
        connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));

        // Wiring-up signals
        connect(worker, SIGNAL (finished()), this, SIGNAL(done()));
        connect(worker, SIGNAL (hasFile(QString, qint64)), this, SIGNAL(hasFile(QString, qint64)));
        connect(worker, SIGNAL (hasDir(QString)), this, SIGNAL(hasDir(QString)));

        // Kick it
        thread->start();
    }

signals:
    void hasFile( QString path, qint64 size );
    void hasDir( QString path );
    void done();

};
}
}
#endif // DUBONODULES_FILESYSTEM_H
