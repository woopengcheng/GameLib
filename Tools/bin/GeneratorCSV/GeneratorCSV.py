#-*- coding: utf-8 -*-
#coding:utf-8
import sys , getopt
import os , re , string
import time , datetime
import binascii 
import collections  
import xml.dom.minidom
import time,datetime
import socket
import csv

from openpyxl import Workbook
from openpyxl.compat import range
from openpyxl.cell import get_column_letter
from openpyxl import load_workbook

from xml.dom import minidom , Node 

from xml.etree.ElementTree import ElementTree
from xml.etree.ElementTree import Element
from xml.etree.ElementTree import SubElement as SE
#sys.reload()
#sys.setdefaultencoding("utf-8") 
#sys.setdefaultencoding("cp936") 

g_xlsImportPath = ""
g_xlsExportCSVPath = ""
g_xlsExportCPPPath = ""

g_xlsDeleteRecord = []
g_xlsRecords = collections.OrderedDict()
g_configPrefix = "S"
g_loadConfigSuffix = "Load"
g_xlsNamespace = "Config"

g_int32Type = "INT32"
g_int64Type = "INT64"
g_boolType = "bool"
g_doubleType = "double"
g_stringType = "std::string"
g_int32ArrayType = "std::vector<INT32>" 
g_int64ArrayType = "std::vector<INT64>"
g_boolArrayType = "std::vector<bool>"
g_doubleArrayType = "std::vector<double>"
g_stringArrayType = "std::vector<std::string>"
g_structType = 1
g_structArrayType = 2
	
g_int32Func = "GetInt32"
g_int64Func = "GetInt64"
g_boolFunc = "GetBool"
g_doubleFunc = "GetDouble"
g_stringFunc = "GetString"
g_int32ArrayFunc = 6
g_int64ArrayFunc = 7
g_boolArrayFunc = 8
g_doubleArrayFunc = 9
g_stringArrayFunc = 10
g_structFunc = 11
g_structArrayFunc = 12

oneTab = "\t"
twoTab = oneTab + "\t"
threeTab = twoTab + "\t"
fourTab = threeTab + "\t"
fiveTab = fourTab + "\t"
sixTab = fiveTab + "\t"

g_rowComent = 0
g_rowName = 1
g_rowType = 2
g_rowCS = 3

def start(): 
	LogOutInfo("start generate csv.\n")   
	CreateExportPathFiles()
	DeleteExportPathFiles()
	GenerateCSVFromXLS()
	LogOutInfo("generate CSV finished.\n") 
			
	LogOutInfo("start generate CPP.\n")   	
	GenerateCPP()
	LogOutInfo("generate CPP finished.\n") 

def GenerateCSVFromXLS():
	root = g_xlsImportPath
	
	files = Search(root ,'.xlsx')
	for result in files:
		LogOutInfo("filename:" , result);
		Xlsx2CSV(result)
		CheckRecords()
		GenerateCSV()
		
def Xlsx2CSV(filepath):
	dirout = g_xlsExportCSVPath
	dirout = dirout + os.sep  #新路径名称
	try:
		# 一个表中的所有sheet输出到一个csv文件中,所以要保证格式一致.文件名用xlsx文件名
		filename = os.path.basename(filepath) #获取文件名
		filename = os.path.splitext(filename.replace(' ', '_'))[0]
		if filename.find('#') >= 0:
			filename = filename.replace('#' , '')
			LogOutInfo("delete filename" , filename )
			g_xlsDeleteRecord.append(filename)

		#csv_filename = '{xlsx}.tabcsv'.format(xlsx=filename)		
		#dirfileout  = dirout + csv_filename
		#LogOutDebug("dirfileout" , dirfileout )
		#csv_file = open(dirfileout , 'w' , newline='')
		
		#QUOTE_MINIMAL QUOTE_NONE所有的都要加引号.
		#csv_file_writer = csv.writer(csv_file , delimiter='	', quotechar='"', quoting=csv.QUOTE_ALL)
		
		id_list = [] #用于重复的ID去除
		cur_sheet_index = 0
		xlsx_file_reader = load_workbook(filepath)
		g_xlsRecords[filename] = collections.OrderedDict()
		for sheet in xlsx_file_reader.get_sheet_names():			
			cur_rows_index = 0
			sheet_ranges = xlsx_file_reader[sheet]
			for row in sheet_ranges.rows:
				if cur_sheet_index >= 1 and cur_rows_index < 4:
					cur_rows_index = cur_rows_index + 1
					continue
				
				row_container = []
				cur_cell_index = 0
				for cell in row:		
					Str = ""	
					cur_cell_index = cur_cell_index + 1
					if type(cell.value) == type(None):
						if cur_cell_index == 1:
							break		
						Str = ""	
#						LogOutError("error parase filepath" , filepath , "  cur_sheet " , sheet , "  cur_rows_index " , cur_rows_index ,"  cur_cell_index " , cur_cell_index , "  type(cell.value) " , type(cell.value))
					else:
						Str = cell.value
						if type(cell.value) != str:
							Str = str(cell.value)
						else:
							Str = Str.encode('gbk').decode('gbk')

					if len(Str) >= 0:						
						if cur_rows_index < 4:
							Str = ''.join([x for x in Str if x != " "]) 
						if cur_cell_index == 1:		# 插入ID
							if Str in id_list:
								LogOutDebug("repeat id \'" , Str , "\' in \'" , filepath , " \' file")
								#LogOutError("repeat id \'" , Str , "\' in \'" , filepath , " \' file")
							id_list.append(Str)
						row_container.append(Str)

				if len(row_container) >= 1:	
					RemovListNewLine(row_container)
					g_xlsRecords[filename][cur_rows_index] = row_container
					#csv_file_writer.writerow(row_container)
					cur_rows_index = cur_rows_index + 1
#					LogOutDebug("cell.row_container:" , row_container)
				
			cur_sheet_index = cur_sheet_index + 1		
		#csv_file.close()
	except Exception as e:
		LogOutError(e)
		
