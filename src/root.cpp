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

#include "libdubonodules/root.h"

/*! \cond */

namespace DuboNodules{

const QString Root::getName(){
    return QString::fromUtf8(PROJECT_NAME);
}

const QString Root::getVendor(){
    return QString::fromUtf8(PROJECT_VENDOR);
}

const QString Root::getVersion(){
    return QString::fromUtf8(VERSION_FULL);
}

const QString Root::getRevision(){
    return QString::fromUtf8(VERSION_GIT);
}

const QString Root::getChangeset(){
    return QString::fromUtf8(VERSION_CHANGE);
}

const QString Root::getBuildType(){
    return QString::fromUtf8(PROJECT_BUILDTYPE);
}

const QString Root::getLinkType(){
    return QString::fromUtf8(PROJECT_LINKTYPE);
}

const QString Root::getQt(){
    return QString::fromLatin1(QT_VERSION_STR);
}

const QString Root::getLibName(){
    return QString();
}

const QString Root::getLibVersion(){
    return QString();
}

const QString Root::getLibRevision(){
    return QString();
}

}

/*! \endcond */
