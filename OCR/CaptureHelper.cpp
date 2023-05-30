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

    Windows::Foundation::IAsyncAction CaptureHelper::CaptureAndRecognizeAsync(Microsoft::UI::Xaml::Window window)
    {
        auto panel = window.Content().try_as<OCR::OverlayPanel>();
        Windows::Media::Ocr::OcrEngine ocrEngine{nullptr};
        if (WindowManager::sourceLanguageTag.empty() || !Windows::Globalization::Language::IsWellFormed(
            WindowManager::sourceLanguageTag))
        {
            ocrEngine =
                Windows::Media::Ocr::OcrEngine::TryCreateFromUserProfileLanguages();
        }
        else
        {
            ocrEngine = Windows::Media::Ocr::OcrEngine::TryCreateFromLanguage(
                Windows::Globalization::Language(WindowManager::sourceLanguageTag));
        }
        const HWND hWnd = GetDesktopWindow();
        const HDC hdcSrc = GetDC(hWnd);
        const HDC hdcMem = CreateCompatibleDC(hdcSrc);
        const int outerWidth = static_cast<int>(panel.MainBorderThickness() * 3);
        const HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, panel.NWidth() - outerWidth * 2,
                                                       panel.NHeight() - outerWidth * 2);
        const HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);
        BitBlt(hdcMem, 0, 0, panel.NWidth() - outerWidth * 2, panel.NHeight() - outerWidth * 2, hdcSrc,
               panel.NXWindow() + outerWidth,
               panel.NYWindow() + outerWidth, SRCCOPY);
        CImage image;
        image.Attach(hBitmap);
        /*// auto* date = new wchar_t[256];
        wchar_t date[256];
        // const auto time = new wchar_t[80];
        wchar_t time[80];
        SYSTEMTIME st;
        // [[maybe_unused]] auto ret = setlocale(LC_ALL, "zh-CN");
        GetLocalTime(&st);
        GetDateFormatEx(L"zh-CN", DATE_LONGDATE, &st, nullptr, date, _countof(date), nullptr);
        GetTimeFormatEx(L"zh-CN", TIME_FORCE24HOURFORMAT, &st, L"HH'-'mm'-'ss", time, _countof(time));
        const auto tempPath = Windows::Storage::ApplicationData::Current().TemporaryFolder().Path() + L"\\" + date +
            time + L".bmp";
        WindowHelper::OpenMessageWindow(L"test " + to_hstring(image.GetWidth()) + L"\n" + tempPath);
        [[maybe_unused]] auto r = image.Save(tempPath.c_str());*/

        const auto buffer = CaptureHelper::CreateBufferFromHBitmap(hdcSrc, hBitmap);
        SelectObject(hdcMem, hOldbmp);
        DeleteObject(hdcMem);
        ReleaseDC(hWnd, hdcSrc);
        const auto bitmap = Windows::Graphics::Imaging::SoftwareBitmap::CreateCopyFromBuffer(
            buffer, Windows::Graphics::Imaging::BitmapPixelFormat::Bgra8, image.GetWidth(),
            image.GetHeight(), Windows::Graphics::Imaging::BitmapAlphaMode::Premultiplied);
        /*auto img = Controls::Image();
        const auto softwareBitmapSource = Media::Imaging::SoftwareBitmapSource();
        co_await softwareBitmapSource.SetBitmapAsync(bitmap);
        img.Source(softwareBitmapSource);
        sp1().Children().Append(img);*/
        const auto ocrResult = co_await ocrEngine.RecognizeAsync(bitmap);
        const auto lines = ocrResult.Lines();
        // hstring results;
        hstring texts;
        for (auto line : lines)
        {
            auto xStart = WindowManager::monitorWidth;
            auto yStart = WindowManager::monitorHeight;
            auto xEnd = 0.0;
            auto yEnd = 0.0;
            for (auto words = line.Words(); auto word : words)
            {
                auto text = word.Text();
                texts = texts + text;
                const auto rect = word.BoundingRect();
                // results = results + text + L" " + to_hstring(rect.X) + L" " + to_hstring(rect.Y) + L" " +
                //     to_hstring(rect.Width) + L" " + to_hstring(rect.Height) + L"\n";
                xStart = min(xStart, rect.X);
                yStart = min(yStart, rect.Y);
                xEnd = max(xEnd, rect.X + rect.Width);
                yEnd = max(yEnd, rect.Y + rect.Height);
                // auto rectangle = Shapes::Rectangle();
                // rectangle.Width(rect.Width);
                // rectangle.Height(rect.Height);
                // rectangle.Stroke(Media::SolidColorBrush(Windows::UI::Colors::Red()));
                // rectangle.StrokeThickness(0.7);
                // mainCanvas().SetLeft(rectangle, rect.X);
                // mainCanvas().SetTop(rectangle, rect.Y);
                // mainCanvas().Children().Append(rectangle);
                auto textBlock = Microsoft::UI::Xaml::Controls::TextBlock();
                textBlock.Text(text);
                textBlock.FontSize(rect.Height);
                textBlock.Width(rect.Width);
                textBlock.Height(rect.Height * 1.25);
                textBlock.ProtectedCursor(ArrowCursor);
                panel.MainCanvas().SetLeft(textBlock, rect.X);
                panel.MainCanvas().SetTop(textBlock, rect.Y);
                panel.MainCanvas().Children().Append(textBlock);
            }
            auto rectangle = Microsoft::UI::Xaml::Shapes::Rectangle();
            rectangle.Width(xEnd - xStart);
            rectangle.Height((yEnd - yStart) * 1.25);
            rectangle.Stroke(Microsoft::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Red()));
            rectangle.StrokeThickness(0.7);
            rectangle.ProtectedCursor(ArrowCursor);
            panel.MainCanvas().SetLeft(rectangle, xStart);
            panel.MainCanvas().SetTop(rectangle, yStart);
            panel.MainCanvas().Children().Append(rectangle);
        }
        WindowManager::rawText = texts;
        // WindowHelper::OpenMessageWindow(results);
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
