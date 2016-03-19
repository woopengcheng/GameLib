#-*- coding: utf-8 -*-
import sys , getopt
import os , re , string
import time , datetime
import binascii 
import collections  
import xml.dom.minidom
import time,datetime
import socket  

from xml.dom import minidom , Node 

from xml.etree.ElementTree import ElementTree
from xml.etree.ElementTree import Element
from xml.etree.ElementTree import SubElement as SE
#sys.reload()
#sys.setdefaultencoding("utf-8") 
#sys.setdefaultencoding("cp936") 
 
g_gameDBPath = "DBDIDL.xml"

#rpc的类型
g_targetTypeClient = 1
g_targetTypeProxy  = 2
g_targetTypeServer = 3
g_paramTypeSTL = '0' #STL类型
g_paramTypeSystem = '1' #系统类型
g_paramTypeStruct = '2' #自定义类型
g_tableMeta = "_T"
################################类定义#####################################
class ParentPoint:
	def __init__(self, parentPoint): 
		#self.parentPoint = parentPoint  
		pass
 
class GameDB(ParentPoint):
	def __init__(self): 
		super(GameDB , self).__init__(self)
		self.defaultParams = collections.OrderedDict()
		self.specialDefaultParams = collections.OrderedDict()
		self.defaultParamsList = collections.OrderedDict()
		self.tables = Tables(self)
		self.config = Config(self)

class Config(ParentPoint): 
	def __init__(self, parentPoint):
		super(Config, self).__init__(parentPoint)
		self.outputPath = None

class DefaultParam(ParentPoint):
	def __init__(self, parentPoint): 
		super(DefaultParam , self).__init__(parentPoint) 
		self.type = None
		self.value = None
		self.paramType = None

class SpecialDefaultParam(ParentPoint):
	def __init__(self, parentPoint): 
		super(SpecialDefaultParam , self).__init__(parentPoint) 
		self.type = None
		self.prefix = None
		self.suffix = None
		self.specialType = None
		
class Tables(ParentPoint):
	def __init__(self , parentPoint):
		super(Tables , self).__init__(parentPoint)  
		self.tables = collections.OrderedDict()
		self.structs = collections.OrderedDict() 
		self.collectionTables = collections.OrderedDict() 

class CollectionTable(ParentPoint):
	def __init__(self , parentPoint):
		super(CollectionTable , self).__init__(parentPoint)  
		self.name = None
		self.slaves = collections.OrderedDict() 
		self.slaveTables = collections.OrderedDict() 
		
class Table(ParentPoint):
	def __init__(self , parentPoint):
		super(Table , self).__init__(parentPoint)  
		self.name = None
		self.slaveFrom = None
		self.slaveTable = None
		self.ignore_reflection_define = None	
		self.ignore_collection = None
		self.fields = collections.OrderedDict() 
		
class Field(ParentPoint):
	def __init__(self , parentPoint):
		super(Field , self).__init__(parentPoint)  
		self.name = None
		self.type = None
		self.primaryKey = None
		self.masterID = None
		self.oldName = None
		self.autoIncr =  None
		self.bitmask = None
		self.count = None
		self.value_type = None
		
class Struct(ParentPoint):
	def __init__(self , parentPoint):
		super(Struct , self).__init__(parentPoint)
		self.name = None
		self.params = collections.OrderedDict()   #这里存放所有的params.用dic结构

class Param(ParentPoint):
	def __init__(self , parentPoint):
		super(Param , self).__init__(parentPoint)
		self.name = None
		self.type = None
		self.default = None
		self.refer = None
		self.unrefer = None

################################函数解析XML内容#####################################

g_gameDB=GameDB() 
g_rpcRecords = collections.OrderedDict()   #记录所以得RPC

def start(): 
	ParseGameDB() 
	CheckAllData() 
	LogOutInfo("ParseGameDB finished.\n") 
	
	SortGameDB(g_gameDB)
	
	GenerateGameDB()  
	LogOutInfo("generate Table finished.\n") 
	
noneDir = collections.OrderedDict()
def SortGameDB(sortObj):
	for name , value in vars(sortObj).items():   
		if isinstance(value , dict): 
			i = 0
			for key , val in value.items(): 
				i = i + 1
				if IsSortNext(val): 
					SortGameDB(val)
				else:
					sorted(value.items() , key=lambda e:e[i] , reverse=True)  
					break 

		elif IsSortNext(value): 
			SortGameDB(value)

def IsSortNext(value): 
	if isinstance(value , Config) or \
		isinstance(value , GameDB) or \
		isinstance(value , DefaultParam) or \
		isinstance(value , Tables) or \
		isinstance(value , Struct) or \
		isinstance(value , Param) or \
		isinstance(value , Field) or \
		isinstance(value , SpecialDefaultParam) or \
		isinstance(value , CollectionTable) or \
		isinstance(value , Table): 

		return True

	return False 

def ParseGameDB():  
#使用ElementTree读取节点
	rpcDoc = ElementTree(None , g_gameDBPath)  
	xmlGameDB = rpcDoc.getroot() 
	handleRpcMsgs(xmlGameDB) 

def handleRpcMsgs(xmlGameDB):
	for xmlData in iter(xmlGameDB.getchildren()): 
		if xmlData.tag.lower() == "DefaultParams".lower():
			handleDefaultParams(g_gameDB.defaultParams , g_gameDB.defaultParamsList , xmlData)
		if xmlData.tag.lower() == "LongDefaultParams".lower():
			handleLongDefaultParams(g_gameDB.defaultParams , g_gameDB.defaultParamsList , xmlData)
		if xmlData.tag.lower() == "SpecialDefaultParam".lower():
			handleSpecialDefaultParams(g_gameDB.specialDefaultParams , xmlData)
		if xmlData.tag.lower() == "Tables".lower():
			handleTables(g_gameDB.tables , xmlData)
		if xmlData.tag.lower() == "Config".lower():
			handleConfig(g_gameDB.config , xmlData)
			
def handleDefaultParams(defaultParams , defaultParamsList, xmlDefaultParams):
	for attr in iter(xmlDefaultParams.attrib):
		defaultParam = DefaultParam(defaultParams)
		defaultParam.type = attr
		defaultParam.value = xmlDefaultParams.attrib[attr]
		
		if IsSystemDefineType(defaultParam.type):
			defaultParam.paramType = g_paramTypeSystem
		elif GetSpecialDefaultParamType(defaultParam.type) == g_paramTypeSTL:
			defaultParam.paramType = g_paramTypeSTL
		else:
			defaultParam.paramType = g_paramTypeStruct
			
		if IsHasSameData(defaultParams , defaultParam.type):
			LogOutError("Parase defaultParams :" , defaultParam.type , "has same defaultParam.") 
			
		defaultParams[attr] = defaultParam
		defaultParamsList[attr] = "g_rpcDefaultParam_" + attr

