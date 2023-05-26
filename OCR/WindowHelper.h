#pragma once

namespace winrt::OCR
{
    class WindowHelper
    {
    public:
        static HWND GetWindowHandle(const Microsoft::UI::Xaml::Window& window);
        static Microsoft::UI::Windowing::AppWindow GetAppWindow(HWND handle);
        // static Microsoft::UI::Xaml::Window GetWindow(HWND handle);
        static RECT GetSystemMargin(HWND handle);
        static void SetActualWindowPos(HWND handle, HWND hWndInsertAfter, int x, int y, int width, int height,
                                       UINT uFlags);
        static void DisableRoundedCorner(HWND handle);
        template <typename T>
        static void GetDesktopResolution(T& horizontal, T& vertical);
        static void OpenMessageWindow(const hstring& message, const hstring& title = L"message",
                                      const Microsoft::UI::Xaml::Window& window = nullptr);
    };

    enum class CursorPosition : UINT
    {
        Undefined,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center
    };

    inline Microsoft::UI::Input::InputCursor SizeAllCursor = Microsoft::UI::Input::InputSystemCursor::Create(
        Microsoft::UI::Input::InputSystemCursorShape::SizeAll);
    inline Microsoft::UI::Input::InputCursor SizeWestEastCursor = Microsoft::UI::Input::InputSystemCursor::Create(
        Microsoft::UI::Input::InputSystemCursorShape::SizeWestEast);
    inline Microsoft::UI::Input::InputCursor SizeNorthSouthCursor = Microsoft::UI::Input::InputSystemCursor::Create(
        Microsoft::UI::Input::InputSystemCursorShape::SizeNorthSouth);
    inline Microsoft::UI::Input::InputCursor SizeNorthwestSoutheastCursor =
        Microsoft::UI::Input::InputSystemCursor::Create(
            Microsoft::UI::Input::InputSystemCursorShape::SizeNorthwestSoutheast);
    inline Microsoft::UI::Input::InputCursor SizeNortheastSouthwestCursor =
        Microsoft::UI::Input::InputSystemCursor::Create(
            Microsoft::UI::Input::InputSystemCursorShape::SizeNortheastSouthwest);
    inline Microsoft::UI::Input::InputCursor ArrowCursor = Microsoft::UI::Input::InputSystemCursor::Create(
        Microsoft::UI::Input::InputSystemCursorShape::Arrow);
}