def CheckRecords():
	for sheet , item in g_xlsRecords.items(): 	#读取sheet
		for row , rowItem in item.items():	#读取每一行
			if row == g_rowName:		#命名重名检测,
				sameName = []
				for col , colItem in enumerate(rowItem):	#读取每一列
					colItemName = colItem
					item_type = GetType(g_xlsRecords[sheet][g_rowType][col])
										
					if colItem[0] == '[' or colItem[0].isdigit():
						LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item=" , colItem , "use error name .")
					if item_type == g_structArrayType:
						if colItem.find('[') <= 0 or colItem.find(']') <= 0:
							LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item=" , colItem , "error  .no \'[\' or \']\'.")							
					
					if item_type == g_structArrayType or item_type == g_structType:
						npos = colItem.find('[')
						colItemName = colItem[0 : npos]
					
					if colItemName in sameName:
						LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item=" , colItemName , " use one same name .")
					else:
						sameName.append(colItemName)						
			elif row == g_rowType:		#类型
				for col , colItem in enumerate(rowItem):	#读取每一列
					item_type = GetType(colItem)					
				
			elif row == g_rowCS:	
				pass
			elif row == g_rowComent:	
				pass	
			else:
				for col , colItem in enumerate(rowItem):	#读取每一列
					item_type = GetType(g_xlsRecords[sheet][g_rowType][col])	
					CheckDataType(item_type , sheet , row , col , colItem)
					#LogOutDebug("g_xlsRecords[sheet][row][col]:" , g_xlsRecords[sheet][row][col])
					
def GenerateCSV():
	
	dirout = g_xlsExportCSVPath
	dirout = dirout + os.sep  #新路径名称
	try:
		for sheet , item in g_xlsRecords.items(): 	#读取sheet
			filename = sheet
			csv_filename = '{xlsx}.tabcsv'.format(xlsx=filename)		
			dirfileout  = dirout + csv_filename
			#LogOutDebug("dirfileout" , dirfileout )
			csv_file = open(dirfileout , 'w' , newline='')
			
			#QUOTE_MINIMAL QUOTE_NONE所有的都要加引号.
			csv_file_writer = csv.writer(csv_file , delimiter='	', quotechar='"', quoting=csv.QUOTE_ALL)

			for row , rowItem in item.items():	#读取每一行
				csv_file_writer.writerow(rowItem)

			csv_file.close()
	except Exception as e:
		LogOutError(e)

def GenerateCPP(): 
	GenerateConfigManagerHeader()
	for sheet , item in g_xlsRecords.items():
		GenerateConfigLoadHeader(sheet , item[2] , item[1] , item[0])
		GenerateConfigLoadCpp(sheet , item[2] , item[1] , item[0])
		GenerateConfigHeader(sheet , item[2] , item[1] , item[0])
		GenerateConfigCpp(sheet , item[2] , item[1] , item[0])
	GenerateConfigManagerCPP()

def GenerateStructData(fileWrite , types , datas , comments):	
	for index , item in enumerate(types):
		item_type = GetType(item)
		if item_type == g_structType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			fileWrite.write("\n" + twoTab + "//" + comments[index] + "\n") 
			fileWrite.write(twoTab + "struct " + g_configPrefix + structName + "\n");
			fileWrite.write(twoTab + "{\n");
			childItems = item.split(',')
			if len(childItems) != len(structDatas):
				LogOutError("parase struct error.invalid size . name=" , structName , ":childItems size=" , len(childItems) , ":structDatas size=" , len(structDatas))
			for indexChild , childItem in enumerate(childItems):
				fileWrite.write(threeTab + GetType(childItem) + GetTypeTab(childItem) + structDatas[indexChild] + ";\n")
			fileWrite.write(twoTab + "}" + structName + ";\n");
		elif item_type == g_structArrayType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			fileWrite.write("\n" + twoTab + "//" + comments[index] + "\n") 
			fileWrite.write(twoTab + "struct " + g_configPrefix + structName + "\n");
			fileWrite.write(twoTab + "{\n");
			item = item.replace('[' , '').replace(']' , '')
			childItems = item.split(',')
			if len(childItems) != len(structDatas):
				LogOutError("parase struct error.invalid size . name=" , structName , ":childItems size=" , len(childItems) , ":structDatas size=" , len(structDatas))
			for indexChild , childItem in enumerate(childItems):
				fileWrite.write(threeTab + GetType(childItem) + GetTypeTab(childItem) + structDatas[indexChild] + ";\n")
			fileWrite.write(twoTab + "}" + ";\n");
			fileWrite.write(twoTab + "std::vector<" + g_configPrefix + structName + ">" + twoTab + "vec" + structName + ";\n");
		else:
			fileWrite.write(twoTab + item_type + GetTypeTab(item) + oneTab + datas[index] + ";"  + oneTab)
			fileWrite.write("//" + comments[index] + "\n") 

def GenerateConfigLoadHeader(filename , types , datas , comments):
	filename = filename + g_loadConfigSuffix
	outputPath = g_xlsExportCPPPath  + os.sep + filename + ".h"
	if os.path.exists(outputPath): 
		os.remove(outputPath)

	fileWrite = open(outputPath , "a")
	
	#以下是生成数据定义
	WriteFileDescription(fileWrite , filename + ".h" , "csv配置文件")
	fileWrite.write("#ifndef __" + filename + "_define_h__\n")
	fileWrite.write("#define __" + filename + "_define_h__\n") 
	fileWrite.write("#include \"CUtil/inc/Common.h \"\n\n") 
	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n") 
	fileWrite.write(oneTab + "struct " + g_configPrefix + filename + "\n") 
	fileWrite.write(oneTab + "{\n") 
	GenerateStructData(fileWrite , types , datas , comments)
	fileWrite.write(oneTab + "};\n\n\n") 
			
	#以下是生成导入数据接口
	fileWrite.write(oneTab + "class " + filename + "\n") 
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "typedef std::vector<" + g_configPrefix + filename + "> CollectionConfigsT;\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "bool LoadFrom(const std::string& filename);\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + g_configPrefix + filename + " & Get(size_t row);\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "inline size_t Count()")
	fileWrite.write("{ ")
	fileWrite.write("return m_vtConfigs.size(); ")
	fileWrite.write("}\n\n")
	fileWrite.write(oneTab + "private:\n")
	fileWrite.write(twoTab + "CollectionConfigsT m_vtConfigs;\n")	
	fileWrite.write(oneTab + "};\n") 


	fileWrite.write("}\n\n" + "#endif// end" + "  __" + filename + "_define_h__\n") 

	fileWrite.close()	  
	
