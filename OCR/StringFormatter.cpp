#include "pch.h"
#include "StringFormatter.h"
#if __has_include("StringFormatter.g.cpp")
#include "StringFormatter.g.cpp"
#endif

namespace winrt::OCR::implementation
{
    Windows::Foundation::IInspectable StringFormatter::Convert(
        Windows::Foundation::IInspectable const& value, Windows::UI::Xaml::Interop::TypeName const&,
        Windows::Foundation::IInspectable const& parameter, hstring const&)
    {
        if (const auto rawString{value.try_as<hstring>()}, fmtString{parameter.try_as<hstring>()}; rawString &&
            fmtString)
        {
            const auto result = std::vformat(fmtString->c_str(), std::make_wformat_args(rawString->c_str()));
            return box_value(result);
        }
        return box_value(L"");
    }

    Windows::Foundation::IInspectable StringFormatter::ConvertBack(
        Windows::Foundation::IInspectable const&, Windows::UI::Xaml::Interop::TypeName const&,
        Windows::Foundation::IInspectable const&, hstring const&)
    {
        throw hresult_not_implemented();
    }
}
