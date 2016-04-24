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
 
#g_platform = "LINUX"
g_platform = "WIN64"
if g_platform == "WIN64":
	import ctypes
	
g_yellow = ""
g_yellow_h = ""
g_green = ""
g_green_h = ""
g_red = ""
g_original = ""
g_cyan = ""
g_stdInputHandle = -10
g_stdOutputHandle = -11
g_stdErrorHandle = -12

#windows下的颜色.
FOREGROUND_BLACK = 0x0
FOREGROUND_BLUE = 0x01
FOREGROUND_GREEN = 0x02
FOREGROUND_RED = 0x04
FOREGROUND_INTENSTITY = 0x08

BACKGROUND_BLUE = 0x20
BACKGROUND_GREEN = 0x30
BACKGROUND_RED = 0x40
BACKGROUND_INTENSTITY = 0x80

g_rpcXmlPath = "GameDB.xml"

#rpc的类型
g_targetTypeClient = 1
g_targetTypeProxy  = 2
g_targetTypeServer = 3
g_specialParamTypeSTL = '0' #STL类型
################################类定义#####################################
class ParentPoint:
	def __init__(self, parentPoint): 
		#self.parentPoint = parentPoint  
		pass
 
class RpcMsgs(ParentPoint):
	def __init__(self): 
		super(RpcMsgs , self).__init__(self)
		self.defaultParams = collections.OrderedDict()
		self.specialDefaultParams = collections.OrderedDict()
		self.defaultParamsList = collections.OrderedDict()
		self.rpcs = Rpcs(self)
		self.rpcServerNames = collections.OrderedDict()
		self.refers = collections.OrderedDict()

class RpcServerName(ParentPoint):
	"""docstring for RpcServerName"""
	def __init__(self, parentPoint):
		super(RpcServerName, self).__init__(parentPoint)
		self.serverName = None
		self.outputPath = None
		self.include = None
		self.namespace = None
		self.rpcInterface = None 

class DefaultParam(ParentPoint):
	def __init__(self, parentPoint): 
		super(DefaultParam , self).__init__(parentPoint) 
		self.type = None
		self.value = None

class SpecialDefaultParam(ParentPoint):
	def __init__(self, parentPoint): 
		super(SpecialDefaultParam , self).__init__(parentPoint) 
		self.type = None
		self.prefix = None
		self.suffix = None
		self.specialType = None
		
class Rpcs(ParentPoint):
	def __init__(self , parentPoint):
		super(Rpcs , self).__init__(parentPoint)  
		self.rpcs = collections.OrderedDict()
		self.rpcDatas = collections.OrderedDict() 

class Rpc(ParentPoint):
	def __init__(self , parentPoint):
		super(Rpc , self).__init__(parentPoint) 
		self.call = Call(self)  #这里calls为dic保存所有的call
		self.returns = Return(self)  #这里returns为dic保存所有的return
		self.name = None
		self.syncType = 0
		self.timeout = 10
		self.targets = collections.OrderedDict()
		# self.client = None
		# self.server = None
		# self.proxy = None
		# self.serverClass = None
		# self.serverInclude = None
		# self.proxyInclude = None
		# self.proxyClass = None
		# self.clientInclude = None
		# self.clientClass = None
		
class TargetAttr(ParentPoint):
	def __init__(self , parentPoint):
		super(TargetAttr , self).__init__(parentPoint)  
		self.targetType = None 
		self.classes = None
		self.include = None 
		self.name = None
		
class RpcData(ParentPoint):
	def __init__(self , parentPoint):
		super(RpcData , self).__init__(parentPoint)
		self.params = collections.OrderedDict()   #这里存放所有的params.用dic结构
		self.name = None

class Call(ParentPoint):
	def __init__(self , parentPoint):
		super(Call , self).__init__(parentPoint) 
		self.params = collections.OrderedDict()  #这里的参数同上

class Return(ParentPoint):
	def __init__(self , parentPoint):
		super(Return , self).__init__(parentPoint)
		self.name = None
		self.params = collections.OrderedDict() #这里的参数同上

class Param(ParentPoint):
	def __init__(self , parentPoint):
		super(Param , self).__init__(parentPoint)
		self.name = None
		self.type = None
		self.default = None
		self.refer = None
		self.unrefer = None

################################函数解析XML内容#####################################

g_rpcMsgs=RpcMsgs() 
g_rpcRecords = collections.OrderedDict()   #记录所以得RPC

def start(): 
	ParseRpcs() 
	CheckAllParams() 
	LogOutInfo("ParseRpcs finished.\n") 
	
	SortRpcs(g_rpcMsgs)
	
	GenerateRpc()  
	LogOutInfo("generate Rpc finished.\n") 
	
noneDir = collections.OrderedDict()
def SortRpcs(sortObj):
	for name , value in vars(sortObj).items():   
		if isinstance(value , dict): 
			i = 0
			for key , val in value.items(): 
				i = i + 1
				if IsSortNext(val): 
					SortRpcs(val)
				else:
					sorted(value.items() , key=lambda e:e[i] , reverse=True)  
					break 

		elif IsSortNext(value): 
			SortRpcs(value)

def IsSortNext(value): 
	if isinstance(value , RpcServerName) or \
		isinstance(value , RpcMsgs) or \
		isinstance(value , DefaultParam) or \
		isinstance(value , Rpcs) or \
		isinstance(value , Rpc) or \
		isinstance(value , RpcData) or \
		isinstance(value , Call) or \
		isinstance(value , Return) or \
		isinstance(value , Param) or \
		isinstance(value , TargetAttr) or \
		isinstance(value , SpecialDefaultParam) or \
		isinstance(value , Rpc)   : 

		return True

	return False 

def ParseRpcs():  
#使用ElementTree读取节点
	rpcDoc = ElementTree(None , g_rpcXmlPath)  
	xmlRpcMsgs = rpcDoc.getroot() 
	handleRpcMsgs(xmlRpcMsgs) 

def handleRpcMsgs(xmlRpcMsgs):
	for xmlData in iter(xmlRpcMsgs.getchildren()): 
		if xmlData.tag.lower() == "DefaultParams".lower():
			handleDefaultParams(g_rpcMsgs.defaultParams , g_rpcMsgs.defaultParamsList , xmlData)
		if xmlData.tag.lower() == "LongDefaultParams".lower():
			handleLongDefaultParams(g_rpcMsgs.defaultParams , g_rpcMsgs.defaultParamsList , xmlData)
		if xmlData.tag.lower() == "SpecialDefaultParam".lower():
			handleSpecialDefaultParams(g_rpcMsgs.specialDefaultParams , xmlData)
		if xmlData.tag.lower() == "Refers".lower():
			handleRefers(g_rpcMsgs.refers , xmlData)
		if xmlData.tag.lower() == "Rpcs".lower():
			handleRpcs(g_rpcMsgs.rpcs , xmlData)
		if xmlData.tag.lower() == "RpcServerName".lower():
			handleRpcServerName(g_rpcMsgs.rpcServerNames , xmlData)
			
def handleRefers(refers , xmlRefers):
	for xmlData in iter(xmlRefers.getchildren()): 
		if xmlData.tag.lower() == "Refer".lower():
			handleRefer(refers , xmlData) 

def handleRefer(refers , xmlData): 
	for attr in iter(xmlData.attrib):
		if attr.lower() == "name".lower():
			refers[xmlData.attrib[attr]] = "&"  

def handleDefaultParams(defaultParams , defaultParamsList, xmlDefaultParams):
	for attr in iter(xmlDefaultParams.attrib):
		defaultParam = DefaultParam(defaultParams)
		defaultParam.type = attr
		defaultParam.value = xmlDefaultParams.attrib[attr]
			
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
			
	defaultParams[defaultParam.type] = defaultParam
#	g_rpcMsgs.defaultParamsList[defaultParam.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(defaultParam.type)

def handleRpcServerName(rpcServerNames , xmlRpcServerName):
	rpcServerName = RpcServerName(g_rpcMsgs)

	for attr in iter(xmlRpcServerName.attrib):
		if attr.lower() == "serverName".lower():
			rpcServerName.serverName = xmlRpcServerName.attrib[attr]
		if attr.lower() == "outputPath".lower():
			rpcServerName.outputPath = xmlRpcServerName.attrib[attr]
		if attr.lower() == "include".lower():
			rpcServerName.include = xmlRpcServerName.attrib[attr]
		if attr.lower() == "namespace".lower():
			rpcServerName.namespace = xmlRpcServerName.attrib[attr]
		if attr.lower() == "rpcInterface".lower():
			rpcServerName.rpcInterface = xmlRpcServerName.attrib[attr]

	if IsHasSameData(rpcServerNames , rpcServerName.serverName):
		LogOutError("Parase rpcServerNames :" , rpcServerName.serverName , "has same rpcServerName.") 
		
	g_rpcRecords[rpcServerName.namespace] = collections.OrderedDict()
	g_rpcRecords[rpcServerName.namespace]["GRpc"] = collections.OrderedDict()
	rpcServerNames[rpcServerName.serverName] = rpcServerName

