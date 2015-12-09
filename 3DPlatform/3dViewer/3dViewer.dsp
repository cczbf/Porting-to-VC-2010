# Microsoft Developer Studio Project File - Name="3dViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=3dViewer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3dViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3dViewer.mak" CFG="3dViewer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3dViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "3dViewer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Platform/2DImageProcessingOnly/3dViewer", SGIBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3dViewer - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /arch:SSE2 /GA /O3 /Qparallel /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib glut32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "3dViewer - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gz /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glut32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"library" /nodefaultlib:"MSVCRT.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "3dViewer - Win32 Release"
# Name "3dViewer - Win32 Debug"
# Begin Group "Other Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CArray2DInt.cpp
# End Source File
# Begin Source File

SOURCE=.\CArray2DInt1.cpp
# End Source File
# Begin Source File

SOURCE=.\CDIBFlood.cpp
# End Source File
# Begin Source File

SOURCE=.\CDiffusionVarCo.cpp

!IF  "$(CFG)" == "3dViewer - Win32 Release"

# ADD CPP /D "_USE_NON_INTEL_COMPILER"
# SUBTRACT CPP /Z<none>

!ELSEIF  "$(CFG)" == "3dViewer - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CGlobals.cpp
# End Source File
# Begin Source File

SOURCE=.\CIntArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CIntArray_.cpp
# End Source File
# Begin Source File

SOURCE=.\cl.cpp
# End Source File
# Begin Source File

SOURCE=.\CL0.cpp
# End Source File
# Begin Source File

SOURCE=.\CL0_.cpp
# End Source File
# Begin Source File

SOURCE=.\CL_.cpp
# End Source File
# Begin Source File

SOURCE=.\CLDIFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CLw.cpp
# End Source File
# Begin Source File

SOURCE=.\CLw_.cpp
# End Source File
# Begin Source File

SOURCE=.\CMyCGLib.cpp
# End Source File
# Begin Source File

SOURCE=.\CTransformedImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CTransformedImage.h
# End Source File
# Begin Source File

SOURCE=.\ctriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\CTriangle_.cpp
# End Source File
# Begin Source File

SOURCE=.\CTriangleArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CTriangleArray_.cpp
# End Source File
# Begin Source File

SOURCE=.\CVertex.cpp
# End Source File
# Begin Source File

SOURCE=.\CVertex_.cpp
# End Source File
# Begin Source File

SOURCE=.\CVertexArray.cpp
# End Source File
# Begin Source File

SOURCE=.\CVertexArray_.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubdivideSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelPts.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyTri.cpp
# End Source File
# Begin Source File

SOURCE=.\predicates.cpp
# End Source File
# Begin Source File

SOURCE=.\SubdivideMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\tetgen.cpp
# End Source File
# End Group
# Begin Group "Other Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CArray.h
# End Source File
# Begin Source File

SOURCE=.\CArray2DFLT.h
# End Source File
# Begin Source File

SOURCE=.\CArray2DInt.h
# End Source File
# Begin Source File

SOURCE=.\CArray2DInt1.h
# End Source File
# Begin Source File

SOURCE=.\CCursorBase.h
# End Source File
# Begin Source File

SOURCE=.\CCursorPan.h
# End Source File
# Begin Source File

SOURCE=.\CCursorZoomIn.h
# End Source File
# Begin Source File

SOURCE=.\CCursorZoomOut.h
# End Source File
# Begin Source File

SOURCE=.\CCursotZoomIn.h
# End Source File
# Begin Source File

SOURCE=.\CCursotZoomOut.h
# End Source File
# Begin Source File

SOURCE=.\CDIBFlood.h
# End Source File
# Begin Source File

SOURCE=.\CDiffusionVarCo.h
# End Source File
# Begin Source File

SOURCE=.\CGlobals.h
# End Source File
# Begin Source File

SOURCE=.\CIntArray.h
# End Source File
# Begin Source File

SOURCE=.\CIntArray_.h
# End Source File
# Begin Source File

SOURCE=.\cl.h
# End Source File
# Begin Source File

SOURCE=.\CL0.h
# End Source File
# Begin Source File

SOURCE=.\CL0_.h
# End Source File
# Begin Source File

SOURCE=.\CL_.h
# End Source File
# Begin Source File

SOURCE=.\classarr.h
# End Source File
# Begin Source File

SOURCE=.\CLDIFile.h
# End Source File
# Begin Source File

SOURCE=.\CLw.h
# End Source File
# Begin Source File

SOURCE=.\CLw_.h
# End Source File
# Begin Source File

SOURCE=.\CMyCGLib.h
# End Source File
# Begin Source File

