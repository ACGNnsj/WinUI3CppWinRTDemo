﻿#include "pch.h"
#include "SharedItem.h"
#if __has_include("SharedItem.g.cpp")
#include "SharedItem.g.cpp"
#endif

namespace winrt::OCR::implementation
{
    OCR::SharedItem SharedItem::Instance()
    {
        static auto instance = OCR::SharedItem();
        return instance;
    }

    hstring SharedItem::PyHome()
    {
        return m_pyHome;
    }

    void SharedItem::PyHome(hstring const& value)
    {
        if (m_pyHome != value)
        {
            m_pyHome = value;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{L"PyHome"});
        }
    }

    hstring SharedItem::SitePackages()
    {
        return m_sitePackages;
    }

    void SharedItem::SitePackages(hstring const& value)
    {
        if (m_sitePackages != value)
        {
            m_sitePackages = value;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{L"SitePackages"});
        }
    }

    SharedItem::SharedItem()
    {
        const auto localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
        const auto pyHome = unbox_value_or(localSettings.Values().TryLookup(L"pyHome"), L"");
        PyHome(pyHome);
        const auto sitePackages = unbox_value_or(localSettings.Values().TryLookup(L"sitePackages"), L"");
        SitePackages(sitePackages);
    }

    event_token SharedItem::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }

    void SharedItem::PropertyChanged(event_token const& token)
    {
        m_propertyChanged.remove(token);
    }
}
