#include "pch.h"

namespace winrt::OCR
{
    concurrency::task<long> CaptureHelper::CaptureCurrentWindowAsync()
    {
        return concurrency::create_task([]
        {
            WindowHelper::OpenMessageWindow(L"CaptureCurrentWindowAsync");
            const auto picker = Windows::Graphics::Capture::GraphicsCapturePicker();
            const auto item{
                picker.PickSingleItemAsync()
            };
            const auto canvasDevice = Microsoft::Graphics::Canvas::CanvasDevice();
            if (canvasDevice == nullptr)
            {
                WindowHelper::OpenMessageWindow(L"CanvasDevice is null");
            }
            const auto framePool = Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(
                canvasDevice, Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized, 2,
                item.get().Size());
            Windows::Graphics::Capture::Direct3D11CaptureFrame frame = nullptr;
            auto is_frame_arrived = false;
            framePool.FrameArrived([&frame, &is_frame_arrived](auto&& frame_pool, [[maybe_unused]] auto&& args)
            {
                if (is_frame_arrived)
                {
                    return;
                }
                frame = frame_pool.TryGetNextFrame();
                is_frame_arrived = true;
            });
            const auto session = framePool.CreateCaptureSession(item.get());
            session.StartCapture();
            // MSG message;
            while (!is_frame_arrived)
            {
                // if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE) > 0)
                // {
                //     DispatchMessage(&message);
                // }
                Sleep(1);
            }
            session.Close();
            const auto surface = frame.Surface();
            // auto bitmap = Microsoft::Graphics::Canvas::CanvasBitmap::CreateFromDirect3D11Surface(canvasDevice, surface);
            const Windows::Media::Ocr::OcrEngine ocrEngine =
                Windows::Media::Ocr::OcrEngine::TryCreateFromUserProfileLanguages();
            const auto bitmap = Windows::Graphics::Imaging::SoftwareBitmap::CreateCopyFromSurfaceAsync(surface);
            const auto ocrResult = ocrEngine.RecognizeAsync(bitmap.get()).get();
            const auto lines = ocrResult.Lines();
            hstring results;
            for (auto line : lines)
            {
                auto words = line.Words();
                for (auto word : words)
                {
                    auto text = word.Text();
                    auto rect = word.BoundingRect();
                    results = results + text + L" ";
                }
            }
            WindowHelper::OpenMessageWindow(results);
            framePool.Close();
            return S_OK;
        });
    }

    Windows::Storage::Streams::IBuffer CaptureHelper::CreateBufferFromHBitmap(const HDC hdc, const HBITMAP hBitmap)
    {
        CBitmap* bitmap = CBitmap::FromHandle(hBitmap);
        BITMAP bmp;
        bitmap->GetBitmap(&bmp);
        // BITMAPFILEHEADER bmfHeader;
        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bmp.bmWidth;
        bi.biHeight = -bmp.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;
        const DWORD bmpSize = (bmp.bmWidth * bi.biBitCount + 31) / 32 * 4 * bmp.bmHeight;
        const HANDLE hDIB = GlobalAlloc(GHND, bmpSize);
        const auto lpbitmap = static_cast<unsigned char*>(GlobalLock(hDIB));
        GetDIBits(hdc, hBitmap, 0, static_cast<UINT>(bmp.bmHeight), lpbitmap, reinterpret_cast<BITMAPINFO*>(&bi),
                  DIB_RGB_COLORS);
        /*auto bitmapImage = Microsoft::UI::Xaml::Media::Imaging::BitmapImage();
        auto memoryStream = Windows::Storage::Streams::InMemoryRandomAccessStream();
        bitmapImage.SetSource(memoryStream);
        auto writeableBitmap = Microsoft::UI::Xaml::Media::Imaging::WriteableBitmap(bmp.bmWidth, bmp.bmHeight);*/
        auto buffer = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(
            array_view(lpbitmap, bmpSize));
        return buffer;
    }
}
