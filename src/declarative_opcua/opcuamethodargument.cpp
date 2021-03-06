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

#include <private/opcuamethodargument_p.h>

/*!
    \qmltype MethodArgument
    \inqmlmodule QtOpcUa
    \brief Arguments for OpcUa method calls.
    \since QtOpcUa 5.13

    When calling methods which require arguments, this type is used.

    This example shows how to call a method with two double arguments.
    \code
    QtOpcUa.MethodNode {
        ...
        inputArguments: [
            QtOpcUa.MethodArgument {
                value: 3
                type: QtOpcUa.Constants.Double
            },
            QtOpcUa.MethodArgument {
                value: 4
                type: QtOpcUa.Constants.Double
            }
        ]
    }
    \endcode
*/

/*!
    \qmlproperty variant MethodArgument::value

    The value of the argument.
*/

/*!
    \qmlproperty QOpcUa::Types MethodNode::type

    Sets the type of the argument that is expected by the server.
    The value variant is converted to that type when calling the method.
    The type has to match the method on the server exactly, otherwise
    the method call will fail.

    \sa MethodNode::callMethod
*/

OpcUaMethodArgument::OpcUaMethodArgument(QObject *parent) : QObject(parent)
{
}

QVariant OpcUaMethodArgument::value() const
{
    return m_value;
}

QOpcUa::Types OpcUaMethodArgument::type() const
{
    return m_type;
}

void OpcUaMethodArgument::setValue(QVariant value)
{
    m_value = value;
}

void OpcUaMethodArgument::setType(QOpcUa::Types type)
{
    m_type = type;
}
