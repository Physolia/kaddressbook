/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "grantleeprintstyle.h"
#include "kaddressbookimportexportcontactfields.h"
#include "printingwizard.h"
#include "printprogress.h"
#include "printstyle.h"
#include <KaddressbookGrantlee/GrantleePrint>

#include <KContacts/Addressee>

#include <KLocalizedString>

#include <QPrinter>
#include <QTextDocument>
#include <QFile>
#include <QDir>

using namespace KABPrinting;

GrantleePrintStyle::GrantleePrintStyle(const QString &themePath, PrintingWizard *parent)
    : PrintStyle(parent)
    , m_themePath(themePath)
{

    QFile previewFile(QString(themePath + QDir::separator() + QStringLiteral("preview.png")));
    if (previewFile.exists()) {
        setPreview(previewFile.fileName());
    }
    setPreferredSortOptions(KAddressBookImportExport::KAddressBookImportExportContactFields::FormattedName, Qt::AscendingOrder);
}

GrantleePrintStyle::~GrantleePrintStyle()
{
}

void GrantleePrintStyle::print(const KContacts::Addressee::List &contacts, PrintProgress *progress)
{
    QPrinter *printer = wizard()->printer();
    printer->setPageMargins(20, 20, 20, 20, QPrinter::DevicePixel);

    progress->addMessage(i18n("Setting up document"));

    KAddressBookGrantlee::GrantleePrint grantleePrint(m_themePath);
    grantleePrint.setApplicationDomain("kaddressbook");
    const QString html = grantleePrint.contactsToHtml(contacts);

    QTextDocument document;
    document.setHtml(html);

    progress->addMessage(i18n("Printing"));

    document.print(printer);

    progress->addMessage(i18nc("Finished printing", "Done"));
}

GrantleeStyleFactory::GrantleeStyleFactory(const QString &name, const QString &themePath, PrintingWizard *parent)
    : PrintStyleFactory(parent)
    , mThemePath(themePath)
    , mName(name)
{
}

PrintStyle *GrantleeStyleFactory::create() const
{
    return new GrantleePrintStyle(mThemePath, mParent);
}

QString GrantleeStyleFactory::description() const
{
    return mName;
}
