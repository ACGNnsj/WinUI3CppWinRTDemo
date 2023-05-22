// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "ConfigPage.xaml.h"
#if __has_include("ConfigPage.g.cpp")
#include "ConfigPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    ConfigPage::ConfigPage()
    {
        InitializeComponent();
    }

    ConfigPage::ConfigPage(const Window& window)
    {
        InitializeComponent();
        this->window = window;
    }

    int32_t ConfigPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void ConfigPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction ConfigPage::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
        auto picker = Windows::Storage::Pickers::FolderPicker();
        HWND hWnd = WindowHelper::GetWindowHandle(window);
        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        auto folder = co_await picker.PickSingleFolderAsync();
        auto path = folder.Path();
        WindowHelper::OpenMessageWindow(L"path: " + path);
    }
}
