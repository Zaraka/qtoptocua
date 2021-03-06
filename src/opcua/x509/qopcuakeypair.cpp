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

#include "qopcuakeypair.h"
#include "qopcuakeypair_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QOpcUaKeyPair
    \inmodule QtOpcUa
    \since 5.14

    \brief QOpcUaKeyPair handles private and public key pairs.

    This class is currently available as a Technology Preview, and therefore the API
    and functionality provided by the class may be subject to change at any time without
    prior notice.

    It can generate, load and store keys for asymmetric encryption.
    Instances of this class have to be passed to functions which need a key.
*/

/*!
    \enum QOpcUaKeyPair::RsaKeyStrength

    This enum type specifies the strength of a RSA key.

    \value Bits1024
           A key strength of 1024 bits.
    \value Bits2048
           A key strength of 2048 bits.
    \value Bits4096
           A key strength of 4096 bits.
*/

/*!
    \enum QOpcUaKeyPair::KeyType

    This enum type specifies the type of a key.

    \value Rsa
           An RSA key
    \value Empty
           No key is available.
    \value Unknown
           The type of key is not handled.
*/

/*!
    \enum QOpcUaKeyPair::Cipher

    Ciphers for encryption of private keys.

    \value Aes128Cbc
           Encrypting AES128 with CBC
    \value Unencrypted
           The Key will not be encrypted.
*/

/*!
    \fn QOpcUaKeyPair::passphraseNeeded(QString &passphrase, int maximumLength, bool writeOperation)

    This signal is emitted when a private key needs a \a passphrase for encryption or
    decryption.

    \a writeOperation is \c true when the passphrase is needed for exporting a key, and
    is \c false when the passphrase is needed for importing a key.

    \a maximumLength specifies the maximum length in bytes for the passphrase.
    All characters in \a passphrase exceeding this limit will be ignored.

    In case you have use this signal crossing thread boundaries you have to connect it
    with \c Qt::BlockingQueuedConnection.
*/

/*!
    Creates a new empty key pair with \a parent as the parent object.
*/
QOpcUaKeyPair::QOpcUaKeyPair(QObject *parent)
    : QObject(*(new QOpcUaKeyPairPrivate()), parent)
{
}
/*!
    Destroys the key pair.
*/
QOpcUaKeyPair::~QOpcUaKeyPair()
{
}

/*!
    Loads a key from PEM encoded data in \a data.
    Returns \c true on success and \c false otherwise.

    It detects from the PEM header if the data contains a private or
    public key. Loading encrypted keys is possible by connecting a
    function to the signal \c passphraseNeeded for provision of the
    passphrase.
*/
bool QOpcUaKeyPair::loadFromPemData(const QByteArray &data)
{
    Q_D(QOpcUaKeyPair);
    return d->loadFromPemData(data);
}

/*!
    Returns the public key as a byte array.
*/
QByteArray QOpcUaKeyPair::publicKeyToByteArray() const
{
    Q_D(const QOpcUaKeyPair);
    return d->publicKeyToByteArray();
}

/*!
    Returns the type of the current key.
*/
QOpcUaKeyPair::KeyType QOpcUaKeyPair::type() const
{
    Q_D(const QOpcUaKeyPair);
    return d->keyType();
}

/*!
    Returns \c true if the current key contains a private key, otherwise \c false.
*/
bool QOpcUaKeyPair::hasPrivateKey() const
{
    Q_D(const QOpcUaKeyPair);
    return d->hasPrivateKey();
}

/*!
    Returns the PEM encoded private key.
    In case there is no private key, an empty byte array is returned.

    The encryption of the key has to be specified using the parameters
    \a cipher and \a password. In order to store the key unencrypted
    the cipher \c Cipher::Unencrypted has to be used.
*/
QByteArray QOpcUaKeyPair::privateKeyToByteArray(Cipher cipher, const QString &password) const
{
    Q_D(const QOpcUaKeyPair);
    return d->privateKeyToByteArray(cipher, password);
}


/*!
    Generates a new asymmetric RSA key pair.

    The length of the key is specified by \a strength.
*/
void QOpcUaKeyPair::generateRsaKey(QOpcUaKeyPair::RsaKeyStrength strength)
{
    Q_D(QOpcUaKeyPair);
    d->generateRsaKey(strength);
}

QT_END_NAMESPACE
