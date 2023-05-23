// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "WindowConfigPage.g.h"

namespace winrt::OCR::implementation
{
    struct WindowConfigPage : WindowConfigPageT<WindowConfigPage>
    {
        WindowConfigPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window window{nullptr};

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    private:
        WindowConfigPage()=delete;
    };
}

namespace winrt::OCR::factory_implementation
{
    struct WindowConfigPage : WindowConfigPageT<WindowConfigPage, implementation::WindowConfigPage>
    {
    };
}
