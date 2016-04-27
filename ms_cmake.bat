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
	IF NOT "%VS120COMNTOOLS%"=="" (
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

	SET DIRECTSHOW_BASECLASS_DIR=""
	SET DIRECTSHOW_BASECLASS_DIR="%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses"
	REM SET DIRECTSHOW_BASECLASS_DIR="C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/multimedia/directshow/baseclasses"
	IF NOT EXIST %DIRECTSHOW_BASECLASS_DIR% (
		COLOR CF
		echo "***************************************"
		echo "*** DirectShow sources not detected ***"
		echo "***************************************"
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