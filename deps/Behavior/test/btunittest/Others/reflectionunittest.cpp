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

#include "behaviac/base/base.h"
#include "behaviac/base/dynamictype.h"
#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/object/typehandler.h"
#include "behaviac/agent/agent.h"
#include "behaviac/base/object/member.h"

#include "behaviac/base/object/uitype.h"
#include "behaviac/base/object/containeruitype.h"
#include "behaviac/base/object/offsetmember.h"
#include "behaviac/base/object/containertypehandler.h"
#include "behaviac/base/object/containertypehandlerex.h"
#include "behaviac/base/object/enummember.h"

#include "behaviac/agent/registermacros.h"
#include "test.h"

//struct TypeTest2_t
//{
//    int			name;
//    float		weight;
//    bool		bLive;
//
//    TypeTest2_t() : name(10), weight(5.0f), bLive(true)
//    {}
//
//    DECLARE_BEHAVIAC_STRUCT(TypeTest2_t);
//};
//
//class ObjectTest : public CTagObject
//{
//public:
//	DECLARE_BEHAVIAC_TAG_OBJECT(ObjectTest, CTagObject);
//
//    ObjectTest() : Property1(10), Property2(true), Property3(10.0f)
//    {}
//
//    struct Param2_t
//    {
//        TypeTest2_t		type;
//        int				color;
//        const char*		id;
//
//        Param2_t() : color(0), id(0)
//        {}
//
//        DECLARE_BEHAVIAC_STRUCT(Param2_t);
//    };
//
//    void method0()
//    {
//    }
//
//    void method1(int count)
//    {
//        BEHAVIAC_UNUSED_VAR(count);
//    }
//
//    void method2(int param, float param1)
//    {
//        BEHAVIAC_UNUSED_VAR(param);
//        BEHAVIAC_UNUSED_VAR(param1);
//    }
//
//    void method3(Param2_t* param, const Param2_t& param1, Param2_t& param2)
//    {
//        BEHAVIAC_UNUSED_VAR(param);
//        BEHAVIAC_UNUSED_VAR(param1);
//        BEHAVIAC_UNUSED_VAR(param2);
//    }
//
//    void method4(int& param, const float param1, const Param2_t* param2)
//    {
//        BEHAVIAC_UNUSED_VAR(param);
//        BEHAVIAC_UNUSED_VAR(param1);
//        BEHAVIAC_UNUSED_VAR(param2);
//    }
//
//    int		Property1;
//    bool	Property2;
//    float	Property3;
//
//    Param2_t Property4;
//    Param2_t Property5;
//
//private:
//    void PostLoad()
//    {
//        BEHAVIAC_LOGINFO("PostLoad\n");
//    }
//
//    void PreSave()
//    {
//        BEHAVIAC_LOGINFO("PreSave\n");
//    }
//
//    void PostSave()
//    {
//        BEHAVIAC_LOGINFO("PostSave\n");
//    }
//};
//
//BEGIN_PROPERTIES_DESCRIPTION(TypeTest2_t)
//{
//    REGISTER_MEMBER("name", name, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("weight", weight, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("bLive", bLive, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//}
//END_PROPERTIES_DESCRIPTION()
//
//BEGIN_PROPERTIES_DESCRIPTION(ObjectTest::Param2_t)
//{
//    REGISTER_MEMBER("type", type, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("color", color, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("id", id, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//}
//END_PROPERTIES_DESCRIPTION()
//
////namespace StringUtils
////{
////    inline bool FromString(const char* str, TypeTest2_t& val)
////    {
////        return false;
////    }
////
////    inline bool FromString(const char* str, ObjectTest::Param2_t& val)
////    {
////        return false;
////    }
////
////    inline behaviac::string ToString(const TypeTest2_t& val)
////    {
////        return "";
////    }
////
////    inline behaviac::string ToString(const ObjectTest::Param2_t& val)
////    {
////        return "";
////    }
////
////}
//
//BEGIN_PROPERTIES_DESCRIPTION(ObjectTest)
//{
//    REGISTER_SERIALIZATION_EVENT(PreSave, EPersistenceType_Description_Save);
//    BEGIN_GROUP_MEMBER("Properties", EPersistenceType_Description);
//    {
//        REGISTER_MEMBER("Property1", Property1, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//        REGISTER_MEMBER("Property2", Property2, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//        REGISTER_MEMBER("Property3", Property3, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//        REGISTER_MEMBER("Property4", Property4, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//        REGISTER_MEMBER("Property5", Property5, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    }
//    END_GROUP_MEMBER();
//    REGISTER_SERIALIZATION_EVENT(PostSave, EPersistenceType_Description_Save);
//    REGISTER_SERIALIZATION_EVENT(PostLoad, EPersistenceType_Description_Load);
//
//    BEGIN_REGISTER_METHOD("method0", method0)
//    END_REGISTER_METHOD()
//
//    BEGIN_REGISTER_METHOD("method1", method1)
//    END_REGISTER_METHOD()
//
//    BEGIN_REGISTER_METHOD("method2", method2)
//    END_REGISTER_METHOD()
//
//    BEGIN_REGISTER_METHOD("method3", method3)
//    END_REGISTER_METHOD()
//
//    BEGIN_REGISTER_METHOD("method4", method4)
//    END_REGISTER_METHOD()
//}
//END_PROPERTIES_DESCRIPTION()
//
//class CGrassMorphData : public CTagObject
//{
//    //-----------------------------------------
//    // type definition
//    //-----------------------------------------
//public:
//    struct SMorph
//    {
//        DECLARE_BEHAVIAC_STRUCT(SMorph);
//
//        //behaviac::CStringID m_name;
//        behaviac::string	m_name;
//        behaviac::CPathID	m_grassFaction[4];
//    };
//
//    struct SMorphResIDContainer
//    {
//        SMorphResIDContainer()
//        {
//            memset(m_collectionResID, 0, sizeof(m_collectionResID));
//        }
//
//        uint64_t	m_collectionResID[4];
//    };
//
//    //-----------------------------------------
//    // operations
//    //-----------------------------------------
//public:
//	DECLARE_BEHAVIAC_TAG_OBJECT(CGrassMorphData, CTagObject);
//
//    static CTagObject* CreateObject()
//    {
//        return BEHAVIAC_NEW CGrassMorphData;
//    }
//    static const char* ms_typeName;
//    static behaviac::CStringID ms_typeId;
//    virtual void OnChange();
//
//    CGrassMorphData();
//    virtual ~CGrassMorphData();
//
//    void Reset();
//
//    //given a pathid, we found which faction it is, and what's the corresponding pathid of the target faction
//    void GetCorrespondingFactionResource(const behaviac::CPathID& currentPath, int32_t targetFaction, int32_t& currentFaction, behaviac::CPathID& targetPath);
//    int32_t GetFaction(const behaviac::CPathID& path);
//
//    //-----------------------------------------
//    // attributes
//    //-----------------------------------------
//public:
//    behaviac::string											m_name;
//    behaviac::vector< SMorph >									m_morphData;
//    behaviac::vector< SMorphResIDContainer >					m_morphResIDList;
//    float													m_morphForce;
//
//    void                    GetSavedValues(behaviac::vector<int>& values) const
//    {
//        BEHAVIAC_UNUSED_VAR(values);
//    }
//    void                    SetSavedValues(const behaviac::vector<int>& values)
//    {
//        BEHAVIAC_UNUSED_VAR(values);
//    }
//};
//
////------------------------------------------
//// CGrassMorphData Impl
////------------------------------------------
//template <> class CEmptyPredicate< CGrassMorphData, behaviac::vector<CGrassMorphData::SMorph>, CGrassMorphData::SMorph >
//{
//public:
//    static bool IsEmpty(const CGrassMorphData::SMorph& morphInfo)
//    {
//        return morphInfo.m_name.empty();
//    }
//};
//
//BEGIN_PROPERTIES_DESCRIPTION(CGrassMorphData::SMorph)
//{
//    REGISTER_MEMBER("Name", m_name, EPersistenceType_Description_UiInfo, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_OFFSET_MEMBER("fileSylvanGrass", m_grassFaction, 0, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_OFFSET_MEMBER("fileHavenGrass", m_grassFaction, 1, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_OFFSET_MEMBER("fileNecroGrass", m_grassFaction, 2, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_OFFSET_MEMBER("fileInfernoGrass", m_grassFaction, 3, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//}
//END_PROPERTIES_DESCRIPTION()
//
//BEGIN_PROPERTIES_DESCRIPTION(CGrassMorphData)
//{
//    REGISTER_CONTAINER_MEMBER_EX("Morph_contain", "Morph_elem", "", m_morphData, EPersistenceType_Description_UiInfo, TagEmptyEntryVectorProvider, DefaultContainedTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("fMorphForce", m_morphForce, EPersistenceType_Description, DefaultTypeHandler, DefaultUiInfo);
//    //REGISTER_VIRTUAL_MEMBER_REF("Values", SetSavedValues, GetSavedValues, EPersistenceType_State, DefaultTypeHandler, DefaultUiInfo);
//}
//END_PROPERTIES_DESCRIPTION()
//
//#if BEHAVIAC_COMPILER_MSVC
//struct SAttachement
//{
//    DECLARE_BEHAVIAC_STRUCT(SAttachement);
//    SAttachement(
//        int _boneIndex = -1,
//        const bool _freezeRotation = false)
//        : boneIndex(_boneIndex)
//        , freezeRotation(_freezeRotation)
//    {}
//
//    bool GetFreezeRotation() const
//    {
//        return freezeRotation;
//    }
//
//    int boneIndex;
//    bool freezeRotation;
//};
//
//BEGIN_PROPERTIES_DESCRIPTION(SAttachement)
//{
//    REGISTER_MEMBER("BoneIndex", boneIndex, EPersistenceType_State, DefaultTypeHandler, DefaultUiInfo);
//    //REGISTER_VIRTUAL_MEMBER("Translation", Translation, EPersistenceType_State, DefaultTypeHandler, DefaultUiInfo);
//    //REGISTER_VIRTUAL_MEMBER_INTRINSIC_GET_COPY("Rotation", Rotation, EPersistenceType_State, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_MEMBER("FreezeRotation", freezeRotation, EPersistenceType_State, DefaultTypeHandler, DefaultUiInfo);
//}
//END_PROPERTIES_DESCRIPTION()
//
//typedef behaviac::map<behaviac::CStringID, SAttachement> MapAttachements;
//
//enum EVisibleAIObjectType
//{
//    VISIBLEOBJECT_TYPE_UNDEFINED,
//    VISIBLEOBJECT_TYPE_GRENADE,
//    VISIBLEOBJECT_TYPE_MOLOTOV,
//    VISIBLEOBJECT_TYPE_ROCKET,
//    VISIBLEOBJECT_TYPE_MORTARSHELL,
//    VISIBLEOBJECT_TYPE_IED,
//    VISIBLEOBJECT_TYPE_STD_PROPANE_TANK,
//    VISIBLEOBJECT_TYPE_THIN_PROPANE_TANK,
//    VISIBLEOBJECT_TYPE_NOT_VISIBLE,
//};
//BEHAVIAC_OVERRIDE_TYPE_NAME(EVisibleAIObjectType);
//
//class CMoveModel
//{
//    DECLARE_BEHAVIAC_STRUCT(CMoveModel);
//
//    CMoveModel() : m_entityObjectType(VISIBLEOBJECT_TYPE_UNDEFINED)
//    {}
//
//    MapAttachements& GetAttachements()
//    {
//        return m_attachements;
//    }
//
//    MapAttachements m_attachements;
//
//    EVisibleAIObjectType m_entityObjectType;
//
//    behaviac::string m_aiObject;
//
//    void CreateAIObject()
//    {}
//
//    void RegisterAIObjectResources()
//    {}
//};
//
////namespace StringUtils
////{
////    inline bool FromString(const char* str, MapAttachements& val)
////    {
////        return false;
////    }
////
////    inline behaviac::string ToString(const MapAttachements& val)
////    {
////        return "";
////    }
////
////    inline bool FromString(const char* str, behaviac::vector<CGrassMorphData::SMorph>& val)
////    {
////        return false;
////    }
////
////    inline behaviac::string ToString(const behaviac::vector<CGrassMorphData::SMorph>& val)
////    {
////        return "";
////    }
////}
////
//
//BEGIN_PROPERTIES_DESCRIPTION(CMoveModel)
//{
//    REGISTER_CONTAINER_MEMBER_BYID("MapAttachements", "Attachement", "Attachement", "AttachementName", m_attachements, EPersistenceType_State, CreateChildNode, TagMapProvider, DefaultTypeHandler, DefaultUiInfo);
//
//    BEGIN_ENUM_MEMBER("VisibleObjectType", m_entityObjectType, EPersistenceType_Description);
//    REGISTER_ENUM_VALUE("Undefined");
//    REGISTER_ENUM_VALUE("Grenade");
//    REGISTER_ENUM_VALUE("Molotov");
//    REGISTER_ENUM_VALUE("Rocket");
//    REGISTER_ENUM_VALUE("MortarShell");
//    REGISTER_ENUM_VALUE("IED");
//    REGISTER_ENUM_VALUE("StandardPropaneTank");
//    REGISTER_ENUM_VALUE("ThinPropaneTank");
//    REGISTER_ENUM_VALUE("NotVisible");
//    END_ENUM_MEMBER();
//
//    REGISTER_SERIALIZATION_EVENT(CreateAIObject, EPersistenceType_Description_Load);
//    REGISTER_MEMBER("AIObject", m_aiObject, EPersistenceType_All_UiInfo, DefaultTypeHandler, DefaultUiInfo);
//    REGISTER_SERIALIZATION_EVENT(RegisterAIObjectResources, EPersistenceType_Description_Load);
//}
//END_PROPERTIES_DESCRIPTION()
//
//class CSequenceManager
//{
//    typedef uint64_t ScriptCallbackId;
//public:
//    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSequenceManager);
//
//public:
//    DECLARE_BEHAVIAC_STRUCT(CSequenceManager);
//
//public:
//    void CleanUp();
//    void Update(float dt);
//
//    ScriptCallbackId CreateListener(const char* sequence, float time)
//    {
//        return 0;
//    }
//    void DeleteListener(ScriptCallbackId listenerId)
//    {}
//
//    static int TestFunction(const char* str)
//    {
//        BEHAVIAC_LOGINFO(str);
//        return 0;
//    }
//
//private:
//    CSequenceManager()
//    {}
//
//    typedef behaviac::vector<int> ListenerArray;
//    ListenerArray m_listeners;
//
//    BEHAVIAC_DELCARE_SINGLETON(CSequenceManager);
//};
//
//BEHAVIAC_IMPLEMNT_SINGLETON(CSequenceManager);
//
//BEGIN_PROPERTIES_DESCRIPTION(CSequenceManager)
//{
//    //
//}
//END_PROPERTIES_DESCRIPTION()
//
//template< typename T >
//class CSceneObjectHandle
//{
//    template< typename U >
//    friend class CSceneObjectContainer;
//
//    DECLARE_BEHAVIAC_STRUCT(CSceneObjectHandle)
//    // construction
//public:
//    CSceneObjectHandle();
//
//    // functions
//public:
//    T&          ModifyOriginal() const;
//    T&          ModifyOriginalNoCopy() const;
//    const T&    ReadOriginal() const;
//    const T&    ReadCopy() const;
//    bool		IsValid() const;
//    void		PrefetchCopy() const;
//    uint32_t       GetIndex() const
//    {
//        return m_index;
//    }
//
//    // variables
//private:
//    uint32_t m_index;
//    uint32_t m_generation;
//};
//
//BEGIN_TEMPLATE1_PROPERTIES_DESCRIPTION(CSceneObjectHandle)
//{
//    //
//}
//END_PROPERTIES_DESCRIPTION()
//
//class CSceneObject
//{
//    // construction
//public:
//    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSceneObject)
//    DECLARE_BEHAVIAC_STRUCT(CSceneObject);
//    CSceneObject();
//};
//
//BEGIN_PROPERTIES_DESCRIPTION(CSceneObject)
//{
//    //
//}
//END_PROPERTIES_DESCRIPTION()
//
//template< typename T >
//class CSceneObjectComponent : public CTagObject
//{
//public:
//    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSceneObjectComponent<T>)
//    BEHAVIAC_DECLARE_TEMPLATE_DYNAMIC_TYPE1(CSceneObjectComponent, T, CTagObject);
//    DECLARE_BEHAVIAC_OBJECT_SIMPLE(CSceneObjectComponent, CTagObject);
//
//    CSceneObjectComponent();
//    virtual ~CSceneObjectComponent();
//
//    static CTagObject* CreateObject(const behaviac::CStringID&)
//    {
//        return BEHAVIAC_NEW CSceneObjectComponent();
//    }
//
//    virtual bool FinalizeLoad();
//
//    CSceneObjectHandle< T > GetSceneObjectHandle() const;
//
//private:
//    void CreateObjectHandle()
//    {}
//
//    CSceneObjectHandle< T > m_handle;
//};
//
//BEGIN_TEMPLATE1_PROPERTIES_DESCRIPTION(CSceneObjectComponent)
//{
//    REGISTER_SERIALIZATION_EVENT(CreateObjectHandle, EPersistenceType_Description_Load)
//    //REGISTER_MEMBER("Handle", m_handle, EPersistenceType_Description, NoChildTypeHandler, DefaultUiInfo)
//}
//END_PROPERTIES_DESCRIPTION()
//
//TEST(ReflectionTest, Register)
//{
//    ObjectTest::RegisterProperties();
//    CGrassMorphData::RegisterProperties();
//    SAttachement::RegisterProperties();
//    CMoveModel::RegisterProperties();
//    CSceneObjectComponent<CSceneObject>::RegisterProperties();
//
//    //after CScriptSystem Init
//    CSequenceManager::RegisterProperties();
//    CSequenceManager::CreateInstance();
//
//    CSequenceManager::DestroyInstance();
//}
//#endif//#if BEHAVIAC_COMPILER_MSVC
