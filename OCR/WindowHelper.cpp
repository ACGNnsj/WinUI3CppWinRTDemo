#include "pch.h"
#include "WindowHelper.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

HWND WindowHelper::GetWindowHandle(winrt::Microsoft::UI::Xaml::Window window)
{
	auto windowNative{ window.try_as<::IWindowNative>() };
	winrt::check_bool(windowNative);
	HWND hWnd{ 0 };
	windowNative->get_WindowHandle(&hWnd);
	return hWnd;
}

RECT WindowHelper::GetSystemMargin(HWND handle)
{
	RECT withMargin{ 0,0,0,0 };
	HRESULT success = DwmGetWindowAttribute(handle, DWMWINDOWATTRIBUTE::DWMWA_EXTENDED_FRAME_BOUNDS, &withMargin, sizeof(RECT));
	if (success != S_OK) {
		return withMargin;
	}
	RECT noMargin{ 0,0,0,0 };
	BOOL result = GetWindowRect(handle, &noMargin);
	if (result == 0) {
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

void WindowHelper::SetActualWindowPos(HWND handle, HWND hWndInsertAfter, int x, int y, int width, int height, UINT uFlags)
{
	SetWindowPos(handle, hWndInsertAfter, x, y, width, height, uFlags);
	RECT systemMargin = GetSystemMargin(handle);
	x -= systemMargin.left;
	y -= systemMargin.top;
	width += systemMargin.left + systemMargin.right;
	height += systemMargin.top + systemMargin.bottom;
	SetWindowPos(handle, hWndInsertAfter, x, y, width, height, uFlags);
}

void WindowHelper::DisableRoundedCorner(HWND handle)
{
	DWORD dwAttribute = DWMWA_WINDOW_CORNER_PREFERENCE;
	DWM_WINDOW_CORNER_PREFERENCE  pvAttribute = DWMWCP_DONOTROUND;
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

void WindowHelper::OpenMessageWindow(winrt::hstring message)
{
	auto window = winrt::Microsoft::UI::Xaml::Window();
	auto textBlock = winrt::Microsoft::UI::Xaml::Controls::TextBlock();
	textBlock.Text(message);
	window.Content(textBlock);
	window.Activate();
}
