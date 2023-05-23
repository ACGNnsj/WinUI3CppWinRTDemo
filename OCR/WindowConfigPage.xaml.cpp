// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "WindowConfigPage.xaml.h"
#if __has_include("WindowConfigPage.g.cpp")
#include "WindowConfigPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    WindowConfigPage::WindowConfigPage()
    {
        m_sharedItem = SharedItem::Instance();
        InitializeComponent();
    }

    WindowConfigPage::WindowConfigPage(const Window& window)
    {
        m_sharedItem = SharedItem::Instance();
        this->window = window;
        InitializeComponent();
    }

    OCR::SharedItem WindowConfigPage::SharedItem()
    {
        return m_sharedItem;
    }

    void WindowConfigPage::Apply_Click(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args)
    {
    }

    void WindowConfigPage::Reset_Click(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args)
    {
    }

    void WindowConfigPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        auto window = e.Parameter().try_as<Window>();
        this->window = window;
    }
}
