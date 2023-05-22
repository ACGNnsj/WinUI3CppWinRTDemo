#pragma once

namespace winrt::OCR
{
    class TranslateHelper
    {
    public:
       static hstring Translate(const char* text, const char* from, const char* to);
    };
}