def handleRpcs(rpcs , xmlRpcs):
	for xmlData in iter(xmlRpcs.getchildren()): 
		if xmlData.tag.lower() == "RpcData".lower():
			handleRpcData(rpcs.rpcDatas , xmlData)
		if xmlData.tag.lower() == "Rpc".lower():
			handleRpc(rpcs.rpcs , xmlData)
	
def handleRpc(rpcs , xmlRpc): 
	rpc = Rpc(rpcs)

	targetAttrClient = TargetAttr(rpc)
	targetAttrProxy = TargetAttr(rpc)
	targetAttrServer = TargetAttr(rpc)
	for attr in iter(xmlRpc.attrib):
		if attr.lower() == "name".lower():
			rpc.name = xmlRpc.attrib[attr]
		if attr.lower() == "syncType".lower():
			rpc.syncType = xmlRpc.attrib[attr]
		if attr.lower() == "timeout".lower():
			rpc.timeout = xmlRpc.attrib[attr]
		if attr.lower() == "serverInclude".lower():
			targetAttrServer.include = xmlRpc.attrib[attr]
		if attr.lower() == "serverClass".lower():
			targetAttrServer.classes = xmlRpc.attrib[attr]
		if attr.lower() == "server".lower():
			targetAttrServer.name = xmlRpc.attrib[attr]
		if attr.lower() == "proxy".lower():
			targetAttrProxy.name = xmlRpc.attrib[attr]
		if attr.lower() == "proxyInclude".lower():
			targetAttrProxy.include = xmlRpc.attrib[attr]
		if attr.lower() == "proxyClass".lower():
			targetAttrProxy.classes = xmlRpc.attrib[attr]
		if attr.lower() == "clientInclude".lower():
			targetAttrClient.include = xmlRpc.attrib[attr]
		if attr.lower() == "clientClass".lower():
			targetAttrClient.classes = xmlRpc.attrib[attr]
		if attr.lower() == "client".lower():
			targetAttrClient.name = xmlRpc.attrib[attr] 

	if IsHasSameData(rpcs , rpc.name):
		LogOutError("Parase Rpc :" , rpc.name , "has same rpcName.") 
					
	if targetAttrClient.name != "" and targetAttrClient.name != None:
		if targetAttrClient.classes == None:
			targetAttrClient.classes = "GRpc"
			targetAttrClient.include = "GRpc.h"
		targetAttrServer.targetType = g_targetTypeClient
		CheckInRpcServerName(targetAttrClient.name)
		rpc.targets[targetAttrClient.name + targetAttrClient.classes + str(targetAttrClient.targetType) ] = targetAttrClient
		AddToGlobalTargets(targetAttrClient , rpc)		
		
	if targetAttrProxy.name != "" and targetAttrProxy.name != None:
		if targetAttrProxy.classes == None:
			targetAttrProxy.classes = "GRpc"
			targetAttrProxy.include = "GRpc.h"
		targetAttrServer.targetType = g_targetTypeProxy
		CheckInRpcServerName(targetAttrProxy.name)
		rpc.targets[targetAttrProxy.name + targetAttrProxy.classes + str(targetAttrProxy.targetType) ] = targetAttrProxy
		AddToGlobalTargets(targetAttrProxy , rpc)	
			
	if targetAttrServer.name != "" and targetAttrServer.name != None:
		if targetAttrServer.classes == None:
			targetAttrServer.classes = "GRpc"
			targetAttrServer.include = "GRpc.h"
		targetAttrServer.targetType = g_targetTypeServer
		CheckInRpcServerName(targetAttrServer.name)
		rpc.targets[targetAttrServer.name + targetAttrServer.classes + str(targetAttrServer.targetType) ] = targetAttrServer
		AddToGlobalTargets(targetAttrServer , rpc)	
			
	for xmlData in iter(xmlRpc.getchildren()): 
		if xmlData.tag.lower() == "Call".lower():
			handleCall(rpc.call , xmlData)
		if xmlData.tag.lower() == "Return".lower():
			handleReturn(rpc.returns , xmlData)
		if xmlData.tag.lower() == "Targets".lower():
			handleTargets(rpc.targets , xmlData , rpc)
			
	rpcs[rpc.name] = rpc

#这个肯定是唯一的
def handleTargets(targets , xmlTarget, rpc): 			
	for xmlData in iter(xmlTarget.getchildren()): 
		if xmlData.tag.lower() == "Client".lower():
			handleTarget(targets , xmlData , rpc, g_targetTypeClient)
		if xmlData.tag.lower() == "Proxy".lower():
			handleTarget(targets , xmlData , rpc, g_targetTypeProxy)
		if xmlData.tag.lower() == "Server".lower():
			handleTarget(targets , xmlData, rpc, g_targetTypeServer)
				
def handleTarget(targets , xmData , rpc , targetType): 
	targetAttr = TargetAttr(rpc) 
	targetAttr.targetType = targetType
	
	for attr in iter(xmData.attrib):
		if attr.lower() == "name".lower():
			targetAttr.name = xmData.attrib[attr]
		if attr.lower() == "include".lower():
			targetAttr.include = xmData.attrib[attr]
		if attr.lower() == "class".lower():
			targetAttr.classes = xmData.attrib[attr]
			
	if targetAttr.classes == "" or targetAttr.classes == None:
		targetAttr.classes = "GRpc" 
		targetAttr.include = "GRpc.h" 
		
	CheckInRpcServerName(targetAttr.name)
	
	if IsHasSameData(targets , targetAttr.name + targetAttr.classes + str(targetType)):
		LogOutError("Parase targets :" , targetAttr.name + targetAttr.classes + str(targetType) , "has same targetAttr.") 
		
	#标示一个唯一的target.因为导致一个target不一样的有可能是类型,class,以及
	targets[targetAttr.name + targetAttr.classes + str(targetType) ] = targetAttr
	AddToGlobalTargets(targetAttr , rpc)
	
def AddToGlobalTargets(targetAttr , rpc): 
	strNamespace = GetServerNamespaceByName(targetAttr.name)
	 
	if targetAttr.classes not in g_rpcRecords[strNamespace]:
		g_rpcRecords[strNamespace][targetAttr.classes] = collections.OrderedDict()
	
	if rpc.name not in g_rpcRecords[strNamespace][targetAttr.classes] :
		g_rpcRecords[strNamespace][targetAttr.classes][rpc.name] = rpc

def handleCall(call , xmlCall): 
	for xmlData in iter(xmlCall.getchildren()): 
		if xmlData.tag.lower() == "Param".lower():
			handleParams(call.params , xmlData)

def handleReturn(_return , xmlReturn): 
	for attr in iter(xmlReturn.attrib):
		if attr.lower() == "name".lower():
			_return.name = xmlReturn.attrib[attr]

	for xmlData in iter(xmlReturn.getchildren()):
		if xmlData.tag.lower() == "Param".lower():
			handleParams(_return.params , xmlData)

