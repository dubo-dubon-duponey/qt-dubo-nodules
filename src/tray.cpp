/**
 * Copyright (c) 2019, Dubo Dubon Duponey <dubodubonduponey+github@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "libdubonodules/tray.h"

namespace DuboNodules{
namespace UI{

Tray::Tray(QWidget * parent) :
    QObject(parent)
{
    icon = new Icon(this);
    connect(icon, SIGNAL(updated()), this, SLOT (iconUpdated()));

    qt = new QSystemTrayIcon( this );
    connect(qt, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(qtActivated(QSystemTrayIcon::ActivationReason)));

    menu = new Menu(this);
    qt->setContextMenu(menu->qt);
}

const QString Tray::tooltip() const
{
    return qt->toolTip();
}

void Tray::setTooltip(const QString &tooltip) const
{
    qt->setToolTip(tooltip);
    emit updated();
}

bool Tray::visible() const
{
    return qt->isVisible();
}

void Tray::setVisible(bool visible) const
{
    qt->setVisible(visible);
    emit updated();
}

}
}
