#pragma once

class StringHelper
{
    static winrt::hstring to_hex_digit(unsigned char value);

public:
    static winrt::hstring to_hex_hstring(__int64 value);
};
