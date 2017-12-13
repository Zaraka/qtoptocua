/****************************************************************************
**
** Copyright (C) 2015 basysKom GmbH, opensource@basyskom.com
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qfreeopcuaclient.h"
#include "qfreeopcuanode.h"
#include "qfreeopcuasubscription.h"
#include "qfreeopcuavalueconverter.h"
#include "qfreeopcuaworker.h"

#include <QtNetwork/qhostinfo.h>
#include <QtCore/qloggingcategory.h>

#include <opc/ua/node.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_OPCUA_PLUGINS_FREEOPCUA)

QFreeOpcUaWorker::QFreeOpcUaWorker(QFreeOpcUaClientImpl *client)
    : QOpcUaBackend()
    , m_client(client)
{}

QFreeOpcUaWorker::~QFreeOpcUaWorker()
{
    qDeleteAll(m_subscriptions);
}

void QFreeOpcUaWorker::asyncConnectToEndpoint(const QUrl &url)
{
    try {
        QString sNodeName = QHostInfo::localHostName();
        SetApplicationURI(QString("urn:%1:%2:%3").arg(
                              sNodeName).arg("qt-project").arg("QOpcUaClient").toStdString());
        SetProductURI("urn:qt-project:QOpcUaClient");
        SetSessionName(GetApplicationURI());

        Connect(url.toString().toStdString());

        // Check connection status by getting the root node
        GetRootNode();
    } catch (const std::exception &e) {
        // FreeOPCUA does not expose the error code, the only information is in ex.what()
        const QString errorString = QString::fromUtf8(e.what());
        QOpcUaClient::ClientError error = QOpcUaClient::UnknownError;
        if (errorString.contains(QStringLiteral("0x801f0000")))
            error = QOpcUaClient::AccessDenied;
        try {
            Disconnect();
        } catch (const std::exception &e) {
            qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << "Disconnect failed";
            qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << e.what();
        }
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << "Client could not connect to endpoint" << url;
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << e.what();
        emit stateAndOrErrorChanged(QOpcUaClient::Disconnected, error);
        return;
    }

    emit stateAndOrErrorChanged(QOpcUaClient::Connected, QOpcUaClient::NoError);
}

void QFreeOpcUaWorker::asyncDisconnectFromEndpoint()
{
    try {
        Disconnect();
        emit m_client->stateAndOrErrorChanged(QOpcUaClient::Disconnected, QOpcUaClient::NoError);
        return;
    } catch (const std::exception &ex) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << "Could not disconnect from endpoint: " << ex.what();
    }

    emit m_client->stateAndOrErrorChanged(QOpcUaClient::Disconnected, QOpcUaClient::UnknownError);
}

void QFreeOpcUaWorker::readAttributes(uintptr_t handle, OpcUa::NodeId id, QOpcUaNode::NodeAttributes attr, QString indexRange)
{
    QVector<QOpcUaReadResult> vec;

    try {
        OpcUa::ReadParameters params;
        OpcUa::ReadValueId attribute;
        attribute.NodeId = id;

        qt_forEachAttribute(attr, [&](QOpcUaNode::NodeAttribute attr) {
            attribute.AttributeId = QFreeOpcUaValueConverter::toUaAttributeId(attr);
            attribute.IndexRange = indexRange.toStdString();
            params.AttributesToRead.push_back(attribute);
            QOpcUaReadResult temp;
            temp.attributeId = attr;
            vec.push_back(temp);
        });

        std::vector<OpcUa::DataValue> res = GetRootNode().GetServices()->Attributes()->Read(params);

        for (size_t i = 0; i < res.size(); ++i) {
            vec[i].statusCode = static_cast<QOpcUa::UaStatusCode>(res[i].Status);
            if (res[i].Status == OpcUa::StatusCode::Good) {
                vec[i].value = QFreeOpcUaValueConverter::toQVariant(res[i].Value);
            }
        }

        emit attributesRead(handle, vec, QOpcUa::UaStatusCode::Good);
    } catch(const std::exception &ex) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "Batch read of node attributes failed: %s", ex.what());
        emit attributesRead(handle, vec, QFreeOpcUaValueConverter::exceptionToStatusCode(ex));
    }
}

void QFreeOpcUaWorker::writeAttribute(uintptr_t handle, OpcUa::Node node, QOpcUaNode::NodeAttribute attr, QVariant value, QOpcUa::Types type, QString indexRange)
{
    std::vector<OpcUa::StatusCode> res;

    try {
        if (type == QOpcUa::Types::Undefined && attr != QOpcUaNode::NodeAttribute::Value)
            type = attributeIdToTypeId(attr);
        OpcUa::Variant toWrite = QFreeOpcUaValueConverter::toTypedVariant(value, type);

        OpcUa::WriteValue val;
        val.NodeId = node.GetId();
        val.AttributeId = QFreeOpcUaValueConverter::toUaAttributeId(attr);
        val.Value = OpcUa::DataValue(toWrite);
        val.IndexRange = indexRange.toStdString();
        std::vector<OpcUa::WriteValue> req;
        req.push_back(val);

        res = node.GetServices()->Attributes()->Write(req);

        emit attributeWritten(handle, attr, res[0] == OpcUa::StatusCode::Good ? value : QVariant(), static_cast<QOpcUa::UaStatusCode>(res[0]));
    } catch (const std::exception &ex) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "Could not write value to node: %s: %s", OpcUa::ToString(node.GetId()).c_str(), ex.what());
        emit attributeWritten(handle, attr, QVariant(), QFreeOpcUaValueConverter::exceptionToStatusCode(ex));
    }
}

void QFreeOpcUaWorker::writeAttributes(uintptr_t handle, OpcUa::Node node, QOpcUaNode::AttributeMap toWrite, QOpcUa::Types valueAttributeType)
{
    if (toWrite.size() == 0) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "No values to be written");
        emit attributeWritten(handle, QOpcUaNode::NodeAttribute::None, QVariant(), QOpcUa::UaStatusCode::BadNothingToDo);
        return;
    }

    std::vector<OpcUa::StatusCode> res;

    try {
        std::vector<OpcUa::WriteValue> req;

        for (auto it = toWrite.constBegin(); it != toWrite.constEnd(); ++it) {
            OpcUa::WriteValue val;
            val.NodeId = node.GetId();
            val.AttributeId = QFreeOpcUaValueConverter::toUaAttributeId(it.key());
            QOpcUa::Types type = it.key() == QOpcUaNode::NodeAttribute::Value ? valueAttributeType : attributeIdToTypeId(it.key());
            val.Value = OpcUa::DataValue(QFreeOpcUaValueConverter::toTypedVariant(it.value(), type));
            req.push_back(val);
        }

        res = node.GetServices()->Attributes()->Write(req);

        size_t index = 0;
        for (auto it = toWrite.constBegin(); it != toWrite.constEnd(); ++it, ++index) {
            emit attributeWritten(handle, it.key(), res[index] == OpcUa::StatusCode::Good ? it.value() : QVariant(), static_cast<QOpcUa::UaStatusCode>(res[index]));
        }

    } catch (const std::exception &ex) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "Could not write attributes: %s", ex.what());
        for (auto it = toWrite.constBegin(); it != toWrite.constEnd(); ++it) {
            emit attributeWritten(handle, it.key(), QVariant(), QFreeOpcUaValueConverter::exceptionToStatusCode(ex));
        }
    }
}

QFreeOpcUaSubscription *QFreeOpcUaWorker::getSubscription(const QOpcUaMonitoringParameters &settings)
{
    if (settings.shared() == QOpcUaMonitoringParameters::SubscriptionType::Shared) {
        for (auto entry : qAsConst(m_subscriptions))
            if (entry->interval() == settings.publishingInterval() && entry->shared() == QOpcUaMonitoringParameters::SubscriptionType::Shared)
                return entry;
    }

    QFreeOpcUaSubscription *sub = new QFreeOpcUaSubscription(this, settings);
    quint32 id = sub->createOnServer();
    if (!id) {
        delete sub;
        return nullptr;
    }
    m_subscriptions[id] = sub;
    return sub;
}

bool QFreeOpcUaWorker::removeSubscription(quint32 subscriptionId)
{
    auto sub = m_subscriptions.find(subscriptionId);
    if (sub != m_subscriptions.end()) {
        sub.value()->removeOnServer();
        delete sub.value();
        m_subscriptions.remove(subscriptionId);
        return true;
    }
    return false;
}

void QFreeOpcUaWorker::enableMonitoring(uintptr_t handle, OpcUa::Node node, QOpcUaNode::NodeAttributes attr, const QOpcUaMonitoringParameters &settings)
{
    QFreeOpcUaSubscription *usedSubscription = nullptr;

    // Create a new subscription if necessary
    if (settings.subscriptionId()) {
        if (!m_subscriptions.contains(settings.subscriptionId())) {
            qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "There is no subscription with id %u", settings.subscriptionId());
            qt_forEachAttribute(attr, [&](QOpcUaNode::NodeAttribute attr) {
                QOpcUaMonitoringParameters s;
                s.setStatusCode(QOpcUa::UaStatusCode::BadSubscriptionIdInvalid);
                emit monitoringEnableDisable(handle, attr, true, s);
            });
            return;
        }
        usedSubscription = m_subscriptions[settings.subscriptionId()]; // Ignore interval != subscription.interval
    } else {
        usedSubscription = getSubscription(settings);
    }

    if (!usedSubscription) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "Could not create subscription with interval %f", settings.publishingInterval());
        qt_forEachAttribute(attr, [&](QOpcUaNode::NodeAttribute attr) {
            QOpcUaMonitoringParameters s;
            s.setStatusCode(QOpcUa::UaStatusCode::BadSubscriptionIdInvalid);
            emit monitoringEnableDisable(handle, attr, true, s);
        });
        return;
    }

    qt_forEachAttribute(attr, [&](QOpcUaNode::NodeAttribute attr) {
        bool success = usedSubscription->addAttributeMonitoredItem(handle, attr, node, settings);
        if (success)
            m_attributeMapping[handle][attr] = usedSubscription;
    });

    if (usedSubscription->monitoredItemsCount() == 0)
        removeSubscription(usedSubscription->subscriptionId()); // No items were added
}

void QFreeOpcUaWorker::disableMonitoring(uintptr_t handle, QOpcUaNode::NodeAttributes attr)
{
    qt_forEachAttribute(attr, [&](QOpcUaNode::NodeAttribute attr) {
        QFreeOpcUaSubscription *sub = getSubscriptionForItem(handle, attr);
        if (sub) {
            sub->removeAttributeMonitoredItem(handle, attr);
            if (sub->monitoredItemsCount() == 0)
                removeSubscription(sub->subscriptionId());
        }
    });
}

void QFreeOpcUaWorker::modifyMonitoring(uintptr_t handle, QOpcUaNode::NodeAttribute attr, QOpcUaMonitoringParameters::Parameter item, QVariant value)
{
    QFreeOpcUaSubscription *subscription = getSubscriptionForItem(handle, attr);
    if (!subscription) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA, "Could not modify parameter for %lu, the monitored item does not exist", handle);
        QOpcUaMonitoringParameters p;
        p.setStatusCode(QOpcUa::UaStatusCode::BadSubscriptionIdInvalid);
        emit monitoringStatusChanged(handle, attr, item, p);
        return;
    }

    subscription->modifyMonitoring(handle, attr, item, value);
}

QFreeOpcUaSubscription *QFreeOpcUaWorker::getSubscriptionForItem(uintptr_t handle, QOpcUaNode::NodeAttribute attr)
{
    auto nodeEntry = m_attributeMapping.find(handle);
    if (nodeEntry == m_attributeMapping.end())
        return nullptr;

    auto subscription = nodeEntry->find(attr);
    if (subscription == nodeEntry->end())
        return nullptr;

    return subscription.value();
}

void QFreeOpcUaWorker::callMethod(uintptr_t handle, OpcUa::NodeId objectId, OpcUa::NodeId methodId, QVector<QOpcUa::TypedVariant> args)
{
    try {
        std::vector<OpcUa::Variant> arguments;
        if (args.size()) {
            arguments.reserve(args.size());
            for (const QOpcUa::TypedVariant &v: qAsConst(args))
                arguments.push_back(QFreeOpcUaValueConverter::toTypedVariant(v.first, v.second));
        }
        OpcUa::CallMethodRequest myCallRequest;
        myCallRequest.ObjectId = objectId;
        myCallRequest.MethodId = methodId;
        myCallRequest.InputArguments = arguments;
        std::vector<OpcUa::CallMethodRequest> myCallVector;
        myCallVector.push_back(myCallRequest);

        OpcUa::Node object = this->GetNode(objectId);
        std::vector<OpcUa::Variant> returnedValues = object.CallMethod(methodId, arguments);

        QVariant result;

        if (returnedValues.size() > 1) {
            QVariantList temp;
            for (auto it = returnedValues.cbegin(); it != returnedValues.cend(); ++it)
                temp.append(QFreeOpcUaValueConverter::toQVariant(*it));

            result = temp;
        } else if (returnedValues.size() == 1) {
            result = QFreeOpcUaValueConverter::toQVariant(returnedValues[0]);
        }

        emit methodCallFinished(handle, QFreeOpcUaValueConverter::nodeIdToString(methodId), result, QOpcUa::UaStatusCode::Good);

    } catch (const std::exception &ex) {
        qCWarning(QT_OPCUA_PLUGINS_FREEOPCUA) << "Method call failed: " << ex.what();
        emit methodCallFinished(handle, QFreeOpcUaValueConverter::nodeIdToString(methodId), QVariant(), QFreeOpcUaValueConverter::exceptionToStatusCode(ex));
    }
}

QT_END_NAMESPACE