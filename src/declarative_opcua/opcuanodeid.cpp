/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include <private/opcuanodeid_p.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype NodeId
    \inqmlmodule QtOpcUa
    \brief Specifies a node by an identifier.
    \since QtOpcUa 5.12

    \code
    import QtOpcUa as QtOpcUa

    QtOpcUa.NodeId {
        identifier: "s=Example.Node"
        ns: "Example Namespace"
    }
    \endcode
*/

/*!
    \qmlproperty string NodeId::ns

    Namespace of the node identifier.
    The identifier can be the index as a number or the name as string.
    A string which can be converted to an integer is considered a namespace index.
*/

/*!
    \qmlproperty string NodeId::identifier

    Identifier of the node.
    The identifier has to be given in one of the following types.

    \table
    \header
        \li Type
        \li Example
    \row
        \li Numeric
        \li \c i=23
    \row
        \li String
        \li \c s=MyStringIdentifier
    \row
        \li GUID
        \li \c g=08081e75-8e5e-319b-954f-f3a7613dc29b
    \row
        \li Opaque (base64)
        \li \c b=UXQgZnR3IQ==
    \endtable

    It is possible but not recommended to include the namespace index \c {ns=X;s=...}.
    In this case the given namespace index is internally stripped off the identifier and set
    to the namespace property.
*/

/*!
    \qmlsignal NodeId::nodeChanged()

    Emitted when the underlying node has changed.
    This happens when the namespace or identifier has changed.
*/

OpcUaNodeId::OpcUaNodeId(QObject *parent)
    : OpcUaNodeIdType(parent)
{
    connect(this, &OpcUaNodeIdType::nodeNamespaceChanged, this, &OpcUaNodeId::nodeNamespaceChanged);
    connect(this, &OpcUaNodeIdType::identifierChanged, this, &OpcUaNodeId::identifierChanged);
    connect(this, &OpcUaNodeIdType::nodeNamespaceChanged, this, &OpcUaNodeId::nodeChanged);
    connect(this, &OpcUaNodeIdType::identifierChanged, this, &OpcUaNodeId::nodeChanged);
}

QT_END_NAMESPACE
