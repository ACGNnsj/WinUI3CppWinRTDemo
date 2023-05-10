#pragma once
class WindowHelper
{
public:
	static HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window window);
	static RECT GetSystemMargin(HWND handle);
	static void SetActualWindowPos(HWND handle, HWND hWndInsertAfter, int x, int y, int width, int height, UINT uFlags);
	static void DisableRoundedCorner(HWND handle);
	static void GetDesktopResolution(int& horizontal, int& vertical);
	static void OpenMessageWindow(winrt::hstring message);
};

