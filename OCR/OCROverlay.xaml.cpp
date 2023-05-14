// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "OCROverlay.xaml.h"
#if __has_include("OCROverlay.g.cpp")
#include "OCROverlay.g.cpp"
#endif
#include <numeric>
#include <winrt/Microsoft.UI.Windowing.h>

#include "StringHelper.h"
#include "WindowHelper.h"

// using namespace winrt;
// using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    using namespace Microsoft::UI::Xaml;
    OCROverlay::OCROverlay()
    {
        InitializeComponent();
    }

    int32_t OCROverlay::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void OCROverlay::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void OCROverlay::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
        /*auto newWindow = Window();
        MicaPage micaPage = MicaPage(newWindow);
        newWindow.ExtendsContentIntoTitleBar(true);
        newWindow.Content(micaPage);
        //micaPage.AfterInit(newWindow);
        auto appWindow = newWindow.AppWindow();
        winrt::Windows::Graphics::SizeInt32 size = { 800,600 };
        appWindow.Resize(size);
        newWindow.Activate();*/
        auto newWindow = FormalOverlay();
        newWindow.Activate();
        HWND hWnd = WindowHelper::GetWindowHandle(newWindow);
        LONG_PTR nStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
        uint32_t nStyleFirst = nStyle >> 32;
        uint32_t nStyleSecond = nStyle & 0xFFFFFFFF;
        std::array<char, 8> hexNStyleFirst = impl::uint32_to_hex<char>(nStyleFirst);
        hstring hexNStyleString = std::accumulate(hexNStyleFirst.begin(), hexNStyleFirst.end(), hstring(L"0x"),
                                                  [](const hstring& a, const char b) { return a + b; });
        std::array<char, 8> hexNStyleSecond = impl::uint32_to_hex<char>(nStyleSecond);
        hexNStyleString = hexNStyleString + std::accumulate(hexNStyleSecond.begin(), hexNStyleSecond.end(),
                                                            hstring(L""),
                                                            [](hstring a, const char& b) { return a + b; });
        WindowHelper::OpenMessageWindow(
            L"new overlay " + hexNStyleString + L" " +
            StringHelper::to_hex_hstring(GetWindowLongPtr(hWnd, GWL_EXSTYLE)));
    }
}
