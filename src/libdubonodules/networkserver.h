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

#ifndef DUBONODULES_NETWORK_SERVER_H
#define DUBONODULES_NETWORK_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

namespace DuboNodules{
namespace Network{

class MuSocket: public QObject
{
    Q_OBJECT
public:
    explicit MuSocket(QTcpSocket * sock, QObject * parent = nullptr): QObject(parent)
    {
        socky = sock;
    }

    Q_INVOKABLE QString read()
    {
        return QString::fromUtf8(socky->readLine());
    }

    Q_INVOKABLE void write(const QString & answer)
    {
        QTextStream os(socky);
        os.setAutoDetectUnicode(true);
        os << answer;
        socky->close();
    }

private:
    QTcpSocket * socky;
};

class Server : public QTcpServer
{
     Q_OBJECT
 public:
     explicit Server(QObject* parent = nullptr)
         : QTcpServer(parent)/*, disabled(false)*/
     {
     }

    Q_INVOKABLE void start(quint16 port)
    {
        listen(QHostAddress::Any, port);
    }

    Q_INVOKABLE void stop()
    {
        close();
    }

protected:
    void incomingConnection(qintptr socket)
    {
        QTcpSocket* s = new QTcpSocket(this);
        connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
        connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
        s->setSocketDescriptor(socket);
    }

signals:
    void incoming(QVariant);

private slots:
    void readClient()
    {
        QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
        MuSocket * mus = new MuSocket(socket, socket);
        emit incoming(QVariant::fromValue(static_cast<QObject*>(mus)));
        if (socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;
        }
     }

     void discardClient()
     {
         QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
         socket->deleteLater();
     }

private:
};

}
}

#endif // DUBONODULES_NETWORK_SERVER_H
