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

#include "qopcuaexpandednodeid.h"
#include "qopcuatype.h"

QT_BEGIN_NAMESPACE

/*!
    \class QOpcUaExpandedNodeId
    \inmodule QtOpcUa
    \brief The OPC UA ExpandedNodeId.

    An expanded node id is a node id where the namespace index can be given as index or as a string URI.
    A list of namespaces and their indices on the server is provided by \l QOpcUaClient::namespaceArray().
*/

class QOpcUaExpandedNodeIdData : public QSharedData
{
public:
    quint32 serverIndex{0};
    QString namespaceUri;
    QString nodeId;
};

QOpcUaExpandedNodeId::QOpcUaExpandedNodeId()
    : data(new QOpcUaExpandedNodeIdData)
{
}

/*!
    Constructs an expanded node id from \a rhs.
*/
QOpcUaExpandedNodeId::QOpcUaExpandedNodeId(const QOpcUaExpandedNodeId &rhs)
    : data(rhs.data)
{
}

/*!
    Constructs an expanded node id from node id string \a nodeId.
*/
QOpcUaExpandedNodeId::QOpcUaExpandedNodeId(const QString &nodeId)
    : data(new QOpcUaExpandedNodeIdData)
{
    data->nodeId = nodeId;
}

/*!
    Constructs an expanded node id from namespace URI \a namespaceUri, node id string \a nodeId
    and server index \a serverIndex.

    \sa setServerIndex
*/
QOpcUaExpandedNodeId::QOpcUaExpandedNodeId(const QString &namespaceUri, const QString &nodeId, quint32 serverIndex)
    : data(new QOpcUaExpandedNodeIdData)
{
    data->namespaceUri = namespaceUri;
    data->nodeId = nodeId;
    data->serverIndex = serverIndex;
}

/*!
    Sets the values from \a rhs in this expanded node id.
*/
QOpcUaExpandedNodeId &QOpcUaExpandedNodeId::operator=(const QOpcUaExpandedNodeId &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

/*!
    Returns \c true if this expanded node id has the same value as \a rhs.
*/
bool QOpcUaExpandedNodeId::operator==(const QOpcUaExpandedNodeId &rhs) const
{
    return data->namespaceUri == rhs.namespaceUri() &&
            QOpcUa::nodeIdEquals(data->nodeId, rhs.nodeId()) &&
            data->serverIndex == rhs.serverIndex();
}

/*!
    Converts this expanded node id to \l QVariant.
*/
QOpcUaExpandedNodeId::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

QOpcUaExpandedNodeId::~QOpcUaExpandedNodeId()
{
}

/*!
    Returns the index of the server containing the node. This index maps to an entry in the server's server table.
    The index of the local server is always \c 0. All remote servers have indexes greater than \c 0.
*/
quint32 QOpcUaExpandedNodeId::serverIndex() const
{
    return data->serverIndex;
}

/*!
    Sets the server index to \a serverIndex.
    The index of the local server is always \c 0. All remote servers have indexes greater than \c 0.
*/
void QOpcUaExpandedNodeId::setServerIndex(quint32 serverIndex)
{
    data->serverIndex = serverIndex;
}

/*!
    Returns the namespace URI of the node id. If this value is specified, the namespace index in
    \l {QOpcUaExpandedNodeId::nodeId} {nodeId} is 0 and must be ignored.
*/
QString QOpcUaExpandedNodeId::namespaceUri() const
{
    return data->namespaceUri;
}

/*!
    Sets the namespace URI to \a namespaceUri.
*/
void QOpcUaExpandedNodeId::setNamespaceUri(const QString &namespaceUri)
{
    data->namespaceUri = namespaceUri;
}

/*!
    Returns the node id. If \l {QOpcUaExpandedNodeId::namespaceUri} {namespaceUri} is specified, the namespace index is invalid.
*/
QString QOpcUaExpandedNodeId::nodeId() const
{
    return data->nodeId;
}

/*!
    Sets the node id to \a nodeId.
*/
void QOpcUaExpandedNodeId::setNodeId(const QString &nodeId)
{
    data->nodeId = nodeId;
}

QT_END_NAMESPACE
