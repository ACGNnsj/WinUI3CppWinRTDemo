#include "pch.h"

namespace winrt::OCR
{
    HWND WindowHelper::GetWindowHandle(const Microsoft::UI::Xaml::Window& window)
    {
        const auto windowNative{window.try_as<IWindowNative>()};
        check_bool(windowNative);
        HWND hWnd{nullptr};
        windowNative->get_WindowHandle(&hWnd);
        return hWnd;
    }

    RECT WindowHelper::GetSystemMargin(const HWND handle)
    {
        RECT withMargin{0, 0, 0, 0};
        if (const HRESULT success = DwmGetWindowAttribute(handle, DWMWA_EXTENDED_FRAME_BOUNDS, &withMargin,
                                                          sizeof(RECT)); success != S_OK)
        {
            return withMargin;
        }
        RECT noMargin{0, 0, 0, 0};
        if (const BOOL result = GetWindowRect(handle, &noMargin); result == 0)
        {
            return noMargin;
        }
        return RECT
        {
            withMargin.left - noMargin.left,
            withMargin.top - noMargin.top,
            noMargin.right - withMargin.right,
            noMargin.bottom - withMargin.bottom,
        };
    }

    void WindowHelper::SetActualWindowPos(const HWND handle, const HWND hWndInsertAfter, int x, int y, int width,
                                          int height, const UINT uFlags)
    {
        SetWindowPos(handle, hWndInsertAfter, x, y, width, height, uFlags);
        const auto [left, top, right, bottom] = GetSystemMargin(handle);
        x -= left;
        y -= top;
        width += left + right;
        height += top + bottom;
        SetWindowPos(handle, hWndInsertAfter, x, y, width, height, uFlags);
    }

    void WindowHelper::DisableRoundedCorner(const HWND handle)
    {
        constexpr DWORD dwAttribute = DWMWA_WINDOW_CORNER_PREFERENCE;
        constexpr DWM_WINDOW_CORNER_PREFERENCE pvAttribute = DWMWCP_DONOTROUND;
        DwmSetWindowAttribute(handle, dwAttribute, &pvAttribute, sizeof(dwAttribute));
    }

    void WindowHelper::GetDesktopResolution(int& horizontal, int& vertical)
    {
        RECT desktop;
        // Get a handle to the desktop window
        const HWND hDesktop = GetDesktopWindow();
        // Get the size of screen to the variable desktop
        GetWindowRect(hDesktop, &desktop);
        // The top left corner will have coordinates (0,0)
        // and the bottom right corner will have coordinates
        // (horizontal, vertical)
        horizontal = desktop.right;
        vertical = desktop.bottom;
    }

    void WindowHelper::OpenMessageWindow(const hstring& message)
    {
        const auto window = Microsoft::UI::Xaml::Window();
        const auto textBlock = Microsoft::UI::Xaml::Controls::TextBlock();
        textBlock.Text(message);
        window.Content(textBlock);
        window.Activate();
    }
}
