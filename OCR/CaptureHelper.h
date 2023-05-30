#pragma once
namespace winrt::OCR
{
    class CaptureHelper
    {
    public:
        static concurrency::task<HRESULT> CaptureCurrentWindowAsync();
        static Windows::Foundation::IAsyncAction CaptureAndRecognizeAsync(Microsoft::UI::Xaml::Window window);
        static Windows::Storage::Streams::IBuffer CreateBufferFromHBitmap(HDC hdc, HBITMAP hBitmap);
    };
}
