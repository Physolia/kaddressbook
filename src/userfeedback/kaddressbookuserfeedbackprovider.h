/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "kaddressbook_export.h"
#include <KUserFeedbackQt6/Provider>
class KADDRESSBOOK_EXPORT KAddressBookUserFeedbackProvider : public KUserFeedback::Provider
{
    Q_OBJECT
public:
    explicit KAddressBookUserFeedbackProvider(QObject *parent = nullptr);
    ~KAddressBookUserFeedbackProvider() override;
};
