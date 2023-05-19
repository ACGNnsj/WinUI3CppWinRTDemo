// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "FormalOverlay.xaml.h"
#if __has_include("FormalOverlay.g.cpp")
#include "FormalOverlay.g.cpp"
#endif
// using namespace winrt;
// using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    using namespace Microsoft::UI::Xaml;

    FormalOverlay::FormalOverlay()
    {
        InitializeComponent();
        this->ExtendsContentIntoTitleBar(true);
        this->try_as<IWindowNative>()->get_WindowHandle(&hWndMain);
        _apw = this->AppWindow();
        _presenter = _apw.Presenter().try_as<Microsoft::UI::Windowing::OverlappedPresenter>();
        _presenter.SetBorderAndTitleBar(false, false);
        _presenter.IsResizable(false);
        //_apw.TitleBar().ExtendsContentIntoTitleBar();
        // auto pvAttribute = DWMWCP_DEFAULT;
        //DwmSetWindowAttribute(hWndMain, DWMWA_WINDOW_CORNER_PREFERENCE, &pvAttribute, sizeof(DWMWINDOWATTRIBUTE));
        WindowHelper::DisableRoundedCorner(hWndMain);
        auto closedRevoker = this->Closed(auto_revoke, [&](auto&&, auto&&)
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
        Gdiplus::GdiplusStartupInput input = {};
        GdiplusStartup(&m_initToken, &input, nullptr);
        Gdiplus::GpBitmap* pImage = nullptr;
        Gdiplus::GpStatus status = Gdiplus::DllExports::GdipCreateBitmapFromFile(
            L"Assets\\SplashScreen.scale-200.png", &pImage);
        if (status == 0)
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
                com_ptr<ISwapChainPanelNative> panelNative = swapChainPanel1().try_as<ISwapChainPanelNative>();
                panelNative->SetSwapChain(m_pDXGISwapChain1.get());
            }
        }
        LONG_PTR nStyle = GetWindowLongPtr(hWndMain, GWL_STYLE);
        //SetWindowLongPtr(hWndMain, GWL_STYLE, nStyle & ~WS_CAPTION);
        SetWindowLongPtr(hWndMain, GWL_STYLE, nStyle | WS_SIZEBOX);
        LONG_PTR nExStyle = GetWindowLongPtr(hWndMain, GWL_EXSTYLE);
        if ((nExStyle & WS_EX_LAYERED) == 0)
        {
            SetWindowLongPtr(hWndMain, GWL_EXSTYLE, nExStyle | WS_EX_LAYERED);
            DWORD nAppsUseLightTheme = 0;
            DWORD nSystemUsesLightTheme = 0;
            auto sPathKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
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
        WindowHelper::SetActualWindowPos(hWndMain, HWND_TOPMOST, 100, 100, 800, 600, 0);
        auto root = this->Content();
        // auto root = sp1();
        auto pointerMovedToken = root.PointerMoved({this, &FormalOverlay::Root_PointerMoved});
        auto pointerPressedToken = root.PointerPressed({this, &FormalOverlay::Root_PointerPressed});
        auto pointerReleasedToken = root.PointerReleased({this, &FormalOverlay::Root_PointerReleased});
        // WindowHelper::OpenMessageWindow(get_class_name(root));
    }

    HRESULT FormalOverlay::CreateD2D1Factory()
    {
        D2D1_FACTORY_OPTIONS options = {D2D1_DEBUG_LEVEL_INFORMATION};
        HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, m_pD2DFactory1.put());
        return result;
    }

    HRESULT FormalOverlay::CreateDeviceContext()
    {
        D3D_FEATURE_LEVEL featureLevel;
        D3D_FEATURE_LEVEL aD3D_FEATURE_LEVEL[] = {
            D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
        };
        HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,NULL,
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

    HRESULT FormalOverlay::CreateSwapChain(HWND hWnd)
    {
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
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

    void FormalOverlay::Root_PointerMoved(Windows::Foundation::IInspectable const& sender,
                                          Input::PointerRoutedEventArgs const& e)
    {
        Microsoft::UI::Input::PointerPoint pp = e.GetCurrentPoint(
            sender.try_as<UIElement>());
        auto properties = pp.Properties();
        if (properties.IsLeftButtonPressed())
        {
            POINT pt;
            GetCursorPos(&pt);
            if (bMoving)
            {
                _apw.Move(Windows::Graphics::PointInt32{nXWindow + (pt.x - nX), nYWindow + (pt.y - nY)});
                e.Handled(true);
            }
        }
    }

    void FormalOverlay::Root_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                            Input::PointerRoutedEventArgs const& e)
    {
        // WindowHelper::OpenMessageWindow(L"pressed");
        UIElement sd = sender.try_as<UIElement>();
        auto properties = e.GetCurrentPoint(sd).Properties();
        if (properties.IsLeftButtonPressed())
        {
            [[maybe_unused]] bool result = sd.CapturePointer(e.Pointer());
            nXWindow = _apw.Position().X;
            nYWindow = _apw.Position().Y;
            POINT pt;
            GetCursorPos(&pt);
            nX = pt.x;
            nY = pt.y;
            HWND hWnd = WindowFromPoint(pt);
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
            }
            bMoving = true;
        }
        else if (properties.IsRightButtonPressed())
        {
            Sleep(200);
            Application::Current().Exit();
        }
    }

    void FormalOverlay::Root_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                             Input::PointerRoutedEventArgs const&)

    {
        sender.try_as<UIElement>().ReleasePointerCaptures();
        bMoving = false;
    }

    void FormalOverlay::tsClickThrough_Toggled(Windows::Foundation::IInspectable const& sender,
                                               RoutedEventArgs const&)
    {
        UIElement ts = sender.try_as<UIElement>();
        if (ts.try_as<Controls::ToggleSwitch>().IsOn())
        {
            tb2().Visibility(Visibility::Visible);
            tb3().Visibility(Visibility::Visible);
        }
        else
        {
            tb2().Visibility(Visibility::Collapsed);
            tb3().Visibility(Visibility::Collapsed);
        }
    }

    void FormalOverlay::border_MoveOver(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        // this->ProtectedCursor(Microsoft::UI::Input::InputSystemCursor::Create(Microsoft::UI::Input::InputSystemCursorShape::SizeAll));
    }

    void FormalOverlay::panel_Loaded(Windows::Foundation::IInspectable const&,
                                     RoutedEventArgs const&)
    {
        // sp1().ProtectedCursor(
        //     Microsoft::UI::Input::InputSystemCursor::Create(Microsoft::UI::Input::InputSystemCursorShape::Arrow));
    }

    void FormalOverlay::border_Loaded(Windows::Foundation::IInspectable const&,
                                      RoutedEventArgs const&)
    {
        mainBorder().ProtectedCursor(
            Microsoft::UI::Input::InputSystemCursor::Create(Microsoft::UI::Input::InputSystemCursorShape::SizeAll));
    }

    int32_t FormalOverlay::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void FormalOverlay::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    double FormalOverlay::BorderThickness()
    {
        return borderThickness;
    }

    void FormalOverlay::BorderThickness(double value)
    {
        borderThickness = value;
    }

    void FormalOverlay::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
