// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "BlankWindow.xaml.h"
#if __has_include("BlankWindow.g.cpp")
#include "BlankWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    BlankWindow::BlankWindow()
    {
        InitializeComponent();
    }

    int32_t BlankWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void BlankWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void BlankWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