SOURCE=.\CTriangle.h
# End Source File
# Begin Source File

SOURCE=.\CTriangle_.h
# End Source File
# Begin Source File

SOURCE=.\CtriangleArray.h
# End Source File
# Begin Source File

SOURCE=.\CTriangleArray_.h
# End Source File
# Begin Source File

SOURCE=.\CVertex.h
# End Source File
# Begin Source File

SOURCE=.\CVertex_.h
# End Source File
# Begin Source File

SOURCE=.\CVertexArray.h
# End Source File
# Begin Source File

SOURCE=.\CVertexArray_.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubdivideSetting.h
# End Source File
# Begin Source File

SOURCE=.\jitter.h
# End Source File
# Begin Source File

SOURCE=.\LabelPts.h
# End Source File
# Begin Source File

SOURCE=.\MeshGlobal.h
# End Source File
# Begin Source File

SOURCE=.\NTTYPE.H
# End Source File
# Begin Source File

SOURCE=.\PolyTri.h
# End Source File
# Begin Source File

SOURCE=.\PolyTriGlobal.h
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STR.h
# End Source File
# Begin Source File

SOURCE=.\SubdivideMesh.h
# End Source File
# Begin Source File

SOURCE=.\tetgen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\3dViewer.ico
# End Source File
# Begin Source File

SOURCE=.\3dViewer.rc
# End Source File
# Begin Source File

SOURCE=.\res\3dViewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\3dViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\Cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\zoomin.cur
# End Source File
# End Group
# Begin Group "code"

# PROP Default_Filter ""
# End Group
# Begin Group "MFC Frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3dViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\3dViewer.h
# End Source File
# Begin Source File

SOURCE=.\3dViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\3dViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\3dViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\3dViewerView.h
# End Source File
# Begin Source File

SOURCE=.\CCursorBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CCursorPan.cpp
# End Source File
# Begin Source File

SOURCE=.\CCursorZoomIn.cpp
# End Source File
# Begin Source File

SOURCE=.\CCursorZoomOut.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab00.cpp

!IF  "$(CFG)" == "3dViewer - Win32 Release"

# ADD CPP /D "_USE_NON_INTEL_COMPILER"

!ELSEIF  "$(CFG)" == "3dViewer - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDlgTab00.h
# End Source File
# Begin Source File

SOURCE=.\cDlgTab01.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgTab01.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab02.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab02.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab03.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab03.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab04.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab04.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab05.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab05.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab06.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab06.h
# End Source File
# Begin Source File

SOURCE=.\CDlgTab07.cpp
# End Source File
# Begin Source File

SOURCE=.\CDlgTab07.h
# End Source File
# Begin Source File

SOURCE=.\cDlgTestPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgTestPanel.h
# End Source File
# Begin Source File

SOURCE=.\CEasyProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CEasyProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\CTestPanelTabs.cpp
# End Source File
# Begin Source File

SOURCE=.\CTestPanelTabs.h
# End Source File
# Begin Source File

SOURCE=.\CTool3D.cpp
# End Source File
# Begin Source File

SOURCE=.\CTool3D.h
# End Source File
# Begin Source File

SOURCE=.\CToolBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CToolBase.h
# End Source File
# Begin Source File

SOURCE=.\CToolPan.cpp
# End Source File
# Begin Source File

SOURCE=.\CToolPan.h
# End Source File
# Begin Source File

SOURCE=.\CToolZoomIn.cpp
# End Source File
# Begin Source File

SOURCE=.\CToolZoomIn.h
# End Source File
# Begin Source File

SOURCE=.\CToolZoomOut.cpp
# End Source File
# Begin Source File

SOURCE=.\CToolZoomOut.h
# End Source File
# Begin Source File

SOURCE=.\Dlg2DMeshInfoDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg2DMeshInfoDisp.h
# End Source File
# Begin Source File

SOURCE=.\DlgLightDispMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLightDispMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptiSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptiSettings.h
# End Source File
# Begin Source File

SOURCE=.\GraphicDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryEdit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=".\MFC 60.h"
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\ScrollDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollDlg.h
# End Source File
# End Group
# Begin Group "OpenGL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\C3D.cpp
# End Source File
# Begin Source File

SOURCE=.\C3D.h
# End Source File
# Begin Source File

SOURCE=.\COpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\COpenGL.h
# End Source File
# Begin Source File

SOURCE=.\COpenGL3D.cpp
# End Source File
# Begin Source File

SOURCE=.\COpenGL3D.h
# End Source File
# Begin Source File

SOURCE=.\COpenGL3DEx.cpp
# End Source File
# Begin Source File

