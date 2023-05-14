#include "pch.h"
#include "DoubleToThicknessConverter.h"
#include "DoubleToThicknessConverter.g.cpp"

namespace winrt::OCR::implementation
{
    Windows::Foundation::IInspectable DoubleToThicknessConverter::Convert(
        Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType,
        Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        if (const auto thickness{value.try_as<double>()})
        {
            return box_value(Microsoft::UI::Xaml::Thickness{*thickness, *thickness, *thickness, *thickness});
        }
        return box_value(Microsoft::UI::Xaml::Thickness{});
    }

    Windows::Foundation::IInspectable DoubleToThicknessConverter::ConvertBack(
        Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const& targetType,
        Windows::Foundation::IInspectable const& parameter, hstring const& language)
    {
        throw hresult_not_implemented();
    }
}