def GenerateConfigLoadCpp(filename , types , datas , comments):
	filename = filename + g_loadConfigSuffix
	outputPath = g_xlsExportCPPPath  + os.sep + filename + ".cpp"
	if os.path.exists(outputPath): 
		os.remove(outputPath)

	fileWrite = open(outputPath , "a")
	
	WriteFileDescription(fileWrite , filename + ".cpp" , "csv读取文件实现")
	fileWrite.write("#include \"" + filename + ".h\"\n") 
	fileWrite.write("#include \"CUtil/inc/CUtil.h\"\n") 
	fileWrite.write("#include \"CUtil/inc/CSVReader.h\"\n\n") 
	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n") 
	fileWrite.write(oneTab + "bool " + filename + "::LoadFrom(const std::string & filepath)\n") 
	fileWrite.write(oneTab + "{\n") 
	
	fileWrite.write(twoTab + "CUtil::CSVReader csv;\n") 
	fileWrite.write(twoTab + "if(csv.Load(filepath) != 0)\n") 
	fileWrite.write(threeTab + "return false;\n\n") 

	for index , item in enumerate(types):
		item_type = GetType(item)
		if item_type == g_structType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			fileWrite.write(twoTab + "size_t index_" + structName + " = csv.GetIndex(\"" + datas[index] + "\", 1);\n") 
			fileWrite.write(twoTab + "MsgAssert_Re0(index_" + structName + " != (size_t)-1 , \"error " + datas[index] + "\");\n\n") 
		elif item_type == g_structArrayType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			fileWrite.write(twoTab + "size_t index_" + structName + " = csv.GetIndex(\"" + datas[index] + "\", 1);\n") 
			fileWrite.write(twoTab + "MsgAssert_Re0(index_" + structName + " != (size_t)-1 , \"error " + datas[index] + "\");\n\n") 
		else:
			fileWrite.write(twoTab + "size_t index_" + datas[index] + " = csv.GetIndex(\"" + datas[index] + "\", 1);\n") 
			fileWrite.write(twoTab + "MsgAssert_Re0(index_" + datas[index] + " != (size_t)-1 , \"error " + datas[index] + "\");\n\n") 

	fileWrite.write(twoTab + "for (size_t row = 3; row < csv.Count(); ++row)\n") 
	fileWrite.write(twoTab + "{\n") 
	fileWrite.write(threeTab + g_configPrefix + filename + " conf;\n\n") 
	for index , item in enumerate(types):
		Str = GetTypeFunc(item)
		if type(Str) != str :
			if Str == g_int32ArrayFunc:
				fileWrite.write(threeTab + "{\n") 
				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + datas[index] + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n") 
				fileWrite.write(fiveTab + "conf." + datas[index] + ".push_back((INT32)CUtil::atoi(vals[i].c_str()));\n") 
				fileWrite.write(threeTab + "}\n\n") 
			elif Str == g_int64ArrayFunc:
				fileWrite.write(threeTab + "{\n") 
				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + datas[index] + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n") 
				fileWrite.write(fiveTab + "conf." + datas[index] + ".push_back((INT64)CUtil::atoi(vals[i].c_str()));\n") 
				fileWrite.write(threeTab + "}\n\n") 
			elif Str == g_boolArrayFunc:
				fileWrite.write(threeTab + "{\n") 
				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + datas[index] + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n") 
				fileWrite.write(fiveTab + "conf." + datas[index] + ".push_back((bool)CUtil::atoi(vals[i].c_str()));\n") 
				fileWrite.write(threeTab + "}\n\n") 
			elif Str == g_doubleArrayFunc:
				fileWrite.write(threeTab + "{\n") 
				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + datas[index] + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n") 
				fileWrite.write(fiveTab + "conf." + datas[index] + ".push_back((float)CUtil::atof(vals[i].c_str()));\n") 
				fileWrite.write(threeTab + "}\n\n") 
			elif Str == g_stringArrayFunc:
				fileWrite.write(threeTab + "{\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + datas[index] + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, conf." + datas[index] + ", \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(threeTab + "}\n\n") 
			elif Str == g_structFunc:
				fileWrite.write(threeTab + "{\n") 

				npos = datas[index].find('[')
				structName = datas[index][0 : npos]
				structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
				childItems = item.split(',')

				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + structName + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n")
				fileWrite.write(fourTab + "{\n") 
				for indexChild , childItem in enumerate(childItems):
					fileWrite.write(fiveTab + "if(i == " + str(indexChild) + ")\n")
					fileWrite.write(fiveTab + "{\n") 

					childItemType = GetType(childItem)
					if childItemType == g_boolType:
						strVal = "bool val = "
						strVal = strVal + "CUtil::strtobool(vals[i].c_str()) >= 1;"
					elif childItemType == g_int32Type:
						strVal = "INT32 val = "
						strVal = strVal + "(INT32)CUtil::atoi(vals[i].c_str());"
					elif childItemType == g_int64Type:
						strVal = "INT64 val = "
						strVal = strVal + "(INT64)CUtil::atoi(vals[i].c_str());"
					elif childItemType == g_doubleType:
						strVal = "double val = "
						strVal = strVal + "(float)CUtil::atof(vals[i].c_str());"
					elif childItemType == g_stringType:
						strVal = "std::string val = "
						strVal = strVal + "vals[i].c_str();"
					
					fileWrite.write(sixTab + strVal + "\n") 
					fileWrite.write(sixTab + "conf." + structName + "." + structDatas[indexChild] + " = val;\n") 

					fileWrite.write(fiveTab + "}\n") 
				fileWrite.write(fourTab + "}\n") 
				fileWrite.write(threeTab + "}\n\n") 

			elif Str == g_structArrayFunc:		
				fileWrite.write(threeTab + "{\n") 

				npos = datas[index].find('[')
				structName = datas[index][0 : npos]
				structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
				childItems = item.replace('[' , '').replace(']' , '').split(',')
				
				fileWrite.write(fourTab + "std::vector<std::string> vals;\n") 
				fileWrite.write(fourTab + "std::string __tmp = csv.GetString(row, index_" + structName + ");\n") 
				fileWrite.write(fourTab + "CUtil::tokenize(__tmp, vals, \"]\", \"\", \"\\\"\");\n") 
				fileWrite.write(fourTab + "for (size_t i = 0; i < vals.size(); ++i)\n")
				fileWrite.write(fourTab + "{\n") 
				fileWrite.write(fiveTab + "std::string strVal = vals[i];\n") 
				fileWrite.write(fiveTab + "if (strVal[0] == '[')\n") 
				fileWrite.write(sixTab + "strVal.assign(vals[i], 1, vals[i].length() - 1);\n\n") 
				fileWrite.write(fiveTab + g_configPrefix + filename + "::" + g_configPrefix + structName + "	" + "array;\n") 
				fileWrite.write(fiveTab + "std::vector<std::string> vals2;\n") 
				fileWrite.write(fiveTab + "CUtil::tokenize(strVal, vals2, \",\", \"\", \"\\\"\");\n") 
				fileWrite.write(fiveTab + "for (size_t j = 0; j < vals2.size(); ++j)\n") 
				fileWrite.write(fiveTab + "{\n")
				#childItems = re.split('(\[.*\])' , item)
				for indexChild , childItem in enumerate(childItems):
					fileWrite.write(sixTab + "if(j == " + str(indexChild) + ")\n")
					fileWrite.write(sixTab + "{\n") 

					childItemType = GetType(childItem)
					if childItemType == g_boolType:
						strVal = "bool val = "
						strVal = strVal + "CUtil::strtobool(vals2[j].c_str()) >= 1;"
					elif childItemType == g_int32Type:
						strVal = "INT32 val = "
						strVal = strVal + "(INT32)CUtil::atoi(vals2[j].c_str());"
					elif childItemType == g_int64Type:
						strVal = "INT64 val = "
						strVal = strVal + "(INT64)CUtil::atoi(vals2[j].c_str());"
					elif childItemType == g_doubleType:
						strVal = "double val = "
						strVal = strVal + "(float)CUtil::atof(vals2[j].c_str());"
					elif childItemType == g_stringType:
						strVal = "std::string val = "
						strVal = strVal + "vals2[j].c_str();"
					
					fileWrite.write(sixTab + oneTab + strVal + "\n") 
					fileWrite.write(sixTab + oneTab  + "array." + structDatas[indexChild] + " = val;\n") 

					fileWrite.write(sixTab + "}\n") 
				fileWrite.write(fiveTab + "}\n") 
				fileWrite.write(fiveTab + "conf.vec" + structName + ".push_back(array);\n") 
				fileWrite.write(fourTab + "}\n") 
				fileWrite.write(threeTab + "}\n\n") 

		else:
			fileWrite.write(threeTab + "conf." + datas[index] + " = csv." + Str + "(row , index_" + datas[index] + ");\n") 
			

	fileWrite.write(threeTab + "m_vtConfigs.push_back(conf);\n") 
	fileWrite.write(twoTab + "}\n\n") 
	fileWrite.write(twoTab + "return true;\n") 
	fileWrite.write(oneTab + "}\n\n") 
			
	fileWrite.write(oneTab + g_configPrefix + filename + " & " + filename + "::Get(size_t row)\n") 
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(twoTab + "return m_vtConfigs.at(row);\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write("}\n\n") 
	
	fileWrite.close()	

