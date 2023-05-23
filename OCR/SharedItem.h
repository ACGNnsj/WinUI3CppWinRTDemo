#pragma once
#include "SharedItem.g.h"

namespace winrt::OCR::implementation
{
    struct SharedItem : SharedItemT<SharedItem>
    {
        SharedItem();
        static OCR::SharedItem Instance();
        hstring PyHome();
        void PyHome(hstring const& value);
        hstring SitePackages();
        void SitePackages(hstring const& value);
        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token);

    private:
        hstring m_pyHome;
        hstring m_sitePackages;
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}


namespace winrt::OCR::factory_implementation
{
    struct SharedItem : SharedItemT<SharedItem, implementation::SharedItem>
    {
    };
}
