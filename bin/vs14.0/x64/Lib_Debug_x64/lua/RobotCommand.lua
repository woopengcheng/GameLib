--此文件是机器人的命令文件

local robotCommand = RobotCommand()
local robotParam = RobotParams()

--下面的类型需要和C++代码对应,正常来说只支持double,越简单越好.默认也为double
local ROBOT_TYPE_DOUBLE = 0
local ROBOT_TYPE_STRING = 1

RobotTypes = {}
RobotTypes[ROBOT_TYPE_DOUBLE] = "double"
RobotTypes[ROBOT_TYPE_STRING] = "std::string" 

RobotCommands = {}
robotCommand.command = "测试命令" 
	robotParam.type = ROBOT_TYPE_DOUBLE
	robotParam.default = "1 "
	robotParam.note = "参数1" 
robotCommand:SetParams(0 , robotParam)
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "-2"
	robotParam.note = "参数1"
robotCommand:SetParams(1 , robotParam)
RobotCommands[0] = robotCommand

local robotCommand = RobotCommand()
local robotParam = RobotParams()
robotCommand.command = "测试命令2"
	robotParam.type = ROBOT_TYPE_STRING
	robotParam.default = "45345"
	robotParam.note = "参数1"
robotCommand:SetParams(0 , robotParam)
RobotCommands[1] = robotCommand