def handleRpcData(rpcDatas , xmlrpcData):
	rpcData = RpcData(rpcDatas)
	for attr in iter(xmlrpcData.attrib):
		if attr.lower() == "name".lower():
			rpcData.name = xmlrpcData.attrib[attr]

	if IsHasSameData(rpcDatas , rpcData.name):
		LogOutError("Parase rpcDatas :" , rpcData.name , "has same rpcData.") 
	
	for xmlData in iter(xmlrpcData.getchildren()): 
		if xmlData.tag.lower() == "Param".lower():
			handleParams(rpcData.params , xmlData)
					
	rpcDatas[rpcData.name] = rpcData
	
	#给RPCData加上默认参数
	defaultParam = DefaultParam(g_rpcMsgs.defaultParams)
	defaultParam.type = rpcData.name
	defaultParam.value = rpcData.name + "()" 
			
	g_rpcMsgs.defaultParams[defaultParam.type] = defaultParam
	g_rpcMsgs.defaultParamsList[defaultParam.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(defaultParam.type)
	
	#给RPCData加上引用		
	g_rpcMsgs.refers[rpcData.name] = "&"
	
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
			
	if (IsInRefers(param.type) or IsInDefaultParams(param.type) != True) and param.unrefer != True:
		param.refer = "&"
	else:
		param.refer = None
		
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
	
def CheckAllParams():
	for indexRpc , rpc in g_rpcMsgs.rpcs.rpcs.items():  
		for indexCall , paramCall in rpc.call.params.items():
			if (not IsInDefaultParams(paramCall.type)) and paramCall.default == None:
				LogOutError("rpc call's param not in default params." , rpc.name , "  param.type:" , paramCall.type , paramCall.default ) 
			if IsInRefers(paramCall.type) and paramCall.unrefer != True:
				paramCall.refer = "&"
			else:
				paramCall.refer = None
				
		for indexReturn , paramReturn in rpc.returns.params.items():  
			if (not IsInDefaultParams(paramReturn.type)) and paramReturn.default == None:
				LogOutError("rpc return's  param not in default params." , rpc.name , "  param.type:" , paramReturn.type) 
				
			if IsInRefers(paramReturn.type) and paramReturn.unrefer != True:
				paramReturn.refer = "&"
			else:
				paramReturn.refer = None
				
	for indexRpcData , rpcData in g_rpcMsgs.rpcs.rpcDatas.items():  
		for index , param in rpcData.params.items():  
			if (not IsInDefaultParams(param.type)) and (param.default == None or param.default == ""):
				LogOutError("rpcData's param not in default params." , index, "  param.type:" , param.type)  
				 
			if IsInRefers(param.type) and param.unrefer != True:
				param.refer = "&"
			else:
				param.refer = None
				
				
################################生成对应文件#####################################

oneTab = "\t"
twoTab = oneTab + "\t"
threeTab = twoTab + "\t"
fourTab = threeTab + "\t"
fiveTab = fourTab + "\t"

def GenerateRpc():
	DeleteServerNameFiles() 
	LogOutInfo("deleted all files but rpc handler file finished.")
	
	CreateServerNamePath() 
	LogOutInfo("created every server name path finished.\n")

	GenerateMsgNameDefine()	
	LogOutInfo("generate MsgNameDefine.h file finished.")
	
	GenerateGlableRpc()
	LogOutInfo("generate GRpc.h file finished.")
	
	GenerateRpcRegister()
	LogOutInfo("generate RpcRegister.cpp file finished.")
	
	GenerateRpcHandlers()
	LogOutInfo("generate RpcHandler.cpp file finished.")
	
	GenerateRpcDatas()
	LogOutInfo("generate RpcData.h file finished.")
	
	GenerateRpcCallFuncs()
	LogOutInfo("generate RpcCallFuncs.h file finished.") 
	
	GenerateRPCDefines()
	LogOutInfo("generate RpcDefine.h file finished.") 

def GenerateMsgNameDefine(): 
	sameNamespace = collections.OrderedDict()
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "MsgNameDefine.h"     
		
		fileRpc = open(outputPath , "a")
		if rpcServerName.namespace not in sameNamespace :
			GenerateRPCParamDefineHeader(fileRpc , rpcServerName.namespace)
			sameNamespace[rpcServerName.namespace] = 1
		
			sameRecord = collections.OrderedDict()
			for index , rpc in g_rpcMsgs.rpcs.rpcs.items():   
				if rpc.name not in sameRecord :
					GenerateRPCParamDefine(rpc , fileRpc) 
					sameRecord[rpc.name] = 1
			
		fileRpc.close()	

	sameNamespace = collections.OrderedDict() 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "MsgNameDefine.h"     
		
		if rpcServerName.namespace not in sameNamespace : 
			fileRpc = open(outputPath , "a")
			fileRpc.write("}\n\n#endif\n\n")
			fileRpc.close()	
			sameNamespace[rpcServerName.namespace] = 1

def GenerateRPCParamDefineHeader(fileRpc , namespace):  
	WriteFileDescription(fileRpc , "MsgNameDefine.h" , "用于定义消息的全局唯一名字")
	fileRpc.write("#ifndef __msg_" + namespace + "_msg_name_define_h__\n")
	fileRpc.write("#define __msg_" + namespace + "_msg_name_define_h__\n") 
	fileRpc.write("#include \"MsgLib/inc/MsgCommon.h\" \n\n") 
	fileRpc.write("namespace Msg\n") 
	fileRpc.write("{\n") 
	

def GenerateRPCParamDefine(rpc , fileRpc): 
	fileRpc.write(oneTab + "//tool " + rpc.name + " declare here\n")
	fileRpc.write(oneTab + "RPC_DEFINE(" + rpc.name + ");\n\n")
	
def GenerateRPCDefines(): 

	rpcRecords = collections.OrderedDict()		
	sameNamespace = collections.OrderedDict()   
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "RPCDefines.h"    
		 
		fileRpc = open(outputPath , "a")
		if rpcServerName.namespace not in sameNamespace :
			GenerateRPCDefinesHeader(fileRpc , rpcServerName.namespace)			
			sameNamespace[rpcServerName.namespace] = 1  
			
			for recordIndex , rpcClasses in g_rpcRecords[rpcServerName.namespace].items():
				GenerateRPCDefine(recordIndex , rpcClasses , fileRpc , rpcServerName.namespace)						
	
		fileRpc.write("\n\n")
		fileRpc.close()	 
			
	sameNamespace = collections.OrderedDict() 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "RPCDefines.h"     
		
		if rpcServerName.namespace not in sameNamespace : 
			fileRpc = open(outputPath , "a")
			fileRpc.write("\n}\n\n#endif\n\n")
			fileRpc.close()	
			sameNamespace[rpcServerName.namespace] = 1
			
def GenerateRPCDefinesHeader(fileRpc , namespace):
	WriteFileDescription(fileRpc , "RPCDefines.h" , "RPC函数定义")
	fileRpc.write("#ifndef __msg_" + namespace + "_rpc_defines_h__\n")
	fileRpc.write("#define __msg_" + namespace + "_rpc_defines_h__\n")  
	fileRpc.write("#include \"RpcDatas.h\" \n\n") 
	fileRpc.write("namespace " + namespace + "\n") 
	fileRpc.write("{\n") 

def GenerateRPCDefine(className , rpcs , fileRpc , serverName): 
	fileRpc.write("\n#define  RPC_DEFINE_" + className + " public:\\\n")
	for rpcIndex , rpc in rpcs.items():
		strParams = GetSpecialParamsIncludeDefaultParam(rpc.call.params)  
		strReturnParams = GetSpecialParamsIncludeDefaultParam(rpc.returns.params) 
		for targetIndex , target in rpc.targets.items():
			if target.targetType == g_targetTypeClient and target.classes == className and GetServerNamespaceByName(target.name ) == serverName:
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) " + strReturnParams + ");\\\n") 
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) " + strParams + ");\\\n")
				
			if target.targetType == g_targetTypeProxy and target.classes == className and GetServerNamespaceByName(target.name ) == serverName:
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) " + strParams + ");\\\n")
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) " + strParams + ");\\\n")
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) " + strReturnParams + ");\\\n") 

			if target.targetType == g_targetTypeServer and target.classes == className and GetServerNamespaceByName(target.name ) == serverName:
				fileRpc.write(oneTab + "Msg::ObjectMsgCall * " + rpc.name + "_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID)" + strParams + ");\\\n")				
								
	GenerateObjectHaveCurFunc(fileRpc , className , rpcs)
		
def GenerateObjectHaveCurFunc(fileRpc , className , rpcs):	
	fileRpc.write("public:\\\n")
	fileRpc.write(oneTab + "static CollectionObjectFuncsT s_setFuncs;\\\n")
	fileRpc.write(oneTab + "static void InitObjectFuncs()\\\n")
	fileRpc.write(oneTab + "{\\\n")
	for index , rpc in rpcs.items(): 
		fileRpc.write(twoTab + className + "::" + "s_setFuncs.insert(\"" + rpc.name + "\");\\\n")
	fileRpc.write(oneTab + "}\\\n")
	fileRpc.write(oneTab + "virtual BOOL IsHasFunc(const std::string & strFunc)\\\n")
	fileRpc.write(oneTab + "{\\\n") 
	fileRpc.write(twoTab + "CollectionObjectFuncsT::iterator iter = " + className + "::s_setFuncs.find(strFunc);\\\n")
	fileRpc.write(twoTab + "if (iter != " + className + "::s_setFuncs.end())\\\n")
	fileRpc.write(twoTab + "{\\\n") 
	fileRpc.write(threeTab + "return TRUE;\\\n")
	fileRpc.write(twoTab + "}\\\n")
	fileRpc.write(oneTab + "return FALSE;\\\n") 
	fileRpc.write(oneTab + "}\\\n")		

	
def GenerateGlableRpc():
	sameNamespace = collections.OrderedDict()
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.namespace)  
		outputPath = outputPath + "GRpc.h"   
					
		fileRpc = open(outputPath , "a")
		if rpcServerName.namespace not in sameNamespace :
			GenerateGlableRpcHeaderNamespace(fileRpc , rpcServerName.namespace)
			sameNamespace[rpcServerName.namespace] = 1
							
		fileRpc.close()	

	sameNamespace = collections.OrderedDict()
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.namespace)  
		outputPath = outputPath + "GRpc.h"   
		
		if rpcServerName.namespace not in sameNamespace :
			fileRpc = open(outputPath , "a")
			
			GenerateGlableRpcLastNamespace(fileRpc , rpcServerName.namespace)
			sameNamespace[rpcServerName.namespace] = 1  
			fileRpc.close()	
		 