def handleLongDefaultParams(defaultParams , defaultParamsList, xmlDefaultParams):
	defaultParam = DefaultParam(defaultParams)
	for attr in iter(xmlDefaultParams.attrib):
		if attr.lower() == "type".lower():
			defaultParam.type = ReplaceSpecialCharForContainerFromType(xmlDefaultParams.attrib[attr]) 
		if attr.lower() == "default".lower(): 
			defaultParam.value = ReplaceSpecialCharForContainerFromType(xmlDefaultParams.attrib[attr])
	
	if IsHasSameData(defaultParams , defaultParam.type):
		LogOutError("Parase defaultParam :" , defaultParam.type , "has same defaultParam.") 

	if IsSystemDefineType(defaultParam.type):
		defaultParam.paramType = g_paramTypeSystem
	elif GetSpecialDefaultParamType(defaultParam.type) == g_paramTypeSTL:
		defaultParam.paramType = g_paramTypeSTL
	else:
		defaultParam.paramType = g_paramTypeStruct
		
	defaultParams[defaultParam.type] = defaultParam
	defaultParamsList[defaultParam.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(defaultParam.type)

def handleSpecialDefaultParams(defaultParams , xmlDefaultParams):
	defaultParam = SpecialDefaultParam(defaultParams)
	for attr in iter(xmlDefaultParams.attrib):
		if attr.lower() == "type".lower():
			defaultParam.type = ReplaceSpecialCharForContainerFromType(xmlDefaultParams.attrib[attr])
		if attr.lower() == "defaultSuffix".lower(): 
			defaultParam.suffix = xmlDefaultParams.attrib[attr]
		if attr.lower() == "specialType".lower(): 
			defaultParam.specialType = xmlDefaultParams.attrib[attr]
	
	if IsHasSameData(defaultParams , defaultParam.type):
		LogOutError("handleSpecialDefaultParams: " , defaultParam.type , " has same defaultParam.") 
			
	if IsSystemDefineType(defaultParam.type):
		defaultParam.paramType = g_paramTypeSystem
	elif GetSpecialDefaultParamType(defaultParam.type) == g_paramTypeSTL:
		defaultParam.paramType = g_paramTypeSTL
	else:
		defaultParam.paramType = g_paramTypeStruct
		
	defaultParams[defaultParam.type] = defaultParam
	
def handleConfig(config , xmlData):
	for attr in iter(xmlData.attrib):
		if attr.lower() == "outputPath".lower():
			config.outputPath = xmlData.attrib[attr]

def handleTables(tables , xmDatas):
	for xmlData in iter(xmDatas.getchildren()): 
		if xmlData.tag.lower() == "Struct".lower():
			handleStructs(tables.structs , xmlData)
		if xmlData.tag.lower() == "Table".lower():
			handleTable(tables.tables , xmlData)
	
def handleStructs(structs , xmlData):
	struct = Struct(structs)
	for attr in iter(xmlData.attrib):
		if attr.lower() == "name".lower():
			struct.name = xmlData.attrib[attr]

	if IsHasSameData(structs , struct.name):
		LogOutError("Parase structs :" , struct.name , "has same struct.") 
	
	for xmlData in iter(xmlData.getchildren()): 
		if xmlData.tag.lower() == "Param".lower():
			handleParams(struct.params , xmlData)
					
	structs[struct.name] = struct
	
	#给Struct加上默认参数
	defaultParam = DefaultParam(g_gameDB.defaultParams)
	defaultParam.type = struct.name
	defaultParam.value = struct.name + "()" 
			
	if IsSystemDefineType(defaultParam.type):
		defaultParam.paramType = g_paramTypeSystem
	elif GetSpecialDefaultParamType(defaultParam.type) == g_paramTypeSTL:
		defaultParam.paramType = g_paramTypeSTL
	else:
		defaultParam.paramType = g_paramTypeStruct
		
	g_gameDB.defaultParams[defaultParam.type] = defaultParam
	g_gameDB.defaultParamsList[defaultParam.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(defaultParam.type)
		
def handleTable(tables , xmData): 
	table = Table(tables)

	for attr in iter(xmData.attrib):
		if attr.lower() == "name".lower():
			table.name = xmData.attrib[attr]
		if attr.lower() == "slaveFrom".lower():
			table.slaveFrom = xmData.attrib[attr]
		if attr.lower() == "slaveTable".lower():
			table.slaveTable = xmData.attrib[attr] 
		if attr.lower() == "ignore_reflection_define".lower():
			table.ignore_reflection_define = xmData.attrib[attr]
		if attr.lower() == "ignore_collection".lower():
			table.ignore_collection = xmData.attrib[attr]
			
	if IsHasSameData(tables , table.name):
		LogOutError("Parase Table :" , table.name , "has same rpcName.") 
 
	if table.slaveFrom != None and table.slaveFrom != "" :
		if table.slaveFrom not in g_gameDB.tables.collectionTables:
			ct = CollectionTable(tables)
			ct.slaves[table.name] = table
			ct.name = table.slaveFrom
			g_gameDB.tables.collectionTables[table.slaveFrom] = ct
		else:
			if IsHasSameData(g_gameDB.tables.collectionTables[table.slaveFrom].slaves , table.name):
				LogOutError("Parase Table :" , table.name , "has same slaveFrom.") 
			g_gameDB.tables.collectionTables[table.slaveFrom].slaves[table.name] = table
			
	if table.slaveTable != None and table.slaveTable != "":
		if table.slaveTable not in g_gameDB.tables.collectionTables:
			ct = CollectionTables(tables)
			ct.name = table.slaveTable
			ct.slaveTables[table.name] = table
			g_gameDB.tables.collectionTables[table.slaveTable] = ct
		else:
			if IsHasSameData(g_gameDB.tables.collectionTables[table.slaveTable].slaveTables , table.name):
				LogOutError("Parase Table :" , table.name , "has same slaveTable.") 				
			g_gameDB.tables.collectionTables[table.slaveTable].slaveTables[table.name] = table			
				
	for xmlData in iter(xmData.getchildren()): 
		if xmlData.tag.lower() == "Field".lower():
			handleField(table.fields , xmlData , table) 
	
	tables[table.name] = table 
	
def handleField(fields , xmData , table): 
	field = Field(table)  
	for attr in iter(xmData.attrib):
		if attr.lower() == "name".lower():
			field.name = xmData.attrib[attr]
		if attr.lower() == "type".lower():
			field.type = xmData.attrib[attr]
		if attr.lower() == "primaryKey".lower():
			field.primaryKey = xmData.attrib[attr]
		if attr.lower() == "masterID".lower():
			field.masterID = xmData.attrib[attr]
		if attr.lower() == "oldName".lower():
			field.oldName = xmData.attrib[attr]
		if attr.lower() == "autoIncr".lower():
			field.autoIncr = xmData.attrib[attr]
		if attr.lower() == "bitmask".lower():
			field.bitmask = xmData.attrib[attr]
		if attr.lower() == "count".lower():
			field.count = xmData.attrib[attr]
		if attr.lower() == "value_type".lower():
			field.value_type = xmData.attrib[attr]
						
	if IsHasSameData(fields , field.name):
		LogOutError("Parase fields :" , field.name , "has same field.") 
		
	fields[field.name] = field 
	 
def handleParams(params , xmlParam):
	param = Param(params)
	for attr in iter(xmlParam.attrib):
		if attr.lower() == "name".lower():
			param.name = xmlParam.attrib[attr] 
		if attr.lower() == "type".lower():  
			param.type = ReplaceSpecialCharForContainerFromType(xmlParam.attrib[attr])
			param.type = GetDefaultParamsType(param.type) #这里获取最相近的类型. 
		if attr.lower() == "default".lower():  
			param.default = ReplaceSpecialCharForContainerFromType(xmlParam.attrib[attr])
		if attr.lower() == "refer".lower():  
			param.refer = "&"
		if attr.lower() == "unrefer".lower():  
			param.unrefer = True 

	param = MakeSpecialDefaultParam(param)
			
	if param.type.lower()  == "SINT8".lower() or param.type.lower()  == "UINT8".lower() or\
		param.type.lower()  == "INT16".lower() or param.type.lower()  == "UINT16".lower() or\
		param.type.lower()  == "INT32".lower() or param.type.lower()  == "UINT32".lower() or\
		param.type.lower()  == "INT64".lower() or param.type.lower()  == "INT64".lower() or\
		param.type.lower()  == "double".lower() or param.type.lower()  == "float".lower() or \
		param.type.lower()  == "char".lower() or param.type.lower()  == "unsigned int".lower() or \
		param.type.lower()  == "short".lower() or param.type.lower()  == "unsigned short".lower() or \
		param.type.lower()  == "long".lower() or param.type.lower()  == "time_t".lower() or \
		param.type.lower()  == "int".lower() or \
		param.unrefer == True: 
		
		param.refer = None
	
	if IsHasSameData(params , param.name):
		LogOutError("Parase params :" , param.name , "has same param.") 
		
	params[param.name] = param
	
def CheckAllData():
	#检查field的类型是否在默认值中.
	for indexTables, table in g_gameDB.tables.tables.items():  
		for indexFields , field in table.fields.items():
			if (not IsInDefaultParams(field.type)):
				LogOutError("table's field type not in default params. Table_name: " , table.name , "  param.type:" , field.type )  
	
	#检查collectionTable需要包含slave的master
	for indexCollection, collectionTable in g_gameDB.tables.collectionTables.items():  
		table = GetMasterTableFromName(indexCollection)
		if table == None:
			LogOutError("table slaveFrom error from Table_name: " , collectionTable.name) 
		
		collectionTable.slaves[table.name] = table
		
	#检查struct的param是否在默认值中.
	for indexStructs , struct in g_gameDB.tables.structs.items():  
		for index , param in struct.params.items():  
			if (not IsInDefaultParams(param.type)) and (param.default == None or param.default == ""):
				LogOutError("struct's param not in default params." , index, "  param.type:" , param.type)  
				
	#检查field各项参数是否合法.
	for indexTables, table in g_gameDB.tables.tables.items():  
		bHasMasterID = None
		bHasPrimaryKey = None
		bHasAutoIncrease = None
		bHasMasterID = None		
		for indexFields , field in table.fields.items():
			if field.masterID != None:
				if bHasMasterID != None:				
					LogOutError("table's field has had masterID table_name: " , table.name , " field_name: " , field.name , " masterID:" , field.masterID )
				else:
					bHasMasterID = True
				
				if not CheckMasterOrPrimaryIDRight(field.type):			
					LogOutError("table's field masterID type error: " , table.name , " field_type: " , field.type , " masterID:" , field.masterID )
					
			
			if field.primaryKey != None:
				if bHasPrimaryKey != None:				
					LogOutError("table's field has had primaryKey table_name: " , table.name , " field_name: " , field.name , " primaryKey: " , field.primaryKey)
				else:
					bHasPrimaryKey = True
				
				if not CheckMasterOrPrimaryIDRight(field.type):			
					LogOutError("table's field primaryKey type error: " , table.name , " field_type: " , field.type , " primaryKey:" , field.primaryKey )
					
			if bHasPrimaryKey != None and bHasMasterID != None :
				LogOutError("table's field may has had primaryKey and masterID table_name: " , table.name , " field_name: " , field.name , " primaryKey: " , field.primaryKey , " masterID:" , field.masterID)
			
			if field.autoIncr != None:
				if bHasAutoIncrease != None:				
					LogOutError("table's field has had AutoIncrease table_name: " , table.name , " field_name: " , field.name , " AutoIncrease : " , field.autoIncr )
				else:
					bHasAutoIncrease  = True
				
				 								
################################生成对应文件#####################################

oneTab = "\t"
twoTab = oneTab + "\t"
threeTab = twoTab + "\t"
fourTab = threeTab + "\t"
fiveTab = fourTab + "\t"
g_ormPrefix = "Orm_"
g_namespace = "Orm"

def GenerateGameDB():
	DeleteGameDBGenerateFiles() 
	LogOutInfo("deleted all generate files finished.\n")
	
	CreateOutputPath() 
	LogOutInfo("created OutputPath finished.\n")

#	GenerateMsgNameDefine()	
#	LogOutInfo("generate MsgNameDefine.h file finished.\n")
	
#	GenerateGlableRpc()
#	LogOutInfo("generate GlobalRpc.h file finished.\n")
	
#	GenerateGameDBRegister()
#	LogOutInfo("generate RpcRegister.cpp file finished.\n")
	
#	GenerateGameDBHandlers()
#	LogOutInfo("generate RpcHandler.cpp file finished.\n")
	
	GenerateOrms()
	LogOutInfo("generate Struct.h file finished.\n")
	
	GenerateOrmStructs()
	LogOutInfo("generate OrmStruct.h file finished.\n")
	
#	GenerateGameDBCallFuncs()
#	LogOutInfo("generate RpcCallFuncs.h file finished.\n") 
	
#	GenerateRPCDefines()
#	LogOutInfo("generate RpcDefine.h file finished.\n") 

def GenerateOrms():
	GenerateOrmsHeadFile()
	GenerateOrmsCPPFile()
	
	GenerateOrmsCollectionHeadFile()
	GenerateOrmsCollectionCPPFile()
	
	GenerateOrmsReflectionHeadFile()
	GenerateOrmsReflectionCPPFile()
	
def GenerateOrmsHeadFile():
	outputPath = GetOutputPath()   
	for index , table in g_gameDB.tables.tables.items():   
		outputFilePath = outputPath + g_ormPrefix + table.name + ".h" 
			
		fileOrm = open(outputFilePath , "a")
		GenerateOrmHeader(fileOrm , table.name)
		
		fileOrm.write(oneTab + "class " + table.name + " : public GameDB::Orm\n") 
		fileOrm.write(oneTab + "{ \n")
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + table.name + "();\n")
		fileOrm.write(threeTab + "virtual ~" + table.name + "();\n")
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "private:\n")
		fileOrm.write(threeTab + table.name + "( const " + table.name + " &);\n") 
		fileOrm.write(threeTab + "const " + table.name + "& operator=( const " + table.name + " &);\n") 
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + table.name + "* Clone();\n\n")
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + "static const char* TableName() { return \"" + table.name + "\"; }\n")
		fileOrm.write(threeTab + "static const INT64 meta_hash = " + GetBKDRHash(table.name) + ";\n")
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")		
		fileOrm.write(threeTab + "virtual std::string GetKey() override;\n")
		fileOrm.write(threeTab + "virtual std::string GetRawKey() override;\n")
		fileOrm.write(threeTab + "virtual std::string GetTableName() override;\n")
