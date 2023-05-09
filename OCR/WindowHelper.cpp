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

void WindowHelper::DisableRoundedCorner(HWND handle)
{
	DWORD dwAttribute = DWMWA_WINDOW_CORNER_PREFERENCE;
	DWM_WINDOW_CORNER_PREFERENCE  pvAttribute = DWMWCP_DONOTROUND;
	DwmSetWindowAttribute(handle, dwAttribute, &pvAttribute, sizeof(dwAttribute));
}
