// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <dwmapi.h>
#include <WindowHelper.h>
#include <winrt/Microsoft.UI.Interop.h> // For the WindowId struct and the GetWindowIdFromWindow function.

#include "StringHelper.h"
using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
        /*auto newAppWindow = winrt::Microsoft::UI::Windowing::AppWindow::Create();
        newAppWindow.Title(L"New App Window");
        newAppWindow.Show(true);*/
        // auto newWindow = OverlayWindow();
        auto newWindow = Window();
        OCROverlay overlay = OCROverlay();
        newWindow.ExtendsContentIntoTitleBar(true);
        newWindow.Content(overlay);
        auto appWindow = newWindow.AppWindow();
        auto presenter = appWindow.Presenter().try_as<Microsoft::UI::Windowing::OverlappedPresenter>();
        presenter.SetBorderAndTitleBar(false, false);
        /*auto brushHolder = newWindow.try_as<winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop>();
        auto compositor = Compositor();
        auto colorBrush = compositor.CreateColorBrush(Windows::UI::ColorHelper::FromArgb(0, 0, 0, 0)).try_as<
            winrt::Windows::UI::Composition::CompositionBrush>();
        brushHolder.SystemBackdrop(colorBrush);*/
        HWND hWnd = WindowHelper::GetWindowHandle(newWindow);
        // int x, y, width, height = (x = 0, y = 0, width = 100, height = 100);
        WindowHelper::SetActualWindowPos(hWnd, HWND_TOPMOST, 100, 100, 100, 100, 0);
        WindowHelper::DisableRoundedCorner(hWnd);
        LONG_PTR lExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        lExStyle |= WS_EX_LAYERED;
        SetWindowLongPtr(hWnd, GWL_EXSTYLE, lExStyle);
        BYTE bAlpha = (255 * 50) / 100;
        bool result = true;
        if (lExStyle & WS_EX_LAYERED)
        {
            result = SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), bAlpha, LWA_COLORKEY);
        }
        else
        {
            WindowHelper::OpenMessageWindow(L"No WS_EX_LAYERED");
        }
        newWindow.Activate();
        LONG_PTR nStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
        hstring hexNStyleString = StringHelper::to_hex_hstring(nStyle);
        WindowHelper::OpenMessageWindow(
            L"old overlay " + hexNStyleString + L" " + StringHelper::to_hex_hstring(
                GetWindowLongPtr(hWnd, GWL_EXSTYLE)));
        /*HWND contentWindowSiteBridge = FindWindowEx(hWnd, NULL, L"Microsoft.UI.Content.DesktopSiteBridge", NULL);
        WindowHelper::OpenMessageWindow(to_hstring((int)contentWindowSiteBridge));*/
        /*BOOL closeResult = PostMessage(contentWindowSiteBridge, WM_CLOSE, 0, 0);
        closeResult = PostMessage(contentWindowSiteBridge, WM_DESTROY, 0, 0);*/
        //closeResult = PostMessage(contentWindowSiteBridge, WM_QUIT, 0, 0);
        /*BOOL closeResult = ShowWindow(contentWindowSiteBridge, SW_HIDE);
        if (closeResult == 0)
        {
            WindowHelper::OpenMessageWindow(L"Close failed");
        }*/
        /*else {
            WindowHelper::OpenMessageWindow(L"Close success " + to_hstring(closeResult));
        }*/
        if (!result)WindowHelper::OpenMessageWindow(L"Failed");
        HWND anotherHWnd;
        this->try_as<IWindowNative>()->get_WindowHandle(&anotherHWnd);
        LONG_PTR anotherLExStyle = GetWindowLongPtr(anotherHWnd, GWL_EXSTYLE);
        anotherLExStyle |= WS_EX_LAYERED | WS_EX_NOREDIRECTIONBITMAP;
        SetWindowLongPtr(anotherHWnd, GWL_EXSTYLE, anotherLExStyle);
        BYTE anotherBAlpha = (255 * 50) / 100;
        bool anotherResult = false;
        if (anotherLExStyle & WS_EX_LAYERED)
        {
            anotherResult = SetLayeredWindowAttributes(anotherHWnd, RGB(0, 0, 0), anotherBAlpha, LWA_COLORKEY);
        }
        if (!anotherResult) WindowHelper::OpenMessageWindow(L"Another Failed");
        //anotherWindow.Activate();
        int horizontal, vertical;
        WindowHelper::GetDesktopResolution(horizontal, vertical);
        /*HINSTANCE hUser = GetModuleHandle(L"user32.dll");
        auto parentNative{ this->try_as<::IWindowNative>() };
        winrt::check_bool(parentNative);
        HWND parent{ 0 };
        parentNative->get_WindowHandle(&parent);*/
        HWND const child = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
                                          L"EDIT", L"Sample",
                                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                          CW_USEDEFAULT, CW_USEDEFAULT,
                                          nullptr, nullptr, nullptr, nullptr);
        SetLayeredWindowAttributes(child, 0, (BYTE)255, LWA_ALPHA);
        //WindowHelper::OpenMessageWindow(L"childHWND: " + to_hstring((int)child)+to_hstring(child == nullptr));
        WindowHelper::SetActualWindowPos(child, HWND_TOPMOST, 0, 0, horizontal, vertical, 0);
        WindowHelper::DisableRoundedCorner(child);
        Microsoft::UI::WindowId windowId = Microsoft::UI::GetWindowIdFromWindow(child);
        auto childAppWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
        childAppWindow.Presenter().try_as<Microsoft::UI::Windowing::OverlappedPresenter>().SetBorderAndTitleBar(
            true, false);
    }
}
