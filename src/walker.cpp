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

#include <QDirIterator>
#include "libdubonodules/walker.h"

namespace DuboNodules{
namespace Private{

void Walker::process()
{
    QDirIterator directory_walker(m_path, QDir::Readable | QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files | QDir::NoSymLinks, m_flag);

    while(directory_walker.hasNext())
    {
        directory_walker.next();
        if(directory_walker.fileInfo().isDir()){
            emit hasDir(directory_walker.fileInfo().filePath());
        }else{
            qint64 fsize = directory_walker.fileInfo().size();
            emit hasFile(directory_walker.fileInfo().filePath(), fsize);
        }
    }
    emit finished();
}

}
}
