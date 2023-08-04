// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "NavigationPage.xaml.h"
#if __has_include("NavigationPage.g.cpp")
#include "NavigationPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    NavigationPage::NavigationPage()
    {
        InitializeComponent();
    }

    NavigationPage::NavigationPage(const Window& window)
    {
        this->window = window;
        InitializeComponent();
    }


    void NavigationPage::ContentFrame_NavigationFailed(Windows::Foundation::IInspectable const&,
                                                       Navigation::NavigationFailedEventArgs const&)
    {
    }

    void NavigationPage::NavView_Loaded(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        [[maybe_unused]] auto hr = ContentFrame().Navigate(xaml_typename<OCR::ConfigPage>(), window);
    }

    void NavigationPage::NavView_ItemInvoked(Windows::Foundation::IInspectable const&,
                                             Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.IsSettingsInvoked())
        {
            // Navigate to Settings.
        }
        else if (args.InvokedItemContainer())
        {
            Windows::UI::Xaml::Interop::TypeName pageTypeName;
            pageTypeName.Name = unbox_value<hstring>(args.InvokedItemContainer().Tag());
            pageTypeName.Kind = Windows::UI::Xaml::Interop::TypeKind::Metadata;
            [[maybe_unused]] auto hr = ContentFrame().Navigate(pageTypeName, window);
        }
    }

    void NavigationPage::NavView_BackRequested(Controls::NavigationView const&,
                                               Controls::NavigationViewBackRequestedEventArgs const&)
    {
    }
}
