#pragma once
#include "DoubleToThicknessConverter.g.h"

namespace winrt::OCR::implementation
{
    struct DoubleToThicknessConverter : DoubleToThicknessConverterT<DoubleToThicknessConverter>
    {
        DoubleToThicknessConverter() = default;
        Windows::Foundation::IInspectable Convert(Windows::Foundation::IInspectable const& value,
                                                  Windows::UI::Xaml::Interop::TypeName const& targetType,
                                                  Windows::Foundation::IInspectable const& parameter,
                                                  hstring const& language);
        Windows::Foundation::IInspectable ConvertBack(Windows::Foundation::IInspectable const& value,
                                                      Windows::UI::Xaml::Interop::TypeName const& targetType,
                                                      Windows::Foundation::IInspectable const& parameter,
                                                      hstring const& language);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct DoubleToThicknessConverter : DoubleToThicknessConverterT<
            DoubleToThicknessConverter, implementation::DoubleToThicknessConverter>
    {
    };
}
