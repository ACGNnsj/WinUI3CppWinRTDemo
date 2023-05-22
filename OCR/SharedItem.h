#pragma once
#include "SharedItem.g.h"

namespace winrt::OCR::implementation
{
    struct SharedItem : SharedItemT<SharedItem>
    {
        SharedItem() = default;
        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token);

    private:
        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}


namespace winrt::OCR::factory_implementation
{
    struct SharedItem : SharedItemT<SharedItem, implementation::SharedItem>
    {
    };
}