#		fileOrm.write(threeTab + "virtual bool        IsDefaultValue() override;\n") 
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")		
		fileOrm.write(threeTab + "bool IsEqual(const " + table.name + " & val);\n" )
		fileOrm.write(threeTab + "bool operator == (const " + table.name + " & val);\n" )
		fileOrm.write(threeTab + "bool operator != (const " + table.name + " & val);\n" )
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + "virtual void		ToBson(std::string & strBuf) override;\n")
		fileOrm.write(threeTab + "virtual void		ToBson(mongo::BSONObj  & objBson) override;\n")
		fileOrm.write(threeTab + "virtual void		FromBson(const char * pData , INT32 nSize) override;\n")
		fileOrm.write(threeTab + "virtual void		FromBson(const mongo::BSONObj  & objBson) override;\n")
		fileOrm.write(threeTab + "virtual void		ToCompress(std::string & strBuf); \n")
		fileOrm.write(threeTab + "virtual void		FromCompress(const char* data,INT32 size) override;\n")
		fileOrm.write(threeTab + "virtual void		FromCompress(const std::string & strBuf) override;\n")		
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + "virtual INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED) override;\n")
		fileOrm.write(threeTab + "virtual INT64		CurHash() override { return __hash; }\n")
		fileOrm.write(threeTab + "virtual void		HashUpdate(INT64 llHash) override { __hash = llHash; }\n")	
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + "virtual void		AutoIncrease(INT64 llKey) override ; \n")
		fileOrm.write(threeTab + "virtual void		SetMasterID(INT64 llID) override {" + GetMasterIDFieldNameINT64(table , 0) + " }\n")
		fileOrm.write(threeTab + "virtual void		SetMasterID(const char* pID) override {" + GetMasterIDFieldNameString(table , 0) + " }\n")
		fileOrm.write(threeTab + "virtual INT64		GetMasterID() override { " + GetMasterIDFieldNameINT64(table , 1) + "}\n")
		fileOrm.write(threeTab + "virtual const char*	GetMasterStrID() override { " + GetMasterIDFieldNameString(table , 1) + "}\n")
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		GenerateOrmHeadFields(fileOrm , table)  
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		GenerateTableFeildDefine(fileOrm , table.fields)  
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "private:\n") 
		fileOrm.write(threeTab + "INT64" + threeTab + "__hash;\n") 
		fileOrm.write(twoTab + "\n")
				
		fileOrm.write(oneTab + "}; \n \n")	
		fileOrm.write("}//" + g_namespace + "\n\n")
		fileOrm.write("#endif\n")	  
		fileOrm.close() 
		
		LogOutInfo("generate " + outputFilePath + " file finished.\n")
			
def  GenerateOrmHeader(fileOrm , table_name):
	WriteFileDescription(fileOrm , g_ormPrefix + table_name + ".h" , "orm操作集合.") 
	fileOrm.write("#ifndef __" + g_namespace + "_" + g_ormPrefix + table_name + "_h__\n") 
	fileOrm.write("#define __" + g_namespace + "_" + g_ormPrefix + table_name + "_h__\n")  
	fileOrm.write("#include \"" + g_ormPrefix +  "OrmStructs" + ".h\"\n")  
	fileOrm.write("#include \"GameDB/inc/Orm.h\"\n\n")  
	  
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n")    
	
def  GenerateTableFeildDefine(fileOrm , fields):  
	for index , field in fields.items():  
		strData = threeTab  
		strData += field.type  
		strData += oneTab  
		strData += field.name
		strData += ";\n"
		
		fileOrm.write(strData)   
	
def GenerateOrmHeadFields(fileOrm , table): 
	for index , field in table.fields.items():  
		fileOrm.write(threeTab + field.type + " Get" + field.name + "() const;\n") 
		fileOrm.write(threeTab + "void Set" + field.name + "(" + field.type + "& value);\n\n") 
		
		if field.count != None:					
			fileOrm.write(threeTab + "void Plus" + field.name + "(" + field.type + " & value);\n")
			fileOrm.write(threeTab + "void Minus" + field.name + "(" + field.type + " & value);\n")
			
		if field.bitmask != None:					
			fileOrm.write(threeTab + "void " + field.name + "Include" + "(" + field.type + " & value);\n")	
			fileOrm.write(threeTab + "BOOL Is" + field.name + "Include" + "(" + field.type + " & value);\n")		
			fileOrm.write(threeTab + "void " + field.name + "Exclude" + "(" + field.type + " & value);\n")
		
def GenerateOrmsCPPFile():
	outputPath = GetOutputPath()   
	for index , table in g_gameDB.tables.tables.items():   
		outputFilePath = outputPath + g_ormPrefix + table.name + ".cpp" 
			
		fileOrm = open(outputFilePath , "a")
		GenerateOrmCppHeader(fileOrm , table.name)
		GenerateOrmCppConstructor(fileOrm , table)  #构造函数
		GenerateOrmCppDestructor(fileOrm , table)	#析构函数 
		GenerateOrmCppIsEqual(fileOrm , table);
		GenerateOrmCppOperatorEqualAndNotEqual(fileOrm , table);
		GenerateOrmCppGetRawKey(fileOrm , table)  
		GenerateOrmCppGetKey(fileOrm , table)   
		GenerateOrmCppGetTableName(fileOrm , table)  
		GenerateOrmCppAutoIncrease(fileOrm , table)   
		GenerateOrmCppClone(fileOrm , table)   
		GenerateOrmCppToCompress(fileOrm , table)  
		GenerateOrmCppToBson(fileOrm , table)
		GenerateOrmCppToBson2(fileOrm , table)  
		GenerateOrmCppFromCompress(fileOrm , table)  
		GenerateOrmCppFromCompress2(fileOrm , table) 
		GenerateOrmCppFromBson(fileOrm , table) 
		GenerateOrmCppFromBson2(fileOrm , table) 
		GenerateOrmCppHashMake(fileOrm , table)
		GenerateOrmCppFields(fileOrm , table)
		
		fileOrm.write("}//" + g_namespace + "\n\n")
		LogOutInfo("generate " + outputFilePath + " file finished.\n")
	
def  GenerateOrmCppHeader(fileOrm , table_name):
	fileOrm.write("#include \"" + g_ormPrefix + table_name + ".h\"\n\n")  
	  
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n") 
	
def GenerateOrmCppConstructor(fileOrm , table): 
	fileOrm.write(oneTab + table.name + "::" + table.name + "()\n")
	
	nCount = 0
	for index , field in table.fields.items():
		if nCount == 0:
			fileOrm.write(twoTab + ": " + field.name + "(" + GetDefaultParamValue(field.type) + ")\n")
		else:
			fileOrm.write(twoTab + ", " + field.name + "(" + GetDefaultParamValue(field.type) + ")\n")
		nCount = nCount + 1
	
	fileOrm.write(oneTab + "{\n" + oneTab + "}\n\n")
	
def GenerateOrmCppDestructor(fileOrm , table): 
	fileOrm.write(oneTab + table.name + "::~" + table.name + "()\n")
	fileOrm.write(oneTab + "{\n")
	 
	for index , field in table.fields.items():
		if GetSpecialDefaultParamType(field.type) == g_paramTypeSTL:
			fileOrm.write(twoTab + field.name + ".clear();")
			
	fileOrm.write(oneTab + "}\n\n")
		
def GenerateOrmCppIsEqual(fileOrm , table):
	fileOrm.write(oneTab + "bool " + table.name + "::IsEqual(const " + table.name + " & val)\n" )
	fileOrm.write(oneTab + "{ \n")
	strParam = twoTab + "if(\n"
	
	count = 0
	for index , field in table.fields.items():  
		count = count + 1
		strParam = strParam + threeTab + field.name + " == val." + field.name
		if count == len(table.fields):
			strParam = strParam +  ")\n"
		else:
			strParam = strParam + "&&\n"
			
	strParam = strParam + twoTab + "{\n"	
	strParam = strParam + threeTab + "return true;\n"	
	strParam = strParam + twoTab + "}\n"	
	strParam = strParam + twoTab + "return false;\n"	
	fileOrm.write(strParam)
	fileOrm.write(oneTab + "} \n\n")
	
def GenerateOrmCppOperatorEqualAndNotEqual(fileOrm , table):
	fileOrm.write(oneTab + "bool " + table.name + "::operator == (const " + table.name + " & val)\n" )
	fileOrm.write(oneTab + "{ \n")
	fileOrm.write(twoTab + "return IsEqual(val);\n") 
	fileOrm.write(oneTab + "} \n\n")
	
	fileOrm.write(oneTab + "bool  " + table.name + "::operator != (const " + table.name + " & val)\n" )
	fileOrm.write(oneTab + "{ \n") 
	fileOrm.write(twoTab + "return !IsEqual(val);\n") 
	fileOrm.write(oneTab + "} \n\n")	
	
def GenerateOrmCppGetRawKey(fileOrm , table): 
	fileOrm.write(oneTab + "std::string " + table.name + "::GetRawKey()\n")
	fileOrm.write(oneTab + "{\n")

	fileOrm.write(twoTab + "std::string result;\n")
	fileOrm.write(twoTab + "result.reserve(64);\n\n")
	
	for index , field in table.fields.items():
		if field.primaryKey != None:	
			if field.type.lower() == "std::string".lower() or field.type.lower() == "std_string".lower():
				fileOrm.write(twoTab + "result.append(" + field.name + ");\n")
			elif GetParamType(field.type) == g_paramTypeSystem:
				fileOrm.write(twoTab + "{\n") 
				fileOrm.write(threeTab + "result.append(CUtil::itoa((INT64)" + field.name + "));\n") 
				fileOrm.write(twoTab + "}\n") 

	fileOrm.write(twoTab + "return result;\n")
	fileOrm.write(oneTab + "}\n\n") 
		
