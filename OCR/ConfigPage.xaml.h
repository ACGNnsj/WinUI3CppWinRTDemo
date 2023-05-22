// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "ConfigPage.g.h"

namespace winrt::OCR::implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage>
    {
        ConfigPage();
        ConfigPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window window{nullptr};
        int32_t MyProperty();
        void MyProperty(int32_t value);

        Windows::Foundation::IAsyncAction myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage, implementation::ConfigPage>
    {
    };
}
