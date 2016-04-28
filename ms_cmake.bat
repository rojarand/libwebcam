@ECHO OFF
COLOR 07
SET BUILD_DIR="build"
REM SET /P COMMAND="Type command to execute (type build, install or clear): "
SET COMMAND=%1

2>NUL CALL :CASE_%COMMAND% # jump to :CASE_build, :CASE_install, etc.
IF ERRORLEVEL 1 CALL :DEFAULT_CASE # if command doesn't exist

EXIT /B

:CASE_test
	
	GOTO END_CASE
:CASE_build
	SET vcvarsall=""
	
	IF NOT "%VS150COMNTOOLS%"=="" (
		SET vcvarsall="%VS150COMNTOOLS:~0,-14%VC\vcvarsall.bat"
	) ELSE IF NOT "%VS140COMNTOOLS%"=="" (
		SET vcvarsall="%VS140COMNTOOLS:~0,-14%VC\vcvarsall.bat"
	) ELSE IF NOT "%VS120COMNTOOLS%"=="" (
		SET vcvarsall="%VS120COMNTOOLS:~0,-14%VC\vcvarsall.bat"
	) ELSE IF NOT "%VS100COMNTOOLS%"=="" (
		SET vcvarsall="%VS100COMNTOOLS:~0,-14%VC\vcvarsall.bat"
	) ELSE (
		COLOR CF
		echo "**********************************"
		echo "*** Visual Studio not detected ***"
		echo "**********************************"
		GOTO END_CASE
	)

	call :InitWindowsSdkDir
	IF "%WIN_SDK_DIR%"=="" (
		
		echo "**********************************"
		echo "*** Windows SDK not detected ***"
		echo "**********************************"
		GOTO END_CASE
	)
	SET DIRECTSHOW_BASECLASS_DIR="%WIN_SDK_DIR%Samples\multimedia\directshow\baseclasses"
	IF NOT EXIST %DIRECTSHOW_BASECLASS_DIR% (
		COLOR CF
		echo "******************************************************"
		echo "*** DirectShow sources not detected in Windows SDK ***"
		echo "******************************************************"
		GOTO END_CASE
	)

	mkdir %BUILD_DIR% 
	cd %BUILD_DIR%
	cmake -DDIRECTSHOW_BASECLASS_DIR=%DIRECTSHOW_BASECLASS_DIR% ..
	echo calling vcvarsall 
	call %vcvarsall% x86
	msbuild libwebcam.sln /P:Configuration=Debug
	msbuild libwebcam.sln /P:Configuration=Release
	cd ..
	GOTO END_CASE
:CASE_install
  REM COLOR 9F
  cd $BUILD_DIR
  make install
  GOTO END_CASE
:CASE_clean
  REM COLOR 0F
  @RD /S /Q %BUILD_DIR%
  GOTO END_CASE
:DEFAULT_CASE
  ECHO "Usage: cmake.bat { clean | build | install }"
  GOTO END_CASE
:END_CASE
  VER > NUL # reset ERRORLEVEL
  GOTO :EOF # return from CALL

:InitWindowsSdkDir
	call :InitWindowsSdkDirHelper HKLM > nul 2>&1
	if errorlevel 1 call :InitWindowsSdkDirHelper HKCU > nul 2>&1
	if errorlevel 1 echo WIN_SDK_DIR not found
	exit /B 0
  
:InitWindowsSdkDirHelper
	SET WIN_SDK_DIR=
	for /F "tokens=1,2*" %%i in ('reg query "%1\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder"') DO (
		if "%%i"=="CurrentInstallFolder" (
			SET "WIN_SDK_DIR=%%k"
		)
	)
	@if "%WIN_SDK_DIR%"=="" exit /B 1
	@exit /B 0
	