#pragma once
class WindowHelper
{
public:
	static HWND GetWindowHandle(winrt::Microsoft::UI::Xaml::Window window);
	static RECT GetSystemMargin(HWND handle);
	static void DisableRoundedCorner(HWND handle);
};

