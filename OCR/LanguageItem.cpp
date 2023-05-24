#include "pch.h"
#include "LanguageItem.h"
#if __has_include("LanguageItem.g.cpp")
#include "LanguageItem.g.cpp"
#endif

namespace winrt::OCR::implementation
{
    void LanguageItem::RegisterDependencyProperty()
    {
        m_displayNameProperty =
            Microsoft::UI::Xaml::DependencyProperty::Register(
                L"DisplayName",
                winrt::xaml_typename<hstring>(),
                winrt::xaml_typename<OCR::LanguageItem>(),
                Microsoft::UI::Xaml::PropertyMetadata{nullptr}
            );
        m_languageTagProperty =
            Microsoft::UI::Xaml::DependencyProperty::Register(
                L"LanguageTag",
                winrt::xaml_typename<hstring>(),
                winrt::xaml_typename<OCR::LanguageItem>(),
                Microsoft::UI::Xaml::PropertyMetadata{nullptr}
            );
    }

    Microsoft::UI::Xaml::DependencyProperty LanguageItem::DisplayNameProperty()
    {
        return m_displayNameProperty;
    }

    hstring LanguageItem::DisplayName()
    {
        return m_displayName;
    }

    void LanguageItem::DisplayName(hstring const& value)
    {
        SetValue(m_displayNameProperty, box_value(value));
    }

    Microsoft::UI::Xaml::DependencyProperty LanguageItem::LanguageTagProperty()
    {
        return m_languageTagProperty;
    }

    hstring LanguageItem::LanguageTag()
    {
        return m_languageTag;
    }

    void LanguageItem::LanguageTag(hstring const& value)
    {
        SetValue(m_languageTagProperty, box_value(value));
    }
}
