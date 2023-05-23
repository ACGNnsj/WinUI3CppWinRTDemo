// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "WindowConfigPage.g.h"

namespace winrt::OCR::implementation
{
    struct WindowConfigPage : WindowConfigPageT<WindowConfigPage>
    {
        WindowConfigPage();
        WindowConfigPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window window{nullptr};
        OCR::SharedItem SharedItem();
        void Apply_Click(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        void Reset_Click(const IInspectable& sender, const Microsoft::UI::Xaml::RoutedEventArgs& args);
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
     private:
        OCR::SharedItem m_sharedItem{nullptr};
    };
}

namespace winrt::OCR::factory_implementation
{
    struct WindowConfigPage : WindowConfigPageT<WindowConfigPage, implementation::WindowConfigPage>
    {
    };
}
