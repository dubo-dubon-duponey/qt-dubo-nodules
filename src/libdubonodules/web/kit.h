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

#ifndef DUBONODULES_WEBKIT_H
#define DUBONODULES_WEBKIT_H

#include <QObject>
#include <QNetworkProxyFactory>
#include <QMainWindow>
//#include <QNetworkAccessManager>

#include <QWebEngineView>

#include <QHash>
#include <QDir>
#include <QtWebEngine>
#include <QWebEngineSettings>

#include "view.h"

namespace DuboNodules{
namespace Web{

class Kit : public QObject
{
    Q_OBJECT
public:
    explicit Kit(DuboNodules::Web::Config * config, QWidget * parent = nullptr): QObject(parent)
    {
        // Ensure the qwebchannel is there
        QFileInfo jsFileInfo(QDir::currentPath() + QString::fromLatin1("/qwebchannel.js"));

        if (!jsFileInfo.exists())
            QFile::copy(QString::fromLatin1(":/qtwebchannel/qwebchannel.js"), jsFileInfo.absoluteFilePath());

        // Init the qwebengine
        QtWebEngine::initialize();

        // Set defaults
        this->setDefaults();

        // Get our webview
        m_webview = new DuboNodules::Web::View(config, parent);

        // A page
        // XXX no custom page implementation yet/at this point
        // webview->setPage(new Page(this));
        QWebEnginePage * pager = m_webview->page();
        // pager->setPluginFactory(new DuboPlayer::WebPluginFactory(pager));
        // pager->setNetworkAccessManager(qnam);

        // Danger! settings are for both the page and the page resets settings obviously
        // The webroxer view has some special settings and priviledges as well
        m_webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
        m_webview->settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, false);
        m_webview->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);

        m_webview->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        m_webview->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
        // XXX Not yet at QT 5.6
        /*
        webview->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
        webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);
        webview->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
        webview->settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, true);
        webview->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
        */

        // webview->settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, false);
        // webview->settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);
        // webview->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
        // webview->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);

        // XXX maybe replace with urlChanged?
        connect(pager, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(windowCleared()));
    }

    void addToJs(const QString & name, QObject * object )
    {
        qDebug() << " [M] Kit: add to js API: " << name;
        m_exposed[name] = object;
        this->inject(name, object);
    }

    /*
    void removeFromJs(const QString & name )
    {
        qDebug() << " [M] WebCul: remove from js API: " << name;
        m_exposed.remove(name);
        QString evalMe = QString::fromLatin1("delete %name;");
        m_webview->page()->runJavaScript(evalMe.replace(QString::fromLatin1("%name"), name));
    }*/

    void start(const QString & name)
    {
        qDebug() << " [M] Kit: start with url " << name;
        m_webview->load(QUrl(name));
    }

    DuboNodules::Web::View * widget()
    {
        return m_webview;
    }



private:
    // Holds our webview
    DuboNodules::Web::View * m_webview;

    // Helper to set global defaults
    void setDefaults()
    {
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, false);
        // Disabling this - why is this on by default?
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, false);
        // XXX do we want that?
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, false);

        // XXX not yet in QT 5.6
        /*
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);

        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::TouchIconsEnabled, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PrintElementBackgrounds, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::ShowScrollBars, true);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, false);
        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, false);
        */
    }

    // Contains the hash of exposed objects
    QHash<QString, QObject *> m_exposed;

    // Helper to inject data on each page load from the exposed hash
    void inject(const QString &name, QObject * object)
    {
        // XXX the injector is all broken right now
        qDebug() << " [M] WebCul: injecting into context: " << name;
        QWebEnginePage * frame = m_webview->page();
        // Add to context
        (frame->webChannel())->registerObject(name, object);

        /*
         * // Rehash the global object so that script know what to look for?
        QHashIterator<QString, QObject *> i(m_exposed);
        while(i.hasNext()){
            i.next();
            this->inject(i.key(), i.value());
        }
        */

        /* (frame->webChannel())->registerObject(QString::fromLatin1("__roxeePrivate"), object);
        // Make sure every depth in the name exists, map onto a local name, remap, clean.
        QString evalMe = QString::fromLatin1("var name ='%name'; var spl = name.split('.'), current = window, nextIt; while(spl.length){nextIt = spl.shift(); if(!(nextIt in current)){current[nextIt] = {};}current = current[nextIt];}; %name = __roxeePrivate; delete __roxeePrivate;");
        evalMe = evalMe.replace(QString::fromLatin1("%name"), name);
        frame->runJavaScript(evalMe);*/
    }

private slots:
    void windowCleared()
    {
        // XXX enforce security here?
        QHashIterator<QString, QObject *> i(m_exposed);
        while(i.hasNext()){
            i.next();
            this->inject(i.key(), i.value());
        }
    }

};

}
}

#endif // DUBONODULES_WEBKIT_H
