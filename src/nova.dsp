# Microsoft Developer Studio Project File - Name="nova" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=nova - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nova.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nova.mak" CFG="nova - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nova - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "nova - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nova - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\yarp\include" /I "..\..\yarp\include\winnt" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "nova - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\yarp\include" /I "..\..\yarp\include\winnt" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\novad.lib"

!ENDIF 

# Begin Target

# Name "nova - Win32 Release"
# Name "nova - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\NovaClient.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaInit.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaSemaphore.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaServer.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaTelnetClient.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaThread.cpp
# End Source File
# Begin Source File

SOURCE=.\NovaTime.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Nova.h
# End Source File
# Begin Source File

SOURCE=.\NovaClient.h
# End Source File
# Begin Source File

SOURCE=.\NovaInit.h
# End Source File
# Begin Source File

SOURCE=.\NovaSemaphore.h
# End Source File
# Begin Source File

SOURCE=.\NovaServer.h
# End Source File
# Begin Source File

SOURCE=.\NovaStream.h
# End Source File
# Begin Source File

SOURCE=.\NovaTelnetClient.h
# End Source File
# Begin Source File

SOURCE=.\NovaThread.h
# End Source File
# Begin Source File

SOURCE=.\NovaTime.h
# End Source File
# End Group
# End Target
# End Project
