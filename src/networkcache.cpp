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

#include <QDir>

#include "libdubonodules/networkcache.h"
#include "libdubonodules/filesystem.h"

namespace DuboNodules{
namespace Network{

Cache::Cache(QObject *parent) :
    QNetworkDiskCache(parent)//, version(v)
{
}

void Cache::setCacheDirectory(const QString &cacheDir)
{
    // XXX Hiding the version for now
    const QString & version = QString::fromLatin1("v1");
    // Do we have a marker here?
    QDir p(cacheDir);
    QFile f(p.filePath(version));
    // No marker?
    if(!f.exists()){
        // Bust dir if it exists
        if (p.exists()){
            DuboNodules::FileSystem::Helpers::removeDir(cacheDir);
        }
        p.mkpath(cacheDir);
        f.open(QFile::WriteOnly);
        f.close();
    }

    QNetworkDiskCache::setCacheDirectory(cacheDir);
}

void Cache::clearCache()
{
    QString path = this->cacheDirectory();
    if(path.isNull())
        return;

    DuboNodules::FileSystem::Helpers::removeDir(path);
    // Re-set cache directory
    this->setCacheDirectory(this->cacheDirectory());
}

void Cache::clearEntry(const QString & url)
{
    this->remove(QUrl(url));
}

}
}
