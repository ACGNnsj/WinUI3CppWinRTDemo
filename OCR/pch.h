// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once
#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/cppwinrt_helpers.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <d3d11_4.h>
#include <d2d1_3.h>
#include <inspectable.h>
#include <pplawait.h>
#include <shobjidl_core.h>
#include <strsafe.h>

#include <winrt/Windows.Globalization.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Ocr.h>
#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Popups.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <microsoft.ui.xaml.media.dxinterop.h>
#include <microsoft.ui.xaml.window.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#define _AFXDLL
// #include <WinSock2.h>
#include <afxwin.h>
#include <atlimage.h>

#include "WindowHelper.h"
#include "StringHelper.h"
#include "CaptureHelper.h"
#include "TranslateHelper.h"
#include "WindowManager.h"
#include "SharedItem.h"
#include "completion_source.h"
// #include "winrt/OCR.h"
#define  WM_SHOWCONFIG (WM_USER + 1)
#define  WM_CLEAN (WM_USER + 2)
