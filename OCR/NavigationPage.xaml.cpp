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

    int32_t NavigationPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void NavigationPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void NavigationPage::ContentFrame_NavigationFailed(Windows::Foundation::IInspectable const&,
                                                       Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const&
                                                       args)
    {
    }

    void NavigationPage::NavView_Loaded(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
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
            ContentFrame().Navigate(pageTypeName, box_value(window));
        }
    }

    void NavigationPage::NavView_BackRequested(Controls::NavigationView const&,
                                               Controls::NavigationViewBackRequestedEventArgs const&)
    {
    }
}
