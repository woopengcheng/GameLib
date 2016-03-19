function defaultAction(osName, actionName)
   if os.is(osName) then
      _ACTION = _ACTION or actionName
   end
end

--defaultAction("vs2008")

--tag_debug_x32_vs2008.lib 
--tag_static_debug_x32_vs2008.lib 
--tagTest_static_Debug_x32_vs2008.exe
--tagTest_Debug_x32_vs2008.exe
--
--targetdir "../lib" or "../bin"
--objdir "../intermediate/debug/tag/x32"	
--targetsuffix("_static_debug_x32_"..action)
function setTargetObjDir(outDir)
	local osplat = {
		x32 = "win32", 
		x64 = "win64", 
		Universal = "universal", 
		}

	for _, cfg in ipairs(configurations()) do
		for _, plat in ipairs(platforms()) do
			local action = _ACTION or ""
			
			local prj = project()
			
			--for dll and app
			local targetPath = outDir or "../bin"
			
			local osPlat = osplat[plat] or "universal"
			
			--"_debug_win32_vs2008"
			local suffix = "_" .. cfg .. "_" .. osplat[plat] .. "_" .. action
			
			if string.match(cfg, "DLL") == nil then
				--static
				--"_static_debug_win32_vs2008"
				--suffix = "_static" .. suffix
			else
				targetPath = "../bin"
			end
			
			suffix = string.lower(suffix)

			--[[
			if string.match(prj.name, "gtest") ~= nil then
				--
			elseif string.match(prj.name, "test") ~= nil or string.match(prj.name, "App") ~= nil  then
				targetPath = "../bin"
			end
			--]]


			--NO PLATFORM INCLUDED HERE, as vs automatically append it
			--"../intermediate/debug/vs2008/tag"
			--"../intermediate/debug/vs2008/tagTest"
			local obj_path = "../intermediate/" .. cfg .. "/" .. action .. "/" .. prj.name
			
			obj_path = string.lower(obj_path)
			
			--[[
			print("--------------"..cfg.." "..plat.."--------------")
			print("targetdir:"..targetPath)
			print("objdir:"..obj_path)
			print("targetsuffix:"..suffix)
			--]]
			 
			configuration {cfg, plat}
				targetdir(targetPath)
				objdir(obj_path)
				targetsuffix(suffix)
		end
	end
end


--used only for project tag
function setTargetPDBFile(outDir)
	local action = _ACTION or ""
	
	--for dll and app
	local targetPath = outDir or "../bin"
	
	for _, cfg in ipairs(configurations()) do
		--"_debugdll_win32_vs2003"
		local suffix = "_" .. cfg .. "_" .. "win32" .. "_" .. action
		suffix = string.lower(suffix)
		local fileName = targetPath .. "/" .. "behaviac" .. suffix .. ".pdb"
		
		--print(fileName)
		
		configuration {cfg, "vs2003"}
			linkoptions('/PDB:' .. fileName)
	end
end

			
--generate the full name for the specifed 'libBaseName' and links
--libBaseName, the lib base name, like "gtest"
--bStatic, if true, libBaseName is only linked as static lib
function linkLib(libBaseName, bStatic, bAll)
	local osplat = {
		x32 = "win32", 
		x64 = "win64", 
		Universal = "universal", 
		}

	for _, cfg in ipairs(configurations()) do
		local cfgName = cfg

		if bAll or string.match(cfgName, "DLL") ~= nil then 
			for _, plat in ipairs(platforms()) do
				local action = _ACTION or ""
				
				local prj = project()
				
				local osPlat = osplat[plat] or "universal"
				
				--if static only, to trim "DLL"
				if bStatic and string.match(cfgName, "DLL") then
					local index = string.find(cfgName, "DLL")
					cfgName = string.sub(cfgName, 0, index - 1)
				end
				
				--"_debug_win32_vs2008"
				local suffix = "_" .. cfgName .. "_" .. osplat[plat] .. "_" .. action
				
				if bStatic or string.match(cfgName, "DLL") == nil then
					--static
					--"_static_debug_win32_vs2008"
					--suffix = "_static" .. suffix
				end
				
				suffix = string.lower(suffix)
				
				local libFullName = libBaseName or ""
				libFullName = libFullName .. suffix
				
				--print(libFullName)
				
				configuration {cfg, plat}
					links(libFullName)
			end --for(plat)
		end
	end
end

local action = _ACTION or ""

