#pragma once
#include "LanguageItem.g.h"

namespace winrt::OCR::implementation
{
    struct LanguageItem : LanguageItemT<LanguageItem>
    {
        static void RegisterDependencyProperty();
        LanguageItem() = default;
        static Microsoft::UI::Xaml::DependencyProperty DisplayNameProperty();
        hstring DisplayName();
        void DisplayName(hstring const& value);
        static Microsoft::UI::Xaml::DependencyProperty LanguageTagProperty();
        hstring LanguageTag();
        void LanguageTag(hstring const& value);

    private:
        inline static Microsoft::UI::Xaml::DependencyProperty m_displayNameProperty = nullptr;
        inline static Microsoft::UI::Xaml::DependencyProperty m_languageTagProperty = nullptr;
        hstring m_displayName;
        hstring m_languageTag;
    };
}

namespace winrt::OCR::factory_implementation
{
    struct LanguageItem : LanguageItemT<LanguageItem, implementation::LanguageItem>
    {
    };
}
