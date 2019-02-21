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

#ifndef DUBONODULES_NETWORK_REPLY_H
#define DUBONODULES_NETWORK_REPLY_H

#include <QObject>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace DuboNodules{
namespace Network{

class Reply : public QObject
{
    Q_OBJECT
public:
    explicit Reply(QNetworkReply * rep, QObject * parent = nullptr): QObject(parent)
    {
        m_rep = rep;
        connect(rep, SIGNAL(finished()), this, SLOT(httpFinished()));
    }

signals:
    void done(int err, int status, QVariant headers, QString data);

private:
    QNetworkReply * m_rep;

private slots:
    void httpFinished()
    {
        QString data = QString::fromLatin1(m_rep->readAll().data());
        int err = m_rep->error();
        int status = m_rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        QMap<QString, QVariant> list;
        QList<QNetworkReply::RawHeaderPair> ck = m_rep->rawHeaderPairs();
        for(int x = 0; x < ck.length(); x++){
            QNetworkReply::RawHeaderPair cook = ck.at(x);
            QString name = QString::fromLatin1(cook.first);
            QString value = QString::fromLatin1(cook.second);
            list[name] = value;
        }

        emit done(err, status, QVariant(list), data);

/*        if(status == 302){
            QVariant u = reply->header(QNetworkRequest::LocationHeader);
            QUrl u2 = qvariant_cast<QUrl >(u);
            qDebug() << "Redirecting as ordered to ";
            QByteArray name = QString::fromLatin1("Location").toLocal8Bit();
            QUrl dest(QString::fromLatin1(reply->rawHeader(name)));
            QString final = reply->url().resolved(dest).toString();
            qDebug() << final;

            this->startRequest(final);
        }else{
        }*/
    }
};

}
}

#endif // DUBONODULES_NETWORK_REPLY_H
