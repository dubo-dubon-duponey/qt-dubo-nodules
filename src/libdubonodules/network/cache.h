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

#ifndef DUBONODULES_NETWORK_CACHE_H
#define DUBONODULES_NETWORK_CACHE_H

#include <QNetworkDiskCache>

namespace DuboNodules{
namespace Network{

class Cache : public QNetworkDiskCache
{
    Q_OBJECT
public:
    explicit Cache(/*const QString & v, */ QObject *parent = nullptr);

    Q_PROPERTY(const QString cacheDir READ cacheDirectory WRITE setCacheDirectory NOTIFY updated)

    Q_PROPERTY(qint64 maximumSize READ maximumCacheSize WRITE setMaximumCacheSize NOTIFY updated)
    Q_PROPERTY(qint64 currentSize READ cacheSize CONSTANT)

    Q_INVOKABLE void clearCache();
    Q_INVOKABLE void clearEntry(const QString & uri);

    void setCacheDirectory(const QString &cacheDir);

signals:
    void updated();
    
public slots:

private:
    // QString version;
};

}
}

#endif // DUBONODULES_NETWORK_CACHE_H
