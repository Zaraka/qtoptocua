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

import QtQuick 2.3
import QtTest 1.0
import QtOpcUa 5.13 as QtOpcUa

Item {
    property string backendName
    property int completedTestCases: 0
    property int availableTestCases: 0
    property bool completed: completedTestCases == availableTestCases
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
        serverDiscovery.discoveryUrl = OPCUA_DISCOVERY_URL;
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": RelativeNodePath"
        when: node1.readyToUse && shouldRun

        function test_nodeRead() {
              tryVerify(function() { return node1.value > 0 });
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: QtOpcUa.NodeId {
                                ns: "Test Namespace"
                                identifier: "s=TestFolder"
                             }
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                        }
                        ]
            }
            id: node1
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": CascadedRelativeNodePath"
        when: node2.readyToUse && shouldRun

        function test_nodeRead() {
              tryVerify(function() { return node2.value > 0 });
        }

        QtOpcUa.RelativeNodeId {
              startNode: QtOpcUa.NodeId {
                            ns: "http://opcfoundation.org/UA/"
                            identifier: "i=85"
                         }
              path: [ QtOpcUa.RelativeNodePath {
                         ns: "Test Namespace"
                         browseName: "TestFolder"
                    }
                    ]
              id: secondLevelNode
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: secondLevelNode
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                        }
                        ]
            }
            id: node2
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": Emitting signals on node changes"
        when: node3.readyToUse && shouldRun

        function test_changeStartNode() {
            node3NamespaceSpy.clear();
            node3NodeIdSpy.clear();
            node3NodeChangedSpy.clear();
            node3NodeIdChangedSpy.clear();
            node3ValueSpy.clear();
            node3PathSpy.clear();
            node3.nodeId.startNode.identifier = "s=TestFolder2";
            node3ValueSpy.wait();

            // value has to be undefined because when node IDs are changed
            // all attributes become undefined before they get the new values.
            if (node3ValueSpy.count < 2)
                node3ValueSpy.wait();
            verify(!node3ValueSpy.signalArguments[0][0]);
            compare(node3ValueSpy.signalArguments[1][0], 0.1);

            compare(node3NodeIdSpy.count, 1);
            compare(node3NamespaceSpy.count, 0);
            verify(node3NodeChangedSpy.count > 0);
            compare(node3PathSpy.count, 0);
            compare(node3NodeIdChangedSpy.count, 1);
            compare(node3ValueSpy.count, 2); // first undefined, then the real value
            compare(node3.value, 0.1);
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: QtOpcUa.NodeId {
                                ns: "Test Namespace"
                                identifier: "s=TestFolder"
                             }
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                        }
                        ]
            }
            id: node3
        }

        SignalSpy {
            id: node3NamespaceSpy
            target: node3.nodeId
            signalName: "nodeNamespaceChanged"
        }

        SignalSpy {
            id: node3NodeIdSpy
            target: node3.nodeId
            signalName: "nodeChanged"
        }

        SignalSpy {
            id: node3NodeChangedSpy
            target: node3
            signalName: "nodeChanged"
        }

        SignalSpy {
            id: node3NodeIdChangedSpy
            target: node3
            signalName: "nodeIdChanged"
        }

        SignalSpy {
            id: node3ValueSpy
            target: node3
            signalName: "valueChanged"
        }

        SignalSpy {
            id: node3PathSpy
            target: node3.nodeId
            signalName: "pathChanged"
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": Emitting signals on cascaded node changes"
        when: node4.readyToUse && shouldRun

        function test_nodeRead() {
            node4NamespaceSpy.clear();
            node4NodeIdSpy.clear();
            node4NodeChangedSpy.clear();
            node4NodeIdChangedSpy.clear();
            node4ValueSpy.clear();
            node4PathSpy.clear();
            node4.nodeId.startNode.path = replacementNode.createObject(parent);
            node4NodeChangedSpy.wait();
            node4ValueSpy.wait();

            // value has to be undefined because when node IDs are changed
            // all attributes become undefined before they get the new values.
            if (node4ValueSpy.count < 2)
                node4ValueSpy.wait();
            verify(!node4ValueSpy.signalArguments[0][0]);
            compare(node4ValueSpy.signalArguments[1][0], 0.1);

            compare(node4NodeIdSpy.count, 2);
            compare(node4NamespaceSpy.count, 0);
            verify(node4NodeChangedSpy.count > 0);
            compare(node4NodeIdChangedSpy.count, 1);
            compare(node4ValueSpy.count, 2); // first undefined, then the real value
            compare(node4.value, 0.1);
            compare(node4PathSpy.count, 2);
        }

        QtOpcUa.RelativeNodeId {
              startNode: QtOpcUa.NodeId {
                            ns: "http://opcfoundation.org/UA/"
                            identifier: "i=85"
                         }
              path: [ QtOpcUa.RelativeNodePath {
                         ns: "Test Namespace"
                         browseName: "TestFolder"
                    }
                    ]
              id: secondLevelNode2
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: secondLevelNode2
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                        }
                        ]
            }
            id: node4
        }

        Component {
            id: replacementNode
            QtOpcUa.RelativeNodePath {
                ns: "Test Namespace";
                browseName: "TestFolder2";
            }
        }

        SignalSpy {
            id: node4NamespaceSpy
            target: node4.nodeId
            signalName: "nodeNamespaceChanged"
        }

        SignalSpy {
            id: node4NodeIdSpy
            target: node4.nodeId
            signalName: "nodeChanged"
        }

        SignalSpy {
            id: node4NodeChangedSpy
            target: node4
            signalName: "nodeChanged"
        }

        SignalSpy {
            id: node4NodeIdChangedSpy
            target: node4
            signalName: "nodeIdChanged"
        }

        SignalSpy {
            id: node4ValueSpy
            target: node4
            signalName: "valueChanged"
        }

        SignalSpy {
            id: node4PathSpy
            target: node4.nodeId.startNode
            signalName: "pathChanged"
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": Relative Node Path with References"
        when: node5.readyToUse && shouldRun

        function test_nodeRead() {
              tryVerify(function() { return node5.value > 0 });
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: QtOpcUa.NodeId {
                                ns: "Test Namespace"
                                identifier: "s=TestFolder"
                             }
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                             referenceType: QtOpcUa.Constants.ReferenceTypeId.References
                        }
                        ]
            }
            id: node5
        }
    }

    CompletionLoggingTestCase {
        name: parent.parent.testName + ": " + backendName + ": Relative Node Path with NodeId reference"
        when: node6.readyToUse && shouldRun

        function test_nodeRead() {
              tryVerify(function() { return node6.value > 0 });
        }

        QtOpcUa.ValueNode {
            connection: connection
            nodeId: QtOpcUa.RelativeNodeId {
                  startNode: QtOpcUa.NodeId {
                                ns: "Test Namespace"
                                identifier: "s=TestFolder"
                             }
                  path: [ QtOpcUa.RelativeNodePath {
                             ns: "Test Namespace"
                             browseName: "TestNode.ReadWrite"
                             referenceType: QtOpcUa.NodeId {
                                  ns: "http://opcfoundation.org/UA/"
                                  identifier: "i=31"
                             }
                        }
                        ]
            }
            id: node6
        }
    }

}