def GenerateOrmCppGetKey(fileOrm , table): 
	fileOrm.write(oneTab + "std::string " + table.name + "::GetKey()\n")
	fileOrm.write(oneTab + "{\n")
	 
	bHasMasterID = None
	for index , field in table.fields.items():
		if field.masterID != None:
			bHasMasterID = True
			fileOrm.write(twoTab + "std::string result;\n")
			fileOrm.write(twoTab + "result.reserve(64);\n\n")
			
			if field.type.lower() == "std::string".lower() or field.type.lower() == "std_string".lower():
				fileOrm.write(twoTab + "result.append(" + field.name + ");\n")
			elif GetParamType(field.type) == g_paramTypeSystem:
				fileOrm.write(threeTab + "{\n") 
				fileOrm.write(threeTab + "result.append(CUtil::itoa((INT64)" + field.name + "));\n") 
				fileOrm.write(twoTab + "}\n")

			fileOrm.write(twoTab + "return result;\n")
	
	if bHasMasterID == None:
		fileOrm.write(twoTab + "return this->GetRawKey();\n\n")
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppGetTableName(fileOrm , table): 
	fileOrm.write(oneTab + "std::string " + table.name + "::GetTableName()\n")
	fileOrm.write(oneTab + "{\n")

	fileOrm.write(twoTab + "return " + table.name + "::TableName();\n")
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppAutoIncrease(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::AutoIncrease(INT64 llKey)\n")
	fileOrm.write(oneTab + "{\n")

	bHasAutoIncrease = None
	for index , field in table.fields.items():
		if field.autoIncr != None:
			bHasAutoIncrease = True
			fileOrm.write(twoTab + field.name + " = llKey;\n")  
	
	if bHasAutoIncrease == None:
		fileOrm.write(twoTab + "MsgAssert(false , \"AutoIncrease key:\" << llKey);\n") 
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppClone(fileOrm , table): 
	fileOrm.write(oneTab + table.name + " * " + table.name + "::Clone()\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj  obj;\n") 
	fileOrm.write(twoTab + "ToBson(obj);\n") 
	fileOrm.write(twoTab + table.name + " * pNew = new " + table.name + "();\n") 
	fileOrm.write(twoTab + "pNew->FromBson(obj);\n") 
	fileOrm.write(twoTab + "return pNew;\n") 
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppToCompress(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::ToCompress(std::string & strBuf)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj  obj;\n") 
	fileOrm.write(twoTab + "ToBson(obj);\n") 
	fileOrm.write(twoTab + "CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppToBson(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::ToBson(std::string & strBuf)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj  obj;\n") 
	fileOrm.write(twoTab + "ToBson(obj);\n") 
	fileOrm.write(twoTab + "strBuf = std::string(obj.objdata(),obj.objsize());\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppToBson2(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::ToBson(mongo::BSONObj  & obj)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObjBuilder builder;\n") 
	fileOrm.write(twoTab + "builder.append(\"" + g_tableMeta + "\",TableName());\n") 
	
	for index , field in table.fields.items():
		fileOrm.write(twoTab + "if(" + field.name + " != " + GetDefaultParamValue(field.type) + ")\n")
		if IsUserDefineType(field.type):
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "CUtil::Parameter p(" + field.name + ");\n")
			fileOrm.write(threeTab + "builder.appendBinData(\"" + field.name + "\" , p.GetDataLen() , mongo::bdtParamter , (const char *)(p.GetStreamData())); \n")
			fileOrm.write(twoTab + "}\n")
		else:
			fileOrm.write(threeTab + "builder.append(\"" + field.name + "\"," + field.name + ");\n")
	
	fileOrm.write(twoTab + "obj = builder.obj();\n")
		
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppFromCompress(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::FromCompress(const std::string& inbuf)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "std::string tmpbuf;\n") 
	fileOrm.write(twoTab + "CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);\n") 
	fileOrm.write(twoTab + "mongo::BSONObj  obj(tmpbuf.c_str());\n")  
	fileOrm.write(twoTab + "MsgAssert(obj.objsize() == tmpbuf.length() , \"\");\n")  
	fileOrm.write(twoTab + "FromBson(obj);\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppFromCompress2(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::FromCompress(const char* pData,INT32 size)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "std::string tmpbuf;\n") 
	fileOrm.write(twoTab + "CUtil::Uncompress(pData,size,tmpbuf);\n") 
	fileOrm.write(twoTab + "mongo::BSONObj  obj(tmpbuf.c_str());\n")  
	fileOrm.write(twoTab + "MsgAssert(obj.objsize() == tmpbuf.length() , \"\");\n")  
	fileOrm.write(twoTab + "FromBson(obj);\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppFromBson(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::FromBson(const char* pData,INT32 size)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj  obj(pData);\n") 
	fileOrm.write(twoTab + "MsgAssert(obj.objsize() == size , \"FromBson error.\");\n") 
	fileOrm.write(twoTab + "FromBson(obj);\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppFromBson2(fileOrm , table): 
	fileOrm.write(oneTab + "void " + table.name + "::FromBson(const mongo::BSONObj  & obj)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObjIterator  iter(obj); \n") 
	fileOrm.write(twoTab + "while(iter.more())\n") 
	fileOrm.write(twoTab + "{\n") 
	
	fileOrm.write(threeTab + "mongo::BSONElement be = iter.next();\n")
	fileOrm.write(threeTab + "const char* fieldName = be.fieldName();\n")
	fileOrm.write(threeTab + "INT64 hash = CUtil::BKDRHashSum(fieldName);\n")
	fileOrm.write(threeTab + "switch(hash)\n")
	fileOrm.write(threeTab + "{\n")
	
	fileOrm.write(threeTab + "case " + GetBKDRHash(g_tableMeta) + ": // " + g_tableMeta + "\n")
	fileOrm.write(fourTab + "{\n")
	fileOrm.write(fiveTab + "MsgAssert(CUtil::strcmp(be.valuestr(), TableName()) == 0 , \"FromBson error.\");\n")
	fileOrm.write(fourTab + "}break;\n") 
	
	for index , field in table.fields.items():
		fileOrm.write(threeTab + "case " + GetBKDRHash(field.name) + ": // " + field.name + "\n")
		fileOrm.write(fourTab + "{\n")
		if IsUserDefineType(field.type):
			fileOrm.write(fiveTab + "CUtil::Parameter p;\n")
			fileOrm.write(fiveTab + "CUtil::BsonToCpp( p , be);\n")
			fileOrm.write(fiveTab + field.name + " = p" + ";\n") 
		elif IsSystemDefineType(field.type):
			fileOrm.write(fiveTab + "CUtil::BsonToCpp( " + field.name + " , be);\n")
		
		fileOrm.write(fourTab + "}break;\n") 
			
	fileOrm.write(threeTab + "}\n")	
	fileOrm.write(twoTab + "}\n")	
	fileOrm.write(twoTab + "__hash = HashMake(0);\n")		
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCppHashMake(fileOrm , table): 
	fileOrm.write(oneTab + "INT64 " + table.name + "::HashMake(INT64 seed)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "INT64 _result = seed;\n")  
	for index , field in table.fields.items():  
		if field.name == IsSystemDefineType(field.name):
			fileOrm.write(twoTab + "_result = CUtil::CityHash(&" + field.name + ",sizeof(" + field.name + "),_result);\n")
		elif IsUserDefineType(field.type):
			fileOrm.write(twoTab + "_result = CUtil::CityHash(&" + field.name + ",sizeof(" + field.name + "),_result);\n")
		elif IsSystemDefineType(field.type):
			fileOrm.write(twoTab + "_result = CUtil::CityHash(&" + field.name + ",sizeof(" + field.name + "),_result);\n")
		
		fileOrm.write(twoTab + "return _result;\n") 
		 	
	fileOrm.write(oneTab + "}\n\n")
		
def GenerateOrmCppFields(fileOrm , table): 
	for index , field in table.fields.items():  
		fileOrm.write(oneTab + field.type + " " + table.name + "::Get" + field.name + "() const\n")
		fileOrm.write(oneTab + "{\n")
		fileOrm.write(twoTab + "return " + field.name + ";\n")  
		fileOrm.write(oneTab + "}\n\n")
				
		fileOrm.write(oneTab + "void " + table.name + "::Set" + field.name + "(" + field.type + " & value)\n")
		fileOrm.write(oneTab + "{\n")
		fileOrm.write(twoTab + field.name + " = value;\n")  
		fileOrm.write(oneTab + "}\n\n")
		
		if field.count != None:					
			fileOrm.write(oneTab + "void " + table.name + "::Plus" + field.name + "(" + field.type + " & value)\n")
			fileOrm.write(oneTab + "{\n")
			fileOrm.write(twoTab + field.name + " = " + field.name + " + value;\n")  
			fileOrm.write(oneTab + "}\n\n")
			
			fileOrm.write(oneTab + "void " + table.name + "::Minus" + field.name + "(" + field.type + " & value)\n")
			fileOrm.write(oneTab + "{\n")
			fileOrm.write(twoTab + field.name + " = " + field.name + " - value;\n")  
			fileOrm.write(oneTab + "}\n\n")
			
		if field.bitmask != None:					
			fileOrm.write(oneTab + "void " + table.name + "::" + field.name + "Include" + "(" + field.type + " & value)\n")
			fileOrm.write(oneTab + "{\n")
			fileOrm.write(twoTab + field.name + " = " + field.name + " | value;\n")  
			fileOrm.write(oneTab + "}\n\n")
			
			fileOrm.write(oneTab + "void " + table.name + "::" + field.name + "Exclude" + "(" + field.type + " & value)\n")
			fileOrm.write(oneTab + "{\n")
			fileOrm.write(twoTab + field.name + " = " + field.name + " & (~value);\n")  
			fileOrm.write(oneTab + "}\n\n")
			
			fileOrm.write(oneTab + "BOOL " + table.name + "::Is" + field.name + "Include" + "(" + field.type + " & value)\n")
			fileOrm.write(oneTab + "{\n")
			fileOrm.write(twoTab + "return " + field.name + " & value;\n")  
			fileOrm.write(oneTab + "}\n\n")
				
def GenerateOrmsCollectionHeadFile():
	outputPath = GetOutputPath()   
	for index , collectionTable in g_gameDB.tables.collectionTables.items():   
		outputFilePath = outputPath + g_ormPrefix + collectionTable.name + "Collection.h" 
			
		fileOrm = open(outputFilePath , "a")
		GenerateOrmCollectionHeader(fileOrm , collectionTable)
		
		type = GetMasterTableTypeFromName(collectionTable.name)
		fileOrm.write(oneTab + "class " + collectionTable.name + "Collection : public  GameDB::OrmCollection<" + type + ">\n") 
		fileOrm.write(oneTab + "{ \n")
		fileOrm.write(twoTab + "public:\n")
		fileOrm.write(threeTab + collectionTable.name + "Collection();\n")
		fileOrm.write(threeTab + "virtual ~" + collectionTable.name + "Collection();\n")
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "private:\n")
		fileOrm.write(threeTab + collectionTable.name + "Collection( const " + collectionTable.name + " &);\n") 
		fileOrm.write(threeTab + "const " + collectionTable.name + "Collection & operator=( const " + collectionTable.name + "Collection &);\n") 
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")  
		fileOrm.write(threeTab + "virtual void		ToBson(std::string & strBuf) override;\n")
		fileOrm.write(threeTab + "virtual void		ToBson(mongo::BSONObj  & objBson) override;\n")
		fileOrm.write(threeTab + "virtual void		FromBson(std::string & compressedBuf) override;\n")
		fileOrm.write(threeTab + "virtual void		FromBson(const char * pData , UINT32 nSize) override;\n")
		fileOrm.write(twoTab + "\n")
				
		fileOrm.write(twoTab + "public:\n")
		GenerateOrmCollectionVisitor(fileOrm , collectionTable)  
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "public:\n")
		GenerateOrmSlavesHead(fileOrm , collectionTable)  
		fileOrm.write(twoTab + "\n")		
		
		fileOrm.write(twoTab + "public:\n")
		GenerateOrmSlaveTablesHead(fileOrm , collectionTable)  
		fileOrm.write(twoTab + "\n")
		
		fileOrm.write(twoTab + "protected:\n")
		GenerateSlaveTableDefine(fileOrm , collectionTable)  
		fileOrm.write(twoTab + "\n") 
				
		fileOrm.write(oneTab + "}; \n \n")	
		fileOrm.write("}//" + g_namespace + "\n\n")
		fileOrm.write("#endif\n")	  
		fileOrm.close() 
		
		LogOutInfo("generate " + outputFilePath + " file finished.\n")
			
def  GenerateOrmCollectionHeader(fileOrm , collectionTable):
	WriteFileDescription(fileOrm , g_ormPrefix + collectionTable.name + "Collection.h" , "针对orm子表操作的集合类.") 
	fileOrm.write("#ifndef __" + g_namespace + "_" + g_ormPrefix + collectionTable.name + "_collection_h__\n") 
	fileOrm.write("#define __" + g_namespace + "_" + g_ormPrefix + collectionTable.name + "_collection_h__\n")  
	fileOrm.write("#include \"GameDB/inc/DBCommon.h\"\n")  
	fileOrm.write("#include \"GameDB/inc/OrmCollection.h\"\n")  
	fileOrm.write("#include \"GameDB/inc/OrmVector.h\"\n")  
	fileOrm.write("#include \"GameDB/inc/DBCommon.h\"\n\n")  
	
	GenerateOrmCollectionHeaderInclude(fileOrm , collectionTable)
	  
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n")    		
						
def  GenerateOrmCollectionHeaderInclude(fileOrm , collectionTable): 
	for index , table in collectionTable.slaves.items():  
		fileOrm.write("#include \"" + g_ormPrefix + table.name + ".h\"\n") 
		
	for index , table in collectionTable.slaveTables.items():  
		fileOrm.write("#include \"" + g_ormPrefix + table.name + ".h\"\n")  
	   
	fileOrm.write("\n")    	 
			
def GenerateOrmSlavesHead(fileOrm , collectionTable): 
	for index , table in collectionTable.slaves.items():  
		if table.ignore_collection == None:
			fileOrm.write(threeTab + table.name + " * Get" + table.name + "();\n")  
			fileOrm.write(threeTab + "void Cleanup" + table.name + "();\n")  
		
def GenerateOrmCollectionVisitor(fileOrm , collectionTable):  	
	fileOrm.write(twoTab + "template<class VISITOR,class PARAM> void visit(VISITOR visitor,PARAM& param)\n" ) 
	fileOrm.write(twoTab + "{\n")    		
	
	for index , table in collectionTable.slaves.items():  
		if table.ignore_collection == None:
			fileOrm.write(threeTab + "if(!visitor(m_p" + table.name + " , param))\n") 
			fileOrm.write(fourTab + "return ;\n")  
			fileOrm.write(twoTab + "\n")   
		
	for index , table in collectionTable.slaveTables.items():  
		if table.ignore_collection == None:
			fileOrm.write(threeTab + "for(GameDB::OrmVectorEx< " + table.name + " *>::iterator iter = m_v" + table.name + ".begin(); iter != m_v" + table.name + ".end(); ++iter)\n") 
			fileOrm.write(threeTab + "{\n")     		
			fileOrm.write(fourTab + "if(!visitor(*iter,param))\n")  
			fileOrm.write(fiveTab + "return ;\n")  
			fileOrm.write(threeTab + "}\n")  
			fileOrm.write(twoTab + "\n")      		
			
	fileOrm.write(twoTab + "}\n")     
	
def GenerateOrmSlaveTablesHead(fileOrm , collectionTable): 
	for index , table in collectionTable.slaveTables.items():  	
		if table.ignore_collection == None:
			fileOrm.write(threeTab + "GameDB::OrmVectorEx< " + table.name + " *> & Get" + table.name + "(){ return m_vec" + table.name + "; }\n")  
			fileOrm.write(threeTab + table.name + " * Create" + table.name + "(BOOL bAddToManager = TRUE);\n") 
			fileOrm.write(threeTab + "BOOL Delete" + table.name + "(" + table.name + " * pValue , bool bFree = false);\n")
			fileOrm.write(threeTab + "void Load" + table.name + "(mongo::BSONObj & obj);\n")   	
			fileOrm.write(threeTab + "void Save" + table.name + "(mongo::BSONArrayBuilder & bab);\n")		
			
def  GenerateSlaveTableDefine(fileOrm , collectionTable):  
	for index , table in collectionTable.slaves.items():  
		if table.ignore_collection == None:
			strData = threeTab  
			strData += table.name  
			strData += oneTab  
			strData += " * m_p"
			strData += table.name
			strData += ";\n"
			
			fileOrm.write(strData)   
		 
	for index , table in collectionTable.slaveTables.items():  
		if table.ignore_collection == None:
			strData = threeTab  
			strData += "GameDB::OrmVectorEx<"   
			strData += table.name  
			strData += " *>"
			strData += oneTab  
			strData += " m_vec"
			strData += table.name
			strData += ";\n"
			
			fileOrm.write(strData)  
		
def GenerateOrmsCollectionCPPFile():			
	outputPath = GetOutputPath()   
	for index , collectionTable in g_gameDB.tables.collectionTables.items():   
		outputFilePath = outputPath + g_ormPrefix + collectionTable.name + "Collection.cpp" 
			
		fileOrm = open(outputFilePath , "a")
		
		GenerateOrmCollectionCppHeader(fileOrm , collectionTable.name)
		GenerateOrmCollectionCppConstructor(fileOrm , collectionTable)  #构造函数
		GenerateOrmCollectionCppDestructor(fileOrm , collectionTable)	#析构函数 
		GenerateOrmCollectionCppSlaves(fileOrm , collectionTable)  
		GenerateOrmCollectionCppSlaveTables(fileOrm , collectionTable)   
		  
		GenerateOrmCollectionCppToBson(fileOrm , collectionTable)
		GenerateOrmCollectionCppToBson2(fileOrm , collectionTable)   
		GenerateOrmCollectionCppFromBson(fileOrm , collectionTable) 
		GenerateOrmCollectionCppFromBson2(fileOrm , collectionTable)  
		
		fileOrm.write("}//" + g_namespace + "\n\n")
		LogOutInfo("generate " + outputFilePath + " file finished.\n")
			
			
def  GenerateOrmCollectionCppHeader(fileOrm , table_name):
	fileOrm.write("#include \"" + g_ormPrefix + table_name + "Collection.h\"\n\n")  
	  
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n") 
	
def GenerateOrmCollectionCppConstructor(fileOrm , collectionTable): 
	fileOrm.write(oneTab + collectionTable.name + "Collection::" + collectionTable.name + "Collection()\n")	 
	fileOrm.write(oneTab + "{\n" );
	
	for index , slave in collectionTable.slaves.items():
		if slave.ignore_collection == None:
			fileOrm.write(twoTab + "m_p" + slave.name + " = NULL;\n")
	
	fileOrm.write(oneTab + "}\n\n")
	
def GenerateOrmCollectionCppDestructor(fileOrm , collectionTable): 
	fileOrm.write(oneTab + collectionTable.name + "Collection::~" + collectionTable.name + "Collection()\n")
	fileOrm.write(oneTab + "{\n")
	  
	for index , slave in collectionTable.slaves.items():
		if slave.ignore_collection == None:
			fileOrm.write(twoTab + "SAFE_DELETE(m_p" + slave.name + ");\n")
		
	fileOrm.write(oneTab + "}\n\n")
		
def GenerateOrmCollectionCppSlaves(fileOrm , collectionTable): 	
	for index , slave in collectionTable.slaves.items():
		if slave.ignore_collection == None:
			fileOrm.write(oneTab + slave.name + " * " + collectionTable.name + "Collection::Get" + slave.name + "()\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + "if(m_p" + slave.name + " == NULL)\n")
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "m_p" + slave.name + " = new " + slave.name + "();\n")
			fileOrm.write(threeTab + "m_p" + slave.name + "->SetMasterID( m_vMasterId );\n")
			fileOrm.write(twoTab + "}\n")
			fileOrm.write(twoTab + "return " + "m_p" + slave.name + ";\n")
			
			fileOrm.write(oneTab + "}\n\n") 
			
			#删除操作
			fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::Cleanup" + slave.name + "()\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + "SAFE_DELETE(m_p" + slave.name + ");\n")
			
			fileOrm.write(oneTab + "}\n\n") 
		
def GenerateOrmCollectionCppSlaveTables(fileOrm , collectionTable): 	
	for index , slave in collectionTable.slaveTables.items():
		if slave.ignore_collection == None:
			fileOrm.write(oneTab + slave.name + " * " + collectionTable.name + "Collection::Create" + slave.name + "(BOOL bAddToManager /*= TRUE*/)\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + slave.name + "* p" + slave.name + " = new " + slave.name + ";\n")
			fileOrm.write(twoTab + "p" + slave.name + "->SetMasterID(m_vMasterId);\n")
			fileOrm.write(twoTab + "if (bAddToManager)\n")
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "m_vec" + slave.name + ".push_back( p" + slave.name + ");\n")
			fileOrm.write(twoTab + "}\n\n")
			fileOrm.write(twoTab + "return " + "p" + slave.name + ";\n")
			
			fileOrm.write(oneTab + "}\n\n") 
			
			#删除操作
			fileOrm.write(oneTab + "BOOL " + collectionTable.name + "Collection::Delete" + slave.name + "(" + slave.name + " * pValue , bool bFree /*= false*/)\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + "return m_vec" + slave.name + ".Remove(pValue , bFree);\n")
			
			fileOrm.write(oneTab + "}\n\n") 
			
			#载入操作
			fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::Load" + slave.name + "(mongo::BSONObj & obj)\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + "m_vec" + slave.name + ".Cleanup();\n")
			fileOrm.write(twoTab + "mongo::BSONObjIterator iter(obj);\n")
			fileOrm.write(twoTab + "while(iter.more())\n")
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "mongo::BSONElement be = iter.next();\n")
			fileOrm.write(threeTab + "Assert(be.isABSONObj());\n\n")
			fileOrm.write(threeTab + slave.name + " * p" + slave.name + " = Create" + slave.name + "();\n")
			fileOrm.write(threeTab + "p" + slave.name + "->FromBson(be.Obj());\n")
			fileOrm.write(twoTab + "}\n")
			
			fileOrm.write(oneTab + "}\n\n") 
			
			#保存操作
			fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::Save" + slave.name + "(mongo::BSONArrayBuilder & bab)\n")
			fileOrm.write(oneTab + "{\n")
			
			fileOrm.write(twoTab + "for (size_t i = 0;i < m_vec" + slave.name + ".size();++i)\n")
			fileOrm.write(twoTab + "{\n") 
			fileOrm.write(threeTab + "mongo::BSONObj obj;\n") 
			fileOrm.write(threeTab + slave.name + " * p" + slave.name + " = m_vec" + slave.name + "[i];\n")
			fileOrm.write(threeTab + "p" + slave.name + "->ToBson(obj);\n")
			fileOrm.write(threeTab + "bab.append(obj);\n")
			fileOrm.write(twoTab + "}\n")
			
			fileOrm.write(oneTab + "}\n\n") 
		 
