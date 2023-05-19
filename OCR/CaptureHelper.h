#pragma once
namespace winrt::OCR
{
    class CaptureHelper
    {
    public:
        static concurrency::task<HRESULT> CaptureCurrentWindowAsync();
        static Windows::Storage::Streams::IBuffer CreateBufferFromHBitmap(HDC hdc,HBITMAP hBitmap); 
    };
}