def GenerateConfigHeader(filename , types , datas , comments):
	outputPath = g_xlsExportCPPPath  + os.sep + filename + ".h"
	if CheckNeedDelete(outputPath , types , datas ):
		return
	#if os.path.exists(outputPath): 
	#	os.remove(outputPath)

	fileWrite = open(outputPath , "a")
	
	loadConfig = filename + g_loadConfigSuffix
	dataConfig = g_configPrefix + filename

	strTitle = MakeTitle(types , datas)
	fileWrite.write(strTitle)	
	WriteFileDescription(fileWrite , filename + ".h" , "csv读取文件")
	fileWrite.write("#ifndef __" + g_xlsNamespace + "_" + filename + "_define_h__\n")
	fileWrite.write("#define __" + g_xlsNamespace + "_" + filename +  "_define_h__\n") 
	fileWrite.write("#include \"" + loadConfig + ".h\"\n\n") 
	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n\n") 

	fileWrite.write(oneTab + "struct " + dataConfig + "\n") 
	fileWrite.write(oneTab + "{\n") 	 
	GenerateStructData(fileWrite , types , datas , comments)
	fileWrite.write(oneTab + "};\n\n\n") 
			
	fileWrite.write(oneTab + "class " + filename + "\n") 
	fileWrite.write(oneTab + "{\n") 
	
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "typedef std_unordered_map<" + GetType(types[0]) + " , " + dataConfig + "> MapConfigsT;\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "bool LoadFrom(const std::string& filepath);\n")
	fileWrite.write(twoTab + dataConfig + " * Get" + filename + "(INT32 nIndex);\n\n")
	fileWrite.write(oneTab + "private:\n")
	fileWrite.write(twoTab + "MapConfigsT m_mapConfigs;\n\n")
	
	fileWrite.write(oneTab + "};\n") 			
	fileWrite.write(oneTab + "extern " + filename + " * g_p" + filename + ";\n") 


	fileWrite.write("}\n\n" + "#endif// end" + "  __" + g_xlsNamespace + "_" + filename + "_define_h__\n") 
	
	fileWrite.close()	 	

