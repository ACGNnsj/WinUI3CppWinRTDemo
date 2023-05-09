// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>
#include <dwmapi.h>
#include <WindowHelper.h>
using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
	MainWindow::MainWindow()
	{
		InitializeComponent();
	}

	int32_t MainWindow::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void MainWindow::MyProperty(int32_t /* value */)
	{
		throw hresult_not_implemented();
	}
	void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
	{
		myButton().Content(box_value(L"Clicked"));
		/*auto newAppWindow = winrt::Microsoft::UI::Windowing::AppWindow::Create();
		newAppWindow.Title(L"New App Window");
		newAppWindow.Show(true);*/
		auto newWindow = Window();
		OCROverlay overlay = OCROverlay();
		//OCROverlay overlay();
		newWindow.ExtendsContentIntoTitleBar(true);
		//overlay.Background(Media::SolidColorBrush(Windows::UI::Colors::Transparent()));
		newWindow.Content(overlay);
		auto appWindow = newWindow.AppWindow();
		/*winrt::Windows::Graphics::SizeInt32 size = { 800,600 };
		appWindow.Resize(size);*/
		auto presenter = appWindow.Presenter().try_as<winrt::Microsoft::UI::Windowing::OverlappedPresenter>();
		presenter.SetBorderAndTitleBar(false, false);
		auto brushHolder = newWindow.try_as<winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop>();
		auto compositor = Compositor();
		auto colorBrush = compositor.CreateColorBrush(Windows::UI::ColorHelper::FromArgb(0, 0, 0, 0)).try_as<winrt::Windows::UI::Composition::CompositionBrush>();
		brushHolder.SystemBackdrop(colorBrush);
		HWND hWnd = WindowHelper::GetWindowHandle(newWindow);
		int x, y, width, height = (x = 0, y = 0, width = 100, height = 100);
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, width, height, 0);
		RECT systemMargin = WindowHelper::GetSystemMargin(hWnd);
		x -= systemMargin.left;
		y -= systemMargin.top;
		width += systemMargin.left + systemMargin.right;
		height += systemMargin.top + systemMargin.bottom;
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, width, height, 0);
		WindowHelper::DisableRoundedCorner(hWnd);
		LONG_PTR lExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		lExStyle |= WS_EX_LAYERED;
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, lExStyle);
		BYTE bAlpha = (255 * 50) / 100;
		bool result = true;
		if (lExStyle & WS_EX_LAYERED) {
			result = SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), bAlpha, LWA_COLORKEY);
		}
		else {
			auto errorWindow = Window();
			auto textBlock = winrt::Microsoft::UI::Xaml::Controls::TextBlock();
			textBlock.Text(L"No WS_EX_LAYERED");
			errorWindow.Content(textBlock);
			errorWindow.Activate();
		}
		newWindow.Activate();
		if (!result)
		{
			auto messageWindow = Window();
			auto textBlock = winrt::Microsoft::UI::Xaml::Controls::TextBlock();
			textBlock.Text(L"Failed");
			messageWindow.Content(textBlock);
			messageWindow.Activate();
		}
	}
}
