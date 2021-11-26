/****************************************************************************
**
** Copyright (C) 2015 basysKom GmbH, opensource@basyskom.com
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QOPCUALOCALIZEDTEXT_H
#define QOPCUALOCALIZEDTEXT_H

#include <QtOpcUa/qopcuaglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE

class QOpcUaLocalizedTextData;
class Q_OPCUA_EXPORT QOpcUaLocalizedText
{
    Q_GADGET
    Q_PROPERTY(QString locale READ locale WRITE setLocale)
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    QOpcUaLocalizedText();
    QOpcUaLocalizedText(const QOpcUaLocalizedText &);
    QOpcUaLocalizedText(const QString &locale, const QString &text);
    QOpcUaLocalizedText &operator=(const QOpcUaLocalizedText &);
    bool operator==(const QOpcUaLocalizedText &rhs) const;
    operator QVariant() const;
    ~QOpcUaLocalizedText();

    QString locale() const;
    void setLocale(const QString &locale);

    QString text() const;
    void setText(const QString &text);

private:
    QSharedDataPointer<QOpcUaLocalizedTextData> data;
};

Q_OPCUA_EXPORT QDebug operator<<(QDebug debug, const QOpcUaLocalizedText &lt);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QOpcUaLocalizedText)

#endif // QOPCUALOCALIZEDTEXT_H
