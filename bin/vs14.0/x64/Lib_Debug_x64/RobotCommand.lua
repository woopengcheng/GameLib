--此文件是机器人的命令文件

--下面的类型需要和C++代码对应
local ROBOT_TYPE_INT64 = 1
local ROBOT_TYPE_DOUBLE = 2
local ROBOT_TYPE_STRING = 3

RobotTypes = {}
RobotTypes[ROBOT_TYPE_DOUBLE] = "double"
RobotTypes[ROBOT_TYPE_INT64] = "INT64"
RobotTypes[ROBOT_TYPE_STRING] = "std::string" 

RobotCommands = {}

local robotCommand = RobotCommand()
robotCommand.command = "测试命令" 
robotCommand.type = 1000
local robotParam = RobotParams()
	robotParam.type = ROBOT_TYPE_INT64
	robotParam.default = "1 "
	robotParam.note = "参数1" 
robotCommand:SetParams(0 , robotParam)
	robotParam.type = ROBOT_TYPE_INT64
	robotParam.default = "-2"
	robotParam.note = "参数1"
robotCommand:SetParams(1 , robotParam)
	robotParam.type = ROBOT_TYPE_DOUBLE
	robotParam.default = "2"
	robotParam.note = "参数1"
robotCommand:SetParams(2 , robotParam)
	robotParam.type = ROBOT_TYPE_DOUBLE
	robotParam.default = "3.6"
	robotParam.note = "参数1"
robotCommand:SetParams(3 , robotParam)
	robotParam.type = ROBOT_TYPE_DOUBLE
	robotParam.default = "-1" 
	robotParam.note = "参数1"
robotCommand:SetParams(4 , robotParam)
RobotCommands[0] = robotCommand

local robotCommand = RobotCommand()
local robotParam = RobotParams()
robotCommand.type = 1001
robotCommand.command = "测试命令2"
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "45345"
	robotParam.note = "参数1"
robotCommand:SetParams(0 , robotParam)
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "1123123"
	robotParam.note = "参数1"
robotCommand:SetParams(1 , robotParam)
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "af发达"
	robotParam.note = "参数1"
robotCommand:SetParams(2 , robotParam)
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "打发斯蒂芬"
	robotParam.note = "参数1"
robotCommand:SetParams(3 , robotParam)
RobotCommands[1] = robotCommand