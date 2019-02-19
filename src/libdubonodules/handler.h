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

#ifndef DUBONODULES_OPENHANDLER_H
#define DUBONODULES_OPENHANDLER_H

#include <QCoreApplication>

/**
 * This emits signals for supported double clicked files, dropped files (or directories) on the dock icon, or custom supported protocol handlers
 */

namespace RoxeeNodules{
namespace Protocols{

class Handler : public QObject
{
    Q_OBJECT
public:
    // http://burnttoys.blogspot.fr/2008/07/adding-url-scheme-to-qt-application.html
    // http://stackoverflow.com/questions/6561661/url-scheme-qt-and-mac
    explicit Handler(QCoreApplication *parent):
        QObject(parent)
    {
        parent->installEventFilter(this);
    }

    Q_INVOKABLE bool addProtocol(const QString &urlScheme, const QString &appPath);
    Q_INVOKABLE bool removeProtocol(const QString &urlScheme);

    bool eventFilter(QObject * obj, QEvent *event);

signals:
    void urlOpened(const QString & url);
    void fileOpened(const QString & path);

};

}
}

#endif // DUBONODULES_OPENHANDLER_H
