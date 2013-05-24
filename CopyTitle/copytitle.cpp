/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2013  David Rosca <nowrep@gmail.com>
* Copyright (C) 2012-2013  Mladen Pejaković <pejakm@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "copytitle.h"
#include "qupzilla.h"
#include "webview.h"
#include "pluginproxy.h"
#include "mainapplication.h"

#include <QTranslator>
#include <QWebHitTestResult>
#include <QClipboard>
#include <QMenu>

CopyTitle::CopyTitle()
    : QObject()
    , m_view(0)
{
}

PluginSpec CopyTitle::pluginSpec()
{
    PluginSpec spec;
    spec.name = "Copy Title";
    spec.info = "Copy page title or image name";
    spec.description = "Menu action to copy page title or image name";
    spec.version = "0.1";
    spec.author = QString::fromUtf8("Mladen Pejaković <pejakm@gmail.com>");
    spec.icon = QPixmap(":/copytitle/data/copytitle.png");
    spec.hasSettings = false;

    return spec;
}

void CopyTitle::init(const QString &sPath)
{
    QZ_REGISTER_EVENT_HANDLER(PluginProxy::MousePressHandler);
}

void CopyTitle::unload()
{
}

bool CopyTitle::testPlugin()
{
    return (QupZilla::VERSION == QLatin1String("1.4.3"));
}

QTranslator* CopyTitle::getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/copytitle/locale/");
    return translator;
}

void CopyTitle::populateWebViewMenu(QMenu* menu, WebView* view, const QWebHitTestResult &r)
{
    m_view = view;

    if (!r.linkUrl().isEmpty() || r.isContentEditable()) return;

    QString title;
    if (!r.imageUrl().isEmpty()) {
        QString m_userFileName = r.imageUrl().toString().trimmed();
        int pos = m_userFileName.lastIndexOf(QLatin1Char('/'));
        if (pos != -1) {
            int size = m_userFileName.size();
            title = m_userFileName.right(size - pos - 1);
        }
        else {
            title += "image";
        }
    }

    else {
        title = view->title();
    }

    menu->addAction(QIcon::fromTheme("edit-copy"), !r.imageUrl().isEmpty() ? tr("Copy Image Name") : tr("Copy Page Title"), this, SLOT(copyPageTitle()))->setData(title);
}

bool CopyTitle::mousePress(const Qz::ObjectName &type, QObject* obj, QMouseEvent* event)
{
    return false;
}

void CopyTitle::copyPageTitle()
{
    if (QAction* action = qobject_cast<QAction*>(sender())) {
        QApplication::clipboard()->setText(action->data().toString());
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CopyTitle, CopyTitle)
#endif
