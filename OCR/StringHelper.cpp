#include "pch.h"
#include "StringHelper.h"

winrt::hstring StringHelper::to_hex_digit(unsigned char value)
{
    value &= 0xF;
    if (value < 10)
    {
        return winrt::hstring({static_cast<wchar_t>(L'0' + value)});
    }
    return winrt::hstring({static_cast<wchar_t>(L'A' + (value - 10))});
}

winrt::hstring StringHelper::to_hex_hstring(long long value)
{
    winrt::hstring hex;
    for (int i = 0; i < 16; i++)
    {
        const char digit = value & 0xF;
        value >>= 4;
        hex = to_hex_digit(digit) + hex;
    }
    return L"0x" + hex;
}