solution "behaviac"
	configurations { "DebugStatic", "ReleaseStatic", "ProfileStatic" }
	if string.match(action, "vs20") ~= nil then
		configurations { "DebugDLL", "ReleaseDLL", "ProfileDLL" }
	end
	
	--configurations { "DebugStatic", "ReleaseStatic", "ProfileStatic" }
	
	if string.match(action, "vs20") ~= nil then		
		--platforms { "x32", "x64" }
		platforms { "x32" }
	elseif string.match(action, "gmake") ~= nil then
		platforms { "x32" }
	elseif string.match(action, "xcode") ~= nil then
		platforms { "Universal" }
		--defines { "__APPLE__" }
		xcodebuildsettings
		{
			'ARCHS = "$(ARCHS_STANDARD_32_BIT)"',
			'VALID_ARCHS = "$(ARCHS_STANDARD_32_BIT)"',
			--'SDKROOT = "/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iphoneos6.0.sdk"',
			--'SDKROOT = "iphoneos6.0"',
			'SDKROOT = "iphoneos"',
			--'SDKROOT = "macosx"',
			"ONLY_ACTIVE_ARCH = YES",
			--"INFOPLIST_FILE = info.plist",
			--'CODE_SIGN_IDENTITY = "iPhone Developer"'
		}
	end

	--clean generated project files
	if action == "clean" then
		for a in premake.action.each() do
			-- action trigger is "vs2008", "gmake", etc.
			local pathT = "./"..a.trigger
			os.rmdir(pathT)
		end
		
		--[[
		for _, cfg in ipairs(configurations()) do
			local objPath = "../intermediate/"..cfg
			objPath = string.lower(objPath)
			--print(objPath)
			os.rmdir(objPath)
		end
		--]]
	end
	
	--set the path where the project files are genrated
	--local path = "./"..string.lower(action)
	local pathGen = "./"..action

	-- if string.match(action, "jni") ~= nil then
	-- 	pathGen = "./android/"..action
	-- end

	location(pathGen)
	
	includedirs { "../inc" }
	
	language "C++"

	--TODO: find a way to conbine cfg, proj, platform's name to make a path for :objpath, targetpath, targetsuffix, etc.
	--set the targetdir for configurations
	--local targetPath = "./bin/"..${cfg.name}
	--targetdir(targetPath)
	--local objPath = "./intermediate/"..${cfg.name}.."/"..${proj.name}
	--objdir(objPath)
	configuration "x32"
		defines "WIN32"
	configuration "x64"
		defines "WIN64"
	
	configuration { "Debug*" }
		defines { "_DEBUG", "DEBUG"}
		flags   { "Symbols" }

	configuration { "Release*" }
		defines { "NDEBUG" }
		flags   { "Optimize" } 
		
	configuration { "Profile*" }
		defines { "NDEBUG" }
		flags   { "Optimize" } 
		--flags   { "Symbols" }
		
	configuration { "vs2010", "Profile*" }
		defines { "_ITERATOR_DEBUG_LEVEL=0" }
	
	configuration { "*" }
		flags {
			"ExtraWarnings",
			-- "FatalWarnings",
			"NoExceptions",
			"NoRTTI",
			--"EnableSSE2",
			"FloatFast",
			}

	--flags { "StaticRuntime" }
	--linkoptions {"/NODEFAULTLIB:msvcrtd.lib", "/NODEFAULTLIB:msvcprtd.lib", 
	--		"/NODEFAULTLIB:msvcrt.lib", "/NODEFAULTLIB:msvcprt.lib"	}
			
	configuration { "Release*" }
		buildoptions { 
			"/Ox", -- Full optimization
			"/Oi", -- Enable intrinsic functions
			"/Ob1", -- inline
			"/Ot",  -- Favor faster code
		}

	configuration { "Profile*" }
		buildoptions { 
			"/Ox", -- Full optimization
			"/Oi", -- Enable intrinsic functions
			"/Ob1", -- inline
			"/Ot",  -- Favor faster code
		}
		linkoptions {
			"/DEBUG "
		}
		flags   { "Symbols" }			

	if string.match(action, "gmake") ~= nil or string.match(action, "xcode") ~= nil or string.match(action, "jni") ~= nil then	
		configuration {}
			buildoptions { 
				--"-Wno-reorder", -- warning ¡®xxx¡¯ will be initialized after when initialized here
				"-Wno-invalid-offsetof", -- invalid access to non-static data member ¡®xxx¡¯  of NULL object
				"-Wno-array-bounds", -- array index 'x' is past the end of the array
				"-Wno-unused-local-typedefs", --warning: typedef '_static_assert_typedef_' locally defined but not used
				"-Wno-maybe-uninitialized", --warning: 'lhs' may be used uninitialized in this function
				--"-finput-charset=UTF-8", -- invalid access to non-static data member ¡®xxx¡¯  of NULL object
			}
	end
	
	if string.match(action, "gmake") ~= nil or string.match(action, "jni") ~= nil then	
		configuration {}
			buildoptions { 
				"-finput-charset=UTF-8", -- invalid access to non-static data member ¡®xxx¡¯  of NULL object
			}
	end
	--vcc
	if string.match(action, "vs20") ~= nil then
		if string.match(action, "vs2003") == nil then
			defines { "_HAS_EXCEPTIONS=0" }
		end
	
		defines { "_CRT_SECURE_NO_WARNINGS" }
	end
	
	project "behaviac"
		uuid "332CEEDC-7568-D84C-B9C6-B710915836ED"
		
		kind "StaticLib"
 
		files  { 
			"../inc/**.h", 
			"../inc/**.hpp", 
			"../inc/**.inl", 
			"../src/**.cpp", 
			"../src/**.h", 
			"../src/**.hpp", 
			"../src/**.incl", 
		}
		
		excludes "../inc/behaviac/base/script/lua/*.*"
		excludes "../src/base/script/lua/*.*"

		--includedirs { "" }
	
		--defines("RAPIDXML_NO_EXCEPTIONS")
		if string.match(action, "vs20") ~= nil then
			configuration { "Debug*", "not vs2003" }
				defines("_CRTDBG_MAP_ALLOC")
		end
		
		if string.match(action, "vs20") ~= nil then
			configuration "*DLL"
				kind "SharedLib"
				implibdir "../lib"
		end
			
		setTargetObjDir("../lib")
		
		setTargetPDBFile("../../bin")
		
		--configuration { "not *DLL" }
		configuration { "Debug or Release or Profile" }
			defines {"_LIB"}

		configuration { "DebugDLL or ReleaseDLL or ProfileDLL" }
			defines {"_WINDOWS", "_USRDLL", "BEHAVIAC_DLL", "BEHAVIACDLL_EXPORTS"}
	
		configuration { "Profile*" }
			defines {"BEHAVIAC_CFG_PROFILE"}
			
	
	project "btunittest"
		if string.match(action, "xcode") ~= nil then
			kind     "WindowedApp"
		else
			kind     "ConsoleApp"
		end
		
		files  { 
			"../test/btunittest/**.h", 
			"../test/btunittest/**.cpp"
			}
			
		includedirs { "../inc", 
			"../../../include", 
			"../test/btunittest/"
			}

		libdirs { "../lib/"  }
			
		links "behaviac"
		linkLib("behaviac", false, true);
		
		setTargetObjDir("../bin")
		
		if string.match(action, "gmake") ~= nil or string.match(action, "xcode") ~= nil or string.match(action, "jni") ~= nil then	
			configuration {}
				buildoptions { 
					"-Wno-unused-parameter",
					"-Wno-unused-variable",
				}
		end

		--configuration  {"vs2008 or vs2010", "Debug or Release or Profile"}
			
		defines "_CONSOLE"
		
	project "btremotetest"
		kind     "ConsoleApp"
		files  { 
			"../test/btremotetest/**.h", 
			"../test/btremotetest/**.cpp",
			}
			
		includedirs { "../inc", 
			"../../../include"
			}

		libdirs { "../lib/"  }
			
		links "behaviac"
		
		setTargetObjDir("../bin")

		--configuration  {"vs2008 or vs2010", "Debug or Release or Profile"}
		--	linkoptions {"/nodefaultlib:libc.lib", "/nodefaultlib:libcd.lib"}
			
		defines "_CONSOLE"
		
	if string.match(action, "vs20") ~= nil then
		project "btperformance"
			kind     "ConsoleApp"
			files  { 
				"../test/btperformance/**.h", 
				"../test/btperformance/**.cpp",
				}
				
			excludes "../test/btperformance/generated_behaviors.cpp"
				
			includedirs { "../inc", 
				"../../../include"
				}

			libdirs { "../lib/"  }
				
			links "behaviac"
			
			setTargetObjDir("../bin")

			--configuration  {"vs2008 or vs2010", "Debug or Release or Profile"}
			--	linkoptions {"/nodefaultlib:libc.lib", "/nodefaultlib:libcd.lib"}
				
			defines "_CONSOLE"
	end