/*
  This file is part of KAddressBook Kontact Plugin.

  Copyright (c) 2009-2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "kaddressbook_plugin.h"
#include "../src/kaddressbook_options.h"
#include "kaddressbookinterface.h"

#include <KontactInterface/Core>

#include <QAction>
#include <KActionCollection>
#include "kaddressbookplugin_debug.h"
#include <KLocalizedString>

#include <QIcon>

#include <QDBusConnection>
#include <QDBusReply>
#include <QStandardPaths>

EXPORT_KONTACT_PLUGIN_WITH_JSON(KAddressBookPlugin, "kaddressbookplugin.json")

KAddressBookPlugin::KAddressBookPlugin(KontactInterface::Core *core, const QVariantList &)
    : KontactInterface::Plugin(core, core, "kaddressbook")
{
    setComponentName(QStringLiteral("kaddressbook"), i18n("KAddressBook"));

    QAction *action
        = new QAction(QIcon::fromTheme(QStringLiteral("contact-new")),
                      i18nc("@action:inmenu", "New Contact..."), this);
    actionCollection()->addAction(QStringLiteral("new_contact"), action);
    connect(action, &QAction::triggered, this, &KAddressBookPlugin::slotNewContact);
    actionCollection()->setDefaultShortcut(action, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    //action->setHelpText(
    //  i18nc( "@info:status", "Create a new contact" ) );
    action->setWhatsThis(
        i18nc("@info:whatsthis",
              "You will be presented with a dialog where you can create a new contact."));
    insertNewAction(action);

    action
        = new QAction(QIcon::fromTheme(QStringLiteral("user-group-new")),
                      i18nc("@action:inmenu", "New Contact Group..."), this);
    actionCollection()->addAction(QStringLiteral("new_contactgroup"), action);
    connect(action, &QAction::triggered, this, &KAddressBookPlugin::slotNewContactGroup);
    actionCollection()->setDefaultShortcut(action, QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
    //action->setHelpText(
    //  i18nc( "@info:status", "Create a new contact group" ) );
    action->setWhatsThis(
        i18nc("@info:whatsthis",
              "You will be presented with a dialog where you can create a new contact group."));
    insertNewAction(action);

    mUniqueAppWatcher = new KontactInterface::UniqueAppWatcher(
        new KontactInterface::UniqueAppHandlerFactory<KAddressBookUniqueAppHandler>(), this);
}

KAddressBookPlugin::~KAddressBookPlugin()
{
}

void KAddressBookPlugin::slotNewContact()
{
    KParts::Part *part = createPart();
    if (!part) {
        return;
    }

    if (part->metaObject()->indexOfMethod("newContact()") == -1) {
        qCWarning(KADDRESSBOOKPLUGIN_LOG) << "KAddressBook part is missing slot newContact()";
        return;
    }

    QMetaObject::invokeMethod(part, "newContact");
}

void KAddressBookPlugin::slotNewContactGroup()
{
    KParts::Part *part = createPart();
    if (!part) {
        return;
    }

    if (part->metaObject()->indexOfMethod("newGroup()") == -1) {
        qCWarning(KADDRESSBOOKPLUGIN_LOG) << "KAddressBook part is missing slot newGroup()";
        return;
    }

    QMetaObject::invokeMethod(part, "newGroup");
}

KParts::Part *KAddressBookPlugin::createPart()
{
    KParts::Part *part = loadPart();
    if (!part) {
        return nullptr;
    }

    // disable the Ctrl+N shortcut, as it is used by Kontact already
    if (part->action("akonadi_contact_create")) {
        QAction *newAction = part->action("akonadi_contact_create");
        if (newAction) {
            newAction->setShortcut(QKeySequence());
        }
    }

    return part;
}

bool KAddressBookPlugin::isRunningStandalone() const
{
    return mUniqueAppWatcher->isRunningStandalone();
}

int KAddressBookPlugin::weight() const
{
    return 300;
}

QStringList KAddressBookPlugin::invisibleToolbarActions() const
{
    QStringList actions;
    actions << QStringLiteral("akonadi_contact_create") << QStringLiteral("akonadi_contact_group_create");
    return actions;
}

void KAddressBookPlugin::shortcutChanged()
{
    KParts::Part *localPart = part();
    if (localPart) {
        if (localPart->metaObject()->indexOfMethod("updateQuickSearchText()") == -1) {
            qCWarning(KADDRESSBOOKPLUGIN_LOG) << "KAddressBook part is missing slot updateQuickSearchText()";
            return;
        }
        QMetaObject::invokeMethod(localPart, "updateQuickSearchText");
    }
}

void KAddressBookUniqueAppHandler::loadCommandLineOptions(QCommandLineParser *parser)
{
    kaddressbook_options(parser);
}

int KAddressBookUniqueAppHandler::activate(const QStringList &arguments, const QString &workingDir)
{
    // Ensure part is loaded
    (void)plugin()->part();
    org::kde::kaddressbook kaddressbook(QStringLiteral("org.kde.kaddressbook"), QStringLiteral("/KAddressBook"), QDBusConnection::sessionBus());
    QDBusReply<bool> reply = kaddressbook.handleCommandLine(arguments);
    Q_UNUSED(reply);

    return KontactInterface::UniqueAppHandler::activate(arguments, workingDir);
}

#include "kaddressbook_plugin.moc"
