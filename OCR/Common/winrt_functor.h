#pragma once
namespace winrt::OCR
{
    template <typename T>
    constexpr T winrt_empty_value() noexcept
    {
        if constexpr (std::is_base_of_v<Windows::Foundation::IUnknown, T>)
        {
            return nullptr;
        }
        else
        {
            return {};
        }
    }

    template <typename T, bool hasSetter = true>
    struct winrt_property
    {
        winrt_property(T const& initial = winrt_empty_value<T>()) : value(initial)
        {
        }

        T operator()() const { return value; }
        // void operator()(T const& newValue) { value = newValue; }
        // If hasSetter is true, provide a setter
        template <bool enabled = hasSetter>
        // std::enable_if_t<enabled>
            requires enabled
        void operator()(T const& newValue)
        {
            value = newValue;
        }

        T value;
    };

    template <typename D>
    struct winrt_event : event<D>
    {
        winrt_event() = default;
        using event<D>::operator(); // prevent hiding base class operator()

        auto operator()(D const& handler)
        {
            return this->add(handler);
        }

        void operator()(event_token const& token)
        {
            this->remove(token);
        }
    };
}