def GenerateOrmCollectionCppToBson(fileOrm , collectionTable): 
	fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::ToBson(std::string & strBuf)\n") 
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj  obj;\n") 
	fileOrm.write(twoTab + "ToBson(obj);\n") 
	fileOrm.write(twoTab + "CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);\n")  
	
	fileOrm.write(oneTab + "}\n\n") 
		 
def GenerateOrmCollectionCppToBson2(fileOrm , collectionTable): 
	fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::ToBson(mongo::BSONObj  & obj)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObjBuilder builder(1 * 1024 * 1024);\n")  
	
	for index , slave in collectionTable.slaves.items(): 
		if slave.ignore_collection == None:
			fileOrm.write(twoTab + "if(m_p" + slave.name + " != NULL)\n")
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "mongo::BSONObj obj;\n")
			fileOrm.write(threeTab + "m_p" + slave.name + "->ToBson(obj);\n")
			fileOrm.write(threeTab + "builder.append(" + slave.name + "::TableName() , obj);\n")
			fileOrm.write(twoTab + "}\n\n") 
		 
	for index , slave in collectionTable.slaveTables.items():  
		if slave.ignore_collection == None:
			fileOrm.write(twoTab + "{\n")
			fileOrm.write(threeTab + "mongo::BSONArrayBuilder objBuilder;\n")
			fileOrm.write(threeTab + "this->Save" + slave.name + "(objBuilder);\n")
			fileOrm.write(threeTab + "objBuilder.append(" + slave.name + "::TableName() , objBuilder.obj());\n")
			fileOrm.write(twoTab + "}\n\n") 
		 
	fileOrm.write(twoTab + "obj = builder.obj();\n\n") 
	fileOrm.write(oneTab + "}\n\n")  
	
