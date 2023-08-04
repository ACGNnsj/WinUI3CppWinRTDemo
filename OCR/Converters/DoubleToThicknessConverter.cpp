#include "pch.h"
#include "DoubleToThicknessConverter.h"
#if __has_include("DoubleToThicknessConverter.g.cpp")
#include "DoubleToThicknessConverter.g.cpp"
#endif

namespace winrt::OCR::implementation
{
    Windows::Foundation::IInspectable DoubleToThicknessConverter::Convert(
        Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&,
        Windows::Foundation::IInspectable const&, hstring const&)
    {
        if (const auto thickness{value.try_as<double>()})
        {
            return box_value(Microsoft::UI::Xaml::Thickness{*thickness, *thickness, *thickness, *thickness});
        }
        return box_value(Microsoft::UI::Xaml::Thickness{});
    }

    Windows::Foundation::IInspectable DoubleToThicknessConverter::ConvertBack(
        Windows::Foundation::IInspectable const&, Windows::UI::Xaml::Interop::TypeName const&,
        Windows::Foundation::IInspectable const&, hstring const&)
    {
        throw hresult_not_implemented();
    }
}
