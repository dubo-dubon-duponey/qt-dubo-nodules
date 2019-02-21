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

#ifndef DUBONODULES_WEBVIEW_H
#define DUBONODULES_WEBVIEW_H

#include <QWidget>
#include <QWebEngineView>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QLayout>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDesktopServices>

namespace DuboNodules{
namespace Web{

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject * parent = nullptr): QObject(parent)
    {
//        m_whitelist_overlay[QString::fromUtf8("www.facebook.com")] = QString::fromUtf8("/login/");
//        m_whitelist_popup[QString::fromUtf8("www.facebook.com")] = QString::fromUtf8("");
    }

    Q_PROPERTY(QVariant overlay READ getWhitelistOverlay WRITE setWhitelistOverlay NOTIFY updated)
    Q_PROPERTY(QVariant popup READ getWhitelistPopup WRITE setWhitelistPopup NOTIFY updated)

    Q_INVOKABLE bool matchOverlay(QUrl url)
    {
        QMap<QString, QString>::const_iterator i = m_whitelist_overlay.constBegin();
        while (i != m_whitelist_overlay.constEnd()) {
            QString host = i.key();
            QString path = i.value();
            if (url.host() == host && (!path.length() || url.path() == path)){
                return true;
            }
            ++i;
        }
        return false;
    }

    Q_INVOKABLE bool matchPopup(QUrl url)
    {
        QMap<QString, QString>::const_iterator i = m_whitelist_popup.constBegin();
        while (i != m_whitelist_popup.constEnd()) {
            QString host = i.key();
            QString path = i.value();
            if (url.host() == host && (!path.length() || url.path() == path)){
                return true;
            }
            ++i;
        }
        return false;
    }

private:
    QMap<QString, QString> m_whitelist_popup = QMap<QString, QString>();
    QMap<QString, QString> m_whitelist_overlay  = QMap<QString, QString>();

    QVariant getWhitelistOverlay() const
    {
        return QVariant::fromValue(m_whitelist_overlay);
    }

    QVariant getWhitelistPopup() const
    {
        return QVariant::fromValue(m_whitelist_popup);
    }

    void setWhitelistOverlay(const QVariant & list)
    {
        m_whitelist_overlay.clear();
        QMap<QString, QVariant> object = list.toMap();
        QMap<QString, QVariant>::const_iterator i = object.constBegin();
        while (i != object.constEnd()) {
            m_whitelist_overlay[i.key()] = i.value().toString();
            ++i;
        }
    }

    void setWhitelistPopup(const QVariant & list)
    {
        m_whitelist_popup.clear();
        QMap<QString, QVariant> object = list.toMap();
        QMap<QString, QVariant>::const_iterator i = object.constBegin();
        while (i != object.constEnd()) {
            m_whitelist_popup[i.key()] = i.value().toString();
            ++i;
        }
    }

signals:
    void updated();

};

class OpenerView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit OpenerView(DuboNodules::Web::Config * config, bool sub = false, QWidget *parent = nullptr): QWebEngineView(parent)
    {
        if(sub){
            connect(this, SIGNAL(urlChanged( const QUrl &)), this, SLOT(urlChanged(const QUrl &)));
            connect(this->page(), SIGNAL(windowCloseRequested()), this, SLOT(closeRequested()));
        }
        m_config = config;
    }

    QWebEngineView * createWindow(QWebEnginePage::WebWindowType /*type*/)
    {
        defered = new OpenerView(m_config, true, this);
        return defered;
    }

protected:
    OpenerView * defered;
    DuboNodules::Web::Config * m_config;

private:
    void displayAsOverlay()
    {
        // Get the parentWidget (that's the parent webview)
        QWidget * p = this->parentWidget();

        // Connect the parent so they know to un-hide back on close for this
        connect(this->page(), SIGNAL(windowCloseRequested()), p, SLOT(overlayClosed()));

        // Move us into their parent
        p->parentWidget()->layout()->addWidget(this);

        // Hide the calling webview
        p->setHidden(true);

        // Unhide us
        this->setHidden(false);
        this->show();
    }

    void displayAsPopup()
    {
        // XXX this is broken with qt 5.12.1 (works with 5.6) - not sure what's going on here
        // Detach
        this->setParent(nullptr);
        // Show standalone
        this->setHidden(false);
        this->show();
    }

    void forwardToSystem(const QUrl url)
    {
        // https://secure.meetup.com/login/
        QDesktopServices::openUrl(url);
        // Call it close
        this->page()->runJavaScript(QString::fromLatin1("window.close();"));
    }

public slots:

    void overlayClosed()
    {
        disconnect(defered->page(), SIGNAL(windowCloseRequested()), this, SLOT(overlayClosed()));
        defered = nullptr;
        // Show again
        this->setHidden(false);
    }

    void urlChanged(const QUrl & url)
    {
        disconnect(this, SIGNAL(urlChanged ( const QUrl &)), this, SLOT(urlChanged(const QUrl &)));

        if(m_config->matchOverlay(url))
            return this->displayAsOverlay();

        if(m_config->matchPopup(url))
            return this->displayAsPopup();

        this->forwardToSystem(url);

    //    this->setStyleSheet(QString::fromLatin1("*{background-color: transparent !important;}"));
    //    this->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    //    this->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    }

    void closeRequested()
    {
        // Drop it
        disconnect(this->page(), SIGNAL(windowCloseRequested()), this, SLOT(closeRequested()));
        this->setParent(nullptr);
    }

};


class View : public OpenerView
{
    Q_OBJECT
public:
    explicit View(Config * config, QWidget *parent): OpenerView(config, 0, parent)
    {

    }

    QWebEngineView * createWindow(QWebEnginePage::WebWindowType /*type*/)
    {
        defered = new OpenerView(m_config, true, this);
        return defered;
    }

private:
    void dropEvent(QDropEvent * e)
    {
        if(e->mimeData()->urls().length()){
            QList<QUrl> ulist = e->mimeData()->urls();
            QFileInfo * inf;
            foreach (QUrl u, ulist){
                inf = new QFileInfo(u.path());
                if(inf->isDir()){
                    qDebug() << ">>>>>>>>>>>>> Emitting has directory drop";
                    emit hasDirectoryDrop(u.path());
                }else if(inf->isFile()){
                    qDebug() << ">>>>>>>>>>>>> Emitting has drops";
                    emit hasFileDrop(u.path());
                }
            }
        }
        e->acceptProposedAction();
    }

    void dragEnterEvent(QDragEnterEvent *e)
    {
        if(e->mimeData()->urls().length()){
            qDebug() << "Start handling drops";
        }
        e->acceptProposedAction();
    }

signals:
    void hasFileDrop(const QString & path);
    void hasDirectoryDrop(const QString & path);
};

}
}

#endif // DUBONODULES_WEBVIEW_H
