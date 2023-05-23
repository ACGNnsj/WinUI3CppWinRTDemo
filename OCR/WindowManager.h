#pragma once
#include "OverlayPanel.xaml.h"

namespace winrt::OCR
{
    class WindowManager
    {
    public:
        inline static HWND mainWindowHandle{nullptr};
        // static const OverlayPanel& overlayPanel;
        // static implementation::OverlayPanel createOverlayPanel(Microsoft::UI::Xaml::Window window);
    };
}
