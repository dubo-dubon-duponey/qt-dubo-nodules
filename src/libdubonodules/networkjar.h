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

#ifndef DUBONODULES_NETWORK_JAR_H
#define DUBONODULES_NETWORK_JAR_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QVariant>

/**
 * A very simple cookie jar meant to expose getting / setting all cookies to script.
 * Consumer who are looking for persistence should leverage this from JavaScript, and store them in localStorage (or any other appropriate storage).
 */

namespace DuboNodules{
namespace Network{

class Jar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit Jar(QObject *parent = nullptr): QNetworkCookieJar(parent){
    }

    Q_PROPERTY(QVariantList cookies READ getCookies WRITE setCookies NOTIFY updated)

    QVariantList getCookies() const
    {
        QVariantList list;
        QList<QNetworkCookie> ck = this->allCookies();
        for(int x = 0; x < ck.length(); x++){
            QNetworkCookie cook = ck.at(x);
            QString name = QString::fromLatin1(cook.name());
            QString value = QString::fromLatin1(cook.value());
            QString domain;
            domain.append(cook.domain());
            QString path;
            path.append(cook.path());

            list << name << value << domain << path;
        }
        return list;
    }

    void setCookies(const QVariantList & cookies)
    {
        QList<QNetworkCookie> l;
        for(int x = 0; x < cookies.length(); x++){
            QNetworkCookie co(cookies[x].toByteArray(), cookies[x+1].toByteArray());
            QString dom = QString::fromLatin1(cookies[x+2].toByteArray().data());
            QString pa = QString::fromLatin1(cookies[x+3].toByteArray().data());
            co.setDomain(dom);
            co.setPath(pa);
            x += 3;
            l << co;
        }
        setAllCookies(l);
    }

    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
    {
        bool ret = QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
        emit updated();
        return ret;
    }

    bool insertCookie(const QNetworkCookie &cookie)
    {
        bool ret = QNetworkCookieJar::insertCookie(cookie);
        emit updated();
        return ret;
    }

    bool updateCookie(const QNetworkCookie &cookie)
    {
        bool ret = QNetworkCookieJar::updateCookie(cookie);
        emit updated();
        return ret;
    }

    bool deleteCookie(const QNetworkCookie &cookie)
    {
        bool ret = QNetworkCookieJar::deleteCookie(cookie);
        emit updated();
        return ret;
    }

signals:
    void updated();
    
public slots:
    
};

}
}

#endif // DUBONODULES_NETWORK_JAR_H

