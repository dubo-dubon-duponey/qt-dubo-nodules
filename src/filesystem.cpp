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

#include <QFileSystemWatcher>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QWidget>
#include <QFileDialog>

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <ostream>
#include <iomanip>
#include <sstream>

#include "libdubonodules/filesystem.h"

namespace DuboNodules{
namespace FileSystem{

const QString Helpers::pick(const QString & path, const QString & label)
{
    QWidget * w = new QWidget();
    QString directory_path = QFileDialog::getExistingDirectory(w, label, path);
    return directory_path;
}


void Helpers::open(const QString & path)
{
    QString helper;
#if defined(Q_OS_MAC)
    helper = QString::fromLatin1("open");
#elif defined(Q_OS_WIN)
    helper = QString::fromLatin1("explorer");
#elif defined(Q_OS_UNIX)
    helper = QString::fromLatin1("nautilus");
#endif
    if(helper.length()){
        QProcess * process = new QProcess(this);
        process->start(helper, QStringList() << path);
    }
}

bool Helpers::dirExists(const QString & path)
{
    QDir dir(path);
    return dir.exists();
}

bool Helpers::fileExists(const QString & path)
{
    QFile file(path);
    return file.exists();
}

qint64 Helpers::fileSize(const QString & path)
{
    QFile file(path);
    return file.size();
}

// https://webcache.googleusercontent.com/search?q=cache:4_9xDvU31HQJ:trac.opensubtitles.org/projects/opensubtitles/wiki/HashSourceCodes+&cd=1&hl=en&ct=clnk&gl=us
const QString Helpers::fileHash(const QString &fileName)
{
    QFile * file = new QFile(fileName);
    file->open(QFile::ReadOnly);
    signed long long limit = 65536;
    signed long long fsize = file->size();
    signed long long pos = fsize - limit;
    signed long long hash = fsize;

    file->seek(0);

    for(signed long long tmp = 0, i = 0; i < limit/(static_cast<signed long long>(sizeof(tmp))) && file->read(reinterpret_cast<char*>(&tmp), sizeof(tmp)); i++, hash += tmp){}

    if(pos > 0){
        file->seek(pos);
    }else{
        file->seek(0);
    }

    for(signed long long tmp = 0, i = 0; i < limit/(static_cast<signed long long>(sizeof(tmp))) && file->read(reinterpret_cast<char*>(&tmp), sizeof(tmp)); i++, hash += tmp){}

    file->close();

    std::ostringstream oss;
    oss << std::setw(16) << std::setfill('0') << std::hex << hash;

    return QString::fromLatin1(oss.str().c_str());
}

bool Helpers::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        for(auto info: dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

bool Helpers::makeDir(const QString & /*dirName*/)
{
    return false;
}

}
}
