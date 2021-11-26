/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt OPC UA module.
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

#include <private/opcuawriteitem_p.h>
#include <private/opcuawriteresult_p.h>

#include <QJSValue>

QT_BEGIN_NAMESPACE

/*!
    \qmltype WriteItem
    \inqmlmodule QtOpcUa
    \brief Specifies an item to be written to the server.
    \since QtOpcUa 5.13

    This type is used to specify items to be written to the server using the function
    \l Connection::writeNodeAttributes.
*/

/*!
    \qmlproperty Constants.NodeAttribute WriteItem::attribute

    Determines the attribute of the node to be written.
*/

/*!
    \qmlproperty string WriteItem::indexRange

    Determines the index range of the attribute to be written.
    If not needed, leave this property empty.
*/

/*!
    \qmlproperty string Writetem::nodeId

    Determines the node id of the node to be written.
*/

/*!
    \qmlproperty variant WriteItem::ns

    Determines the namespace of the node to be written.
    The namespace can be given by name or index.
    If this property is given, any namespace in the node id will be
    ignored.
*/

/*!
    \qmlproperty datetime WriteItem::serverTimestamp

    Sets the server timestamp for the value to be written.
    If the server timestamp is invalid, it is ignored by the client and not sent to the server.
    If the server doesn't support writing timestamps, the write operation for this item
    will fail.
*/

/*!
    \qmlproperty datetime WriteItem::sourceTimestamp

    Sets the source timestamp for the value to write.
    If the source timestamp is invalid, it is ignored by the client and not sent to the server.
    If the server doesn't support writing timestamps, the write operation for this item
    will fail.
*/

/*!
    \qmlproperty variant WriteItem::value

    Actual data that is requested to be written.
*/

/*!
    \qmlproperty variant WriteItem::valueType

    If given, the type information will be used in converting
    the value to a SDK specific data type.
*/

/*!
    \qmlproperty OpcUaStatus WriteItem::statusCode

    Sets the status code for the value to write.
    If no status code is set, no status code is sent to the server.
*/

class OpcUaWriteItemData : public QSharedData
{
public:
    QOpcUa::NodeAttribute attribute;
    QString indexRange;
    QString nodeId;
    QVariant namespaceIdentifier;
    QDateTime serverTimestamp;
    QDateTime sourceTimestamp;
    QVariant value;
    QOpcUa::Types valueType = QOpcUa::Types::Undefined;
    OpcUaStatus::Status statusCode;
    bool hasStatusCode = false;
};

OpcUaWriteItem::OpcUaWriteItem()
    : data(new OpcUaWriteItemData)
{
    data->attribute = QOpcUa::NodeAttribute::Value;
}

OpcUaWriteItem::OpcUaWriteItem(const OpcUaWriteItem &other)
    : data(other.data)
{
}

OpcUaWriteItem &OpcUaWriteItem::operator=(const OpcUaWriteItem &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

OpcUaWriteItem::~OpcUaWriteItem() = default;

const QString &OpcUaWriteItem::indexRange() const
{
    return data->indexRange;
}

void OpcUaWriteItem::setIndexRange(const QString &indexRange)
{
    data->indexRange = indexRange;
}

const QString &OpcUaWriteItem::nodeId() const
{
    return data->nodeId;
}

void OpcUaWriteItem::setNodeId(const QString &nodeId)
{
    data->nodeId = nodeId;
}

QOpcUa::NodeAttribute OpcUaWriteItem::attribute() const
{
    return data->attribute;
}

void OpcUaWriteItem::setAttribute(QOpcUa::NodeAttribute attribute)
{
    data->attribute = attribute;
}

const QVariant &OpcUaWriteItem::namespaceIdentifier() const
{
    return data->namespaceIdentifier;
}

void OpcUaWriteItem::setNamespaceIdentifier(const QVariant &namespaceIdentifier)
{
    data->namespaceIdentifier = namespaceIdentifier;
}

void OpcUaWriteItem::setServerTimestamp(const QDateTime &serverTimestamp)
{
    data->serverTimestamp = serverTimestamp;
}

const QDateTime &OpcUaWriteItem::serverTimestamp() const
{
    return data->serverTimestamp;
}

void OpcUaWriteItem::setSourceTimestamp(const QDateTime &sourceTimestamp)
{
    data->sourceTimestamp = sourceTimestamp;
}

const QDateTime &OpcUaWriteItem::sourceTimestamp() const
{
    return data->sourceTimestamp;
}

void OpcUaWriteItem::setValue(const QVariant &value)
{
    auto tmp = value;
    // In case of an array value the type is QJSValue,
    // which has to be unpacked to recognized.
    if (tmp.userType() == qMetaTypeId<QJSValue>())
        tmp = tmp.value<QJSValue>().toVariant();

    data->value = tmp;
}

const QVariant &OpcUaWriteItem::value() const
{
    return data->value;
}

QOpcUa::Types OpcUaWriteItem::valueType() const
{
    return data->valueType;
}

void OpcUaWriteItem::setValueType(QOpcUa::Types type)
{
    data->valueType = type;
}

OpcUaStatus::Status OpcUaWriteItem::statusCode() const
{
    return data->statusCode;
}

bool OpcUaWriteItem::hasStatusCode() const
{
    return data->hasStatusCode;
}

void OpcUaWriteItem::setStatusCode(OpcUaStatus::Status statusCode)
{
    data->statusCode = statusCode;
    data->hasStatusCode = true;
}

OpcUaWriteItem OpcUaWriteItemFactory::create()
{
    return OpcUaWriteItem();
}

QT_END_NAMESPACE

