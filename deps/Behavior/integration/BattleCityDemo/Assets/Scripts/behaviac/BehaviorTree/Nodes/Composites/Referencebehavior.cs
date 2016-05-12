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

using System;
using System.Collections.Generic;

namespace behaviac
{
    public class ReferencedBehavior : BehaviorNode
    {
        public ReferencedBehavior()
        {
            this.m_referencedBehaviorPath_var = null;
            this.m_referencedBehaviorPath_m = null;
        }

        //~ReferencedBehavior()
        //{
        //}

        public override bool decompose(BehaviorNode node, PlannerTaskComplex seqTask, int depth, Planner planner)
        {
            ReferencedBehavior taskSubTree = (ReferencedBehavior)node;
            bool bOk = false;
            Debug.Check(taskSubTree != null);
            int depth2 = planner.GetAgent().Variables.Depth;
            using(AgentState currentState = planner.GetAgent().Variables.Push(false))
            {
                //planner.agent.Variables.Log(planner.agent, true);
                taskSubTree.SetTaskParams(planner.GetAgent());

                Task task = taskSubTree.RootTaskNode(planner.GetAgent());

                if (task != null)
                {
                    planner.LogPlanReferenceTreeEnter(planner.GetAgent(), taskSubTree);
                    task.Parent.InstantiatePars(planner.GetAgent());

                    PlannerTask childTask = planner.decomposeNode(task, depth);

                    if (childTask != null)
                    {
                        seqTask.AddChild(childTask);
                        bOk = true;
                    }

                    task.Parent.UnInstantiatePars(planner.GetAgent());
                    planner.LogPlanReferenceTreeExit(planner.GetAgent(), taskSubTree);
                    Debug.Check(true);
                }
            }

            Debug.Check(planner.GetAgent().Variables.Depth == depth2);
            return bOk;
        }

        protected override void load(int version, string agentType, List<property_t> properties)
        {
            base.load(version, agentType, properties);

            for (int i = 0; i < properties.Count; ++i)
            {
                property_t p = properties[i];

                if (p.name == "ReferenceBehavior")
                {
                    int pParenthesis = p.value.IndexOf('(');

                    if (pParenthesis == -1)
                    {
                        string typeName = null;
                        this.m_referencedBehaviorPath_var = Condition.LoadRight(p.value, ref typeName);
                    }
                    else
                    {
                        //method
                        this.m_referencedBehaviorPath_m = Action.LoadMethod(p.value);
                    }

                    string szTreePath = this.GetReferencedTree(null);

                    //conservatively make it true
                    bool bHasEvents = true;
                    if (!string.IsNullOrEmpty(szTreePath))
                    {
                        BehaviorTree behaviorTree = Workspace.Instance.LoadBehaviorTree(szTreePath);
                        Debug.Check(behaviorTree != null);

                        if (behaviorTree != null)
                        {
                            bHasEvents = behaviorTree.HasEvents();
                        }

                        this.m_bHasEvents |= bHasEvents;
                    }
                }
                else if (p.name == "Task")
                {
                    Debug.Check(!string.IsNullOrEmpty(p.value));
                    CMethodBase m = Action.LoadMethod(p.value);
                    Debug.Check(m is CTaskMethod);

                    this.m_taskMethod = m as CTaskMethod;
                }
                else
                {
                    //Debug.Check(0, "unrecognised property %s", p.name);
                }
            }
        }

        public override void Attach(BehaviorNode pAttachment, bool bIsPrecondition, bool bIsEffector, bool bIsTransition)
        {
            if (bIsTransition)
            {
                Debug.Check(!bIsEffector && !bIsPrecondition);

                if (this.m_transitions == null)
                {
                    this.m_transitions = new List<Transition>();
                }

                Transition pTransition = pAttachment as Transition;
                Debug.Check(pTransition != null);
                this.m_transitions.Add(pTransition);

                return;
            }

            Debug.Check(bIsTransition == false);
            base.Attach(pAttachment, bIsPrecondition, bIsEffector, bIsTransition);
        }

        public override bool IsValid(Agent pAgent, BehaviorTask pTask)
        {
            if (!(pTask.GetNode() is ReferencedBehavior))
            {
                return false;
            }

            return base.IsValid(pAgent, pTask);
        }

        protected override BehaviorTask createTask()
        {
            ReferencedBehaviorTask pTask = new ReferencedBehaviorTask();

            return pTask;
        }

