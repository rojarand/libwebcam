TODO: Add visual studio path detection, force visual studio build by explicit parameter, change enumerate dir name to enumerator, add exception handling

## Installation

#### Prepare sources
Download [libwebcam.zip] (https://github.com/rojarand/libwebcam/archive/master.zip). Extract content to desired libwebcam development directory. 

#### What is inside
Root libwebcam directory contains serveral directories:  

| Directory     | Content                 |
| ------------- |-------------------------|
| libwebcam     | library source files    |
| examples      | example program sources |
| scripts       | build scripts           |

The installation guide differs for Linux and Windows, so it is described separately.

* [Windows](#windows)
* [Linux](#linux)

#### Windows

> Note: This guide was tested on Windows 8 and 10 but should work for other Windows version as well.

###### Complete required software

The installation guide assumes you have already installed:
- [Visual Studio IDE (for C++) 2010 or newer] (https://www.visualstudio.com/),
- [CMake 2.8.12 or newer] (https://cmake.org/),
- [Windows 7 SDK] (https://www.microsoft.com/en-us/download/details.aspx?id=8442)

###### Setup environment

Add cmake to your system path  
Open command window and type:  
<pre>
  <code>cmake --version</code>
</pre>
If cmake is not recognized, open Run window (WIN+R) and type:  
<pre>
  <code>sysdm.cpl</code>
</pre>
In advanced tab click Environment Variables button. Find Path variable in System Variables list. Click edit button and add your CMake\bin directory path(ie. C:\Program Files (x86)\CMake\bin).  
> Note: Each path entry is separated by ;  

Reopen your command window and type again:  
<pre>
  <code>cmake --version</code>
</pre>
If command is recognized, make sure cmake meets minimal version requirement. If command is not recognized, fix your cmake availibity in command window.

Update baseclass project

Launch Visual Studio IDE in administrator mode. Open baseclass.vcproj placed in Windows 7 SDK installation (ie. C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses\) and proceed coversion. Baseclass.vcproj will be converted to digestible baseclass.vxcproj version.

> Note: Visual Studio must be launched in administrator mode because conversion writes to write protected directory

###### Build library

Build process creates (inside build directory):
- static webcam.lib library (debug and release),
- Visual Studio project files,
- Visual Studio solution files.

Open command window in administrator mode. Navigate to extracted libwebcam directory and type: 
<pre>
  <code>ms_cmake.bat build</code>
</pre>
The command will start libwebcam build process.



#### Linux
qwerty  
qwerty  
qwerty  
qwerty  