def GenerateOrmCollectionCppFromBson(fileOrm , collectionTable): 
	fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::FromBson(std::string & compressedBuf)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "std::string tmpbuf;\n") 
	fileOrm.write(twoTab + "CUtil::Uncompress(compressedBuf.c_str(),(UINT32)compressedBuf.length(),tmpbuf);\n") 
	fileOrm.write(twoTab + "this->FromBson(tmpbuf.c_str(),tmpbuf.length());\n")  
	
	fileOrm.write(oneTab + "}\n\n")
	
	
def GenerateOrmCollectionCppFromBson2(fileOrm , collectionTable): 
	fileOrm.write(oneTab + "void " + collectionTable.name + "Collection::FromBson(const char* buf,UINT32 len)\n")
	fileOrm.write(oneTab + "{\n")
	
	fileOrm.write(twoTab + "mongo::BSONObj obj(buf);\n") 
	fileOrm.write(twoTab + "Assert(obj.objsize() == len);\n") 
	fileOrm.write(twoTab + "mongo::BSONObjIterator  iter(obj); \n") 
	fileOrm.write(twoTab + "while(iter.more())\n") 
	fileOrm.write(twoTab + "{\n") 
	
	fileOrm.write(threeTab + "mongo::BSONElement be = iter.next();\n")
	fileOrm.write(threeTab + "if(!be.isABSONObj()){ continue; }\n")
	fileOrm.write(threeTab + "mongo::BSONObj tmpobj = be.Obj();\n")
	fileOrm.write(threeTab + "INT64 hash = CUtil::BKDRHashSum(be.fieldName());\n")
	fileOrm.write(threeTab + "switch(hash)\n")
	fileOrm.write(threeTab + "{\n")
	
	for index , slave in collectionTable.slaves.items():
		if slave.ignore_collection == None:
			fileOrm.write(threeTab + "case " + GetBKDRHash(slave.name) + ": // " + slave.name + "\n")
			fileOrm.write(fourTab + "{\n")
			
			fileOrm.write(fiveTab + "Get" + slave.name +"()->FromBson(tmpobj);\n") 
			
			fileOrm.write(fourTab + "}break;\n") 
		
	for index , slave in collectionTable.slaveTables.items():
		if slave.ignore_collection == None:
			fileOrm.write(threeTab + "case " + GetBKDRHash(slave.name) + ": // " + slave.name + "\n")
			fileOrm.write(fourTab + "{\n")
			
			fileOrm.write(fiveTab + "Load" + slave.name +"(tmpobj);\n") 
			
			fileOrm.write(fourTab + "}break;\n") 
		
	fileOrm.write(threeTab + "default:\n")
	fileOrm.write(fourTab + "{\n")	
	fileOrm.write(fiveTab + "MsgAssert(false , \" invalid table hash \");\n") 	
	fileOrm.write(fourTab + "}break;\n") 
	
	fileOrm.write(threeTab + "}\n")	
	fileOrm.write(twoTab + "}\n")			
	fileOrm.write(oneTab + "}\n\n")
			
					
def GenerateOrmsReflectionHeadFile():
	outputPath = GetOutputPath()   
	outputFilePath = outputPath + g_ormPrefix + "Reflection.h" 
	fileOrm = open(outputFilePath , "a")
	
	GenerateOrmReflectionHeader(fileOrm)
	
	for index , collectionTable in g_gameDB.tables.collectionTables.items():			
		fileOrm.write(oneTab + "extern const char * cst_slaves_of_" + collectionTable.name + "[];\n") 
		fileOrm.write(oneTab + "extern INT64 cst_slaves_hash_of_" + collectionTable.name + "[];\n\n")   
	   
	fileOrm.write(oneTab + "extern GameDB::Orm * CreateObject(bson::bo & obj);\n") 
	fileOrm.write(oneTab + "extern GameDB::Orm * CreateObject(const char * data,size_t size);\n")  
	fileOrm.write(oneTab + "\n")
		 
	fileOrm.write("}//" + g_namespace + "\n\n")
	fileOrm.write("#endif\n")	  
	fileOrm.close() 
		
	LogOutInfo("generate " + outputFilePath + " file finished.\n")
			
def  GenerateOrmReflectionHeader(fileOrm):
	WriteFileDescription(fileOrm , g_ormPrefix + "Reflection.h" , "针对slave的类产生反射.") 
	fileOrm.write("#ifndef __" + g_namespace + "_" + g_ormPrefix + "_feflection_h__\n") 
	fileOrm.write("#define __" + g_namespace + "_" + g_ormPrefix + "_feflection_h__\n")  
	fileOrm.write("#include \"GameDB/inc/Orm.h\"\n\n")   
	 
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n")   
		
def GenerateOrmsReflectionCPPFile():			
	outputPath = GetOutputPath()   
	outputFilePath = outputPath + g_ormPrefix + "Reflection.cpp" 
	fileOrm = open(outputFilePath , "a")
	
	fileOrm.write("#include \"" + g_ormPrefix + "Reflection.h\"\n\n")  	
	GenerateOrmReflectionCppHeader(fileOrm)
	GenerateOrmReflectionCppSlaves(fileOrm) 
	GenerateOrmReflectionCreateObject(fileOrm)  
	
	fileOrm.write("}//" + g_namespace + "\n\n")
	
	LogOutInfo("generate " + outputFilePath + " file finished.\n")		
			
def  GenerateOrmReflectionCppHeader(fileOrm):
	for index , table in g_gameDB.tables.tables.items(): 
		fileOrm.write("#include \"" + g_ormPrefix + table.name + ".h\"\n")  
			
	fileOrm.write("\n")  
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n") 
	 
def GenerateOrmReflectionCppSlaves(fileOrm):
	for index , collectionTable in g_gameDB.tables.collectionTables.items():   
		fileOrm.write(oneTab + "const char* cst_slaves_of_" + collectionTable.name + "[] = \n " + oneTab + "{\n") 	
		for index , slave in collectionTable.slaves.items():
			if slave.name != collectionTable.name and slave.ignore_reflection_define == None:
				fileOrm.write(twoTab + "\"" + slave.name + "\",\n") 
		for index , slave in collectionTable.slaveTables.items():
			if slave.name != collectionTable.name and slave.ignore_reflection_define == None:
				fileOrm.write(twoTab + "\"" + slave.name + "\",\n") 
		fileOrm.write(twoTab + "NULL \n" + oneTab + "};\n\n") 	
			
		fileOrm.write(oneTab + "INT64 cst_slaves_hash_of_" + collectionTable.name + "[] = \n " + oneTab + "{\n") 	
		for index , slave in collectionTable.slaves.items():
			if slave.name != collectionTable.name and slave.ignore_reflection_define == None:
				fileOrm.write(twoTab + GetBKDRHash(slave.name) + ",\n")
		for index , slave in collectionTable.slaveTables.items():
			if slave.name != collectionTable.name and slave.ignore_reflection_define == None:
				fileOrm.write(twoTab + GetBKDRHash(slave.name) + ",\n")
		fileOrm.write(twoTab + "NULL \n" + oneTab + "};\n\n") 
		
def GenerateOrmReflectionCreateObject(fileOrm): 	
	fileOrm.write(oneTab + "GameDB::Orm * CreateObject(const char * data,size_t size)\n")
	fileOrm.write(oneTab + "{\n")
	fileOrm.write(twoTab + "mongo::BSONObj obj(data);\n")
	fileOrm.write(twoTab + "MsgAssert_Re(obj.objsize() == size , NULL , \"\");\n")
	fileOrm.write(twoTab + "return CreateObject(obj);\n")
	fileOrm.write(oneTab + "}\n\n")
	
	fileOrm.write(oneTab + "GameDB::Orm * createObject(bson::bo& obj)\n")
	fileOrm.write(oneTab + "{\n")
	fileOrm.write(twoTab + "std::string metaname;\n")
	fileOrm.write(twoTab + "mongo::BSONObjIterator iter(obj);\n")
	fileOrm.write(twoTab + "while(iter.more())\n")
	fileOrm.write(twoTab + "{\n")
	fileOrm.write(threeTab + "mongo::BSONElement be = iter.next();\n")
	fileOrm.write(threeTab + "if(CUtil::strcmp(be.fieldName(),\"" + g_tableMeta + "\") == 0)\n")
	fileOrm.write(threeTab + "{\n")
	fileOrm.write(fourTab + "metaname = be.str();\n")
	fileOrm.write(fourTab + "break;\n")
	fileOrm.write(threeTab + "}\n")
	fileOrm.write(twoTab + "}\n")
	fileOrm.write(twoTab + "GameDB::Orm * result = NULL;\n")
	fileOrm.write(twoTab + "INT64 hash = CUtil::BKDRHashSum(metaname.c_str());\n")
	fileOrm.write(twoTab + "switch(hash)\n")
	fileOrm.write(twoTab + "{\n") 
	
	for index , collectionTable in g_gameDB.tables.collectionTables.items():  
		for index , slave in collectionTable.slaveTables.items():
			if slave.name != collectionTable.name :
				fileOrm.write(twoTab + "case " + g_namespace + "::" + slave.name + "::meta_hash" + ":\n")			
				fileOrm.write(threeTab + "result = new " + g_namespace + "::" + slave.name + "();\n")				
				fileOrm.write(twoTab + "break;\n")
				 						
	fileOrm.write(twoTab + "}\n\n") 	 			
	fileOrm.write(twoTab + "if(result != NULL)\n") 
	fileOrm.write(threeTab + "result->FromBson(obj);\n") 
	fileOrm.write(twoTab + "return result;\n") 		
	fileOrm.write(oneTab + "}\n") 	  
			
			

