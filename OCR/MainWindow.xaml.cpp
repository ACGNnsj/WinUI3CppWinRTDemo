// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.Windowing.h>

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
		//newWindow.ExtendsContentIntoTitleBar(true);
		//overlay.Background(Media::SolidColorBrush(Windows::UI::Colors::Transparent()));
		newWindow.Content(overlay);
		auto appWindow = newWindow.AppWindow();
		winrt::Windows::Graphics::SizeInt32 size = { 800,600 };
		appWindow.Resize(size);
		auto presenter = appWindow.Presenter().try_as<winrt::Microsoft::UI::Windowing::OverlappedPresenter>();
		//presenter.SetBorderAndTitleBar(false, false);
		auto brushHolder = newWindow.try_as<winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop>();
		auto compositor = Compositor();
		auto colorBrush = compositor.CreateColorBrush(Windows::UI::ColorHelper::FromArgb(0, 0, 0, 0)).try_as<winrt::Windows::UI::Composition::CompositionBrush>();
		brushHolder.SystemBackdrop(colorBrush);
		auto windowNative{ newWindow.try_as<::IWindowNative>() };
		winrt::check_bool(windowNative);
		HWND hWnd{ 0 };
		windowNative->get_WindowHandle(&hWnd);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		LONG_PTR lExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		lExStyle ^= WS_EX_LAYERED;
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, lExStyle);
		BYTE bAlpha = (lExStyle & WS_EX_LAYERED) ? (255 * 50) / 100 : 255;
		bool result = SetLayeredWindowAttributes(hWnd, RGB(255, 0, 0), bAlpha, LWA_COLORKEY);
		newWindow.Activate();
		auto messageWindow = Window();
		auto textBlock = winrt::Microsoft::UI::Xaml::Controls::TextBlock();
		if (result)
		{
			textBlock.Text(L"Success");
			messageWindow.Content(textBlock);
		}
		else
		{
			textBlock.Text(L"Failed");
			messageWindow.Content(textBlock);
		}
		messageWindow.Activate();
	}
}
