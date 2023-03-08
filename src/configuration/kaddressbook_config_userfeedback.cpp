/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kaddressbook_config_userfeedback.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QHBoxLayout>
#ifdef WITH_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.h"
#ifdef USE_KUSERFEEDBACK_QT6
#include <KUserFeedbackQt6/FeedbackConfigWidget>
#else
#include <KUserFeedback/FeedbackConfigWidget>
#endif
#endif

using namespace KAddressBook;

K_PLUGIN_CLASS_WITH_JSON(KCMKaddressbookUserFeedBackConfig, "kaddressbook_config_userfeedback.json")

KCMKaddressbookUserFeedBackConfig::KCMKaddressbookUserFeedBackConfig(QWidget *parent, const QVariantList &args)
    : KCModule(parent, args)
    , mUserFeedbackWidget(new KUserFeedback::FeedbackConfigWidget(this))
{
    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins({});

    auto about = new KAboutData(QStringLiteral("kcmaddressbookuserfeedbackconfig"),
                                i18n("Configure User FeedBack"),
                                QString(),
                                QString(),
                                KAboutLicense::GPL,
                                i18n("(c), 2020-2021 Laurent Montel"));

    about->addAuthor(i18n("Laurent Montel"), QString(), QStringLiteral("montel@kde.org"));

    setAboutData(about);
    connect(mUserFeedbackWidget, &KUserFeedback::FeedbackConfigWidget::configurationChanged, this, &KCMKaddressbookUserFeedBackConfig::markAsChanged);

    lay->addWidget(mUserFeedbackWidget);
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

void KCMKaddressbookUserFeedBackConfig::save()
{
    UserFeedBackManager::self()->userFeedbackProvider()->setTelemetryMode(mUserFeedbackWidget->telemetryMode());
    UserFeedBackManager::self()->userFeedbackProvider()->setSurveyInterval(mUserFeedbackWidget->surveyInterval());
}

void KCMKaddressbookUserFeedBackConfig::load()
{
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

void KCMKaddressbookUserFeedBackConfig::defaults()
{
    // FIXME ?
    mUserFeedbackWidget->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
}

#include "kaddressbook_config_userfeedback.moc"