SOURCE=.\COpenGL3DEx.h
# End Source File
# Begin Source File

SOURCE=.\glut.h
# End Source File
# Begin Source File

SOURCE=.\TrackBall.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackBall.h
# End Source File
# End Group
# Begin Group "Algorithms"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BitAdjacentMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\BitAdjacentMatrix.h
# End Source File
# Begin Source File

SOURCE=.\CFFT.cpp
# End Source File
# Begin Source File

SOURCE=.\CFFT.h
# End Source File
# Begin Source File

SOURCE=.\CFWT.cpp
# End Source File
# Begin Source File

SOURCE=.\CFWT.h
# End Source File
# Begin Source File

SOURCE=.\CFWT_FFT_Util.cpp
# End Source File
# Begin Source File

SOURCE=.\CFWT_FFT_Util.h
# End Source File
# Begin Source File

SOURCE=.\CMeshOpti.cpp
# End Source File
# Begin Source File

SOURCE=.\CMeshOpti.h
# End Source File
# Begin Source File

SOURCE=.\CRelxationLabelling.cpp
# End Source File
# Begin Source File

SOURCE=.\CRelxationLabelling.h
# End Source File
# Begin Source File

SOURCE=.\CWaveletFilterBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CWaveletFilterBase.h
# End Source File
# Begin Source File

SOURCE=.\DigitalGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\DigitalGeometry.h
# End Source File
# Begin Source File

SOURCE=.\fdajc.cpp
# End Source File
# Begin Source File

SOURCE=.\fmin.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsAlgorithms.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsAlgorithms.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\lnsrch.cpp
# End Source File
# Begin Source File

SOURCE=.\lubksb.cpp
# End Source File
# Begin Source File

SOURCE=.\ludcmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\MemoryPool.h
# End Source File
# Begin Source File

SOURCE=.\Mesh3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh3D.h
# End Source File
# Begin Source File

SOURCE=.\MeshForGeoImg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshForGeoImg.h
# End Source File
# Begin Source File

SOURCE=.\mnewt.cpp
# End Source File
# Begin Source File

SOURCE=.\newt.cpp
# End Source File
# Begin Source File

SOURCE=.\nrutil.cpp
# End Source File
# Begin Source File

SOURCE=.\nrutil.h
# End Source File
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangles.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangles.h
# End Source File
# End Group
# Begin Group "Model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\2DMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\2DMesh.h
# End Source File
# Begin Source File

SOURCE=.\CMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\cmesh.h
# End Source File
# Begin Source File

SOURCE=.\GLM.CPP
# End Source File
# Begin Source File

SOURCE=.\GLM.H
# End Source File
# Begin Source File

SOURCE=.\ply.cpp
# End Source File
# Begin Source File

SOURCE=.\ply.h
# End Source File
# Begin Source File

SOURCE=.\ReadOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadOBJ.h
# End Source File
# Begin Source File

SOURCE=.\ReadPLY.cpp
# End Source File
# Begin Source File

SOURCE=.\readply.h
# End Source File
# Begin Source File

SOURCE=.\TriMesh.h
# End Source File
# Begin Source File

SOURCE=.\TriMesh_connectivity.cpp
# End Source File
# Begin Source File

SOURCE=.\TriMesh_io.cpp
# End Source File
# Begin Source File

SOURCE=.\TriMesh_normals.cpp
# End Source File
# Begin Source File

SOURCE=.\TriMesh_stats.cpp
# End Source File
# Begin Source File

SOURCE=.\TriMesh_tstrips.cpp
# End Source File
# Begin Source File

SOURCE=.\typedef.h
# End Source File
# Begin Source File

SOURCE=.\Vec.h
# End Source File
# End Group
# Begin Group "Bmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CBmpDblBuffering.cpp
# End Source File
# Begin Source File

SOURCE=.\CBmpDblBuffering.h
# End Source File
# Begin Source File

SOURCE=.\cbmpviewerdata.cpp
# End Source File
# Begin Source File

SOURCE=.\cbmpviewerdata.h
# End Source File
# Begin Source File

SOURCE=.\cdib.cpp
# End Source File
# Begin Source File

SOURCE=.\cdib.h
# End Source File
# Begin Source File

SOURCE=.\CDIBOrg.cpp
# End Source File
# Begin Source File

SOURCE=.\CDIBOrg.h
# End Source File
# Begin Source File

SOURCE=.\CHtArray2Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\CHtArray2Dib.h
# End Source File
# Begin Source File

SOURCE=.\CRawImage16.cpp
# End Source File
# Begin Source File

SOURCE=.\CRawImage16.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