        public virtual string GetReferencedTree(Agent pAgent)
        {
            object treePath = null;

            if (this.m_referencedBehaviorPath_var != null)
            {
                treePath = this.m_referencedBehaviorPath_var.GetValue(pAgent);
            }
            else
            {
                Debug.Check(this.m_referencedBehaviorPath_m != null);
                if (this.m_referencedBehaviorPath_m != null)
                {
                    treePath = this.m_referencedBehaviorPath_m.Invoke(pAgent);
                }
            }

            if (treePath != null)
            {
                return Convert.ToString(treePath);
            }

            return string.Empty;
        }

        public void SetTaskParams(Agent pAgent)
        {
            if (this.m_taskMethod != null)
            {
                this.m_taskMethod.SetTaskParams(pAgent);
            }
        }

        protected List<Transition> m_transitions;
        protected Property m_referencedBehaviorPath_var;
        protected CMethodBase m_referencedBehaviorPath_m;
        protected CTaskMethod m_taskMethod;

        private Task m_taskNode;

        public Task RootTaskNode(Agent pAgent)
        {
            if (this.m_taskNode == null)
            {
                string szTreePath = this.GetReferencedTree(pAgent);
                BehaviorTree bt = Workspace.Instance.LoadBehaviorTree(szTreePath);

                if (bt != null && bt.GetChildrenCount() == 1)
                {
                    BehaviorNode root = bt.GetChild(0);
                    this.m_taskNode = root as Task;
                }
            }

            return this.m_taskNode;
        }

        public class ReferencedBehaviorTask : SingeChildTask
        {
            private AgentState currentState;

            public ReferencedBehaviorTask()
            {
            }

            ~ReferencedBehaviorTask()
            {
            }

            public override void Init(BehaviorNode node)
            {
                base.Init(node);
            }

            public override void copyto(BehaviorTask target)
            {
                base.copyto(target);
            }

            public override void save(ISerializableNode node)
            {
                base.save(node);
            }

            public override void load(ISerializableNode node)
            {
                base.load(node);
            }

            protected override bool CheckPreconditions(Agent pAgent, bool bIsAlive)
            {
                this.currentState = pAgent.Variables.Push(false);
                Debug.Check(currentState != null);

                bool bOk = base.CheckPreconditions(pAgent, bIsAlive);

                if (!bOk)
                {
                    this.currentState.Pop();
                    this.currentState = null;
                }

                return bOk;
            }

            int m_nextStateId = -1;
            public override int GetNextStateId()
            {
                return m_nextStateId;
            }

            BehaviorTreeTask m_subTree = null;

            public override bool onevent(Agent pAgent, string eventName)
            {
                if (this.m_status == EBTStatus.BT_RUNNING && this.m_node.HasEvents())
                {
                    Debug.Check(this.m_subTree != null);
                    if (!this.m_subTree.onevent(pAgent, eventName))
                    {
                        return false;
                    }
                }

                return true;
            }

            protected override bool onenter(Agent pAgent)
            {
                ReferencedBehavior pNode = this.GetNode() as ReferencedBehavior;
                Debug.Check(pNode != null);

                this.m_nextStateId = -1;

                string szTreePath = pNode.GetReferencedTree(pAgent);
                this.m_subTree = Workspace.Instance.CreateBehaviorTreeTask(szTreePath);

                pNode.SetTaskParams(pAgent);

                return true;
            }

            protected override void onexit(Agent pAgent, EBTStatus s)
            {
                this.m_subTree = null;
                Debug.Check(this.currentState != null);
                this.currentState.Pop();
                base.onexit(pAgent, s);
            }

            protected override EBTStatus update(Agent pAgent, EBTStatus childStatus)
            {
                ReferencedBehavior pNode = this.GetNode() as ReferencedBehavior;
                Debug.Check(pNode != null);

#if !BEHAVIAC_RELEASE
                pAgent.m_debug_count++;
                if (pAgent.m_debug_count > 20)
                {
                    Debug.LogWarning(string.Format("{0} might be in a recurrsive inter calling of trees\n", pAgent.GetName()));
                    Debug.Check(false);
                }
#endif
                EBTStatus result = this.m_subTree.exec(pAgent);

                bool bTransitioned = State.UpdateTransitions(pAgent, pNode, pNode.m_transitions, ref this.m_nextStateId, result);

                if (bTransitioned)
                {
                    result = EBTStatus.BT_SUCCESS;
                }

                return result;
            }
        }
    }
}