#生成GlableRpc头部部分.
def GenerateGlableRpcHeaderNamespace(fileRpc , namespace):
	WriteFileDescription(fileRpc , "GlableRpc.h" , "静态函数,无对象时通过这个类处理.") 
	fileRpc.write("#ifndef __" + namespace + "_global_rpc_h__\n") 
	fileRpc.write("#define __" + namespace + "_global_rpc_h__\n") 
#	fileRpc.write("#include \"CUtil/inc/Chunk.h\" \n") 
#	fileRpc.write("#include \"MsgLib/inc/Object.h\" \n") 
#	fileRpc.write("#include \"MsgLib/inc/RPCMsgCall.h\" \n") 
#	fileRpc.write("#include \"MsgLib/inc/RpcManager.h\" \n") 
	fileRpc.write("#include \"MsgLib/inc/IRpcMsgCallableObject.h\" \n") 
#	fileRpc.write("#include \"RpcDatas.h\" \n") 
	fileRpc.write("#include \"RpcDefines.h\" \n") 
	fileRpc.write("\n") 
	fileRpc.write("namespace " +  namespace  + "\n{\n") 
	
	fileRpc.write(oneTab + "class GRpc : public Msg::IRpcMsgCallableObject\n") 
	fileRpc.write(oneTab + "{\n") 
	fileRpc.write(twoTab + "RPC_DEFINE_GRpc;\n") 
	fileRpc.write(oneTab + "public:\n") 
	fileRpc.write(twoTab + "GRpc(Msg::Object nID , Msg::RpcManager * pRpcManager)\n") 
	fileRpc.write(threeTab + ": Msg::IRpcMsgCallableObject(nID , pRpcManager)\n") 
	fileRpc.write(twoTab + "{\n")  
	fileRpc.write(threeTab +"GRpc::InitObjectFuncs();\n")  
	fileRpc.write(twoTab +"}\n")   
	fileRpc.write(oneTab +"public:\n")  
			 
#生成GlableRpc尾部部分.
def GenerateGlableRpcLastNamespace(fileRpc , namespace):  
	fileRpc.write(oneTab + "};\n\n")
	fileRpc.write("}\n\n")
	fileRpc.write("#endif\n\n")
	
def GenerateRpcRegister():   
	#生成注册的头 
	sameNamespace = collections.OrderedDict()   
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "RpcRegister.cpp"    
 
		fileRpc = open(outputPath , "a")    
		if rpcServerName.namespace not in sameNamespace :
			GenerateRpcRegisterHeader(fileRpc , rpcServerName.namespace)
			sameNamespace[rpcServerName.namespace] = 1  
			
		fileRpc.write(oneTab + "void " + rpcServerName.rpcInterface + "::OnRegisterRpcs( void )\n")
		fileRpc.write(oneTab + "{\n")
		fileRpc.write(twoTab + "Assert(m_pRpcManager && Msg::RpcCheckParams::GetInstance());	\n")
		fileRpc.write(twoTab + "static " + rpcServerName.namespace + "::GRpc g_pGRpc( Msg::DEFAULT_RPC_CALLABLE_ID , m_pRpcManager); \n\n") 
		
		rpcRecords = collections.OrderedDict()
		for index , rpc in g_rpcMsgs.rpcs.rpcs.items():  
			fileRpc.write(twoTab + "//tool " + rpc.name + " generate default deliver and return check param here\n")
			fileRpc.write(twoTab + "{\n")
			fileRpc.write(threeTab + "CUtil::Parameters objDeliverParams , objReturnParams;\n")
			strDefaultParams = GetRpcSpecialParamsIncludeDefault(rpc.call)
			fileRpc.write(threeTab + "CUtil::GenParamHelper::GenParams(objDeliverParams " + strDefaultParams +  ");\n") 
			strDefaultParams = GetRpcSpecialParamsIncludeDefault(rpc.returns)
			fileRpc.write(threeTab + "CUtil::GenParamHelper::GenParams(objReturnParams " + strDefaultParams +  ");\n") 
			fileRpc.write(threeTab + "Msg::RpcCheckParams::GetInstance()->InsertDeliverParams(" + "\"" + rpc.name  + "\", objDeliverParams);\n") 
			fileRpc.write(threeTab + "Msg::RpcCheckParams::GetInstance()->InsertReturnParams(" + "\"" + rpc.name  +  "\", objReturnParams);\n") 
#			fileRpc.write(threeTab + "objDeliverParams.Clear();\n") 
#			fileRpc.write(threeTab + "objReturnParams.Clear();\n") 
			GenerateRpcRegisterFuncs(rpc , fileRpc , rpcServerName.serverName)
			fileRpc.write(twoTab +"}\n\n") 
			
			for index , target in rpc.targets.items():  
				if target.classes not in rpcRecords and rpcServerName.serverName == target.name:
					rpcRecords[target.classes] = 1		

		GenerateInitStaticFunc(fileRpc , rpcServerName.namespace , rpcRecords)
				
		fileRpc.write(oneTab + "}\n\n")
		
		fileRpc.close() 
		
	sameNamespace = collections.OrderedDict()
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.namespace)  
		outputPath = outputPath + "RpcRegister.cpp"    
		
		if rpcServerName.namespace not in sameNamespace :
			fileRpc = open(outputPath , "a")
			
			GenerateDefineStaticFunc(fileRpc , rpcServerName.namespace)
				
			fileRpc.write("}//" + rpcServerName.namespace + "\n\n") 
			
			sameNamespace[rpcServerName.namespace] = 1 
			fileRpc.close()	 

def GenerateInitStaticFunc(fileRpc , namespace , rpcRecords):	  
	for index , rpcs in rpcRecords.items(): 
		if index != "GRpc":
			fileRpc.write(twoTab + namespace + "::"+ index + "::" +"InitObjectFuncs();\n")
		
def GenerateDefineStaticFunc(fileRpc , namespace):	  
	for index , rpcs in g_rpcRecords[namespace].items(): 
		fileRpc.write(oneTab + "CollectionObjectFuncsT " + namespace + "::"+ index + "::" +"s_setFuncs;\n")
	
def GenerateRpcRegisterHeader(fileRpc , rpcNamespace) :
	WriteFileDescription(fileRpc , "RpcRegister.cpp" , "注册每个函数.以及检测网络传递的消息是否是正确的参数.") 
	fileRpc.write("#include \"MsgLib/inc/RpcManager.h\"\n")
	fileRpc.write("#include \"MsgLib/inc/RpcCheckParams.h\"\n") 
	fileRpc.write("#include \"CUtil/inc/Chunk.h\"\n")  
	fileRpc.write("#include \"MsgNameDefine.h\"\n")  
	fileRpc.write("#include \"GRpc.h\"\n") 
	GenerateRpcRegisterHeaderInclude(fileRpc , rpcNamespace)

	sameNamespace = collections.OrderedDict()
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():
		if rpcServerName.namespace == rpcNamespace : 
			sameNamespace[rpcServerName.serverName] = 1 
			
	for index , foo in sameNamespace.items():   
		GenerateRpcRegisterServerHeader(g_rpcMsgs.rpcs.rpcs , fileRpc , index)
 
	fileRpc.write("\n") 

	fileRpc.write("namespace " + rpcNamespace + "\n{\n")
	fileRpc.write(oneTab + "//tool defaultParams define here.\n")
	WriteDefaultParams(fileRpc)
	
