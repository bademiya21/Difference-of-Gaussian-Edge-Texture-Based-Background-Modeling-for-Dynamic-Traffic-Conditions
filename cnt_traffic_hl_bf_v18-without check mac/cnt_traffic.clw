; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCnt_trafficDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "cnt_traffic.h"

ClassCount=6
Class1=CCnt_trafficApp
Class2=CCnt_trafficDlg

ResourceCount=5
Resource1=IDR_MAINFRAME
Class3=CTracking
Resource2=IDD_CALIBRATION (English (U.S.))
Class4=CPointSelection
Class5=CSelectPoint
Class6=CCalibration
Resource3=IDD_CNT_TRAFFIC_DIALOG
Resource4=IDD_CNT_TRAFFIC_DIALOG (English (U.S.))
Resource5=IDD_CALIBRATION

[CLS:CCnt_trafficApp]
Type=0
HeaderFile=cnt_traffic.h
ImplementationFile=cnt_traffic.cpp
Filter=N

[CLS:CCnt_trafficDlg]
Type=0
HeaderFile=cnt_trafficDlg.h
ImplementationFile=cnt_trafficDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STATIC_CURRENT_FRAME



[DLG:IDD_CNT_TRAFFIC_DIALOG]
Type=1
Class=CCnt_trafficDlg
ControlCount=12
Control1=IDC_STATIC_INPUT,static,1342177806
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_LIST_COUNT,SysListView32,1350631425
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816
Control8=IDC_AXISMEDIACONTROL1,{745395C8-D0E1-4227-8586-624CA9A10A8D},1073807360
Control9=IDC_STATIC_CURRENT_FRAME,static,1342308352
Control10=IDC_EDIT_EDGETHRESHOLD,edit,1350631552
Control11=IDC_STATIC_EDGE_THRESHOLD,static,1342308352
Control12=IDC_BUTTON6,button,1342242816

[CLS:CPointSelection]
Type=0
HeaderFile=PointSelection.h
ImplementationFile=PointSelection.cpp
BaseClass=CDialog
Filter=D
LastObject=CPointSelection
VirtualFilter=dWC

[CLS:CSelectPoint]
Type=0
HeaderFile=SelectPoint.h
ImplementationFile=SelectPoint.cpp
BaseClass=CDialog
Filter=D
LastObject=IDCANCEL

[DLG:IDD_CALIBRATION]
Type=1
Class=CCalibration
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_INPUT1,static,1342177806
Control4=IDC_LIST_POINT,SysListView32,1350631425

[CLS:CCalibration]
Type=0
HeaderFile=Calibration.h
ImplementationFile=Calibration.cpp
BaseClass=CDialog
Filter=D
LastObject=CCalibration
VirtualFilter=dWC

[DLG:IDD_CNT_TRAFFIC_DIALOG (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC_INPUT,static,1342177806
Control2=IDC_BUTTON1,button,1342242816
Control3=IDC_BUTTON2,button,1342242816
Control4=IDC_BUTTON3,button,1342242816
Control5=IDC_LIST_COUNT,SysListView32,1350631425
Control6=IDC_BUTTON4,button,1342242816
Control7=IDC_BUTTON5,button,1342242816
Control8=IDC_AXISMEDIACONTROL1,{745395C8-D0E1-4227-8586-624CA9A10A8D},1073807360
Control9=IDC_STATIC_CURRENT_FRAME,static,1342308352
Control10=IDC_EDIT_EDGETHRESHOLD,edit,1350631552
Control11=IDC_STATIC_EDGE_THRESHOLD,static,1342308352
Control12=IDC_BUTTON6,button,1342242816

[DLG:IDD_CALIBRATION (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_INPUT1,static,1342177806
Control4=IDC_LIST_POINT,SysListView32,1350631425

