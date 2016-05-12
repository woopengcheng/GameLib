using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using NUnit.Framework;
using UnityEngine;

namespace BehaviorNodeUnitTest
{
    [TestFixture]
    [Category("ArrayAccessTest")]
    internal class ArrayAccessTest
    {
        public TestNS.AgentArrayAccessTest testAgent = null;

        [TestFixtureSetUp]
        public void initGlobalTestEnv() {
            BehaviacSystem.Instance.Init();

            GameObject testAgentObject = new GameObject();
            testAgentObject.name = "@UnitTestAgent";
            testAgentObject.transform.localPosition = Vector3.zero;
            testAgentObject.transform.localRotation = Quaternion.identity;
            testAgentObject.transform.localScale = Vector3.one;
            testAgent = testAgentObject.AddComponent<TestNS.AgentArrayAccessTest>();
            testAgent.init();

            //Debug.Log("InitTestFixture");
        }

        [TestFixtureTearDown]
        public void finlGlobalTestEnv() {
            testAgent.finl();

            BehaviacSystem.Instance.Uninit();
            //Debug.Log("FinlTestFixture");
        }

        [SetUp]
        public void initTestEnv() {
        }

        [TearDown]
        public void finlTestEnv() {
            testAgent.btunloadall();
        }

        [Test]
        [Category("test_vector_accessor")]
        public void test_vector_accessor() {
            testAgent.btsetcurrent("par_test/vector_test");
            testAgent.resetProperties();
            testAgent.btexec();

            int Int1 = testAgent.GetVariable<int>("Int");
            Assert.AreEqual(1, Int1);

            int Int2 = testAgent.Int;
            Assert.AreEqual(1, Int2);

            int c_Int = testAgent.GetVariable<int>("c_Int");
            Assert.AreEqual(10, c_Int);

            int Int0 = testAgent.ListInts[0];
            Assert.AreEqual(110, Int0);

            int c_Count = testAgent.GetVariable<int>("c_Count");
            Assert.AreEqual(5, c_Count);

            List<int> c_ListInts = testAgent.GetVariable<List<int>>("c_ListInts");
            Assert.AreEqual(5, c_ListInts.Count);
            Assert.AreEqual(20, c_ListInts[0]);
        }
    }
}