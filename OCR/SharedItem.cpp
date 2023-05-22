#include "pch.h"
#include "SharedItem.h"
#if __has_include("SharedItem.g.cpp")
#include "SharedItem.g.cpp"
#endif

namespace winrt::OCR::implementation
{
    event_token SharedItem::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }

    void SharedItem::PropertyChanged(event_token const& token)
    {
        m_propertyChanged.remove(token);
    }
}
