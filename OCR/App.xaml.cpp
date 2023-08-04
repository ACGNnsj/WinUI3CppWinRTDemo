// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"

// #include "MainWindow.xaml.h"
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
    window = ::Window();
    // auto overlayPanel = OverlayPanel(window);
    auto overlayPanel = make<OverlayPanel>(window);
    window.Content(overlayPanel);
    window.Activate();

    const HWND hWnd = WindowHelper::GetWindowHandle(window);

    WindowManager::mainWindowHandle = hWnd;
    // WindowManager::overlayPanel = overlayPanel;
    const auto windowPtr = (::Window*)&WindowManager::mainWindow;
    *windowPtr = window;

    WindowHelper::GetDesktopResolution(WindowManager::monitorWidth, WindowManager::monitorHeight);
    SetWindowSubclass(hWnd, SubclassProc, 0, 0);

    RegisterHotKey(hWnd, 1, WindowManager::modifierMask, WindowManager::hotkey);

    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.dwInfoFlags = NIIF_LARGE_ICON;
    // {06936F19-FC12-4DA1-96ED-FADB06E31FA3}
    nid.guidItem = {0x6936f19, 0xfc12, 0x4da1, {0x96, 0xed, 0xfa, 0xdb, 0x6, 0xe3, 0x1f, 0xa3}};
    nid.hWnd = hWnd;
    [[maybe_unused]] HRESULT hr = StringCchCopy(nid.szTip, ARRAYSIZE(nid.szTip), L"Test application");
    nid.uCallbackMessage = WM_SHOWCONFIG;
    nid.uFlags = NIF_GUID | NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_SHOWTIP;
    nid.uVersion = NOTIFYICON_VERSION_4;
    HICON hIcon;

    const hstring currentDirectory = StringHelper::GetCurrentDirectory() + L"Assets\\Square44x44Logo.scale-200.png";
    Gdiplus::Bitmap bitmap(currentDirectory.c_str());
    WindowHelper::OpenMessageWindow(L"current path: " + currentDirectory, L"directory", window);

    bitmap.GetHICON(&hIcon);
    nid.hIcon = hIcon;
    Shell_NotifyIcon(NIM_ADD, &nid);
    Shell_NotifyIcon(NIM_SETVERSION, &nid);
    // WindowHelper::OpenMessageWindow(TranslateHelper::Translate("おはようございます", "ja", "zh"));
    // WindowHelper::OpenMessageWindow(
    //     winrt::xaml_typename<OCR::ConfigPage>().Name + L" " + to_hstring(
    //         static_cast<int32_t>(winrt::xaml_typename<OCR::ConfigPage>().Kind)));
    wil::SetResultLoggingCallback([](wil::FailureInfo const& failure) noexcept
    {
        constexpr std::size_t sizeOfLogMessageWithNul = 2048;

        wchar_t logMessage[sizeOfLogMessageWithNul];
        if (SUCCEEDED(wil::GetFailureLogString(logMessage, sizeOfLogMessageWithNul, failure)))
        {
            std::fputws(logMessage, stderr);
        }
    });
    LoggingManager::Initiate();
    auto& item = SharedItem::Instance();
    auto addr = &item;
    decltype(auto) another_item = SharedItem::Instance();
    auto another_addr = &another_item;
    if (addr == another_addr)
    {
        WindowHelper::OpenMessageWindow(to_hstring(reinterpret_cast<std::uintptr_t>(addr)), L"Same instance");
    }
    else
    {
        WindowHelper::OpenMessageWindow(
            to_hstring(reinterpret_cast<std::uintptr_t>(addr)) + L" " + to_hstring(
                reinterpret_cast<std::uintptr_t>(another_addr)), L"Different instance");
    }
}

Window App::Window()
{
    return window;
}

LRESULT implementation::SubclassProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam,
                                     UINT_PTR, DWORD_PTR)
{
    switch (uMsg)
    {
    case WM_SHOWCONFIG:
        {
            switch (lParam)
            {
            case WM_RBUTTONUP:
                {
                    // WindowHelper::OpenMessageWindow(L"WM_SHOWCONFIG");
                    POINT pt;
                    GetCursorPos(&pt);
                    const HMENU hMenu = CreatePopupMenu();
                    AppendMenu(hMenu, MF_STRING, 1, L"Configure");
                    AppendMenu(hMenu, MF_STRING, 2, L"Exit");
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, nullptr);
                    DestroyMenu(hMenu);
                    break;
                }
            default: ;
            }
            break;
        }
    case WM_COMMAND:
        {
            switch (wParam)
            {
            case 1:
                {
                    // WindowHelper::OpenMessageWindow(L"Configure");
                    const auto configWindow = Window();
                    // configWindow.Content(ConfigPage(configWindow));
                    configWindow.Content(NavigationPage(configWindow));
                    // configWindow.Content(WindowConfigPage(configWindow));
                    configWindow.Activate();
                    break;
                }
            case 2:
                {
                    // WindowHelper::OpenMessageWindow(L"Exit");
                    /*NOTIFYICONDATA nid = {};
                    nid.cbSize = sizeof(NOTIFYICONDATA);
                    nid.guidItem = {0x1, 0x2, 0x3, {0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb}};
                    nid.hWnd = hWnd;
                    Shell_NotifyIcon(NIM_DELETE, &nid);*/
                    // PostQuitMessage(0);
                    // PostMessage(hWnd, WM_CLOSE, 0, 0);
                    // PostMessage(hWnd, WM_QUIT, 0, 0);
                    // Application::Current().Exit();
                    // ExitProcess(0);
                    // exit(0);
                    // TerminateProcess(GetCurrentProcess(), 0);
                    if (WindowManager::threadCreated)
                    {
                        WindowManager::keepAlive = false;
                    }
                    else
                    {
                        ExitProcess(0);
                    }
                    break;
                }
            default: ;
            }
            break;
        }
    /*case WM_CLOSE:
        {
            // Py_Finalize();
            // WindowHelper::OpenMessageWindow(L"WM_CLOSE");
            PostMessage(hWnd, WM_QUIT, 0, 0);
            break;
        }*/
    case WM_HOTKEY:
        {
            switch (wParam)
            {
            case 1:
                {
                    if (static auto count = 0; count == 0)
                    {
                        WindowHelper::OpenMessageWindow(L"Initializing Python Interpreter and Modules", L"Processing");
                        ++count;
                        TranslateHelper::CreateThread();
                    }
                    // TranslateHelper::TranslateAsync(WindowManager::rawText, WindowManager::sourceLanguageTag,
                    //                                    WindowManager::targetLanguageTag);
                    if (!WindowManager::isThreadRunning)
                    {
                        WindowManager::hasTaskToRun = true;
                        // TranslateHelper::TranslateInNewThread();
                    }
                    else
                    {
                        WindowHelper::OpenMessageWindow(L"Thread is running", L"Error");
                    }
                    /*else
                    {
                        const auto translatedText
                            = TranslateHelper::Translate(to_string(WindowManager::rawText).c_str(),
                                                         to_string(WindowManager::sourceLanguageTag).substr(0, 2).
                                                         c_str(),
                                                         to_string(WindowManager::targetLanguageTag).substr(0, 2).
                                                         c_str());
                        WindowHelper::OpenMessageWindow(WindowManager::rawText + L"\n\n" + translatedText, L"Result");
                    }
                    ++count;*/
                    break;
                }
            default: /* WindowHelper::OpenMessageWindow(hstring(L"wParam=") + wParam + L" lParam=" + lParam)*/;
            }
            break;
        }
    case WM_CLEAN:
        {
            break;
        }

    default: ;
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
