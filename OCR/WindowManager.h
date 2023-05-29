#pragma once
#include "OverlayPanel.xaml.h"

namespace winrt::OCR
{
    class WindowManager
    {
    public:
        inline static HWND mainWindowHandle{nullptr};
        inline static constexpr Windows::Graphics::RectInt32 defaultWindowRect{100, 100, 800, 600};
        inline static hstring rawText;
        inline static double monitorWidth;
        inline static double monitorHeight;
        inline static UINT modifierMask = MOD_CONTROL | MOD_ALT;
        inline static UINT hotkey = 'P';
        inline static hstring sourceLanguageTag = L"";
        inline static hstring targetLanguageTag = L"";
        inline static const Windows::UI::Xaml::Window& mainWindow = nullptr;
        inline static bool isThreadRunning = false;
        inline static bool hasTaskToRun = false;
        // inline static const OCR::OverlayPanel& overlayPanel = nullptr;
        // static implementation::OverlayPanel createOverlayPanel(Microsoft::UI::Xaml::Window window);
    };
}
