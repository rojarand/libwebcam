@ECHO OFF
COLOR 07
SET BUILD_DIR="build"
REM SET /P COMMAND="Type command to execute (type build, install or clear): "
SET COMMAND=%1

2>NUL CALL :CASE_%COMMAND% # jump to :CASE_build, :CASE_install, etc.
IF ERRORLEVEL 1 CALL :DEFAULT_CASE # if command doesn't exist

EXIT /B

:CASE_test

	goto END_CASE
	
:CASE_build

	call init_vars.bat
	IF NOT "%LIBWEBCAM_VARS_INITED%"=="YES" (
		
		echo "*** Error: build variables are not set ***"
		GOTO END_CASE
	)
	
	call :GotoRootDir
	mkdir %BUILD_DIR%
	call :GotoBuildDir
	cmake -DDIRECTSHOW_BASECLASS_DIR=%DIRECTSHOW_BASECLASS_DIR% ..
	call %VC_VARS_ALL% x86
	
	msbuild libwebcam.sln /P:Configuration=Debug
	msbuild libwebcam.sln /P:Configuration=Release
	call :GotoInstallationDir
	goto END_CASE
:CASE_install
  goto END_CASE
:CASE_clean
  REM COLOR 0F
  call :GotoRootDir
  @RD /S /Q %BUILD_DIR%
  call :GotoInstallationDir
  GOTO END_CASE
:DEFAULT_CASE
  ECHO "Usage: cmake.bat { clean | build | install }"
  goto END_CASE
:END_CASE
  VER > NUL # reset ERRORLEVEL
  goto :EOF # return from CALL

:GotoBuildDir
	cd %~dp0
	pushd ..\%BUILD_DIR%
	goto :EOF
	
:GotoRootDir
	cd %~dp0
	pushd ..\
	goto :EOF

:GotoInstallationDir
  cd %~dp0
  goto :EOF