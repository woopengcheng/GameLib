--lua语言支持加括号或者不加括号.但是连接两个字符串或者变量的时候都需要加括号.所以还是加括号比较好.

--工程名字
solution("HelloWorld")

--定义一个solution,确认所有的项目按照下面那个编译.premake能自动识别debug,release,debugDLL,releaseDLL,
--但是如果自定义的,需要用filter来做一些处理.
configurations({ "Debug", "Release" })

--项目名字
project("HelloWorld")
	
	--控制台还是windowsApp,SharedLib,StaticLib,MakeFile(这个非常特殊,需要看https://github.com/premake/premake-core/wiki/Makefile_Projects),None(更特殊.)
	kind("ConsoleApp")

	--编译的语言,貌似只有"C++"和"C"两种
	language("C++")
	
	targetdir "bin/%{cfg.buildcfg}"

	--编译所包含的文件.**代表的是遍历所有的包括子文件夹符合的文件,当然也可以写具体的文件
	files({ "**.h", "**.c","**.cpp" })
	
	--在编译的时候不需要包含的文件.例如下面含有这个文件夹的都不编译.
	--excludes { "**/Win32Specific/**" }
	--excludes { "tests/*.c" } 这个文件夹下后缀为.c的不编译.
	excludes({"tests/*.c"})

	--这里对于自定义的configurations做处理。
	--filter的用法:filter({ "prefix:keywords" })
	filter("configurations:Debug")
	
		--一些宏定义
		--去除宏定义在前面加remove:例如
		--removedefines({"TRACE"})
		defines({ "DEBUG" })
		
		-- 允许编译出调试信息.
		flags({ "Symbols" })

	filter "configurations:Release"
		defines { "NDEBUG" }
		
		-- 编译优化,为release做准备的.
		optimize "On"