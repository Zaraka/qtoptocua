/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

void updateEnvironment()
{
#ifdef Q_CC_MSVC
    const QByteArray uaEnvVar("CI_UACPP_"
        #if _MSC_VER >= 1900
            "msvc2017"
        #else
            "msvc2015"
        #endif
            "_"
        #if defined(Q_OS_WIN64)
            "x64"
        #else
            "x86"
        #endif
            "_PREFIX");

    const QString uaPath = qEnvironmentVariable(uaEnvVar.constData());
    if (uaPath.isEmpty() || !QFileInfo(uaPath).exists())
        qWarning() << uaPath << " pointing to a non existing location";

    if (!qputenv("PATH", (uaPath + QLatin1String("\\bin;") + qEnvironmentVariable("PATH")).toLocal8Bit()))
        qWarning() << "Could not update PATH";
#endif
}
