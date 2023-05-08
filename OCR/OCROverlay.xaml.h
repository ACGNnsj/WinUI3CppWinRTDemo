// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "OCROverlay.g.h"

namespace winrt::OCR::implementation
{
    struct OCROverlay : OCROverlayT<OCROverlay>
    {
        OCROverlay();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct OCROverlay : OCROverlayT<OCROverlay, implementation::OCROverlay>
    {
    };
}
