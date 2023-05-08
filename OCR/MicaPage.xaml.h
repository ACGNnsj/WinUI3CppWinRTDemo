// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MicaPage.g.h"
namespace winrt
{
	namespace MUC = Microsoft::UI::Composition;
	namespace MUCSB = Microsoft::UI::Composition::SystemBackdrops;
	namespace MUX = Microsoft::UI::Xaml;
	namespace WS = Windows::System;
}
namespace winrt::OCR::implementation
{
	struct MicaPage : MicaPageT<MicaPage>
	{
		winrt::MUCSB::SystemBackdropConfiguration m_configuration{ nullptr };
		winrt::MUCSB::MicaController m_backdropController{ nullptr };
		winrt::MUX::Window::Activated_revoker m_activatedRevoker;
		winrt::MUX::Window::Closed_revoker m_closedRevoker;
		winrt::MUX::FrameworkElement::ActualThemeChanged_revoker m_themeChangedRevoker;
		winrt::MUX::FrameworkElement m_rootElement{ nullptr };
		winrt::WS::DispatcherQueueController m_dispatcherQueueController{ nullptr };
		MicaPage();
		MicaPage(winrt::Microsoft::UI::Xaml::Window window);

		void AfterInit(winrt::Microsoft::UI::Xaml::Window window);
		int32_t MyProperty();

		void SetBackground();

		winrt::WS::DispatcherQueueController CreateSystemDispatcherQueueController();

		void SetupSystemBackdropConfiguration();

		winrt::MUCSB::SystemBackdropTheme ConvertToSystemBackdropTheme(
			winrt::MUX::ElementTheme const& theme);

		void MyProperty(int32_t value);

		void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
	private:
		winrt::Microsoft::UI::Xaml::Window parentWindow{ nullptr };
	};
}

namespace winrt::OCR::factory_implementation
{
	struct MicaPage : MicaPageT<MicaPage, implementation::MicaPage>
	{
	};
}
