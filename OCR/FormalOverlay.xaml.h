// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "FormalOverlay.g.h"

namespace winrt::OCR::implementation
{
    struct FormalOverlay : FormalOverlayT<FormalOverlay>
    {
        FormalOverlay();
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
        double borderThickness=10;
        bool bMoving = false;
        int nX = 0, nY = 0, nXWindow = 0, nYWindow = 0;
        HRESULT CreateD2D1Factory();
        HRESULT CreateDeviceContext();
        HRESULT CreateSwapChain(HWND hWnd);
        void Root_PointerMoved(Windows::Foundation::IInspectable const& sender,
                               Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void Root_PointerPressed(Windows::Foundation::IInspectable const& sender,
                                 Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void Root_PointerReleased(Windows::Foundation::IInspectable const& sender,
                                  Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);
        void tsClickThrough_Toggled(Windows::Foundation::IInspectable const& sender,
                                    Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void border_MoveOver(Windows::Foundation::IInspectable const& sender,
                             Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void panel_Loaded(Windows::Foundation::IInspectable const& sender,
                          Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void border_Loaded(Windows::Foundation::IInspectable const& sender,
                           Microsoft::UI::Xaml::RoutedEventArgs const& e);
        int32_t MyProperty();
        void MyProperty(int value);
        double BorderThickness();
        void BorderThickness(double value);

        void myButton_Click(Windows::Foundation::IInspectable const& sender,
                            Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::OCR::factory_implementation
{
    struct FormalOverlay : FormalOverlayT<FormalOverlay, implementation::FormalOverlay>
    {
    };
}
