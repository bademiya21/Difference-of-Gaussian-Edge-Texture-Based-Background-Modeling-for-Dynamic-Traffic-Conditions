# Microsoft Developer Studio Project File - Name="cnt_traffic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=cnt_traffic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cnt_traffic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cnt_traffic.mak" CFG="cnt_traffic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cnt_traffic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "cnt_traffic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cnt_traffic - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\cvblobslib_OpenCV_v6" /I "..\ximage" /I "..\OpenCV_1.0" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /Zm1000 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 cximage.lib Jpeg.lib cvblobslib.lib highgui.lib cv.lib cxcore.lib urlmon.lib Wininet.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\cvblobslib_OpenCV_v6\Release" /libpath:"../ximage" /libpath:"../OpenCV_1.0"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "cnt_traffic - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\cvblobslib_OpenCV_v6" /I "..\ximage" /I "..\OpenCV_1.0" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cximaged.lib Jpegd.lib cvblobslibd.lib highgui.lib cv.lib cxcore.lib urlmon.lib Wininet.lib iphlpapi.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /out:"Debug/cnt_trafficd.exe" /pdbtype:sept /libpath:"..\cvblobslib_OpenCV_v6\Debug" /libpath:"..\ximage" /libpath:"..\OpenCV_1.0" /libpath:"..\mac" /HEAP:10000000
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "cnt_traffic - Win32 Release"
# Name "cnt_traffic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\axismediacontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\Calibration.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt_traffic.cpp
# End Source File
# Begin Source File

SOURCE=.\cnt_traffic.rc
# End Source File
# Begin Source File

SOURCE=.\cnt_trafficDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tracking.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\axismediacontrol.h
# End Source File
# Begin Source File

SOURCE=.\Calibration.h
# End Source File
# Begin Source File

SOURCE=.\cnt_traffic.h
# End Source File
# Begin Source File

SOURCE=.\cnt_trafficDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tracking.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cnt_traffic.ico
# End Source File
# Begin Source File

SOURCE=.\res\cnt_traffic.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section cnt_traffic : {19722801-5830-4EC4-BD69-C3CF70776E0E}
# 	2:5:Class:CAxisMediaControl
# 	2:10:HeaderFile:axismediacontrol.h
# 	2:8:ImplFile:axismediacontrol.cpp
# End Section
# Section cnt_traffic : {745395C8-D0E1-4227-8586-624CA9A10A8D}
# 	2:21:DefaultSinkHeaderFile:axismediacontrol.h
# 	2:16:DefaultSinkClass:CAxisMediaControl
# End Section
