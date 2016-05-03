@echo off
color 07
SET BUILD_DIR="build"
REM SET /P COMMAND="Type command to execute (type build, install or clear): "
SET COMMAND=%1

2>NUL CALL :CASE_%COMMAND%
IF ERRORLEVEL 1 CALL :DEFAULT_CASE # if command doesn't exist

EXIT /B

:CASE_test
	goto END_CASE
:CASE_build

	pushd ..\..\
	set LIBWEBCAM_DIR=%CD%
	cd %~dp0
	
	call %LIBWEBCAM_DIR%\installation\init_vars.bat
	
	mkdir %BUILD_DIR% 
	cd %BUILD_DIR%
	cmake -DLIBWEBCAM_DIR=%LIBWEBCAM_DIR% .. 
	call %VC_VARS_ALL% x86
	msbuild enumerator.sln /P:Configuration=Debug
	msbuild enumerator.sln /P:Configuration=Release
	xcopy /y ..\..\..\libwebcam\usb.ids Debug\
	xcopy /y ..\..\..\libwebcam\usb.ids Release\
	cd ..
	goto END_CASE
:CASE_clean
  REM COLOR 0F
  @RD /S /Q %BUILD_DIR%
  goto END_CASE
:DEFAULT_CASE
  ECHO "Usage: cmake.bat { clean | build | install }"
  goto END_CASE
:END_CASE
  VER > NUL # reset ERRORLEVEL
  goto :EOF # return from CALL