def GenerateConfigCpp(filename , types , datas , comments):
	outputPath = g_xlsExportCPPPath  + os.sep + filename + ".cpp"
	if CheckNeedDelete(outputPath , types , datas ):
		return
	#if os.path.exists(outputPath): 
	#	os.remove(outputPath)

	loadConfig = filename + g_loadConfigSuffix
	dataConfig = g_configPrefix + filename
	fileWrite = open(outputPath , "a")
	
	strTitle = MakeTitle(types , datas)
	fileWrite.write(strTitle)	
	WriteFileDescription(fileWrite , filename + ".cpp" , "csv读取数据文件实现")
	fileWrite.write("#include \"" + filename + ".h\"\n") 
	fileWrite.write("#include \"LogLib/inc/Log.h\"\n\n") 
	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n") 
	fileWrite.write(oneTab + "bool " + filename + "::LoadFrom(const std::string & filepath)\n") 
	fileWrite.write(oneTab + "{\n") 
	
	fileWrite.write(twoTab + g_xlsNamespace + "::" + loadConfig + " loadConfig;\n") 
	fileWrite.write(twoTab + "MsgAssert_Re0(loadConfig.LoadFrom(filepath) , \"Error " + filename + "LoadFrom \" << filepath);\n\n") 
	
	fileWrite.write(twoTab + "for(size_t i = 0; i < loadConfig.Count(); ++i)\n") 
	fileWrite.write(twoTab + "{\n") 
	fileWrite.write(threeTab + g_xlsNamespace + "::" + g_configPrefix + loadConfig + "& config = loadConfig.Get(i);\n") 
	fileWrite.write(threeTab + g_xlsNamespace + "::" + dataConfig + " data = {0};\n") 
	
	for index , item in enumerate(types):
		itemType = GetType(item)
		if itemType == g_structType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			childItems = item.split(',')

			fileWrite.write(threeTab + "{\n") 
			for indexChild , childItem in enumerate(childItems):
				fileWrite.write(fourTab + "data." + structName + "." + structDatas[indexChild] + " = config." + structName + "." + structDatas[indexChild] + ";\n") 
			fileWrite.write(threeTab + "}\n")

		elif itemType == g_structArrayType:			
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			childItems = item.split(',')

			fileWrite.write(threeTab + "{\n") 
			fileWrite.write(fourTab + "std::vector<" + g_configPrefix + loadConfig + "::" + g_configPrefix + structName + ">::iterator iter = config.vec" + structName + ".begin();\n") 
			fileWrite.write(fourTab + "std::vector<" + g_configPrefix + loadConfig + "::" + g_configPrefix + structName + ">::iterator end = config.vec" + structName + ".end();\n") 
			fileWrite.write(fourTab + "for (; iter != end;++iter)\n") 
			fileWrite.write(fourTab + "{\n") 
			fileWrite.write(fiveTab + g_configPrefix + filename + "::" + g_configPrefix + structName + " array;\n")
			for indexChild , childItem in enumerate(childItems):
				fileWrite.write(fiveTab + "array." + structDatas[indexChild] + " = iter->" + structDatas[indexChild] + ";\n")
			fileWrite.write(fiveTab + "data.vec" + structName + ".push_back(array);\n")
			fileWrite.write(fourTab + "}\n")
			fileWrite.write(threeTab + "}\n")
		else:
			fileWrite.write(threeTab + "data." + datas[index] + " = config." + datas[index] + ";\n") 

	fileWrite.write(threeTab + "m_mapConfigs.insert(std::make_pair(data." + datas[0] + ",data));\n") 
	fileWrite.write(twoTab + "}\n") 
	fileWrite.write(twoTab + "return true;\n") 
	fileWrite.write(oneTab + "}\n\n") 
			
	fileWrite.write(oneTab + dataConfig + " * " + filename + "::Get" + filename + "(INT32 nIndex)\n")
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(twoTab + "MapConfigsT::iterator iter = m_mapConfigs.find(nIndex);\n") 
	fileWrite.write(twoTab + "if(iter == m_mapConfigs.end())\n") 
	fileWrite.write(twoTab + "{\n") 
	fileWrite.write(threeTab + "gWarniStream( \"" + filename + "::Get" + filename + " NotFound \" << nIndex);\n") 
	fileWrite.write(threeTab + "return NULL;\n") 
	fileWrite.write(twoTab + "}\n\n") 
	fileWrite.write(twoTab + "return &iter->second;\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write(oneTab + filename + " * g_p" + filename + " = NULL;\n") 
	fileWrite.write("}\n\n") 
	
	fileWrite.close()

def GenerateConfigManagerHeader():
	outputPath = g_xlsExportCPPPath  + os.sep + "ConfigManager.h"
	if os.path.exists(outputPath): 
		os.remove(outputPath) 

	fileWrite = open(outputPath , "a") 

	WriteFileDescription(fileWrite , "ConfigManager.h" , "ConfigManager文件声明")
	fileWrite.write("#ifndef __" + g_xlsNamespace + "_ConfigManager" + "_define_h__\n")
	fileWrite.write("#define __" + g_xlsNamespace + "_ConfigManager" +  "_define_h__\n") 
	fileWrite.write("#include \"CUtil/inc/Common.h\"\n\n") 
	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n\n") 
			
	fileWrite.write(oneTab + "class ConfigManager\n") 
	fileWrite.write(oneTab + "{\n") 
	
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "ConfigManager();\n")
	fileWrite.write(twoTab + "~ConfigManager();\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "static ConfigManager & GetInstance();\n\n")
	fileWrite.write(oneTab + "public:\n")
	fileWrite.write(twoTab + "INT32		Init(std::string  strCsvPath);\n")
	fileWrite.write(twoTab + "INT32		Cleanup();\n")
	fileWrite.write(oneTab + "private:\n")
	fileWrite.write(twoTab + "\n\n")
	fileWrite.write(oneTab + "};\n") 		


	fileWrite.write("}\n\n" + "#endif// end" + "  __" + g_xlsNamespace + "_ConfigManager_define_h__\n") 
	
	fileWrite.close()	 	


def GenerateConfigManagerCPP():
	outputPath = g_xlsExportCPPPath  + os.sep + "ConfigManager.cpp"
	if os.path.exists(outputPath): 
		os.remove(outputPath)

	fileWrite = open(outputPath , "a")
	
	WriteFileDescription(fileWrite , "ConfigManager.cpp" , "ConfigManager数据管理文件实现")
	fileWrite.write("#include \"" + "ConfigManager.h\"\n") 
	fileWrite.write("#include \"LogLib/inc/Log.h\"\n") 
	for sheet , item in g_xlsRecords.items():
		fileWrite.write("#include \"" + sheet + ".h\"\n") 
	fileWrite.write(oneTab + "\n\n") 

	fileWrite.write("namespace " + g_xlsNamespace + "\n") 
	fileWrite.write("{\n") 
	fileWrite.write(oneTab + "ConfigManager::ConfigManager()\n") 
	fileWrite.write(oneTab + "{\n") 
	for sheet , item in g_xlsRecords.items():
		if sheet in g_xlsDeleteRecord:
			fileWrite.write("// " + twoTab + "g_p" + sheet + " = new " + g_xlsNamespace + "::" + sheet + ";\n") 
		else:
			fileWrite.write(twoTab + "g_p" + sheet + " = new " + g_xlsNamespace + "::" + sheet + ";\n") 
	fileWrite.write(oneTab + "}\n\n") 
	
	fileWrite.write(oneTab + "ConfigManager::~ConfigManager()\n") 
	fileWrite.write(oneTab + "{\n") 
	for sheet , item in g_xlsRecords.items():
		if sheet in g_xlsDeleteRecord:
			fileWrite.write("// " + twoTab + "SAFE_DELETE(" + g_xlsNamespace + "::" +  "g_p" + sheet + ");\n") 
		else:
			fileWrite.write(twoTab + "SAFE_DELETE(" + g_xlsNamespace + "::" +  "g_p" + sheet + ");\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write(oneTab + "ConfigManager & ConfigManager::GetInstance()\n") 
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(twoTab + "static ConfigManager instance;\n") 
	fileWrite.write(twoTab + "return instance;\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write(oneTab + "INT32 ConfigManager::Init(std::string  strCsvPath)\n") 
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(twoTab + "MsgAssert_ReF1(strCsvPath.length(), \"ConfigManager::Init error.\");\n\n") 
	fileWrite.write(twoTab + "if (strCsvPath[strCsvPath.length() - 1] != '/')\n") 
	fileWrite.write(twoTab + "{\n") 
	fileWrite.write(threeTab + "strCsvPath = strCsvPath + \"/\";\n") 
	fileWrite.write(twoTab + "}\n\n") 
	for sheet , item in g_xlsRecords.items():
		if sheet in g_xlsDeleteRecord:
			fileWrite.write("// " + twoTab + "MsgAssert_ReF1(" + g_xlsNamespace + "::" +  "g_p" + sheet + " , \"ConfigManager not Init\")\n") 
			fileWrite.write("// " + twoTab + "" + g_xlsNamespace + "::" +  "g_p" + sheet + "->LoadFrom(strCsvPath + \"" + sheet + ".tabcsv\");\n\n") 
		else:
			fileWrite.write(twoTab + "MsgAssert_ReF1(" + g_xlsNamespace + "::" +  "g_p" + sheet + " , \"ConfigManager not Init\")\n") 
			fileWrite.write(twoTab + "" + g_xlsNamespace + "::" +  "g_p" + sheet + "->LoadFrom(strCsvPath + \"" + sheet + ".tabcsv\");\n\n") 
	fileWrite.write(twoTab + "return 0;\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write(oneTab + "INT32 ConfigManager::Cleanup()\n") 
	fileWrite.write(oneTab + "{\n") 
	fileWrite.write(twoTab + "return -1;\n") 
	fileWrite.write(oneTab + "}\n\n") 

	fileWrite.write("}\n\n") 
	
	fileWrite.close()

################################流程相关函数处理#####################################
def RemoveNewLine(str):
	pp = ""
	for tt in str.splitlines():
		tt = tt.strip().lstrip().rstrip()
		pp = pp+tt
	return pp

def RemovListNewLine(List): 
	for index , item2 in enumerate(List):
		List[index] = RemoveNewLine(item2)

def GetType(item):
	if  item.lower() == "int".lower() or\
		item.lower() == "int32".lower():
		return g_int32Type
	elif item.lower() == "int64".lower() or\
		item.lower() == "long".lower():
		return g_int64Type
	elif item.lower() == "double".lower() or\
		item.lower() == "float".lower():
		return g_doubleType
	elif item.lower() == "bool".lower():
		return g_boolType
	elif item.lower() == "string".lower():
		return g_stringType
	elif item.lower() == "int[]".lower() or\
		item.lower() == "[]int".lower() or\
		item.lower() == "[]int32".lower() or\
		item.lower() == "int32[]".lower():
		return g_int32ArrayType
	elif item.lower() == "int64[]".lower() or\
		item.lower() == "[]int64".lower() or\
		item.lower() == "[]long".lower() or\
		item.lower() == "long[]".lower():
		return g_int64ArrayType
	elif item.lower() == "double[]".lower() or\
		item.lower() == "[]double".lower() or\
		item.lower() == "[]float".lower() or\
		item.lower() == "float[]".lower():
		return g_doubleArrayType
	elif item.lower() == "string[]".lower() or\
		item.lower() == "[]string".lower() or\
		item.lower() == "[]std::string".lower() or\
		item.lower() == "std::string[]".lower():
		return g_stringArrayType
	elif item.lower() == "bool[]".lower() or\
		item.lower() == "[]bool".lower():
		return g_boolArrayType
	elif item.lower().find(',') >= 0 and \
		item.lower()[0] != "[" and\
		item.lower()[len(item.lower()) - 1] != "]":
		return g_structType
	elif item.lower().find(',') >= 0 and \
		item.lower()[0] == "[" and\
		item.lower()[len(item.lower()) - 1] == "]":
		return g_structArrayType
	else:
		LogOutError("GetType error." , item)

	return "None"

def GetTypeFunc(item):
	if  item.lower() == "int".lower() or\
		item.lower() == "int32".lower():
		return g_int32Func
	elif item.lower() == "int64".lower() or\
		item.lower() == "long".lower():
		return g_int64Func
	elif item.lower() == "double".lower() or\
		item.lower() == "float".lower():
		return g_doubleFunc
	elif item.lower() == "bool".lower():
		return g_boolFunc
	elif item.lower() == "string".lower():
		return g_stringFunc
	elif item.lower() == "int[]".lower() or\
		item.lower() == "[]int".lower() or\
		item.lower() == "[]int32".lower() or\
		item.lower() == "int32[]".lower():
		return g_int32ArrayFunc
	elif item.lower() == "int64[]".lower() or\
		item.lower() == "[]int64".lower() or\
		item.lower() == "[]long".lower() or\
		item.lower() == "long[]".lower():
		return g_int64ArrayFunc
	elif item.lower() == "double[]".lower() or\
		item.lower() == "[]double".lower() or\
		item.lower() == "[]float".lower() or\
		item.lower() == "float[]".lower():
		return g_doubleArrayFunc
	elif item.lower() == "string[]".lower() or\
		item.lower() == "[]string".lower() or\
		item.lower() == "[]std::string".lower() or\
		item.lower() == "std::string[]".lower():
		return g_stringArrayFunc
	elif item.lower() == "bool[]".lower() or\
		item.lower() == "[]bool".lower():
		return g_boolArrayFunc
	elif item.lower().index(",") >= 0 and \
		item.lower()[0] != "[" and\
		item.lower()[len(item.lower()) - 1] != "]":
		return g_structFunc
	elif item.lower().index(",") >= 0 and \
		item.lower()[0] == "[" and\
		item.lower()[len(item.lower()) - 1] == "]":
		return g_structArrayFunc
	else:
		LogOutError("GetTypeFunc error." , item)

	return None

def GetTypeTab(item):
	if  item.lower() == "int".lower() or\
		item.lower() == "int32".lower():
		return sixTab
	elif item.lower() == "bool".lower():
		return sixTab
	elif item.lower() == "int64".lower() or\
		item.lower() == "long".lower():
		return sixTab
	elif item.lower() == "double".lower() or\
		item.lower() == "float".lower():
		return sixTab
	elif item.lower() == "string".lower():
		return fiveTab
	elif item.lower() == "int[]".lower() or\
		item.lower() == "[]int".lower() or\
		item.lower() == "[]int32".lower() or\
		item.lower() == "int32[]".lower():
		return threeTab
	elif item.lower() == "bool[]".lower() or\
		item.lower() == "[]bool".lower():
		return threeTab
	elif item.lower() == "int64[]".lower() or\
		item.lower() == "[]int64".lower() or\
		item.lower() == "[]long".lower() or\
		item.lower() == "long[]".lower():
		return threeTab
	elif item.lower() == "double[]".lower() or\
		item.lower() == "[]double".lower() or\
		item.lower() == "[]float".lower() or\
		item.lower() == "float[]".lower():
		return threeTab
	elif item.lower() == "string[]".lower() or\
		item.lower() == "[]string".lower() or\
		item.lower() == "[]std::string".lower() or\
		item.lower() == "std::string[]".lower():
		return oneTab
	elif item.lower().index(",") >= 0 and \
		item.lower()[0] != "[" and\
		item.lower()[len(item.lower()) - 1] != "]":
		return twoTab
	elif item.lower().index(",") >= 0 and \
		item.lower()[0] == "[" and\
		item.lower()[len(item.lower()) - 1] == "]":
		return oneTab
	else:
		return twoTab

	return oneTab

def RemoveSpecialWord(item):
	return item.replace(' ','').replace('	','').replace('[','').replace(']','').strip().rstrip()

def CheckDataArray(colItem):
	pp = ""
	childItems = colItem.split(',')
	for childIndex , childItem in enumerate(childItems):							
		childItem = RemoveSpecialWord(childItem)
		if len(childItem) > 0:
			pp = pp + childItem
			if len(childItems) - 1 != childIndex:
				pp = pp + ","

	#LogOutInfo("pp " , pp) 
	return pp

def CheckDataType(item_type , sheet , row , col , colItem):					
	if item_type == g_boolType:
		item = RemoveSpecialWord(colItem)
		if item.lower() == "true".lower() or\
			item.lower() == "false".lower() or\
			item.lower() == "0".lower() or\
			item.lower() == "1".lower():	
			pass
		else:
			LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item="  , " bool type error.")							

		if row != -1:
			g_xlsRecords[sheet][row][col] = item
		else:
			return item
	elif item_type == g_int32Type:
		item = RemoveSpecialWord(colItem)
		if not item.lower().isdigit():	
			LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item="  , " int32 type error.")							

		if row != -1:
			g_xlsRecords[sheet][row][col] = item
		else:
			return item
	elif item_type == g_int64Type:
		item = RemoveSpecialWord(colItem)
		if not item.lower().isdigit():	
			LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item=" , " int64 type error.")							

		if row != -1:
			g_xlsRecords[sheet][row][col] = item
		else:
			return item
	elif item_type == g_doubleType:						
		item = RemoveSpecialWord(colItem)
		#if !item.lower().isdigit():	
		#	LogOutError("sheet=" , sheet , " :row=" , row , " :col=" , col , " item=" , " double type error.")							

		if row != -1:
			g_xlsRecords[sheet][row][col] = item
		else:
			return item
	elif item_type == g_stringType:
		return colItem
	elif item_type == g_int32ArrayType:
		g_xlsRecords[sheet][row][col] = CheckDataArray(colItem)
	elif item_type == g_int64ArrayType:
		g_xlsRecords[sheet][row][col] = CheckDataArray(colItem)
	elif item_type == g_doubleArrayType:
		g_xlsRecords[sheet][row][col] = CheckDataArray(colItem)
	elif item_type == g_stringArrayType:
		pass
	elif item_type == g_structType:			
		childItems = colItem.split(',')		
		itemContent = ""
		for childIndex , childItem in enumerate(childItems):	
			childItem = RemoveSpecialWord(childItem)
			childType = GetTypeByIndex(g_xlsRecords[sheet][g_rowType][col] , childIndex)
			#LogOutDebug("childType" , childType , " childItem" , childItem)
			childItem = CheckDataType(childType , sheet , -1 , col , childItem)
			itemContent = itemContent + childItem
			if len(childItems) - 1 != childIndex:
				itemContent = itemContent + ","

		#LogOutDebug("itemContent:" , itemContent)
		if row != -1:
			g_xlsRecords[sheet][row][col] = itemContent
		else:
			item = "["
			item = item + itemContent
			item = item + "]"
			return item

	elif item_type == g_structArrayType:
		if row != -1:
			childItems = colItem.split(']')		
			itemContent = ""
			for childIndex , childItem in enumerate(childItems):
				if len(childItem) > 0:
					#LogOutDebug("itemContent:" , itemContent , "childIndex" , childIndex , "size=" , len(childItems) , "childItems" , childItems , "colItem:" , colItem)
					childItem = childItem.replace('[' , '').replace(']' , '').strip().rstrip().lstrip()
					childItem = CheckDataType(g_structType , sheet , -1 , col , childItem)
					itemContent = itemContent + childItem

			g_xlsRecords[sheet][row][col] = itemContent
		else:
			return colItem
	else:
		return colItem

def GetTypeByIndex(types , index):
	typeItems = types.split(',')
	for childIndex , childItem in enumerate(typeItems):
		childItem = RemoveSpecialWord(childItem)
		if index == childIndex:
			return GetType(childItem)

	LogOutError("GetTypeByIndex error.types=" , types , " index=" ,index)

def MakeTitle(types , datas ):
	Str = "// attention dont't change this line:"		
	for index , item in enumerate(types):
		item_type = GetType(item)
		if item_type == g_structType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			childItems = item.split(',')
			Str = Str + structName
			if len(childItems) != len(structDatas):
				LogOutError("parase struct error.invalid size . name=" , structName , ":childItems size=" , len(childItems) , ":structDatas size=" , len(structDatas))
			for indexChild , childItem in enumerate(childItems):
				Str = Str + GetType(childItem) + " " + structDatas[indexChild]  + ";"
		elif item_type == g_structArrayType:
			npos = datas[index].find('[')
			structName = datas[index][0 : npos]
			structDatas = datas[index][npos + 1 : len(datas[index]) - 1].split(',')
			
			Str = Str + structName
			item = item.replace('[' , '').replace(']' , '')
			childItems = item.split(',')
			if len(childItems) != len(structDatas):
				LogOutError("parase struct error.invalid size . name=" , structName , ":childItems size=" , len(childItems) , ":structDatas size=" , len(structDatas))
			for indexChild , childItem in enumerate(childItems):
				Str = Str + GetType(childItem) + " " + structDatas[indexChild]  + ";"
		else:
			Str = Str + item_type + " " + datas[index]   + ";"
	Str += "\n"
	return Str
	
def CheckNeedDelete(outputFile , types , datas):
	if os.path.exists(outputFile): 
		strNew = MakeTitle(types , datas)
		
		fileRead = open(outputFile , "r")
		for line in fileRead: 
			strOld = line
			break

		#LogOutDebug("\nold comments:" , strOld , "new comments:" , strNew , "\n")
		if strNew != strOld:
			LogOutInfo(outputFile , " not match.\n")
			LogOutInfo("old comments:" , strOld)
			LogOutInfo("new comments:" , strNew)		
		fileRead.close()
		return True
	else:
		return False

################################流程无关函数处理#####################################
def Usage():
    print('GenerateCSV.py usage:')
    print('-h,--help: print help message.')
    print('-v, --version: print script version')
    print('-o, --output: input an csv output verb')
    print('-c, --output: input an cpp output verb')
    print('--foo: Test option ')
    print('--fre: another test option')

def Version():
	print('GenerateCSV.py 1.0.0.0.1')

def LogOutDebug(*string):
	longStr = "debug: "
	for item in range(len(string)):  
		longStr += str(string[item])

	print(longStr)

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
	sys.exit(3)
	
def WriteFileDescription(fileWrite , sfile , desc):
	fileWrite.write("/************************************" + "\n")
	fileWrite.write("FileName	:	" + sfile + "\n")
	fileWrite.write("Author		:	generate by tools" + "\n")
	fileWrite.write("HostName	:	" + socket.gethostname() + "\n")
	fileWrite.write("IP			:	" + socket.gethostbyname(socket.gethostname()) + "\n")
	fileWrite.write("Version		:	0.0.1" + "\n")
	fileWrite.write("Date		:	" + time.strftime('%Y-%m-%d %H:%M:%S') + "\n")
	fileWrite.write("Description	:	" + desc + "\n")
	fileWrite.write("************************************/" + "\n")

def Search(base , suffix):
    pattern = suffix
    fileresult = []
    cur_list = os.listdir(base)
    for item in cur_list:
        full_path = os.path.join(base, item)
        if os.path.isdir(full_path):            
            fileresult += Search(full_path , suffix)
        if os.path:
            if full_path.endswith(pattern):
                fileresult.append(full_path)
    return fileresult
	
def DeleteExportPathFiles():
	files = Search(g_xlsExportCSVPath , '.tabcsv')
	for sfile in files:
		if os.path.exists(sfile): 
			os.remove(sfile)

def CreateExportPathFiles(): 
	if False == os.path.exists(g_xlsExportCSVPath):
		os.makedirs(g_xlsExportCSVPath)
		LogOutInfo("create dir: " , g_xlsExportCSVPath)
		
	if False == os.path.exists(g_xlsExportCPPPath):
		os.makedirs(g_xlsExportCPPPath)
		LogOutInfo("create dir: " , g_xlsExportCPPPath)


################################main函数处理#####################################
def handleArgs(argv): 
	global g_xlsImportPath 
	global g_xlsExportCSVPath
	global g_xlsExportCPPPath
	
	try:
		opts, args = getopt.getopt(argv[1:], 'hvi:o:c:', ['import='])
	except: 
		Usage()
		sys.exit(2) 
	if len(argv) < 2: 
		Usage()
		return  
	for o, a in opts:
		if o in ('-h', '--help'):
			Usage()
			sys.exit(1)
		elif o in ('-v', '--version'):
			Version()
			sys.exit(0) 
		elif o in ('-i','--import',):
			g_xlsImportPath = a
		elif o in ('-o','--export',):
			g_xlsExportCSVPath = a
		elif o in ('-c','--cppexport',):
			g_xlsExportCPPPath = a
		elif o in ('--fre',):
			Fre=a
		else:
			print('unhandled option')
			sys.exit(3) 
			
def main(argv):
	global g_xlsImportPath 
	global g_xlsExportCSVPath
	global g_xlsExportCPPPath
	#handleArgs(argv)
	g_xlsImportPath = "./xls_config"
	g_xlsExportCSVPath = "../../../bin/vs14.0/x64/DLL_Debug_x64/csv_config"
	g_xlsExportCPPPath = "../../../vsproject/TestLibCore/CSVConfigs"
	LogOutInfo("generate csv from path:" + g_xlsImportPath + " csv will export to:" + g_xlsExportCSVPath + " cpp will export to:" + g_xlsExportCPPPath) 
	start()
	LogOutInfo("complete generate csv.") 
	
if __name__ == '__main__': 
	main(sys.argv)