def WriteDefaultParams(fileRpc):
	for index , defaultParam in g_rpcMsgs.defaultParams.items():
		fileRpc.write(oneTab + "static " + defaultParam.type + " g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(defaultParam.type) + " = " + defaultParam.value + ";\n")
#	fileRpc.write("\n")
 
	for index , defaultParam in g_rpcMsgs.defaultParamsList.items():
		if index not in g_rpcMsgs.defaultParams:
			fileRpc.write(oneTab + "static " + index + " " + defaultParam + ";\n")
	fileRpc.write("\n")
	
def GenerateRpcRegisterHeaderInclude(fileRpc , rpcNamespace): 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		if rpcNamespace == rpcServerName.namespace :
			fileRpc.write("#include \"" + rpcServerName.include + "\"\n")
	
def GenerateRpcRegisterServerHeader(rpcs , fileRpc , serverName):
	
	sameRecord = collections.OrderedDict()
	#生成所有的rpc
	for index , rpc in rpcs.items():  
		for index , target in rpc.targets.items():     
			if target.name == serverName and target.include not in sameRecord: 			
				fileRpc.write("#include \"" + target.include + "\"\n") 
				sameRecord[target.include] = 1
 
def GenerateRpcRegisterFuncs(rpc , fileRpc , serverName):

	#生成所有的rpc  
	for index , target in rpc.targets.items():  

		className = target.classes
				
		if target.targetType == g_targetTypeClient and serverName == target.name:
		
			fileRpc.write(threeTab + "\n") 
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcClient , &" + className + "::" + rpc.name + "_RpcClient); \n") 
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcTimeout ,&" + className + "::" + rpc.name + "_RpcTimeout); \n") 
				
		elif target.targetType == g_targetTypeProxy and serverName == target.name:
		
			fileRpc.write(threeTab + "\n")
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcServerProxy , &" + className + "::" + rpc.name + "_RpcServerProxy); \n") 
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcClientProxy , &" + className + "::" + rpc.name + "_RpcClientProxy); \n") 
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcTimeoutProxy ,&" + className + "::" + rpc.name + "_RpcTimeoutProxy); \n") 
				
		elif target.targetType == g_targetTypeServer and serverName == target.name:
			fileRpc.write(threeTab + "m_pRpcManager->RegisterFunc<"+ className + " >(Msg::g_sz" + rpc.name + "_RpcServer , &" + className + "::" + rpc.name + "_RpcServer); \n") 
	
def GenerateRpcHandlers():
	#生成注册的头 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():   
		GenerateRpcHandler(g_rpcMsgs.rpcs.rpcs , rpcServerName.serverName, rpcServerName.namespace) 

def GenerateRpcHandler(rpcs , serverName , old_namespace):
	namespace = old_namespace

	#生成所有的rpc
	for index , rpc in rpcs.items():			
		for index , target in rpc.targets.items(): 
			if serverName != target.name:
				continue 
				
			className = target.classes
				
			outputPath = GetOutputPath(serverName)  
			if target.classes.lower() == "GRpc".lower():
				outputPath = outputPath + serverName + "_" + target.classes + "_" + rpc.name
			else:
				outputPath = outputPath + target.classes + "_" + rpc.name
				
			if target.targetType == g_targetTypeClient and serverName == target.name:
				outputPath = outputPath + "_Client.cpp"
			elif target.targetType == g_targetTypeProxy and serverName == target.name:
				outputPath = outputPath + "_Proxy.cpp" 
			elif target.targetType == g_targetTypeServer and serverName == target.name:
				outputPath = outputPath + "_Server.cpp"
			else:
				continue
			
			if rpc.name == "testMulitServerNode" and IsPathExist(outputPath):
				os.remove(outputPath)
				
			if IsPathExist(outputPath):
#				os.remove(outputPath) or rpc.name == "testParamsAndRpcDatas" 
				continue
				
			fileRpc = open(outputPath , "a")
			fileRpc.write("#include \"" + target.include + "\"\n\n") 
 				
			if target.targetType == g_targetTypeClient and serverName == target.name:			
			
				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" +  rpc.name + "_RpcClient(INT32 nSessionID, Msg::Object objSrc ")
				strParams = GetSpecialParamsExcludeDefaultParam(rpc.returns.params) 
				fileRpc.write(strParams + ")\n{\n\n\n")

				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcClient\" << std::endl;\n")
				fileRpc.write(oneTab + "ReturnNULL;\n}\n\n")

				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" + rpc.name + "_RpcTimeout(INT32 nSessionID, Msg::Object objSrc ") 
				strParams = GetSpecialParamsExcludeDefaultParam(rpc.call.params) 
				fileRpc.write(strParams + ")\n{\n\n\n")
					
				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcTimeout\" << std::endl;\n")
				fileRpc.write(oneTab + "ReturnNULL;\n}\n\n")

				fileRpc.close()
				
			elif target.targetType == g_targetTypeProxy and serverName == target.name:
				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" + rpc.name + "_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc ")
				strParams = GetSpecialParamsExcludeDefaultParam(rpc.call.params) 
				fileRpc.write(strParams + ")\n{\n")
				
				WriteDefineParams(fileRpc , rpc.returns.params)
				fileRpc.write("\n\n");			
				
				strParamsNoType = GetParamsExcludeDefaultAndType(rpc.call.params)
				if len(rpc.call.params) == 0:
					fileRpc.write(oneTab + "if(-1 == ProxySendMsg(\"tcp://127.0.0.1:8002\" , 0))\n")
				else:
					fileRpc.write(oneTab + "if(-1 == ProxySendMsg(\"tcp://127.0.0.1:8002\" , 0 , " + strParamsNoType + "))\n")
					
				fileRpc.write(oneTab + "{\n")
				strParamsNoType= GetParamsExcludeDefaultAndType(rpc.returns.params)
				strReturnCount = len(rpc.returns.params)
				fileRpc.write(twoTab + "Return(" + strParamsNoType + ");\n") 
				fileRpc.write(oneTab + "}\n\n\n")
				
				strParamsNoType= GetParamsExcludeDefaultAndType(rpc.returns.params)
				strReturnCount = len(rpc.returns.params)
				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcServerProxy\" << std::endl;\n")
				fileRpc.write(oneTab + "ReturnNULL;\n}\n\n")

				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" + rpc.name + "_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc  ,") 
				strParams = GetParamsExcludeDefault(rpc.returns.params)
				if len(strParams) != 0:
					fileRpc.write(strParams + ")\n{\n\n\n")
				else:
					fileRpc.write(")\n{\n\n\n")	  
				fileRpc.write("\n\n")
				
				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcClientProxy\" << std::endl;\n")

				strParamsNoType= GetParamsExcludeDefaultAndType(rpc.returns.params)
				strReturnCount = len(rpc.returns.params)
				fileRpc.write(oneTab + "Return(" + strParamsNoType + ");\n}\n\n")

				strParams = GetParamsExcludeDefault(rpc.call.params)
				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" + rpc.name + "_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc,")
				fileRpc.write(strParams + " )\n{\n\n\n ")
				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcTimeoutProxy\" << std::endl;\n")
				fileRpc.write(oneTab + "ReturnNULL;\n}\n\n")

				fileRpc.close()
			elif target.targetType == g_targetTypeServer and serverName == target.name:
			
				fileRpc.write("Msg::ObjectMsgCall * " + namespace + "::" + className + "::" + rpc.name + "_RpcServer(INT32 nSessionID, Msg::Object objSrc ")
				strParams = GetSpecialParamsExcludeDefaultParam(rpc.call.params)
				fileRpc.write(strParams + ")\n{\n")
					
				WriteDefineParams(fileRpc , rpc.returns.params)
				fileRpc.write("\n\n")
				
				strParamsNoType= GetParamsExcludeDefaultAndType(rpc.returns.params)
				strReturnCount = len(rpc.returns.params)
				fileRpc.write(oneTab + "std::cout << \"" + target.classes + "::" + rpc.name + "_RpcServer \"<< std::endl;\n")
				fileRpc.write(oneTab + "Return(" + strParamsNoType + ");\n}\n\n")
	  
				fileRpc.close() 
 
def WriteDefineParams(fileRpc , params):
	for index , param in params.items():  
		strParams = oneTab 
		strParams += param.type
		strParams += " "
		strParams += param.name
		strParams += " = "

		strParams += GetAndCheckDefaultParam(param)

		strParams += ";\n" 
		fileRpc.write(strParams) 
		
