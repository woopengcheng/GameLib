using System;
using System.Threading;
using NUnit.Framework;
using UnityEngine;

namespace BehaviorNodeUnitTest
{
    [TestFixture]
    [Category("HTNTest")]
    internal class HTNTravelUnitTest : HTNAgentTravel_0
    {
        [Test]
        [Category("test_travel")]
        public void test_travel() {
            testAgent.btsetcurrent("node_test/htn/travel/root");
            testAgent.resetProperties();
            testAgent.SetStartFinish(HTNAgentTravel.sh_td, HTNAgentTravel.sz_td);
            testAgent.btexec();

            Assert.AreEqual(3, testAgent.Path.Count);
            Assert.AreEqual("ride_taxi", testAgent.Path[0].name);
            Assert.AreEqual(HTNAgentTravel.sh_td, testAgent.Path[0].x);
            Assert.AreEqual(HTNAgentTravel.airport_sh_hongqiao, testAgent.Path[0].y);

            Assert.AreEqual("fly", testAgent.Path[1].name);
            Assert.AreEqual(HTNAgentTravel.airport_sh_hongqiao, testAgent.Path[1].x);
            Assert.AreEqual(HTNAgentTravel.airport_sz_baoan, testAgent.Path[1].y);

            Assert.AreEqual("ride_taxi", testAgent.Path[2].name);
            Assert.AreEqual(HTNAgentTravel.airport_sz_baoan, testAgent.Path[2].x);
            Assert.AreEqual(HTNAgentTravel.sz_td, testAgent.Path[2].y);

            //
            testAgent.resetProperties();
            testAgent.SetStartFinish(HTNAgentTravel.sh_td, HTNAgentTravel.sh_home);
            testAgent.btexec();

            Assert.AreEqual(1, testAgent.Path.Count);
            Assert.AreEqual("ride_taxi", testAgent.Path[0].name);
            Assert.AreEqual(HTNAgentTravel.sh_td, testAgent.Path[0].x);
            Assert.AreEqual(HTNAgentTravel.sh_home, testAgent.Path[0].y);
        }
    }
}