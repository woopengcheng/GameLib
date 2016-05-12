/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using UnityEngine;
using System.Collections;
using NUnit.Framework;

namespace BehaviorNodeUnitTest
{
    [TestFixture]
    [Category("QueryUnitTest")]
    internal class QueryUnitTest
    {
        AgentNodeTest testAgent = null;

        [TestFixtureSetUp]
        public void initGlobalTestEnv() {
            BehaviacSystem.Instance.Init();

            GameObject testAgentObject = new GameObject();
            testAgentObject.name = "@UnitTestAgent";
            testAgentObject.transform.localPosition = Vector3.zero;
            testAgentObject.transform.localRotation = Quaternion.identity;
            testAgentObject.transform.localScale = Vector3.one;
            testAgent = testAgentObject.AddComponent<AgentNodeTest>();
            testAgent.init();
        }

        [TestFixtureTearDown]
        public void finlGlobalTestEnv() {
            testAgent.finl();
            BehaviacSystem.Instance.Uninit();
        }

        [SetUp]
        public void initTestEnv() {
        }

        [TearDown]
        public void finlTestEnv() {
            behaviac.Workspace.Instance.UnLoadAll();
        }

        [Test]
        [Category("test_query_0")]
        public void test_query_0() {
            bool loadResult = testAgent.btload("node_test/query_subtree_0");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_0 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_1");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_1 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_2");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_2 " + "load failed!");
                Assert.Fail();
            }

            testAgent.btsetcurrent("node_test/query_ut_0");
            testAgent.resetProperties();

            behaviac.EBTStatus status = testAgent.btexec();
            //			status = testAgent.btexec();
            //			status = testAgent.btexec();

            Assert.AreEqual(1, testAgent.testVar_0);
            Assert.AreEqual(behaviac.EBTStatus.BT_SUCCESS, status);
        }

        [Test]
        [Category("test_query_1")]
        public void test_query_1() {
            bool loadResult = testAgent.btload("node_test/query_subtree_0");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_0 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_1");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_1 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_2");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_2 " + "load failed!");
                Assert.Fail();
            }

            testAgent.btsetcurrent("node_test/query_ut_1");
            testAgent.resetProperties();

            behaviac.EBTStatus status = testAgent.btexec();
            //			status = testAgent.btexec();
            //			status = testAgent.btexec();
            //			status = testAgent.btexec();

            Assert.AreEqual(2, testAgent.testVar_0);
            Assert.AreEqual(behaviac.EBTStatus.BT_SUCCESS, status);
        }

        [Test]
        [Category("test_query_2")]
        public void test_query_2() {
            bool loadResult = testAgent.btload("node_test/query_subtree_0");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_0 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_1");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_1 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_2");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_2 " + "load failed!");
                Assert.Fail();
            }

            testAgent.btsetcurrent("node_test/query_ut_2");
            testAgent.resetProperties();

            behaviac.EBTStatus status = testAgent.btexec();
            //			status = testAgent.btexec();
            //			status = testAgent.btexec();

            Assert.AreEqual(2, testAgent.testVar_0);
            Assert.AreEqual(behaviac.EBTStatus.BT_SUCCESS, status);
        }

        [Test]
        [Category("test_query_3")]
        public void test_query_3() {
            bool loadResult = testAgent.btload("node_test/query_subtree_0");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_0 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_1");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_1 " + "load failed!");
                Assert.Fail();
            }

            loadResult = testAgent.btload("node_test/query_subtree_2");

            if (!loadResult) {
                Debug.LogError("node_test/query_subtree_2 " + "load failed!");
                Assert.Fail();
            }

            testAgent.btsetcurrent("node_test/query_ut_3");
            testAgent.resetProperties();

            behaviac.EBTStatus status = testAgent.btexec();
            //            status = testAgent.btexec();
            //            status = testAgent.btexec();

            Assert.AreEqual(0, testAgent.testVar_0);
            Assert.AreEqual(behaviac.EBTStatus.BT_SUCCESS, status);
        }
    }
}