def GenerateOrmStructs():		
	outputPath = GetOutputPath()    
	outputFilePath = outputPath + g_ormPrefix + "OrmStructs.h" 
	fileOrm = open(outputFilePath , "a") 
	GenerateOrmStructsHeader(fileOrm) 
	
	for index , ormStruct in g_gameDB.tables.structs.items():   
		fileOrm.write(oneTab + "class " + ormStruct.name + ": public CUtil::Marshal\n") 
		fileOrm.write(oneTab + "{ \n")
		fileOrm.write(twoTab + "public:\n")
		WriteDefineParamsWithoutDefault(fileOrm , ormStruct.params) 
		
		fileOrm.write("\n")
		fileOrm.write(twoTab + ormStruct.name + "()\n")
		WriteDefineParamsWithDefault(fileOrm , ormStruct.params)  
		fileOrm.write(threeTab + "{}\n")				
		
		WriteOrmStructCopyFunc(fileOrm , ormStruct)
		WriteOrmStructIsEqual(fileOrm , ormStruct)
		WriteOrmStructOperatorEqualAndNotEqual(fileOrm , ormStruct) 
		
		WriteOrmStructMarshal(fileOrm , ormStruct)
		WriteOrmStructunMarshal(fileOrm , ormStruct) 
		
		fileOrm.write(oneTab + "}; \n \n")
	
	fileOrm.write("}//" + g_namespace + "\n\n")
	
	fileOrm.write("namespace CUtil\n")
	fileOrm.write("{ \n")
	fileOrm.write(oneTab + "enum PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINES \n")
	fileOrm.write(oneTab + "{ \n")
	fileOrm.write(twoTab + "PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_FIRST = CUtil::PARAMETER_TYPE_USER_DEFINE + 156 ,\n")			
	for index , ormStruct in g_gameDB.tables.structs.items():   
		fileOrm.write(twoTab + "PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_" + ormStruct.name + ",\n")  
	fileOrm.write(oneTab + "}; \n \n") 
	
	for index , ormStruct in g_gameDB.tables.structs.items():   			
		WriteParamHelper(g_namespace , fileOrm , ormStruct) 
	fileOrm.write("}\n\n")	
	
	fileOrm.write("#endif\n")				
	fileOrm.close() 
		
def  GenerateOrmStructsHeader(fileOrm):
	WriteFileDescription(fileOrm , "OrmStructs.h" , "数据库中定义的数据.") 
	fileOrm.write("#ifndef __" + g_namespace + "_orm_structs_h__\n") 
	fileOrm.write("#define __" + g_namespace + "_orm_structs_h__\n")  
	fileOrm.write("#include \"CUtil/inc/Common.h\"\n") 
	fileOrm.write("#include \"CUtil/inc/Chunk.h\"\n")
	fileOrm.write("#include \"CUtil/inc/Parameter.h\"\n")   
	fileOrm.write("#include \"CUtil/inc/ParameterHelper.h\"\n\n")   
	fileOrm.write("namespace " + g_namespace)   
	fileOrm.write("\n{\n") 
	
def WriteDefineParamsWithDefault(fileOrm , params):
	fileOrm.write(threeTab + ":")
	nCount = 0
	for index , param in params.items():  
		nCount = nCount + 1
		strParams = ""   
		strParams += " "
		strParams += param.name  
		strParams += "( "

		strParams += GetAndCheckDefaultParam(param)
		
		strParams += " ) \n" 

		if nCount != len(params):
			strParams += threeTab + ","  
		fileOrm.write(strParams)   
		
def WriteDefineParamsWithoutDefault(fileOrm , params):
	for index , param in params.items():  
		strParams = twoTab 
		strParams += param.type
		strParams += " "
		strParams += param.name 

		strParams += ";\n" 
		fileOrm.write(strParams) 
		
def WriteOrmStructCopyFunc(fileOrm , ormStruct):
	fileOrm.write(twoTab + ormStruct.name + "(const " + ormStruct.name + " & val)\n" )
	fileOrm.write(twoTab + "{ \n")
	strParam = ""
	for index , param in ormStruct.params.items():  
		strParam = strParam + threeTab + param.name + " = val." + param.name + ";\n"
	fileOrm.write(strParam)
	fileOrm.write(twoTab + "} \n\n")

def WriteOrmStructIsEqual(fileOrm , ormStruct):
	fileOrm.write(twoTab + "bool IsEqual(const " + ormStruct.name + " & val)\n" )
	fileOrm.write(twoTab + "{ \n")
	strParam = threeTab + "if(\n"
	
	count = 0
	for index , param in ormStruct.params.items():  
		count = count + 1
		strParam = strParam + fourTab + param.name + " == val." + param.name
		if count == len(ormStruct.params):
			strParam = strParam +  ")\n"
		else:
			strParam = strParam + "&&\n"
			
	strParam = strParam + threeTab + "{\n"	
	strParam = strParam + fourTab + "return true;\n"	
	strParam = strParam + threeTab + "}\n"	
	strParam = strParam + threeTab + "return false;\n"	
	fileOrm.write(strParam)
	fileOrm.write(twoTab + "} \n\n")
	
def WriteOrmStructOperatorEqualAndNotEqual(fileOrm , ormStruct):
	fileOrm.write(twoTab + "bool operator == (const " + ormStruct.name + " & val)\n" )
	fileOrm.write(twoTab + "{ \n")
	fileOrm.write(threeTab + "return IsEqual(val);\n") 
	fileOrm.write(twoTab + "} \n\n")
	
	fileOrm.write(twoTab + "bool operator != (const " + ormStruct.name + " & val)\n" )
	fileOrm.write(twoTab + "{ \n") 
	fileOrm.write(threeTab + "return !IsEqual(val);\n") 
	fileOrm.write(twoTab + "} \n\n")	
	
def WriteOrmStructMarshal(fileOrm , ormStruct):
	fileOrm.write(twoTab + "virtual CUtil::CStream & marshal(CUtil::CStream & cs) const override\n" )
	fileOrm.write(twoTab + "{ \n")
	strParam = threeTab + "cs"
	for index , param in ormStruct.params.items():  
		strParam = strParam + " << " + param.name
	strParam += ";\n"
	fileOrm.write(strParam)
	fileOrm.write(threeTab + "return cs; \n")
	fileOrm.write(twoTab + "} \n\n")
	
def WriteOrmStructunMarshal(fileOrm , ormStruct):
	fileOrm.write(twoTab + "virtual CUtil::CStream & unMarshal(CUtil::CStream & cs) override\n" )
	fileOrm.write(twoTab + "{ \n")
	strParam = threeTab + "cs"
	for index , param in ormStruct.params.items():  
		strParam = strParam + " >> " + param.name
	strParam += ";\n"
	fileOrm.write(strParam)
	fileOrm.write(threeTab + "return cs; \n")
	fileOrm.write(twoTab + "} \n\n")

def WriteParamHelper(namespace , fileOrm , ormStruct):
	fileOrm.write(oneTab + "GEN_PARAMTER_HELPER(" + namespace + "::" + ormStruct.name + " , PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_" + ormStruct.name + ");\n") 			
			
			 
################################流程无关函数处理#####################################
def Usage():
    print('PyTest.py usage:')
    print('-h,--help: print help message.')
    print('-v, --version: print script version')
    print('-o, --output: input an output verb')
    print('--foo: Test option ')
    print('--fre: another test option')

def Version():
	print('GenerateGameDB.py 1.0.0.0.1')

def LogOutDebug(*string):
	longStr = "debug: "
	for item in range(len(string)):  
		longStr += str(string[item])

	print(longStr)
	pass

def LogOutInfo(*string):
	longStr = "info: "
	for item in range(len(string)):  
		longStr += str(string[item])
	
	print(longStr)
	
def LogOutError(*string):
	longStr = "error: "
	for item in range(len(string)):  
		longStr += str(string[item])
	
	print(longStr)
	sys.exit()
	
def GetBKDRHash(name):
	sum = 0
	seed = 131 #31 131 1313 13131 131313 etc
	hash = 0
	
	for index  , char in enumerate(name):
		sum += ord(char)
		hash = hash * seed + ord(char)
	
	return str((sum << 32) + (hash & 0x7FFFFFFF))
	
def GetMasterIDFieldNameINT64(table , type):
	for index  , field in table.fields.items():
		if field.masterID != None and (field.type.lower() == "INT64".lower() or field.type.lower() == "long long".lower()):
			if type == 0:
				return field.name + " = llID;"
			else:
				return "return " + field.name + ";"
	
	if type == 0:
		return ""
	else:
		return "return -1;"
	
def GetMasterIDFieldNameString(table , type):
	for index  , field in table.fields.items():
		if field.masterID != None and (field.type.lower() == "std::string".lower() or field.type.lower() == "std_string".lower()):
			if type == 0:
				return field.name + " = pID;"
			else:
				return "return " + field.name + ";"

	if type == 0:
		return ""
	else:
		return "return \"\";"
	
def WriteFileDescription(fileOrm , file , desc):
	fileOrm.write("/************************************" + "\n")
	fileOrm.write("FileName	:	" + file + "\n")
	fileOrm.write("Author		:	generate by tools" + "\n")
	fileOrm.write("HostName	:	" + socket.gethostname() + "\n")
	fileOrm.write("IP			:	" + socket.gethostbyname(socket.gethostname()) + "\n")
	fileOrm.write("Version		:	0.0.1" + "\n")
	fileOrm.write("Date		:	" + time.strftime('%Y-%m-%d %H:%M:%S') + "\n")
	fileOrm.write("Description	:	" + desc + "\n")
	fileOrm.write("************************************/" + "\n")


#检查在RPCServerName中是否存在这样的服务器
def CheckInRpcServerName(name):
	for index , rpcServerName in g_gameDB.rpcServerNames.items():
		if rpcServerName.serverName == name: 
			return True

	LogOutError("no this serverName in serverNamelist") 

#通过服务器名字获取命名空间
def GetServerNamespaceByName(strName):
	for index , rpcServerName in g_gameDB.rpcServerNames.items():
		if rpcServerName.serverName == strName: 
			return rpcServerName.namespace
			
	LogOutError("GetServerNamespaceByName error. serverName:" + strName) 
	
def GetOutputPath():
	if os.path.exists(g_gameDB.config.outputPath):  
		return g_gameDB.config.outputPath 

	LogOutError("path error" , g_gameDB.config.outputPath) 

def IsPathExist(path):
	if os.path.exists(path):
		return True
	else:
		return False

