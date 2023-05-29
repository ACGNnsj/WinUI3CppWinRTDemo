#pragma once

namespace winrt::OCR
{
    class TranslateHelper
    {
    public:
        static hstring Translate(const char* text, const char* from, const char* to);
        static Windows::Foundation::IAsyncAction TranslateAsync(hstring text, hstring from, hstring to);
        static void TranslateInNewThread();
        static void CreateThread();
    };
}
