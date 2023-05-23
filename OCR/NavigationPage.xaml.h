// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "NavigationPage.g.h"

namespace winrt::OCR::implementation
{
    struct NavigationPage : NavigationPageT<NavigationPage>
    {
        NavigationPage();
        NavigationPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window window{nullptr};
        int32_t MyProperty();
        void MyProperty(int32_t value);
        void ContentFrame_NavigationFailed(
            Windows::Foundation::IInspectable const& /* sender */,
            Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const& args);
        void NavView_Loaded(
            Windows::Foundation::IInspectable const& /* sender */,
            Microsoft::UI::Xaml::RoutedEventArgs const& /* args */);
        void NavView_ItemInvoked(
            Windows::Foundation::IInspectable const& /* sender */,
            Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);
        void NavView_BackRequested(
            Microsoft::UI::Xaml::Controls::NavigationView const& /* sender */,
            Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const& /* args */);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct NavigationPage : NavigationPageT<NavigationPage, implementation::NavigationPage>
    {
    };
}
