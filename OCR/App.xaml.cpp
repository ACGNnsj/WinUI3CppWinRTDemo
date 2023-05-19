// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"
// #include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace OCR;
using namespace OCR::implementation;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const&)
{
    // window = make<MainWindow>();
    window = Window();
    window.Content(OverlayPanel(window));
    window.Activate();
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.dwInfoFlags = NIIF_LARGE_ICON;
    nid.guidItem = {0x1, 0x2, 0x3, {0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb}};
    nid.hWnd = WindowHelper::GetWindowHandle(window);
    StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Test application");
    nid.uFlags = NIF_GUID | NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_SHOWTIP;
    nid.uVersion = NOTIFYICON_VERSION_4;
 
    HICON hIcon;
 
    const hstring currentDirectory = StringHelper::GetCurrentDirectory() + L"Assets\\Square44x44Logo.scale-200.png";
    Gdiplus::Bitmap bitmap(currentDirectory.c_str());
    WindowHelper::OpenMessageWindow(L"current path: " + currentDirectory);
  
    bitmap.GetHICON(&hIcon);
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_ADD, &nid);
    Shell_NotifyIcon(NIM_SETVERSION, &nid);
}
