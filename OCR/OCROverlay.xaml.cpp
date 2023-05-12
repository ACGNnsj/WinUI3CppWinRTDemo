// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "OCROverlay.xaml.h"
#if __has_include("OCROverlay.g.cpp")
#include "OCROverlay.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
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
        auto newWindow = OverlayWindow();
        newWindow.Activate();
    }
}
