@echo off
setlocal enableextensions enabledelayedexpansion
set VARS_INITED=NO

call :InitWindowsSdkDirUsingDefaultPath
if "%WIN_SDK_DIR%"=="" (
	color cf
	echo "**********************************"
	echo "*** Windows SDK not detected ***"
	echo "**********************************"
	goto :eof
)

echo "%WIN_SDK_DIR%"
set DIRECTSHOW_BASECLASS_DIR="%WIN_SDK_DIR%Samples\multimedia\directshow\baseclasses"
if not exist %DIRECTSHOW_BASECLASS_DIR% (
	color cf
	echo "******************************************************"
	echo "*** DirectShow sources not detected in Windows SDK ***"
	echo "******************************************************"
	goto :eof
)

set VC_VARS_ALL=""

FOR /L %%n IN (20,-1,10) DO (
  
  set vs_tools=VS%%n0COMNTOOLS
  
  if defined !vs_tools! (
	goto :break
  )
)
:break

if "!vs_tools!"=="" (

	color cf
	echo "**********************************"
	echo "*** Visual Studio not detected ***"
	echo "**********************************"
	goto :eof
)
set _vs_tools_=!vs_tools!
call set vs_tools_path=%%%_vs_tools_%%%
set VC_VARS_ALL="%vs_tools_path:~0,-14%VC\vcvarsall.bat"
set LIBWEBCAM_VARS_INITED=YES

endlocal & SET LIBWEBCAM_VARS_INITED=%LIBWEBCAM_VARS_INITED%& SET VC_VARS_ALL=%VC_VARS_ALL%& SET DIRECTSHOW_BASECLASS_DIR=%DIRECTSHOW_BASECLASS_DIR%

goto :eof

:InitWindowsSdkDirUsingDefaultPath
	set "WIN_SDK_DIR=C:\Program Files\Microsoft SDKs\Windows\v7.1\"
	goto :eof
	
:InitWindowsSdkDirUsingRegistry
	call :InitWindowsSdkDirHelperUsingRegistry HKLM > nul 2>&1
	if errorlevel 1 call :InitWindowsSdkDirHelperUsingRegistry HKCU > nul 2>&1
	if errorlevel 1 echo WIN_SDK_DIR not found
	exit /B 0
  
:InitWindowsSdkDirHelperUsingRegistry
	set WIN_SDK_DIR=
	for /F "tokens=1,2*" %%i in ('reg query "%1\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder"') DO (
		if "%%i"=="CurrentInstallFolder" (
			set "WIN_SDK_DIR=%%k"
		)
	)
	@if "%WIN_SDK_DIR%"=="" exit /B 1
	@exit /B 0