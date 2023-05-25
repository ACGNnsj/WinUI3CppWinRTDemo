#include "pch.h"
#include <winrt/Windows.UI.Xaml.Hosting.h>

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

    Microsoft::UI::Windowing::AppWindow WindowHelper::GetAppWindow(const HWND handle)
    {
        const auto windowId = Microsoft::UI::GetWindowIdFromWindow(handle);
        return Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);
    }

    Microsoft::UI::Xaml::Window WindowHelper::GetWindow(HWND handle)
    {
        // Microsoft::UI::Windowing::AppWindow appWindow = GetAppWindow(handle);
        /*Windows::UI::WindowManagement::AppWindow appWindow= Windows::UI::WindowManagement::AppWindow::GetFromWindowHandle(handle);
        Windows::UI::Xaml::Hosting::ElementCompositionPreview::GetAppWindowContent(appWindow);*/
        return nullptr;
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

    /*void WindowHelper::GetDesktopResolution(auto& horizontal, auto& vertical)
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
    }*/

    template <typename T>
    void WindowHelper::GetDesktopResolution(T& horizontal, T& vertical)
    {
        RECT desktop;
        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &desktop);
        horizontal = desktop.right;
        vertical = desktop.bottom;
    }

    template void WindowHelper::GetDesktopResolution(int& horizontal, int& vertical);
    template void WindowHelper::GetDesktopResolution(double& horizontal, double& vertical);

    void WindowHelper::OpenMessageWindow(const hstring& message, const hstring& title,
                                         const Microsoft::UI::Xaml::Window& window)
    {
        /*if (window)
        {
            const Microsoft::UI::Xaml::Controls::ContentDialog msg{};
            msg.XamlRoot(window.Content().XamlRoot());
            msg.Title(box_value(title));
            msg.Content(box_value(message));
            msg.PrimaryButtonText(L"OK");
            msg.CloseButtonText(L"Cancel");
            msg.as<IInitializeWithWindow>()->Initialize(GetWindowHandle(window));
            auto result = msg.ShowAsync();
            return;
        }*/
        if (const auto hWnd = WindowManager::mainWindowHandle; hWnd)
        {
            const Windows::UI::Popups::MessageDialog msg{message, title};
            msg.DefaultCommandIndex(0);
            msg.CancelCommandIndex(1);
            msg.as<IInitializeWithWindow>()->Initialize(hWnd);
            auto command = msg.ShowAsync();
            return;
        }
        const auto msgWindow = Microsoft::UI::Xaml::Window();
        const auto textBlock = Microsoft::UI::Xaml::Controls::TextBlock();
        textBlock.Text(message);
        msgWindow.Content(textBlock);
        msgWindow.Title(title);
        msgWindow.Activate();
    }
}
