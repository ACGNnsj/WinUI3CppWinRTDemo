// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MicaPage.xaml.h"
#if __has_include("MicaPage.g.cpp")
#include "MicaPage.g.cpp"
#endif
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>
#include <winrt/Windows.System.h>
#include <dispatcherqueue.h>
using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
	MicaPage::MicaPage()
	{
		InitializeComponent();
	}

	int32_t MicaPage::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void MicaPage::AfterInit(winrt::Microsoft::UI::Xaml::Window window)
	{
		parentWindow = window;
		SetBackground();
		m_closedRevoker = parentWindow.Closed(winrt::auto_revoke, [&](auto&&, auto&&)
			{
				if (nullptr != m_backdropController)
				{
					m_backdropController.Close();
					m_backdropController = nullptr;
				}

				if (nullptr != m_dispatcherQueueController)
				{
					m_dispatcherQueueController.ShutdownQueueAsync();
					m_dispatcherQueueController = nullptr;
				}
			});
	}

	void MicaPage::SetBackground()
	{
		if (winrt::MUCSB::MicaController::IsSupported())
		{
			// We ensure that there is a Windows.System.DispatcherQueue on the current thread.
			// Always check if one already exists before attempting to create a new one.
			if (nullptr == winrt::WS::DispatcherQueue::GetForCurrentThread() &&
				nullptr == m_dispatcherQueueController)
			{
				m_dispatcherQueueController = CreateSystemDispatcherQueueController();
			}

			// Setup the SystemBackdropConfiguration object.
			SetupSystemBackdropConfiguration();

			// Setup Mica on the current Window.
			m_backdropController = winrt::MUCSB::MicaController();
			m_backdropController.LuminosityOpacity(0);
			m_backdropController.TintOpacity(0);
			m_backdropController.TintColor(winrt::Windows::UI::Colors::Transparent());
			m_backdropController.FallbackColor(winrt::Windows::UI::Colors::Transparent());

			m_backdropController.SetSystemBackdropConfiguration(m_configuration);
			m_backdropController.AddSystemBackdropTarget(
				parentWindow.try_as<winrt::MUC::ICompositionSupportsSystemBackdrop>());
		}
		else
		{
			// The backdrop material is not supported.
		}
	}

	winrt::WS::DispatcherQueueController MicaPage::CreateSystemDispatcherQueueController()
	{
		DispatcherQueueOptions options
		{
			sizeof(DispatcherQueueOptions),
			DQTYPE_THREAD_CURRENT,
			DQTAT_COM_NONE
		};

		::ABI::Windows::System::IDispatcherQueueController* ptr{ nullptr };
		winrt::check_hresult(CreateDispatcherQueueController(options, &ptr));
		return { ptr, take_ownership_from_abi };
	}

	void MicaPage::SetupSystemBackdropConfiguration()
	{
		m_configuration = winrt::MUCSB::SystemBackdropConfiguration();

		// Activation state.
		m_activatedRevoker = parentWindow.Activated(winrt::auto_revoke,
			[&](auto&&, MUX::WindowActivatedEventArgs const& args)
			{
				m_configuration.IsInputActive(
					winrt::MUX::WindowActivationState::Deactivated != args.WindowActivationState());
			});

		// Initial state.
		m_configuration.IsInputActive(true);

		// Application theme.
		m_rootElement = parentWindow.Content().try_as<winrt::MUX::FrameworkElement>();
		if (nullptr != m_rootElement)
		{
			m_themeChangedRevoker = m_rootElement.ActualThemeChanged(winrt::auto_revoke,
				[&](auto&&, auto&&)
				{
					m_configuration.Theme(
						ConvertToSystemBackdropTheme(m_rootElement.ActualTheme()));
				});

			// Initial state.
			m_configuration.Theme(
				ConvertToSystemBackdropTheme(m_rootElement.ActualTheme()));
		}
	}

	winrt::MUCSB::SystemBackdropTheme MicaPage::ConvertToSystemBackdropTheme(winrt::MUX::ElementTheme const& theme)
	{
		switch (theme)
		{
		case winrt::MUX::ElementTheme::Dark:
			return winrt::MUCSB::SystemBackdropTheme::Dark;
		case winrt::MUX::ElementTheme::Light:
			return winrt::MUCSB::SystemBackdropTheme::Light;
		default:
			return winrt::MUCSB::SystemBackdropTheme::Default;
		}
	}

	void MicaPage::MyProperty(int32_t /* value */)
	{
		throw hresult_not_implemented();
	}

	void MicaPage::myButton_Click(IInspectable const&, RoutedEventArgs const&)
	{
		myButton().Content(box_value(L"Clicked"));
		auto window = BlankWindow();
		window.Activate();
	}
	MicaPage::MicaPage(winrt::Microsoft::UI::Xaml::Window window)
	{
		InitializeComponent();
		AfterInit(window);
	}
}
