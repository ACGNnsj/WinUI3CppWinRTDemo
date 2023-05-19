// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "OverlayPanel.g.h"


namespace winrt::OCR::implementation
{
    struct OverlayPanel : OverlayPanelT<OverlayPanel>
    {
        OverlayPanel();
        OverlayPanel(const Microsoft::UI::Xaml::Window& outerWindow);
        Microsoft::UI::Xaml::Window window{nullptr};
        com_ptr<ID2D1Factory1> m_pD2DFactory1;
        com_ptr<ID3D11Device> m_pD3D11DevicePtr = nullptr;
        com_ptr<ID3D11DeviceContext> m_pD3D11DeviceContext;
        com_ptr<IDXGIDevice1> m_pDXGIDevice;
        com_ptr<ID2D1DeviceContext> m_pD2DDeviceContext;
        com_ptr<IDXGISwapChain1> m_pDXGISwapChain1;
        HWND hWndMain;
        Microsoft::UI::Windowing::AppWindow _apw = nullptr;
        Microsoft::UI::Windowing::OverlappedPresenter _presenter = nullptr;
        ULONG_PTR m_initToken;
        HBITMAP m_hBitmap;
        double mainBorderThickness = 3;
        double mainBorderMargin = 5;
        CursorPosition cursorPosition = CursorPosition::Undefined;
        bool bMoving = false;
        bool borderPressed = false;
        int nX = 0, nY = 0, nXWindow = 0, nYWindow = 0, nWidth = 0, nHeight = 0;
        void AfterInitialization(const Microsoft::UI::Xaml::Window& outerWindow);
        HRESULT CreateD2D1Factory();
        HRESULT CreateDeviceContext();
        HRESULT CreateSwapChain(HWND hWnd);
        void Root_PointerMoved(Windows::Foundation::IInspectable const& sender,
                               Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction Root_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                 Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void Root_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                  Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void border_PointerMoved(Windows::Foundation::IInspectable const& sender,
                                 Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void panel_Loaded(Windows::Foundation::IInspectable const& sender,
                          Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void border_Loaded(Windows::Foundation::IInspectable const& sender,
                           Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void border_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                   Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void border_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                    Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        int32_t MyProperty();
        void MyProperty(int value);
        double MainBorderThickness();
        void MainBorderThickness(double value);
        double MainBorderMargin();
        void MainBorderMargin(double value);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct OverlayPanel : OverlayPanelT<OverlayPanel, implementation::OverlayPanel>
    {
    };
}
