@ECHO OFF

SET BUILD_DIR="build"
#SET /P COMMAND="Type command to execute (type build, install or clear): "
SET COMMAND=%1

2>NUL CALL :CASE_%COMMAND% # jump to :CASE_build, :CASE_install, etc.
IF ERRORLEVEL 1 CALL :DEFAULT_CASE # if command doesn't exist

EXIT /B

:CASE_build
  #COLOR CF
  mkdir %BUILD_DIR% 
  cd %BUILD_DIR%
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  #make # VERBOSE=1
  GOTO END_CASE
:CASE_install
  #COLOR 9F
  GOTO END_CASE
:CASE_clear
  #COLOR 0F
  del %BUILD_DIR%
  GOTO END_CASE
:DEFAULT_CASE
  ECHO "Usage: cmake.bat { clean | build | install }"
  GOTO END_CASE
:END_CASE
  VER > NUL # reset ERRORLEVEL
  GOTO :EOF # return from CALL