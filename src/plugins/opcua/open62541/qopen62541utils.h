/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#ifndef QOPEN62541UTILS_H
#define QOPEN62541UTILS_H

#include "qopen62541.h"

#include <QString>

#include <functional>

QT_BEGIN_NAMESPACE

template <typename T>
class UaDeleter
{
public:
    UaDeleter(T *data, std::function<void(T *value)> f)
        : m_data(data)
        , m_function(f)
    {
    }
    ~UaDeleter()
    {
        if (m_data)
            m_function(m_data);
    }
    void release()
    {
        m_data = nullptr;
        m_function = nullptr;
    }
private:
    T *m_data {nullptr};
    std::function<void(T *attribute)> m_function;
};

template <uint TYPEINDEX>
class UaArrayDeleter
{
public:
    UaArrayDeleter(void *data, size_t arrayLength)
        : m_data(data)
        , m_arrayLength(arrayLength)
    {
        static_assert (TYPEINDEX < UA_TYPES_COUNT, "Invalid index outside the UA_TYPES array.");
    }
    ~UaArrayDeleter()
    {
        if (m_data && m_arrayLength > 0)
            UA_Array_delete(m_data, m_arrayLength, &UA_TYPES[TYPEINDEX]);
    }
    void release() {
        m_data = nullptr;
        m_arrayLength = 0;
    }
private:
    void *m_data {nullptr};
    size_t m_arrayLength {0};
};

namespace Open62541Utils {
    UA_NodeId nodeIdFromQString(const QString &name);
    QString nodeIdToQString(UA_NodeId id);
}

QT_END_NAMESPACE

#endif // QOPEN62541UTILS_H
