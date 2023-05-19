#pragma once
namespace winrt::OCR
{
    class StringHelper
    {
        static hstring to_hex_digit(unsigned char value);

    public:
        static hstring to_hex_hstring(__int64 value);
        static hstring GetCurrentDirectory();
    };
}
