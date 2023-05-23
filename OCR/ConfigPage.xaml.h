// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "ConfigPage.g.h"

namespace winrt::OCR::implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage>
    {
        ConfigPage() = delete;
        ConfigPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window window{nullptr};
        int32_t MyProperty();
        void MyProperty(int32_t value);
        OCR::SharedItem SharedItem();
        // void SharedItem(OCR::SharedItem const& value);
        Windows::Foundation::IAsyncAction pyHomeButton_Click(Windows::Foundation::IInspectable const& sender,
                                                             Microsoft::UI::Xaml::RoutedEventArgs const& args);
        Windows::Foundation::IAsyncAction sitePackagesButton_Click(Windows::Foundation::IInspectable const& sender,
                                                                   Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void saveButton_Click(Windows::Foundation::IInspectable const& sender,
                              Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        OCR::SharedItem m_sharedItem{nullptr};
    };
}

namespace winrt::OCR::factory_implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage, implementation::ConfigPage>
    {
    };
}
