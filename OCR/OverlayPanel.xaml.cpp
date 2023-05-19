// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "OverlayPanel.xaml.h"
#if __has_include("OverlayPanel.g.cpp")
#include "OverlayPanel.g.cpp"
#endif
#include <windows.graphics.directx.direct3d11.interop.h>
// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    using namespace Microsoft::UI::Xaml;

    OverlayPanel::OverlayPanel()
    {
        InitializeComponent();
    }

    OverlayPanel::OverlayPanel(const Window& outerWindow)
    {
        InitializeComponent();
        AfterInitialization(outerWindow);
    }


    void OverlayPanel::AfterInitialization(const Window& outerWindow)
    {
        window = outerWindow;
        // window.ExtendsContentIntoTitleBar(true);
        hWndMain = WindowHelper::GetWindowHandle(window);
        // this->try_as<IWindowNative>()->get_WindowHandle(&hWndMain);
        _apw = window.AppWindow();
        _presenter = _apw.Presenter().try_as<Microsoft::UI::Windowing::OverlappedPresenter>();
        _presenter.SetBorderAndTitleBar(false, false);
        _presenter.IsResizable(false);
        //_apw.TitleBar().ExtendsContentIntoTitleBar();
        // auto pvAttribute = DWMWCP_DEFAULT;
        //DwmSetWindowAttribute(hWndMain, DWMWA_WINDOW_CORNER_PREFERENCE, &pvAttribute, sizeof(DWMWINDOWATTRIBUTE));
        WindowHelper::DisableRoundedCorner(hWndMain);
        auto closedRevoker = window.Closed(auto_revoke, [&](auto&&, auto&&)
        {
            m_pD2DDeviceContext = nullptr;
            m_pDXGISwapChain1 = nullptr;
            m_pDXGIDevice = nullptr;
            m_pD3D11DeviceContext = nullptr;
            m_pD3D11DevicePtr = nullptr;
            m_pD2DFactory1 = nullptr;
            //m_pD2DFactory = nullptr;
            Gdiplus::GdiplusShutdown(m_initToken);
        });
        const Gdiplus::GdiplusStartupInput input = {};
        GdiplusStartup(&m_initToken, &input, nullptr);
        Gdiplus::GpBitmap* pImage = nullptr;
        if (const Gdiplus::GpStatus status = Gdiplus::DllExports::GdipCreateBitmapFromFile(
            L"Assets\\SplashScreen.scale-200.png", &pImage); status == 0)
        {
            Gdiplus::DllExports::GdipCreateHBITMAPFromBitmap(pImage, &m_hBitmap, RGB(0, 0, 0));
            Gdiplus::DllExports::GdipDisposeImage(pImage);
        }
        HRESULT result = CreateD2D1Factory();
        if (result == S_OK)
        {
            CreateDeviceContext();
            result = CreateSwapChain(nullptr);
            if (result == S_OK)
            {
                const com_ptr<ISwapChainPanelNative> panelNative = this->try_as<ISwapChainPanelNative>();
                panelNative->SetSwapChain(m_pDXGISwapChain1.get());
            }
        }
        const LONG_PTR nStyle = GetWindowLongPtr(hWndMain, GWL_STYLE);
        //SetWindowLongPtr(hWndMain, GWL_STYLE, nStyle & ~WS_CAPTION);
        SetWindowLongPtr(hWndMain, GWL_STYLE, nStyle | WS_SIZEBOX);
        if (const LONG_PTR nExStyle = GetWindowLongPtr(hWndMain, GWL_EXSTYLE); (nExStyle & WS_EX_LAYERED) == 0)
        {
            SetWindowLongPtr(hWndMain, GWL_EXSTYLE, nExStyle | WS_EX_LAYERED);
            DWORD nAppsUseLightTheme = 0;
            DWORD nSystemUsesLightTheme = 0;
            const auto sPathKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
            HKEY rk;
            RegOpenKeyEx(HKEY_CURRENT_USER, sPathKey, 0, KEY_READ | KEY_WOW64_64KEY, &rk);
            DWORD dwBufferSize(sizeof(DWORD));
            RegQueryValueEx(rk, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&nAppsUseLightTheme),
                            &dwBufferSize);
            RegQueryValueEx(rk, L"SystemUsesLightTheme", nullptr, nullptr,
                            reinterpret_cast<LPBYTE>(&nSystemUsesLightTheme), &dwBufferSize);
            COLORREF nColorBackground = RGB(0, 0, 0);
            if (nAppsUseLightTheme == 1)
            {
                nColorBackground = RGB(255, 255, 255);
            }
            SetLayeredWindowAttributes(hWndMain, nColorBackground, 255, LWA_COLORKEY);
        }
        nXWindow = 100;
        nYWindow = 100;
        nWidth = 800;
        nHeight = 600;
        WindowHelper::SetActualWindowPos(hWndMain, HWND_TOPMOST, nXWindow, nYWindow, nWidth, nHeight, 0);
        // WindowHelper::OpenMessageWindow(
        //     to_hstring(_apw.Position().X) + L" " + to_hstring(_apw.Position().Y) + L" " + to_hstring(_apw.Size().Width)
        //     + L" " + to_hstring(_apw.Size().Height));
        const UIElement root = *this;
        [[maybe_unused]] auto pointerMovedToken = root.PointerMoved({this, &OverlayPanel::Root_PointerMoved});
        [[maybe_unused]] auto pointerPressedToken = root.PointerPressed({this, &OverlayPanel::Root_PointerPressed});
        [[maybe_unused]] auto pointerReleasedToken = root.PointerReleased({this, &OverlayPanel::Root_PointerReleased});
    }

    HRESULT OverlayPanel::CreateD2D1Factory()
    {
        constexpr D2D1_FACTORY_OPTIONS options = {D2D1_DEBUG_LEVEL_INFORMATION};
        const HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, m_pD2DFactory1.put());
        return result;
    }

    HRESULT OverlayPanel::CreateDeviceContext()
    {
        D3D_FEATURE_LEVEL featureLevel;
        D3D_FEATURE_LEVEL aD3D_FEATURE_LEVEL[] = {
            D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
        };
        HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                           D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
                                           aD3D_FEATURE_LEVEL,
                                           ARRAYSIZE(aD3D_FEATURE_LEVEL),D3D11_SDK_VERSION, m_pD3D11DevicePtr.put(),
                                           &featureLevel, m_pD3D11DeviceContext.put());
        if (result == S_OK)
        {
            m_pDXGIDevice = m_pD3D11DevicePtr.try_as<IDXGIDevice1>();
            if (m_pD2DFactory1 != nullptr)
            {
                com_ptr<ID2D1Device> pD2DDevice;
                result = m_pD2DFactory1->CreateDevice(m_pDXGIDevice.get(), pD2DDevice.put());
                if (result == S_OK)
                {
                    result = pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                                                             m_pD2DDeviceContext.put());
                    pD2DDevice = nullptr;
                }
            }
        }
        return result;
    }

    HRESULT OverlayPanel::CreateSwapChain(HWND hWnd)
    {
        const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, false, {1, 0},DXGI_USAGE_RENDER_TARGET_OUTPUT, 2,
            hWnd ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH, DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
            DXGI_ALPHA_MODE_UNSPECIFIED, 0
        };
        com_ptr<IDXGIAdapter> pDXGIAdapter;
        HRESULT result = m_pDXGIDevice->GetAdapter(pDXGIAdapter.put());
        if (result == S_OK)
        {
            com_ptr<IDXGIFactory2> pDXGIFactory2Ptr;
            result = pDXGIAdapter->GetParent(__uuidof(pDXGIFactory2Ptr), pDXGIFactory2Ptr.put_void());
            if (result == S_OK)
            {
                com_ptr pDXGIFactory2(pDXGIFactory2Ptr);
                if (hWnd)
                {
                    pDXGIFactory2->CreateSwapChainForHwnd(
                        m_pD3D11DevicePtr.get(), hWnd, &swapChainDesc, nullptr, nullptr, m_pDXGISwapChain1.put());
                }
                else
                {
                    pDXGIFactory2->CreateSwapChainForComposition(
                        m_pD3D11DevicePtr.get(), &swapChainDesc, nullptr, m_pDXGISwapChain1.put());
                }
                result = m_pDXGIDevice->SetMaximumFrameLatency(1);
                pDXGIFactory2 = nullptr;
                pDXGIFactory2Ptr = nullptr;
            }
            pDXGIAdapter = nullptr;
        }
        return result;
    }

    void OverlayPanel::Root_PointerMoved(Windows::Foundation::IInspectable const& sender,
                                         Input::PointerRoutedEventArgs const& e)
    {
        if (cursorPosition == CursorPosition::Undefined) return;
        const Microsoft::UI::Input::PointerPoint pp = e.GetCurrentPoint(
            sender.try_as<UIElement>());
        const auto properties = pp.Properties();
        if (properties.IsLeftButtonPressed())
        {
            POINT pt;
            GetCursorPos(&pt);
            if (cursorPosition == CursorPosition::Center)
            {
                _apw.Move(Windows::Graphics::PointInt32{nXWindow + (pt.x - nX), nYWindow + (pt.y - nY)});
                e.Handled(true);
            }
        }
    }

    Windows::Foundation::IAsyncAction OverlayPanel::Root_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                                                        Input::PointerRoutedEventArgs const& e)
    {
        const auto sd = sender.try_as<UIElement>();
        if (const auto properties = e.GetCurrentPoint(sd).Properties(); properties.IsLeftButtonPressed())
        {
            if (borderPressed) co_return;
            [[maybe_unused]] bool result = sd.CapturePointer(e.Pointer());
#pragma region
            /*HWND hWnd = WindowFromPoint(pt);
            Microsoft::UI::Input::PointerPoint pp = e.GetCurrentPoint(sd);
            auto elementStack = Media::VisualTreeHelper::FindElementsInHostCoordinates(
                pp.Position(), sd);
            int nCpt = 0;
            bool bOK = true;
            for (auto element : elementStack)
            {
                if (nCpt == 0)
                {
                    if (xaml_typename<Controls::Border>().Name != get_class_name(element))
                    {
                        bOK = false;
                        break;
                    }
                }
                if (nCpt == 1)
                {
                    if (xaml_typename<Controls::SwapChainPanel>().Name != get_class_name(
                        element))
                    {
                        bOK = false;
                        break;
                    }
                }
                nCpt++;
            }
            if (bOK && tsClickThrough().IsOn())
            {
                SwitchToThisWindow(hWnd, true);
            }*/
            // bMoving = true;
#pragma endregion
            POINT pt;
            GetCursorPos(&pt);
            nX = pt.x;
            nY = pt.y;
            nXWindow = _apw.Position().X;
            nYWindow = _apw.Position().Y;
            nWidth = _apw.Size().Width;
            nHeight = _apw.Size().Height;
            cursorPosition = CursorPosition::Center;
        }
        else if (properties.IsRightButtonPressed())
        {
#pragma region
            // Sleep(200);
            // Application::Current().Exit();

            /*init_apartment();
            // To get the native DirectX device that is wrapped by a Direct3DDevice.
            Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice d3dDevice /* = ... #1#;
            com_ptr dxgiInterfaceAccess{
                d3dDevice.as<::Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess>()
            };
            com_ptr<IDXGIDevice> nativeDevice;
            check_hresult(dxgiInterfaceAccess->GetInterface(
                __uuidof(nativeDevice),
                nativeDevice.put_void()));
            // To create a new Direct3DDevice object wrapping a native DirectX device.
            com_ptr<IDXGIDevice> nativeDevice2 /* = ... #1#;
            Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice d3dDevice2{nullptr};
            check_hresult(::CreateDirect3D11DeviceFromDXGIDevice(
                nativeDevice2.get(),
                reinterpret_cast<::IInspectable**>(put_abi(d3dDevice2))));*/

            // init_apartment();
            /*const auto item = Windows::Graphics::Capture::GraphicsCaptureItem::TryCreateFromWindowId({
                Microsoft::UI::GetWindowIdFromWindow(hWndMain).Value
            });*/
#pragma endregion
            // co_await CaptureHelper::CaptureCurrentWindowAsync();
            /*Windows::Storage::Pickers::FileOpenPicker picker;
            picker.ViewMode(Windows::Storage::Pickers::PickerViewMode::Thumbnail);
            picker.FileTypeFilter().Append(L".mp4");
            picker.as<IInitializeWithWindow>()->Initialize(hWndMain);
            auto filename = co_await picker.PickSingleFileAsync();*/
            /*auto picker = Windows::Graphics::Capture::GraphicsCapturePicker();
            picker.as<IInitializeWithWindow>()->Initialize(hWndMain);
            auto item{
                picker.PickSingleItemAsync()
            };*/
            mainCanvas().Children().Clear();
            const HWND hWnd = GetDesktopWindow();
            const HDC hdcSrc = GetDC(hWnd);
            const HDC hdcMem = CreateCompatibleDC(hdcSrc);
            const int outerWidth = static_cast<int>(mainBorderThickness * 3);
            const HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, nWidth - outerWidth * 2, nHeight - outerWidth * 2);
            const HGDIOBJ hOldbmp = SelectObject(hdcMem, hBitmap);
            BitBlt(hdcMem, 0, 0, nWidth - outerWidth * 2, nHeight - outerWidth * 2, hdcSrc, nXWindow + outerWidth,
                   nYWindow + outerWidth, SRCCOPY);
            CImage image;
            image.Attach(hBitmap);
            // auto* date = new wchar_t[256];
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
            [[maybe_unused]] auto r = image.Save(tempPath.c_str());

            const Windows::Media::Ocr::OcrEngine ocrEngine =
                Windows::Media::Ocr::OcrEngine::TryCreateFromUserProfileLanguages();
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
            hstring results;
            for (auto line : lines)
            {
                for (auto words = line.Words(); auto word : words)
                {
                    auto text = word.Text();
                    const auto rect = word.BoundingRect();
                    results = results + text + L" " + to_hstring(rect.X) + L" " + to_hstring(rect.Y) + L" " +
                        to_hstring(rect.Width) + L" " + to_hstring(rect.Height) + L"\n";
                    auto rectangle = Shapes::Rectangle();
                    rectangle.Width(rect.Width);
                    rectangle.Height(rect.Height);
                    rectangle.Stroke(Media::SolidColorBrush(Windows::UI::Colors::Red()));
                    rectangle.StrokeThickness(0.7);
                    mainCanvas().SetLeft(rectangle, rect.X);
                    mainCanvas().SetTop(rectangle, rect.Y);
                    mainCanvas().Children().Append(rectangle);
                    auto textBlock = Controls::TextBlock();
                    textBlock.Text(text);
                    textBlock.FontSize(rect.Height * 0.8);
                    textBlock.Width(rect.Width);
                    textBlock.Height(rect.Height);
                    mainCanvas().SetLeft(textBlock, rect.X);
                    mainCanvas().SetTop(textBlock, rect.Y);
                    mainCanvas().Children().Append(textBlock);
                }
            }
            WindowHelper::OpenMessageWindow(results);
        }
    }

    void OverlayPanel::Root_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                            Input::PointerRoutedEventArgs const&)

    {
        if (!borderPressed)
        {
            sender.try_as<UIElement>().ReleasePointerCaptures();
        }
        // bMoving = false;
        cursorPosition = CursorPosition::Undefined;
        borderPressed = false;
        // WindowHelper::OpenMessageWindow(L"border released");
        nXWindow = _apw.Position().X;
        nYWindow = _apw.Position().Y;
    }

    void OverlayPanel::border_PointerMoved(Windows::Foundation::IInspectable const& sender,
                                           RoutedEventArgs const&)
    {
        const auto totalBorderThickness = mainBorderThickness * 3;
        const auto sd = sender.try_as<UIElement>();
        POINT p;
        GetCursorPos(&p);
        if (const auto x = p.x, y = p.y; x <= nXWindow + totalBorderThickness && y <= nYWindow + totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNorthwestSoutheastCursor);
            if (borderPressed)
            {
                _apw.MoveAndResize({
                    nXWindow + (p.x - nX), nYWindow + (p.y - nY), nWidth - (p.x - nX), nHeight -
                    (p.y - nY)
                });
            }
        }
        else if (x >= nXWindow + nWidth - totalBorderThickness && y >= nYWindow + nHeight -
            totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNorthwestSoutheastCursor);
            if (borderPressed)
            {
                _apw.Resize({nWidth + (p.x - nX), nHeight + (p.y - nY)});
            }
        }
        else if (x >= nXWindow + nWidth - totalBorderThickness && y <= nYWindow + totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNortheastSouthwestCursor);
            if (borderPressed)
            {
                _apw.MoveAndResize({
                    nXWindow, nYWindow + (p.y - nY), nWidth + (p.x - nX), nHeight - (p.y - nY)
                });
            }
        }
        else if (x <= nXWindow + totalBorderThickness && y >= nYWindow + nHeight - totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNortheastSouthwestCursor);
            if (borderPressed)
            {
                _apw.MoveAndResize({
                    nXWindow + (p.x - nX), nYWindow, nWidth - (p.x - nX), nHeight + (p.y - nY)
                });
            }
        }
        else if (x <= nXWindow + totalBorderThickness)
        {
            sd.ProtectedCursor(SizeWestEastCursor);
            if (borderPressed)
            {
                _apw.MoveAndResize({nXWindow + (p.x - nX), nYWindow, nWidth - (p.x - nX), nHeight});
            }
        }
        else if (x >= nXWindow + nWidth - totalBorderThickness)
        {
            sd.ProtectedCursor(SizeWestEastCursor);
            if (borderPressed)
            {
                _apw.Resize({nWidth + (p.x - nX), nHeight});
            }
        }
        else if (y <= nYWindow + totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNorthSouthCursor);
            if (borderPressed)
            {
                _apw.MoveAndResize({nXWindow, nYWindow + (p.y - nY), nWidth, nHeight - (p.y - nY)});
            }
        }
        else if (y >= nYWindow + nHeight - totalBorderThickness)
        {
            sd.ProtectedCursor(SizeNorthSouthCursor);
            if (borderPressed)
            {
                _apw.Resize({nWidth, nHeight + (p.y - nY)});
            }
        }
    }

    void OverlayPanel::panel_Loaded(Windows::Foundation::IInspectable const&,
                                    RoutedEventArgs const&)
    {
        // sp1().ProtectedCursor(
        //     Microsoft::UI::Input::InputSystemCursor::Create(Microsoft::UI::Input::InputSystemCursorShape::Arrow));
    }

    void OverlayPanel::border_Loaded(Windows::Foundation::IInspectable const& sender,
                                     RoutedEventArgs const&)
    {
        sender.try_as<UIElement>().ProtectedCursor(SizeAllCursor);
    }

    void OverlayPanel::border_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                             Input::PointerRoutedEventArgs const& e)
    {
        borderPressed = true;
        // WindowHelper::OpenMessageWindow(L"border pressed");
        POINT pt;
        GetCursorPos(&pt);
        nX = pt.x;
        nY = pt.y;
        nXWindow = _apw.Position().X;
        nYWindow = _apw.Position().Y;
        nWidth = _apw.Size().Width;
        nHeight = _apw.Size().Height;
        [[maybe_unused]] auto result = sender.try_as<UIElement>().CapturePointer(e.Pointer());
    }

    void OverlayPanel::border_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                              Input::PointerRoutedEventArgs const&)
    {
        sender.try_as<UIElement>().ReleasePointerCaptures();
        // borderPressed = false;
        // cursorPosition = CursorPosition::Undefined;
        nWidth = _apw.Size().Width;
        nHeight = _apw.Size().Height;
    }

    int32_t OverlayPanel::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void OverlayPanel::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    double OverlayPanel::MainBorderThickness()
    {
        return mainBorderThickness;
    }

    void OverlayPanel::MainBorderThickness(double value)
    {
        mainBorderThickness = value;
    }

    double OverlayPanel::MainBorderMargin()
    {
        return mainBorderMargin;
    }

    void OverlayPanel::MainBorderMargin(double value)
    {
        mainBorderMargin = value;
    }
}
