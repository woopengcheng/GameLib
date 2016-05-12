

md d:\Temp

call cocos new -p com.tencent.airbattledemo -l cpp -d d:\Temp airbattledemo_exe

set GENERATED_PATH_=d:\Temp\airbattledemo_exe

rem echom press any key to copy Classes and Resources
rem pause

rd %GENERATED_PATH_%\Classes /s/q
rd %GENERATED_PATH_%\Resources /s/q
rem rd ./airbattledemo/proj.win32 /s/q

robocopy ./Classes/ %GENERATED_PATH_%/Classes /s
robocopy ./Resources/ %GENERATED_PATH_%/Resources /s
robocopy ./proj.win32/ %GENERATED_PATH_%/proj.win32 /s

attrib -R %GENERATED_PATH_%/Resources\*.*  /S

robocopy %GENERATED_PATH_%/ ./ /s /xd Classes Resources

copy .\Classes\CMakeLists.txt .\CMakeLists.txt /Y

rd %GENERATED_PATH_%\ /s/q

