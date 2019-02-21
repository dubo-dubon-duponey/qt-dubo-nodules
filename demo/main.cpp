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

#include <QApplication>
#include <QDebug>
#include <QWidget>

#include <QtWebEngine>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QFileInfo>
#include <QDir>
#include <QWebChannel>
#include <QStackedLayout>

#include <libdubonodules/root.h>
#include <libdubonodules/filesystem.h>
#include <libdubonodules/network/controller.h>
#include <libdubonodules/network/server.h>
#include <libdubonodules/menubar.h>
#include <libdubonodules/tray.h>
#include <libdubonodules/web/kit.h>

QWebChannel * SetupWebView(DuboNodules::Web::Config * config)
{
    QFileInfo jsFileInfo(QDir::currentPath() + QString::fromLatin1("/qwebchannel.js"));

    if (!jsFileInfo.exists())
        QFile::copy(QString::fromLatin1(":/qtwebchannel/qwebchannel.js"), jsFileInfo.absoluteFilePath());

    QtWebEngine::initialize();
    QWidget * w = new QWidget();

    DuboNodules::Web::View * view = new DuboNodules::Web::View(config, w);

    QWebChannel * channel = new QWebChannel(view->page());
    view->page()->setWebChannel(channel);

    view->load(QUrl(QString::fromLatin1("qrc:/demo.html")));

    w->setLayout( new QBoxLayout(QBoxLayout::LeftToRight, w) );
    // w->setLayout( new QStackedLayout(w) );

    w->layout()->addWidget(view);
    w->show();

    view->show();
    return channel;
}

void OutputLibraryInfo(){
    DuboNodules::Root * root = new DuboNodules::Root();
    qDebug() << root->property("NAME");
    qDebug() << root->property("VENDOR");
    qDebug() << root->property("VERSION");
    qDebug() << root->property("REVISION");
    qDebug() << root->property("CHANGESET");
    qDebug() << root->property("BUILD");
    qDebug() << root->property("LINK");
    qDebug() << root->property("QT");
    qDebug() << root->property("PLUGIN_NAME");
    qDebug() << root->property("PLUGIN_VERSION");
    qDebug() << root->property("PLUGIN_REVISION");
}

int mainNoJavascript(int argc, char *argv[])
{
    // Get your app going
    QApplication app(argc, argv);

    // From QT side
    QWidget * w = new QWidget();
    w->show();

    // Just spit out library info
    OutputLibraryInfo();

    return app.exec();
}

int mainJavascript(int argc, char *argv[])
{
    // Get your app going
    QApplication app(argc, argv);

    DuboNodules::Web::Config * wcon = new DuboNodules::Web::Config();
    // Display the webview
    QWebChannel * chan = SetupWebView(wcon);

    // Attach objects to the javascript context
    DuboNodules::Root * root = new DuboNodules::Root();
    DuboNodules::FileSystem::Helpers * help = new DuboNodules::FileSystem::Helpers(chan);
    DuboNodules::FileSystem::Watcher * watch = new DuboNodules::FileSystem::Watcher(chan);
    DuboNodules::FileSystem::Walker * walk = new DuboNodules::FileSystem::Walker(chan);
    DuboNodules::Network::Controller * net = new DuboNodules::Network::Controller(QString::fromLatin1("/Users/dmp/tmp/qttestcache"), chan);
    DuboNodules::Network::Server * nets = new DuboNodules::Network::Server(chan);
    DuboNodules::UI::MenuBar * mb = new DuboNodules::UI::MenuBar(chan);
    DuboNodules::UI::Tray * tray = new DuboNodules::UI::Tray();

    chan->registerObject(QString::fromLatin1("Root"), root);
    chan->registerObject(QString::fromLatin1("Helpers"), help);
    chan->registerObject(QString::fromLatin1("Watcher"), watch);
    chan->registerObject(QString::fromLatin1("Walker"), walk);
    chan->registerObject(QString::fromLatin1("Network"), net);
    chan->registerObject(QString::fromLatin1("NetServe"), nets);
    chan->registerObject(QString::fromLatin1("MenuBar"), mb);
    chan->registerObject(QString::fromLatin1("Tray"), tray);
    chan->registerObject(QString::fromLatin1("ViewConfig"), wcon);

    return app.exec();
}

int main(int argc, char *argv[]){
    // Delegated to javascript
    return mainJavascript(argc, argv);
    // Purely c++
    // return mainNoJavascript(argc, argv);
}
