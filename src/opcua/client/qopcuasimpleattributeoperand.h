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

#ifndef QOPCUASIMPLEATTRIBUTEOPERAND_H
#define QOPCUASIMPLEATTRIBUTEOPERAND_H

#include <QtOpcUa/qopcuatype.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QOpcUaQualifiedName;

// OPC-UA part 4, 7.4.4.5
class QOpcUaSimpleAttributeOperandData;
class Q_OPCUA_EXPORT QOpcUaSimpleAttributeOperand
{
public:
    QOpcUaSimpleAttributeOperand();
    QOpcUaSimpleAttributeOperand(const QOpcUaSimpleAttributeOperand &);
    QOpcUaSimpleAttributeOperand(const QString &name, quint16 namespaceIndex = 0,
                            const QString &typeId = QStringLiteral("ns=0;i=2041"), // BaseEventType
                            QOpcUa::NodeAttribute attributeId = QOpcUa::NodeAttribute::Value);
    QOpcUaSimpleAttributeOperand(QOpcUa::NodeAttribute attributeId,
                            const QString &typeId = QStringLiteral("ns=0;i=2041")); // BaseEventType
    QOpcUaSimpleAttributeOperand &operator=(const QOpcUaSimpleAttributeOperand &);
    bool operator==(const QOpcUaSimpleAttributeOperand &rhs) const;
    operator QVariant() const;
    ~QOpcUaSimpleAttributeOperand();

    QString typeId() const;
    void setTypeId(const QString &typeId);

    QList<QOpcUaQualifiedName> browsePath() const;
    QList<QOpcUaQualifiedName> &browsePathRef();
    void setBrowsePath(const QList<QOpcUaQualifiedName> &browsePath);

    QOpcUa::NodeAttribute attributeId() const;
    void setAttributeId(QOpcUa::NodeAttribute attributeId);

    QString indexRange() const;
    void setIndexRange(const QString &indexRange);

private:
    QSharedDataPointer<QOpcUaSimpleAttributeOperandData> data;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QOpcUaSimpleAttributeOperand)

#endif // QOPCUASIMPLEATTRIBUTEOPERAND_H
