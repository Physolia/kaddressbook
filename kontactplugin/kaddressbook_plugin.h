/*
  This file is part of KAddressBook Kontact Plugin.

  SPDX-FileCopyrightText: 2009-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KontactInterface/UniqueAppHandler>

namespace KontactInterface
{
class Plugin;
}

class KAddressBookUniqueAppHandler : public KontactInterface::UniqueAppHandler
{
    Q_OBJECT
public:
    explicit KAddressBookUniqueAppHandler(KontactInterface::Plugin *plugin)
        : KontactInterface::UniqueAppHandler(plugin)
    {
    }

    void loadCommandLineOptions(QCommandLineParser *parser) override;
    int activate(const QStringList &args, const QString &workingDir) override;
};

class KAddressBookPlugin : public KontactInterface::Plugin
{
    Q_OBJECT

public:
    explicit KAddressBookPlugin(KontactInterface::Core *core, const KPluginMetaData &data, const QVariantList &);
    ~KAddressBookPlugin() override;

    [[nodiscard]] bool isRunningStandalone() const override;
    [[nodiscard]] int weight() const override;

    [[nodiscard]] QStringList invisibleToolbarActions() const override;
    void shortcutChanged() override;

protected:
    KParts::Part *createPart() override;

private:
    void slotNewContact();
    void slotNewContactGroup();

    KontactInterface::UniqueAppWatcher *mUniqueAppWatcher = nullptr;
};
