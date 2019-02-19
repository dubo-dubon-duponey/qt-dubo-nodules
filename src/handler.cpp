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

#include <QSettings>
#include <QFileOpenEvent>

#include "libdubonodules/handler.h"

namespace DuboNodules{
namespace Protocols{


bool Handler::addProtocol(const QString &urlScheme, const QString &appPath)
{

   QSettings settings; // (QString::fromLatin1("RoxeeRunner"), QSettings::NativeFormat);
   if (settings.isWritable()) {
      QString strGroupPath = QString::fromLatin1("%1/shell/open/command").arg(urlScheme);
      settings.beginGroup(strGroupPath);
      settings.endGroup();

      // Set default value: "HKCR/roxee" = "URL: roxee Protocol"
      settings.setValue(
         QString::fromLatin1("%1/Default").arg(urlScheme),
         QString::fromLatin1("\"URL: %1 protocol\"").arg(urlScheme));

      // This is a url protocol: "HKCR/roxee/URL Protocol" = ""
      settings.setValue(
         QString::fromLatin1("%1/URL Protocol").arg(urlScheme),
         QString::fromLatin1("\"\""));

      // We handle that:   "HKCR/roxee/Shell/Open/Command" = "ApplicationExecutable "%1$""
      settings.setValue(
         QString::fromLatin1("%1/Default").arg(strGroupPath),
         QString::fromLatin1("\"%1\"").arg(appPath));

      return true;
   }

   return false;
}

bool Handler::removeProtocol(const QString &urlScheme)
{
    QSettings settings; // (QString::fromLatin1("%1").arg(QString::fromLatin1("RoxeeRunner")), QSettings::NativeFormat);
    if (settings.isWritable()) {
        settings.beginGroup(urlScheme);
        settings.remove(QString::fromLatin1(""));
        settings.endGroup();
        return true;
    }
    return false;
}

bool Handler::eventFilter(QObject * obj, QEvent *event){
    if (event->type() == QEvent::FileOpen)
    {
        QFileOpenEvent* fileEvent = static_cast<QFileOpenEvent*>(event);
        // Custom protocol open request like roxee:foobar
        if (!fileEvent->url().isEmpty())
        {
            emit urlOpened(fileEvent->url().toString());
        }
        // Request to open a file (double click) for an extension we support like foobar.mkv
        else if (!fileEvent->file().isEmpty())
        {
            emit fileOpened(fileEvent->file());
        }
    }
    return QObject::eventFilter(obj, event);
}

}

}
