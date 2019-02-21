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

#ifndef DUBONODULES_NETWORK_H
#define DUBONODULES_NETWORK_H

#include "jar.h"
#include "cache.h"
#include "reply.h"

#include <QNetworkAccessManager>
#include <QBuffer>
#include <QJsonValue>

namespace DuboNodules{
namespace Network{

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString & cacheDir, QObject * parent = nullptr): QObject(parent)
    {
        m_cache = new DuboNodules::Network::Cache(this);
        m_cache->setCacheDirectory(cacheDir);

        m_jar = new DuboNodules::Network::Jar(this);

        m_manager = new QNetworkAccessManager(this);
        m_manager->setCache(m_cache);
        m_manager->setCookieJar(m_jar);
        // Only in QT 5.10+
        // manager->enableStrictTransportSecurityStore(true);
        // manager->setStrictTransportSecurityEnabled(true);
        // 5.9+
        // manager->redirectPolicy

        connect(m_manager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), this, SLOT(networkSlot(QNetworkAccessManager::NetworkAccessibility)));
    }

    Q_INVOKABLE const QVariant request(const QString & url, const QString & verb, QJsonValue headers, const QString &data)
    {
        QNetworkRequest * req = new QNetworkRequest(QUrl(url));
        QString scheme = req->url().scheme().toLower().trimmed();

        if(scheme == QString::fromLatin1("http") || scheme == QString::fromLatin1("https"))
        {
            req->setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
            req->setAttribute(QNetworkRequest::SpdyAllowedAttribute, true);
            req->setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
        }

        // Process headers
        QMap<QString, QVariant> object = headers.toVariant().toMap();
        QMap<QString, QVariant>::const_iterator i = object.constBegin();
        while (i != object.constEnd()) {
            req->setRawHeader(i.key().toLocal8Bit().data(), i.value().toString().toLocal8Bit().data());
            ++i;
        }

        QNetworkRequest request(*req);
        QBuffer * buff = new QBuffer(this);
        buff->setData(data.toLocal8Bit().data());

        QNetworkReply * reply;
        // XXX not sure why, but otherwise HEAD never comes back
        if (verb.toLatin1().toLower() == "head"){
            reply = m_manager->head(request);
        }else{
            reply = m_manager->sendCustomRequest(request, verb.toLatin1(), buff);
        }

        return QVariant::fromValue(static_cast<QObject *>(new DuboNodules::Network::Reply(reply, this)));
    }

    Q_PROPERTY(DuboNodules::Network::Jar * cookieJar READ cookieJar CONSTANT)
    Q_PROPERTY(DuboNodules::Network::Cache * diskCache READ diskCache CONSTANT)
    Q_PROPERTY(int accessible READ accessible NOTIFY networkChanged)

signals:
    void updated();
    void networkChanged(int acc);

public slots:


private slots:
    void networkSlot(QNetworkAccessManager::NetworkAccessibility acc)
    {
        emit networkChanged(int(acc));
    }


private:
    int accessible()
    {
        return int(m_manager->networkAccessible());
    }

    DuboNodules::Network::Jar * cookieJar()
    {
        return m_jar;
    }

    DuboNodules::Network::Cache * diskCache()
    {
        return m_cache;
    }

    DuboNodules::Network::Cache * m_cache;
    DuboNodules::Network::Jar * m_jar;
    QNetworkAccessManager * m_manager;
};

}
}

#endif // DUBONODULES_NETWORK_H