def MakeSpecialDefaultParam(param):
	if param.default == None or param.default == "":
		for index , paramSpecial in g_gameDB.specialDefaultParams.items():
			if param.type.lower().find(index.lower()) != -1:
				if paramSpecial.specialType == g_paramTypeSTL:
					param.type = paramSpecial.type + param.type[len(paramSpecial.type):]
					param.default = paramSpecial.type + param.type[len(paramSpecial.type):] + paramSpecial.suffix 
					g_gameDB.defaultParamsList[param.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(param.type)
					g_gameDB.refers[param.type] = "&"
	return param
			
def GetSpecialDefaultParamType(type):
	for index , paramSpecial in g_gameDB.specialDefaultParams.items():
		if type.lower().find(index.lower()) != -1:
			return paramSpecial.specialType
				
	return -1
	
def ReplaceSpecialCharToUnderlineFromType(type):
	return type.replace(' ','_').replace(':' , '_').replace('(' ,'_').replace(')' ,'_').replace(',' ,'_').replace('<' ,'_').replace('>' ,'_')
	
def ReplaceSpecialCharForContainerFromType(type):
	return type.replace('[' ,'<').replace(']' ,'>')
	
def IsInRefers(refer):
	for index , referData in g_gameDB.refers.items(): 
		if refer.lower() == index.lower():
			return True
	
	return False
	
#未用上
def GetParamsIncludeDefaultParam(params , type = 0):
	strParams = ""
	
	if len(params) != 0:		#如果有参数,则第一个加上 ,
		strParams += ", "
		
	nCount = 0
	for index , param in params.items(): 
		nCount += 1

		strParams = strParams + param.type
		if param.refer == "&":
			strParams = strParams + " "
			strParams = strParams + "&"
		strParams = strParams + " "
		strParams = strParams + param.name
		strParams = strParams + " = "

		strParams = strParams + GetAndCheckDefaultParam(param)
		
		if type == 0:
			if nCount != len(params):              #默认最后一个不加
				strParams = strParams + " , " 
		else: #其他类型每一个都加
			strParams = strParams + " , " 
			
	return strParams
	
def GetSpecialParamsIncludeDefaultParam(params , type = 0):
	strParams = ""
	
	if len(params) != 0:		#如果有参数,则第一个加上 ,
		strParams += ", "
		
	nCount = 0
	for index , param in params.items(): 
		nCount += 1

		strParams = strParams + param.type 
			
		if param.refer == "&":
			strParams = strParams + " "
			strParams = strParams + "&"
		strParams = strParams + " "
		strParams = strParams + param.name
		strParams = strParams + " = "

		strParams = strParams + GetAndCheckDefaultParam(param)
		
		if type == 0:
			if nCount != len(params):              #默认最后一个不加
				strParams = strParams + " , " 
		else: #其他类型每一个都加
			strParams = strParams + " , " 
			
	return strParams
	
def IsInDefaultParams(type):
	for index , defaultParam in g_gameDB.defaultParams.items():
		if defaultParam.type == type:
			return True
	
	return False

def GetDefaultParamsType(theSameType):
	for index , defaultParam in g_gameDB.defaultParams.items():
		if index.lower() == theSameType.lower():
			return index
			
	return theSameType
			
def GetRpcSpecialParamsIncludeDefault(call):
	strParams = ""

	nCount = 0
	for index , param in call.params.items():
		strParams += " , "
		
		nCount = nCount + 1
		defaultParam = GetDefaultParamDefineValue(param)
		
		strParams += defaultParam

	return strParams
	
#未用上
def GetParamsExcludeDefaultParam(params , type = 0):
	strParams = ""
	
	if len(params) != 0:
		strParams += ", "
		
	nCount = 0
	for index , param in params.items(): 
		nCount += 1

		strParams = strParams + param.type
		if param.refer == "&":
			strParams = strParams + " "
			strParams = strParams + "&"
		strParams = strParams + " "
		strParams = strParams + param.name
		strParams = strParams + "/* = "

		strParams = strParams + GetAndCheckDefaultParam(param)

		strParams += "*/"
 
		if type == 0:
			if nCount != len(params):              #默认最后一个不加
				strParams = strParams + " , " 
		else: #其他类型每一个都加
			strParams = strParams + " , " 
			
	return strParams

def GetSpecialParamsExcludeDefaultParam(params , type = 0):
	strParams = ""
	
	if len(params) != 0:
		strParams += ", "
		
	nCount = 0
	for index , param in params.items(): 
		nCount += 1

		strParams = strParams + param.type
			
		if param.refer == "&":
			strParams = strParams + " "
			strParams = strParams + "&"
		strParams = strParams + " "
		
		strParams += param.name 
		strParams = strParams + "/* = "

		strParams = strParams + GetAndCheckDefaultParam(param)

		strParams += "*/"
 
		if type == 0:
			if nCount != len(params):              #默认最后一个不加
				strParams = strParams + " , " 
		else: #其他类型每一个都加
			strParams = strParams + " , " 
			
	return strParams

def GetParamsExcludeDefaultParamAndType(params , type = 0):
	strParams = ""
	if len(params) != 0:
		strParams += ", "
		
	nCount = 0
	for index , param in params.items(): 

		nCount += 1
		strParams += param.name

		if type == 0:
			if nCount != len(params):              #默认最后一个不加
				strParams = strParams + " , " 
		else: #其他类型每一个都加
			strParams = strParams + " , " 			
		
	return strParams

def GetParamsExcludeDefault(params):
	strParams = ""
	nCount = 0
	for index , param in params.items(): 
		nCount += 1

		strParams = strParams + param.type
		if param.refer == "&":
			strParams = strParams + " "
			strParams = strParams + "&"
		strParams = strParams + " "
		strParams = strParams + param.name
		strParams = strParams + "/* = "

		strParams = strParams + GetAndCheckDefaultParam(param)

		strParams += "*/"
 
		if nCount != len(params):
			strParams = strParams + " , " 

	return strParams

def GetParamsExcludeDefaultAndType(params):
	strParams = ""
	nCount = 0
	for index , param in params.items(): 

		nCount += 1
		strParams = strParams + param.name

		if nCount != len(params):
			strParams = strParams + " , " 

	return strParams

def GetAndCheckDefaultParam(param):
	if param.default != None and param.default != "":
		return param.default
		
	for index , defaultParam in g_gameDB.defaultParams.items():
		if defaultParam.type == param.type: 
			return defaultParam.value

	LogOutError("no this value in defaultParamsList") 

def DeleteGameDBGenerateFiles():
	outputPath = GetOutputPath()
	for index , table in g_gameDB.tables.tables.items():   
		outputFilePath = outputPath + g_ormPrefix + table.name + ".h" 
		if os.path.exists(outputFilePath): 
			os.remove(outputFilePath) 
		outputFilePath = outputPath + g_ormPrefix + table.name + ".cpp" 
		if os.path.exists(outputFilePath): 
			os.remove(outputFilePath) 
			
	for index , table in g_gameDB.tables.collectionTables.items():    
		outputFilePath = outputPath + g_ormPrefix + table.name + "Collection.h"  
		if os.path.exists(outputFilePath): 
			os.remove(outputFilePath) 
		outputFilePath = outputPath + g_ormPrefix + table.name + "Collection.cpp"  
		if os.path.exists(outputFilePath): 
			os.remove(outputFilePath) 
			
	outputFilePath = outputPath + g_ormPrefix + "Reflection.h" 
	if os.path.exists(outputFilePath): 
		os.remove(outputFilePath) 
		
	outputFilePath = outputPath + g_ormPrefix + "Reflection.cpp" 
	if os.path.exists(outputFilePath): 
		os.remove(outputFilePath) 
		
	outputFilePath = outputPath + g_ormPrefix + "OrmStructs.h" 
	if os.path.exists(outputFilePath): 
		os.remove(outputFilePath) 
		
def CreateOutputPath(): 
	if False == os.path.exists(g_gameDB.config.outputPath):
		LogOutDebug(g_gameDB.config.outputPath)
		os.makedirs(g_gameDB.config.outputPath)
			
def GetDefaultParamDefineValue(param):
	for index , paramDefault in g_gameDB.defaultParamsList.items():
		if param.type == index:
			return paramDefault 
			
	LogOutError("defaultParamsList does't has this defaultParam type." , param.type  , " index:" ,g_gameDB.defaultParamsList) 

def GetDefaultParamValue(type):
	for index , paramDefault in g_gameDB.defaultParams.items():
		if type == index:
			return paramDefault.value
			
	if GetSpecialDefaultParamType(type) == g_paramTypeSTL:
		return 
		
	LogOutError("defaultParamsList does't has this defaultParam type." , param.type  , " index:" ,g_gameDB.defaultParamsList) 
	
def IsSystemDefineType(type): 
	if type.lower()  == "SINT8".lower() or type.lower()  == "UINT8".lower() or\
		type.lower()  == "INT16".lower() or type.lower()  == "UINT16".lower() or\
		type.lower()  == "INT32".lower() or type.lower()  == "UINT32".lower() or\
		type.lower()  == "INT64".lower() or type.lower()  == "INT64".lower() or\
		type.lower()  == "double".lower() or type.lower()  == "float".lower() or \
		type.lower()  == "char".lower() or type.lower()  == "unsigned int".lower() or \
		type.lower()  == "short".lower() or type.lower()  == "unsigned short".lower() or \
		type.lower()  == "long".lower() or type.lower()  == "time_t".lower() or \
		type.lower()  == "std_string".lower() or type.lower()  == "std::string".lower() or \
		type.lower()  == "int".lower():
		return True
	
	return False

def IsUserDefineType(type): 
	if GetParamType(type)  == g_paramTypeSTL or GetParamType(type) == g_paramTypeStruct:
		return True
	
	return False

def GetParamType(type): 
	return g_gameDB.defaultParams[type].paramType

def GetMasterTableTypeFromName(name): 
	for index , table in g_gameDB.tables.tables.items():
		if table.name == name:
			for index2 , field in table.fields.items():
				if field.masterID != None:
					return field.type
	
	return None
	
def GetMasterTableFromName(name): 
	for index , table in g_gameDB.tables.tables.items():
		if table.name == name:
			for index2 , field in table.fields.items():
				if field.masterID != None:
					return table
	
	return None
	
def CheckMasterOrPrimaryIDRight(type):
	if type.lower() == "std::string".lower() or\
		type.lower() == "std_string".lower() or\
		type.lower() == "INT64".lower() or\
		type.lower() == "long long".lower():
		return True
	
	return False
	 
def IsHasSameData(dic , name):
	for index , param in dic.items():
		if name == index:
			return True 
	return False
	
################################main函数处理#####################################
def handleArgs(argv): 
	global g_gameDBPath
	try:
		opts, args = getopt.getopt(argv[1:], 'hvo:', ['import='])
	except: 
		Usage()
		sys.exit(2) 
	if len(argv) == 1: 
		g_gameDBPath = "testRpc.xml"
		return
	elif len(argv) == 2:  
		g_gameDBPath = argv[1]
		return  
	for o, a in opts:
		if o in ('-h', '--help'):
			Usage()
			sys.exit(1)
		elif o in ('-v', '--version'):
			Version()
			sys.exit(0) 
		elif o in ('--import',):
			g_gameDBPath = a
		elif o in ('--fre',):
			Fre=a
		else:
			print('unhandled option')
			sys.exit(3) 
			
def main(argv):
	handleArgs(argv)
	LogOutInfo("start generate rpc from path:" + g_gameDBPath) 
	start()  
	LogOutInfo("complete generate rpc.") 
	
if __name__ == '__main__': 
	main(sys.argv)


