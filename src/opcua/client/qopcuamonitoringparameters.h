/****************************************************************************
**
** Copyright (C) 2017 basysKom GmbH, opensource@basyskom.com
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

#ifndef QOPCUAMONITORINGPARAMETERS_H
#define QOPCUAMONITORINGPARAMETERS_H

#include <QtOpcUa/qopcuacontentfilterelement.h>
#include <QtOpcUa/qopcuasimpleattributeoperand.h>

#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE

class QOpcUaEventFilterResult;

class QOpcUaMonitoringParametersPrivate;

class Q_OPCUA_EXPORT QOpcUaMonitoringParameters
{
    Q_GADGET

public:

    enum class MonitoringMode {
        Disabled = 0,
        Sampling = 1,
        Reporting = 2
    };

    enum class SubscriptionType {
        Shared,
        Exclusive
    };

    enum class Parameter {
        PublishingEnabled = (1 << 0),
        PublishingInterval = (1 << 1),
        LifetimeCount = (1 << 2),
        MaxKeepAliveCount = (1 << 3),
        MaxNotificationsPerPublish = (1 << 4),
        Priority = (1 << 5),
        SamplingInterval = (1 << 6),
        Filter = (1 << 7),
        QueueSize  = (1 << 8),
        DiscardOldest  = (1 << 9),
        MonitoringMode = (1 << 10)
    };
    Q_ENUM(Parameter)
    Q_DECLARE_FLAGS(Parameters, Parameter)

    // This type and the enums are defined in OPC-UA part 4, 7.12.2
    class DataChangeFilterData;
    class Q_OPCUA_EXPORT DataChangeFilter
    {
    public:
        enum class DataChangeTrigger {
            Status = 0,
            StatusOrValue = 1,
            StatusOrValueOrTimestamp = 2
        };

        enum class DeadbandType {
            None = 0,
            Absolute = 1,
            Percent = 2
        };

        DataChangeFilter();
        DataChangeFilter(const DataChangeFilter &);
        DataChangeFilter(DataChangeTrigger trigger, DeadbandType deadbandType, double deadbandValue);
        DataChangeFilter &operator=(const DataChangeFilter &);
        bool operator==(const DataChangeFilter &rhs) const;
        operator QVariant() const;
        ~DataChangeFilter();

        DataChangeTrigger trigger() const;
        void setTrigger(DataChangeTrigger trigger);

        DeadbandType deadbandType() const;
        void setDeadbandType(DeadbandType deadbandType);

        double deadbandValue() const;
        void setDeadbandValue(double deadbandValue);

    private:
        QSharedDataPointer<DataChangeFilterData> data;
    };

    class EventFilterData;
    class Q_OPCUA_EXPORT EventFilter
    {
    public:
        EventFilter();
        EventFilter(const EventFilter &);
        EventFilter &operator=(const EventFilter &);
        operator QVariant const();
        bool operator==(const QOpcUaMonitoringParameters::EventFilter &rhs) const;
        EventFilter &operator<<(const QOpcUaContentFilterElement &whereClauseElement);
        EventFilter &operator<<(const QOpcUaSimpleAttributeOperand &selectClauseElement);
        ~EventFilter();

        QList<QOpcUaSimpleAttributeOperand> selectClauses() const;
        QList<QOpcUaSimpleAttributeOperand> &selectClausesRef();
        void setSelectClauses(const QList<QOpcUaSimpleAttributeOperand> &selectClauses);

        QList<QOpcUaContentFilterElement> whereClause() const;
        QList<QOpcUaContentFilterElement> &whereClauseRef();
        void setWhereClause(const QList<QOpcUaContentFilterElement> &whereClause);

    private:
        QSharedDataPointer<QOpcUaMonitoringParameters::EventFilterData> data;
    };

    QOpcUaMonitoringParameters();
    ~QOpcUaMonitoringParameters();
    QOpcUaMonitoringParameters(double publishingInterval, SubscriptionType subscriptionType = SubscriptionType::Shared, quint32 subscriptionId = 0);
    QOpcUaMonitoringParameters(const QOpcUaMonitoringParameters &other);
    QOpcUaMonitoringParameters &operator=(const QOpcUaMonitoringParameters &other);

    double samplingInterval() const;
    void setSamplingInterval(double samplingInterval);
    QVariant filter() const;
    void setFilter(const QOpcUaMonitoringParameters::DataChangeFilter &filter);
    void setFilter(const QOpcUaMonitoringParameters::EventFilter &eventFilter);
    void clearFilter();
    QVariant filterResult() const;
    void setFilterResult(const QOpcUaEventFilterResult &eventFilterResult);
    void clearFilterResult();
    quint32 queueSize() const;
    void setQueueSize(quint32 queueSize);
    bool discardOldest() const;
    void setDiscardOldest(bool discardOldest);
    QOpcUaMonitoringParameters::MonitoringMode monitoringMode() const;
    void setMonitoringMode(MonitoringMode monitoringMode);
    quint32 subscriptionId() const;
    void setSubscriptionId(quint32 subscriptionId);
    quint32 monitoredItemId() const;
    void setMonitoredItemId(quint32 monitoredItemId);
    double publishingInterval() const;
    void setPublishingInterval(double publishingInterval);
    quint32 lifetimeCount() const;
    void setLifetimeCount(quint32 lifetimeCount);
    quint32 maxKeepAliveCount() const;
    void setMaxKeepAliveCount(quint32 maxKeepAliveCount);
    quint32 maxNotificationsPerPublish() const;
    void setMaxNotificationsPerPublish(quint32 maxNotificationsPerPublish);
    quint8 priority() const;
    void setPriority(quint8 priority);
    bool isPublishingEnabled() const;
    void setPublishingEnabled(bool publishingEnabled);
    QOpcUa::UaStatusCode statusCode() const;
    void setStatusCode(QOpcUa::UaStatusCode statusCode);
    QOpcUaMonitoringParameters::SubscriptionType subscriptionType() const;
    void setSubscriptionType(SubscriptionType subscriptionType);
    QString indexRange() const;
    void setIndexRange(const QString &indexRange);

private:
    QSharedDataPointer<QOpcUaMonitoringParametersPrivate> d_ptr;
};

Q_DECLARE_TYPEINFO(QOpcUaMonitoringParameters::SubscriptionType, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QOpcUaMonitoringParameters::DataChangeFilter::DataChangeTrigger, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QOpcUaMonitoringParameters::DataChangeFilter::DeadbandType, Q_PRIMITIVE_TYPE);
Q_DECLARE_OPERATORS_FOR_FLAGS(QOpcUaMonitoringParameters::Parameters)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QOpcUaMonitoringParameters)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::SubscriptionType)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::DataChangeFilter)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::DataChangeFilter::DataChangeTrigger)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::DataChangeFilter::DeadbandType)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::Parameter)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::Parameters)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::MonitoringMode)
Q_DECLARE_METATYPE(QOpcUaMonitoringParameters::EventFilter)

#endif // QOPCUAMONITORINGPARAMETERS_H
