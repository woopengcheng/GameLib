@echo off
setlocal enabledelayedexpansion
mode con cols=80 lines=25
set /a sum=0
cd ..
echo.
echo.
echo 删除当前所在目录及其子目录下10层内的所有空文件夹，以节省空间。
echo.
echo       当前工作目录为： %cd%
echo.
echo     任意键   开始删除空文件夹
pause>nul
echo -----------------------------------------------------------------------------

for /l %%i in (1 1 10) do (
for /f "delims=" %%i in ('dir /ad /b /s') do (
dir /b "%%i"|findstr .>nul||rd "%%i"&&echo 成功删除空文件夹：%%i&&set /a sum=sum+1
)

)
echo -----------------------------------------------------------------------------
echo      共删除%cd%目录下 空文件夹个数：%sum%
echo.
echo.
echo    按任意键退出
pause>nul
set sum=
exit