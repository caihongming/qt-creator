/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

#include <QAbstractItemModel>
#include <QPointer>
#include <QWidget>

namespace LanguageClient {

constexpr char noLanguageFilter[] = "No Filter";

class BaseClient;

class BaseSettings
{
public:
    BaseSettings() = default;
    BaseSettings(const QString &name, bool enabled, const QString &mimeTypeName)
        : m_name(name)
        , m_enabled(enabled)
        , m_mimeType(mimeTypeName)
    {}

    virtual ~BaseSettings() = default;

    QString m_name = QString("New Language Server");
    bool m_enabled = true;
    QString m_mimeType = QLatin1String(noLanguageFilter);
    QPointer<BaseClient> m_client; // not owned

    virtual BaseSettings *copy() const { return new BaseSettings(*this); }
    virtual bool needsRestart() const;
    virtual bool isValid() const ;
    virtual BaseClient *createClient() const;
    virtual QVariantMap toMap() const;
    virtual void fromMap(const QVariantMap &map);

protected:
    BaseSettings(const BaseSettings &other) = default;
    BaseSettings(BaseSettings &&other) = default;
    BaseSettings &operator=(const BaseSettings &other) = default;
    BaseSettings &operator=(BaseSettings &&other) = default;
};

class StdIOSettings : public BaseSettings
{
public:
    StdIOSettings() = default;
    StdIOSettings(const QString &name, bool enabled, const QString &mimeTypeName,
                  const QString &executable, const QString &arguments)
        : BaseSettings(name, enabled, mimeTypeName)
        , m_executable(executable)
        , m_arguments(arguments)
    {}

    ~StdIOSettings() override = default;

    QString m_executable;
    QString m_arguments;

    BaseSettings *copy() const override { return new StdIOSettings(*this); }
    bool needsRestart() const override;
    bool isValid() const override;
    BaseClient *createClient() const override;
    QVariantMap toMap() const override;
    void fromMap(const QVariantMap &map) override;

protected:
    StdIOSettings(const StdIOSettings &other) = default;
    StdIOSettings(StdIOSettings &&other) = default;
    StdIOSettings &operator=(const StdIOSettings &other) = default;
    StdIOSettings &operator=(StdIOSettings &&other) = default;
};

class LanguageClientSettings
{
public:
    static void init();
    static QList<StdIOSettings *> fromSettings(QSettings *settings);
    static void toSettings(QSettings *settings, const QList<StdIOSettings *> &languageClientSettings);
};

} // namespace LanguageClient
