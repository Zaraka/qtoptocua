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

import QtQuick
import QtTest
import QtOpcUa as QtOpcUa

Item {
    property string backendName
    property int completedTestCases: 0
    property int availableTestCases: 0
    property bool completed: completedTestCases == availableTestCases && availableTestCases > 0
    property bool shouldRun: false

    onShouldRunChanged: {
        if (shouldRun)
            console.log("Running", parent.testName, "with", backendName);
    }

    QtOpcUa.Connection {
        id: connection
        backend: backendName
        defaultConnection: true
    }

    QtOpcUa.ServerDiscovery {
        id: serverDiscovery
        onServersChanged: {
            if (!count)
                return;
            endpointDiscovery.serverUrl = at(0).discoveryUrls[0];
        }

        Binding on discoveryUrl {
            when: shouldRun && Component.completed
            value: OPCUA_DISCOVERY_URL
        }
    }

    QtOpcUa.EndpointDiscovery {
        id: endpointDiscovery
        onEndpointsChanged: {
            if (!count)
                return;
            connection.connectToEndpoint(at(0));
        }
    }

    Component.onCompleted: {
        for (var i in children) {
            if (children[i].objectName == "TestCase")
                availableTestCases += 1;
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": Create String Node Id"
        when: node1.readyToUse && shouldRun

        function test_nodeTest() {
            tryCompare(node1, "monitored", true);
            compare(node1.publishingInterval, 100.0);
            node1ValueSpy.clear();
            verify(node1.value != "foo");
            node2.value = "foo";
            node1ValueSpy.wait(10000);
            compare(node1ValueSpy.count, 1);

            compare(node1.monitored, true);
            node1MonitoredSpy.clear();
            node1.monitored = false;
            node1MonitoredSpy.wait();
            compare(node1MonitoredSpy.count, 1);
            compare(node1IntervalSpy.count, 0);
            compare(node1.monitored, false);

            node1ValueSpy.clear();
            node2.value = "bar";
            timer.running = true;
            timerSpy.wait();
            compare(node1ValueSpy.count, 0);

            node1MonitoredSpy.clear();
            node1.monitored = true;
            node1MonitoredSpy.wait();
            compare(node1MonitoredSpy.count, 1);
            compare(node1IntervalSpy.count, 0);
            compare(node1.monitored, true);

            // Wait for values to settle
            tryCompare(node1, "value", "bar");

            // The value needs to be reset to "Value" for follow up tests to succeed.
            node1ValueSpy.clear();
            node2.value = "Value";
            node1ValueSpy.wait();
            compare(node1ValueSpy.count, 1);

            node1MonitoredSpy.clear();
            node1IntervalSpy.clear();
            node1.publishingInterval = 200.0;
            node1IntervalSpy.wait();
            compare(node1MonitoredSpy.count, 0);
            compare(node1IntervalSpy.count, 1);
            compare(node1.publishingInterval, 200.0);
        }

        SignalSpy {
            id: node1ValueSpy
            target: node1
            signalName: "valueChanged"
        }

        SignalSpy {
            id: node1IntervalSpy
            target: node1
            signalName: "publishingIntervalChanged"
        }

        SignalSpy {
            id: node1MonitoredSpy
            target: node1
            signalName: "monitoredChanged"
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.NodeId {
                ns: "Test Namespace"
                identifier: "s=theStringId"
            }
            id: node1
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.NodeId {
                ns: "Test Namespace"
                identifier: "s=theStringId"
            }
            id: node2
        }

        Timer {
            id: timer
            interval: 2000
        }

        SignalSpy {
            id: timerSpy
            target: timer
            signalName: "triggered"
        }
    }
}
