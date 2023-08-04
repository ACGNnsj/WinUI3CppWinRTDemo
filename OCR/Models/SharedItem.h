#pragma once
#include "SharedItem.g.h"

namespace winrt::OCR::implementation
{
    struct SharedItem : SharedItemT<SharedItem>
    {
        SharedItem();
        static OCR::SharedItem& Instance();
        hstring PyHome();
        void PyHome(hstring const& value);
        hstring SitePackages();
        double Width();
        void Width(double value);
        double Height();
        void Height(double value);
        double X();
        void X(double value);
        double Y();
        void Y(double value);
        void SitePackages(hstring const& value);
        // event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        // void PropertyChanged(event_token const& token);
        winrt_event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> PropertyChanged;
        
    private:
        hstring m_pyHome;
        hstring m_sitePackages;
        double m_width;
        double m_height;
        double m_x;
        double m_y;
        // event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}


namespace winrt::OCR::factory_implementation
{
    struct SharedItem : SharedItemT<SharedItem, implementation::SharedItem>
    {
    };
}