def GenerateRpcDatas():
	sameNamespace = collections.OrderedDict() 
	#生成所有的rpc
	for index , serverName in g_rpcMsgs.rpcServerNames.items():    
		outputPath = GetOutputPath(serverName.serverName)   
		outputPath += "RpcDatas.h" 
			 
		fileRpc = open(outputPath , "a")
		if serverName.namespace not in sameNamespace :
			GenerateRpcDatasHeader(fileRpc , serverName)
			sameNamespace[serverName.namespace] = 1 

			for index , rpcData in g_rpcMsgs.rpcs.rpcDatas.items():   
				fileRpc.write(oneTab + "class " + rpcData.name + ": public CUtil::Marshal\n") 
				fileRpc.write(oneTab + "{ \n")
				fileRpc.write(twoTab + "public:\n")
				WriteDefineParamsWithoutDefault(fileRpc , rpcData.params) 
				
				fileRpc.write("\n")
				fileRpc.write(twoTab + rpcData.name + "()\n")
				WriteDefineParamsWithDefault(fileRpc , rpcData.params)  
				fileRpc.write(threeTab + "{}\n")				
				
				WriteRpcDataCopyFunc(fileRpc , rpcData)
				WriteRpcDataMarshal(fileRpc , rpcData)
				WriteRpcDataunMarshal(fileRpc , rpcData) 
				
				fileRpc.write(oneTab + "}; \n \n")
			
			fileRpc.write("}//" + serverName.namespace + "\n\n")
			
			fileRpc.write("namespace CUtil\n")
			fileRpc.write("{ \n")
			fileRpc.write(oneTab + "enum PARAMETER_TYPE_USER_DEFINES \n")
			fileRpc.write(oneTab + "{ \n")
			fileRpc.write(twoTab + "PARAMETER_TYPE_USER_DEFINE_FIRST = CUtil::PARAMETER_TYPE_USER_DEFINE  ,\n")			
			for index , rpcData in g_rpcMsgs.rpcs.rpcDatas.items():   
				fileRpc.write(twoTab + "PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ",\n")  
			fileRpc.write(oneTab + "}; \n \n") 
			
			for index , rpcData in g_rpcMsgs.rpcs.rpcDatas.items():   			
				WriteParamHelper(serverName.namespace , fileRpc , rpcData) 
			fileRpc.write("}\n\n")	
		fileRpc.close()

	sameNamespace = collections.OrderedDict() 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "RpcDatas.h"     
		
		if rpcServerName.namespace not in sameNamespace : 
			fileRpc = open(outputPath , "a")
			fileRpc.write("#endif\n")				
			fileRpc.close()
			sameNamespace[rpcServerName.namespace] = 1
			
def WriteDefineParamsWithDefault(fileRpc , params):
	fileRpc.write(threeTab + ":")
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
		fileRpc.write(strParams)   
		
def WriteDefineParamsWithoutDefault(fileRpc , params):
	for index , param in params.items():  
		strParams = twoTab 
		strParams += param.type
		strParams += " "
		strParams += param.name 

		strParams += ";\n" 
		fileRpc.write(strParams) 
		
def WriteRpcDataCopyFunc(fileRpc , rpcData):
	fileRpc.write(twoTab + rpcData.name + "(const " + rpcData.name + " & val)\n" )
	fileRpc.write(twoTab + "{ \n")
	strParam = ""
	for index , param in rpcData.params.items():  
		strParam = strParam + threeTab + param.name + " = val." + param.name + ";\n"
	fileRpc.write(strParam)
	fileRpc.write(twoTab + "} \n\n")

def WriteRpcDataMarshal(fileRpc , rpcData):
	fileRpc.write(twoTab + "virtual CUtil::CStream & marshal(CUtil::CStream & cs) const override\n" )
	fileRpc.write(twoTab + "{ \n")
	strParam = threeTab + "cs"
	for index , param in rpcData.params.items():  
		strParam = strParam + " << " + param.name
	strParam += ";\n"
	fileRpc.write(strParam)
	fileRpc.write(threeTab + "return cs; \n")
	fileRpc.write(twoTab + "} \n\n")
	
def WriteRpcDataunMarshal(fileRpc , rpcData):
	fileRpc.write(twoTab + "virtual CUtil::CStream & unMarshal(CUtil::CStream & cs) override\n" )
	fileRpc.write(twoTab + "{ \n")
	strParam = threeTab + "cs"
	for index , param in rpcData.params.items():  
		strParam = strParam + " >> " + param.name
	strParam += ";\n"
	fileRpc.write(strParam)
	fileRpc.write(threeTab + "return cs; \n")
	fileRpc.write(twoTab + "} \n\n")

def WriteParamHelper(namespace , fileRpc , rpcData):
	fileRpc.write(oneTab + "GEN_PARAMTER_HELPER(" + namespace + "::" + rpcData.name + " , PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ");\n") 
	
# def WriteParamHelper(namespace , fileRpc , rpcData):
	# fileRpc.write(oneTab + "template<> class CUtil::ParameterHelper<" + namespace + "::" + rpcData.name + ">\n") 
	# fileRpc.write(oneTab + "{ \n")
	# fileRpc.write(oneTab + "public:\n")
	# fileRpc.write(twoTab + "static UINT32 GetParameterType()\n")
	# fileRpc.write(twoTab + "{ \n")
	# fileRpc.write(threeTab + "return PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ";\n") 
	# fileRpc.write(twoTab + "} \n\n")
	
	# fileRpc.write(twoTab + "static " + namespace + "::" + rpcData.name + " GetParameterValue(Parameter & objParam)\n") 
	# fileRpc.write(twoTab + "{ \n")
	# fileRpc.write(threeTab + "INT32 unType = 0;\n")
	# fileRpc.write(threeTab + namespace + "::" + rpcData.name + " val;\n\n")
	# strParam = threeTab + "objParam.GetParamStream() >> unType "
	# for index , param in rpcData.params.items():  
		# strParam = strParam + " >> val." + param.name
	# fileRpc.write(strParam + ";\n")
	# fileRpc.write(threeTab + "MsgAssert_Re(unType == PARAMETER_TYPE_USER_DEFINE_" + rpcData.name +" , val , \"get param error.\");\n\n")
	# fileRpc.write(threeTab + "return val;\n")
	# fileRpc.write(twoTab + "}\n\n")
	
	# fileRpc.write(twoTab + "static void MakeParameter(Parameter & objParam , " + namespace + "::" + rpcData.name + " val)\n") 
	# fileRpc.write(twoTab + "{ \n") 
	# strParam = threeTab + "objParam.GetParamStream() << (INT32)PARAMETER_TYPE_USER_DEFINE_" + rpcData.name 
	# for index , param in rpcData.params.items():  
		# strParam = strParam + " << val." + param.name
	# fileRpc.write(strParam + ";\n")
	# fileRpc.write(twoTab + "} \n\n")
	
	# fileRpc.write(twoTab + "static BOOL CheckParamType(Parameter & objParam)\n") 
	# fileRpc.write(twoTab + "{ \n") 
	# fileRpc.write(threeTab + "if (objParam.GetType() == PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ")\n")
	# fileRpc.write(threeTab + "{ \n") 
	# fileRpc.write(fourTab + "return TRUE; \n") 	 
	# fileRpc.write(threeTab + "} \n\n")
	# fileRpc.write(threeTab + "return FALSE; \n") 	 
	# fileRpc.write(twoTab + "} \n\n")	
	# fileRpc.write(oneTab + "}; \n \n")
	
			
	
	# fileRpc.write(oneTab + "template<> class CUtil::ParameterHelper<" + namespace + "::" + rpcData.name + "&>\n") 
	# fileRpc.write(oneTab + "{ \n")
	# fileRpc.write(oneTab + "public:\n")
	# fileRpc.write(twoTab + "static UINT32 GetParameterType()\n")
	# fileRpc.write(twoTab + "{ \n")
	# fileRpc.write(threeTab + "return PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ";\n") 
	# fileRpc.write(twoTab + "} \n\n")
	
	# fileRpc.write(twoTab + "static " + namespace + "::" + rpcData.name + " GetParameterValue(Parameter & objParam)\n") 
	# fileRpc.write(twoTab + "{ \n") 
	# fileRpc.write(threeTab + "return ParameterHelper<" + namespace + "::" + rpcData.name + ">::GetParameterValue(objParam);\n\n")
	# fileRpc.write(twoTab + "} \n")
		
	# fileRpc.write(twoTab + "static void MakeParameter(Parameter & objParam , " + namespace + "::" + rpcData.name + " val)\n") 
	# fileRpc.write(twoTab + "{ \n") 
	# fileRpc.write(threeTab + "return ParameterHelper<" + namespace + "::" + rpcData.name + ">::MakeParameter(objParam , val);\n")
	# fileRpc.write(twoTab + "} \n\n")
	
	# fileRpc.write(twoTab + "static BOOL CheckParamType(Parameter & objParam)\n") 
	# fileRpc.write(twoTab + "{ \n") 
	# fileRpc.write(threeTab + "if (objParam.GetType() == PARAMETER_TYPE_USER_DEFINE_" + rpcData.name + ")\n")
	# fileRpc.write(threeTab + "{ \n") 
	# fileRpc.write(fourTab + "return TRUE; \n") 	 
	# fileRpc.write(threeTab + "} \n\n")
	# fileRpc.write(threeTab + "return FALSE; \n") 	 
	# fileRpc.write(twoTab + "} \n\n")	
	# fileRpc.write(oneTab + "}; \n \n")
	
def  GenerateRpcDatasHeader(fileRpc , serverName):
	WriteFileDescription(fileRpc , "RpcDatas.h" , "网络消息的数据域.") 
	fileRpc.write("#ifndef __" + serverName.namespace + "_rpc_datas_h__\n") 
	fileRpc.write("#define __" + serverName.namespace + "_rpc_datas_h__\n")  
	fileRpc.write("#include \"CUtil/inc/Common.h\"\n") 
	fileRpc.write("#include \"CUtil/inc/Chunk.h\"\n")
	fileRpc.write("#include \"CUtil/inc/Parameter.h\"\n")   
	fileRpc.write("#include \"CUtil/inc/ParameterHelper.h\"\n\n")   
	fileRpc.write("namespace " + serverName.namespace)   
	fileRpc.write("\n{\n")   
	
def GenerateRpcCallFuncs(): 
	sameNamespace = collections.OrderedDict() 
	for index , serverName in g_rpcMsgs.rpcServerNames.items(): 
		outputPath = GetOutputPath(serverName.serverName)   
		outputPath += "RPCCallFuncs.h" 
			 
		fileRpc = open(outputPath , "a")
		if serverName.namespace not in sameNamespace :
			GenerateRpcCallFuncsHeader(fileRpc , serverName)
			sameNamespace[serverName.namespace] = 1 
		
		for index , rpc in g_rpcMsgs.rpcs.rpcs.items(): 
			sameTarget = collections.OrderedDict() 
			sameTarget[serverName.outputPath] = 0
			for targetInde , target in rpc.targets.items():
				if serverName.serverName == target.name and target.targetType == g_targetTypeClient and sameTarget[serverName.outputPath] != 1:
					sameTarget[serverName.outputPath] = 1
					strDefaultParams = GetSpecialParamsIncludeDefaultParam(rpc.call.params) 
					syncType = GetSyncTypeInString(rpc.syncType)	
					
					strExcludeDefaultParams = GetParamsExcludeDefaultParamAndType(rpc.call.params) 
					strSpecialExcludeDefaultParams = GetParamsExcludeDefaultParamAndType(rpc.call.params) 
					strExcludeDefaultParamsCount = len(rpc.call.params)
					
					#生成sessionName方式带有vec发送的RPC					
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n" + oneTab + "{\n")
					fileRpc.write(twoTab + "GEN_RPC_CALL_" + str(strExcludeDefaultParamsCount) + "((&(" + serverName.namespace + "::" + serverName.rpcInterface + "::GetInstance())) , pSessionName , " + "Msg::g_sz" + rpc.name + "_RpcCall " + strSpecialExcludeDefaultParams + ", vecTargets , objSrc , pCallback , usPriority , objSyncType , " + str(rpc.timeout) + ");\n" + oneTab + "}\n\n")
					
					#生成sessionName方式不带有vec发送的RPC
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					fileRpc.write(oneTab + "{\n")
					
					fileRpc.write(twoTab + "std::vector<Msg::Object> vecTargets;\n" + twoTab + "vecTargets.push_back(objTarget);\n" )
					fileRpc.write(twoTab + "return rpc_" + rpc.name + "( pSessionName ,vecTargets , objSrc " + strExcludeDefaultParams + ", pCallback , usPriority , objSyncType);\n" )
					fileRpc.write(oneTab + "}\n\n")		
					
#					#生成sessionName方式不带有vec且全局发送的RPC
#					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const char * pSessionName , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
#					fileRpc.write(oneTab + "{\n")
#					  
#					fileRpc.write(twoTab + "return rpc_" + rpc.name + "( pSessionName , 0 , objSrc " + strExcludeDefaultParams + ", pCallback , usPriority , objSyncType);\n" )
#					fileRpc.write(oneTab + "}\n\n")	
					
					#生成strNodeName方式带有vec发送的RPC
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					fileRpc.write(oneTab + "{\n")
					
					fileRpc.write(twoTab + "GEN_RPC_CALL_" + str(strExcludeDefaultParamsCount) + "((&(" + serverName.namespace + "::" + serverName.rpcInterface + "::GetInstance())) , pSessionName , " + "Msg::g_sz" + rpc.name + "_RpcCall " + strSpecialExcludeDefaultParams + ", vecTargets , objSrc , pCallback , usPriority , objSyncType , " + str(rpc.timeout) + ");\n")
					fileRpc.write(oneTab + "}\n\n")	
																	
					#生成strNodeName方式不带有vec发送的RPC
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					fileRpc.write(oneTab + "{\n")
					
					fileRpc.write(twoTab + "std::vector<Msg::Object> vecTargets;\n" + twoTab + "vecTargets.push_back(objTarget);\n" )
					fileRpc.write(twoTab + "return rpc_" + rpc.name + "( pSessionName ,vecTargets , objSrc " + strExcludeDefaultParams + ", pCallback , usPriority , objSyncType);\n" )
					fileRpc.write(oneTab + "}\n\n")	
							
					#生成strNodeName方式不带有vec且全局发送的RPC
					# fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(const std::string & pSessionName , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					# fileRpc.write(oneTab + "{\n")
					#
					# fileRpc.write(twoTab + "return rpc_" + rpc.name + "( pSessionName , 0 , objSrc " + strExcludeDefaultParams + ", pCallback , usPriority , objSyncType);\n" )
					# fileRpc.write(oneTab + "}\n\n")	
					
					#生成sessionID方式带有vec发送的RPC
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					fileRpc.write(oneTab + "{\n")
					
					fileRpc.write(twoTab + "GEN_RPC_CALL_" + str(strExcludeDefaultParamsCount) + "((&(" + serverName.namespace + "::" + serverName.rpcInterface + "::GetInstance())) , nSessionID , " + "Msg::g_sz" + rpc.name + "_RpcCall " + strSpecialExcludeDefaultParams + ", vecTargets , objSrc , pCallback , usPriority , objSyncType , " + str(rpc.timeout) + ");\n")
					fileRpc.write(oneTab + "}\n\n")	
																	
					#生成sessionID方式不带有vec发送的RPC
					fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					fileRpc.write(oneTab + "{\n")
					
					fileRpc.write(twoTab + "std::vector<Msg::Object> vecTargets;\n" + twoTab + "vecTargets.push_back(objTarget);\n" )
					fileRpc.write(twoTab + "return rpc_" + rpc.name + "( nSessionID ,vecTargets , objSrc " + strExcludeDefaultParams + " , pCallback , usPriority , objSyncType);\n" )	
					fileRpc.write(oneTab + "}\n\n")	
										
					#生成sessionID方式不带有vec且全局发送的RPC
					#fileRpc.write(oneTab + "static INT32  rpc_" + rpc.name + "(INT32 nSessionID , Msg::Object objSrc " + strDefaultParams + " , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = " +syncType + ")\n")
					# fileRpc.write(oneTab + "{\n")
					
					# fileRpc.write(twoTab + "return rpc_" + rpc.name + "( nSessionID , 0 , objSrc " + strExcludeDefaultParams + ", pCallback , usPriority , objSyncType);\n" )
					# fileRpc.write(oneTab + "}\n\n")	
					
		fileRpc.close()
			
	sameNamespace = collections.OrderedDict() 
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():  
		outputPath = GetOutputPath(rpcServerName.serverName)  
		outputPath = outputPath + "RPCCallFuncs.h"     
		
		if rpcServerName.namespace not in sameNamespace : 
			fileRpc = open(outputPath , "a")
			fileRpc.write("}\n\n")	
			fileRpc.write("#endif\n")				
			fileRpc.close()
			sameNamespace[rpcServerName.namespace] = 1
			

def  GenerateRpcCallFuncsHeader(fileRpc , serverName):
	WriteFileDescription(fileRpc , "RpcCallFuncs.h" , "客户端调用的rpc.") 
	fileRpc.write("#ifndef __msg_rpc_call_funcs_h__\n")
	fileRpc.write("#define __msg_rpc_call_funcs_h__\n")
	fileRpc.write("\n")
	fileRpc.write("#include \"MsgLib/inc/MsgHelper.h\"\n") 
	fileRpc.write("#include \"MsgLib/inc/RPCMsgCall.h\"\n")  
	fileRpc.write("#include \"MsgNameDefine.h\"\n") 
	fileRpc.write("#include \"RpcDatas.h\"\n") 
	for index , serverNames in g_rpcMsgs.rpcServerNames.items(): 
		if serverNames.namespace == serverName.namespace:
			fileRpc.write("#include \"" + serverNames.include + "\"\n") 
	
	fileRpc.write("\n") 
	fileRpc.write("namespace " + serverName.namespace + "\n") 
	fileRpc.write("{\n") 
		
################################流程无关函数处理#####################################
def Usage():
    print('GenerateRpc.py usage:')
    print('-h,--help: print help message.')
    print('-v, --version: print script version')
    print('-o, --output: input an output verb')
    print('--foo: Test option ')
    print('--fre: another test option')

def Version():
	print('GenerateRpc.py 1.0.0.0.1')

def LogOutDebug(*string):
	cp = GetColor("debug")
	longStr = cp + "[ DEBUG ]"
	for item in range(len(string)):  
		longStr += str(string[item])

	print(longStr)
	GetColor("reset")

def LogOutInfo(*string):
	cp = GetColor("info")
	longStr = cp + "[ INFO ] "
	for item in range(len(string)):  
		longStr += str(string[item])
	
	print(longStr)
	GetColor("reset")
	
def LogOutError(*string):
	cp = GetColor("error")
	longStr = cp + "[ ERR ] "
	for item in range(len(string)):  
		longStr += str(string[item])
	
	print(longStr)
	GetColor("reset")
	sys.exit()
	
def InitColor():
	if g_platform == "LINUX":
		cp = '\033['
		g_yellow = cp + '33m'
		g_yellow_h = cp + '1;33m'
		g_green = cp + '32m'
		g_green_h = cp + '1;32m'
		g_red = cp + '31m'
		g_cyan = cp + '36m'
		g_original = cp + '0m'
	else:
		g_yellow = ""
		g_yellow_h = ""
		g_green = ""
		g_green_h = ""
		g_red = ""
		g_original = ""
		g_cyan = ""

def GetColor(type):
	stdOutHandle = ctypes.windll.kernel32.GetStdHandle(g_stdOutputHandle)
	if type == "error":
		if g_platform == "LINUX":
			return g_red
		else:
			ctypes.windll.kernel32.SetConsoleTextAttribute(stdOutHandle , FOREGROUND_RED | FOREGROUND_INTENSTITY)
			return ""
	elif type == "info":
		if g_platform == "LINUX":
			return g_green
		else:
			ctypes.windll.kernel32.SetConsoleTextAttribute(stdOutHandle , FOREGROUND_GREEN | FOREGROUND_INTENSTITY)
			return ""
	elif type == "debug" or type == "reset":
		if g_platform == "LINUX":
			return g_original
		else:
			ctypes.windll.kernel32.SetConsoleTextAttribute(stdOutHandle , FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN)
			return ""
	elif type == "warning" :
		if g_platform == "LINUX":
			return g_yellow
		else:
			ctypes.windll.kernel32.SetConsoleTextAttribute(stdOutHandle , FOREGROUND_BLUE | FOREGROUND_INTENSTITY)
			return ""
				
def WriteFileDescription(fileRpc , file , desc):
	fileRpc.write("/************************************" + "\n")
	fileRpc.write("FileName	:	" + file + "\n")
	fileRpc.write("Author		:	generate by tools" + "\n")
	fileRpc.write("HostName	:	" + socket.gethostname() + "\n")
	fileRpc.write("IP			:	" + socket.gethostbyname(socket.gethostname()) + "\n")
	fileRpc.write("Version		:	0.0.1" + "\n")
#	fileRpc.write("Date		:	" + time.strftime('%Y-%m-%d %H:%M:%S') + "\n")
	fileRpc.write("Description	:	" + desc + "\n")
	fileRpc.write("************************************/" + "\n")


#检查在RPCServerName中是否存在这样的服务器
def CheckInRpcServerName(name):
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():
		if rpcServerName.serverName == name: 
			return True

	LogOutError("no this serverName in serverNamelist") 

#通过服务器名字获取命名空间
def GetServerNamespaceByName(strName):
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items():
		if rpcServerName.serverName == strName: 
			return rpcServerName.namespace
			
	LogOutError("GetServerNamespaceByName error. serverName:" + strName) 
	
def GetOutputPath(name):
	for index , rpcServerName in g_rpcMsgs.rpcServerNames.items(): 
		if rpcServerName.serverName == name:
			if os.path.exists(rpcServerName.outputPath):  
				return rpcServerName.outputPath
			else: 
				return rpcServerName.outputPath

	LogOutError("no path is serverNamelist") 

def IsPathExist(path):
	if os.path.exists(path):
		return True
	else:
		return False

def MakeSpecialDefaultParam(param):
	if param.default == None or param.default == "":
		for index , paramSpecial in g_rpcMsgs.specialDefaultParams.items():
			if param.type.lower().find(index.lower()) != -1:
				if paramSpecial.specialType == g_specialParamTypeSTL:
					param.type = paramSpecial.type + param.type[len(paramSpecial.type):]
					param.default = paramSpecial.type + param.type[len(paramSpecial.type):] + paramSpecial.suffix 
					g_rpcMsgs.defaultParamsList[param.type] = "g_rpcDefaultParam_" + ReplaceSpecialCharToUnderlineFromType(param.type)
					g_rpcMsgs.refers[param.type] = "&"
	return param
			
def IsInSpecialDefaultParam(param):
	for index , paramSpecial in g_rpcMsgs.specialDefaultParams.items():
		if param.type.lower().find(index.lower()) != -1:
			return paramSpecial.specialType
				
	return -1111111
	
def ReplaceSpecialCharToUnderlineFromType(type):
	return type.replace(' ','_').replace(':' , '_').replace('(' ,'_').replace(')' ,'_').replace(',' ,'_').replace('<' ,'_').replace('>' ,'_')
	
def ReplaceSpecialCharForContainerFromType(type):
	return type.replace('[' ,'<').replace(']' ,'>')
	
def IsInRefers(refer):
	for index , referData in g_rpcMsgs.refers.items(): 
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
	for index , defaultParam in g_rpcMsgs.defaultParams.items():
		if defaultParam.type == type:
			return True
	
	return False

def GetDefaultParamsType(theSameType):
	for index , defaultParam in g_rpcMsgs.defaultParams.items():
		if index.lower() == theSameType.lower():
			return index
			
	return theSameType
			
def GetRpcSpecialParamsIncludeDefault(call):
	strParams = ""

	nCount = 0
	for index , param in call.params.items():
		strParams += " , "
		
		nCount = nCount + 1
		defaultParam = GetDefaultParamValue(param)
		
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
		
	for index , defaultParam in g_rpcMsgs.defaultParams.items():
		if defaultParam.type == param.type: 
			return defaultParam.value

	LogOutError("no this value in defaultParamsList") 

def DeleteServerNameFiles():
	for index , serverName in g_rpcMsgs.rpcServerNames.items():
		if os.path.exists(serverName.outputPath + "MsgNameDefine.h"): 
			os.remove(serverName.outputPath + "MsgNameDefine.h")
		if os.path.exists(serverName.outputPath + "RPCCallFuncs.h"): 
			os.remove(serverName.outputPath + "RPCCallFuncs.h")
		if os.path.exists(serverName.outputPath + "RpcDatas.h"): 
			os.remove(serverName.outputPath + "RpcDatas.h")
		if os.path.exists(serverName.outputPath + "RPCDefines.h"): 
			os.remove(serverName.outputPath + "RPCDefines.h")
		if os.path.exists(serverName.outputPath + "GRpc.h"): 
			os.remove(serverName.outputPath + "GRpc.h") 
		if os.path.exists(serverName.outputPath + "RpcRegister.cpp"): 
			os.remove(serverName.outputPath + "RpcRegister.cpp") 

def CreateServerNamePath(): 
	for index , serverName in g_rpcMsgs.rpcServerNames.items():
		if False == os.path.exists(serverName.outputPath):
			LogOutDebug(serverName.outputPath)
			os.makedirs(serverName.outputPath)
			
def GetDefaultParamValue(param):
	for index , paramDefault in g_rpcMsgs.defaultParamsList.items():
		if param.type == index:
			return paramDefault 
			
	LogOutError("defaultParamsList does't has this defaultParam type." , param.type  , " index:" ,g_rpcMsgs.defaultParamsList) 

def GetSyncTypeInString(syncType):
	if syncType == "0":
		return "Msg::SYNC_TYPE_SYNC"
	elif syncType == "1":
		return "Msg::SYNC_TYPE_ASYNC"
	else:
		return "Msg::SYNC_TYPE_SYNC" 
		
def IsHasSameData(dic , name):
	for index , param in dic.items():
		if name == index:
			return True 
	return False
	
################################main函数处理#####################################
def handleArgs(argv): 
	global g_rpcXmlPath
	try:
		opts, args = getopt.getopt(argv[1:], 'hvo:', ['import='])
	except: 
		Usage()
		sys.exit(2) 
	if len(argv) == 1: 
		g_rpcXmlPath = "testRpc.xml"
		return
	elif len(argv) == 2:  
		g_rpcXmlPath = argv[1]
		return  
	for o, a in opts:
		if o in ('-h', '--help'):
			Usage()
			sys.exit(1)
		elif o in ('-v', '--version'):
			Version()
			sys.exit(0) 
		elif o in ('--import',):
			g_rpcXmlPath = a
		elif o in ('--fre',):
			Fre=a
		else:
			print('unhandled option')
			sys.exit(3) 
			
def main(argv):
	InitColor()
	handleArgs(argv)
	LogOutInfo("start generate rpc from path:" + g_rpcXmlPath) 
	start()  
	LogOutInfo("complete generate rpc.") 
	
if __name__ == '__main__': 
	main(sys.argv)


