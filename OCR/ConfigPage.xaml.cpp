// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "ConfigPage.xaml.h"

#include "LanguageItem.h"

#if __has_include("ConfigPage.g.cpp")
#include "ConfigPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    ConfigPage::ConfigPage()
    {
        m_sharedItem = SharedItem::Instance();
        InitializeComponent();
        InitializeLanguageSettings();
    }

    ConfigPage::ConfigPage(const Window& window)
    {
        this->window = window;
        m_sharedItem = SharedItem::Instance();
        InitializeComponent();
        InitializeLanguageSettings();
        // this->DataContext(m_sharedItem);
    }

    int32_t ConfigPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void ConfigPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    OCR::SharedItem ConfigPage::SharedItem()
    {
        return m_sharedItem;
    }

    Windows::Foundation::IAsyncAction ConfigPage::pyHomeButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        const auto picker = Windows::Storage::Pickers::FolderPicker();
        const HWND hWnd = WindowHelper::GetWindowHandle(window);
        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        const auto folder = co_await picker.PickSingleFolderAsync();
        const auto path = folder.Path();
        // WindowHelper::OpenMessageWindow(L"path: " + path);
        m_sharedItem.PyHome(path);
    }

    Windows::Foundation::IAsyncAction ConfigPage::sitePackagesButton_Click(
        Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        const auto picker = Windows::Storage::Pickers::FolderPicker();
        const HWND hWnd = WindowHelper::GetWindowHandle(window);
        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        const auto folder = co_await picker.PickSingleFolderAsync();
        m_sharedItem.SitePackages(folder.Path());
    }

    void ConfigPage::saveButton_Click(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        const auto localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
        localSettings.Values().Insert(L"pyHome", box_value(m_sharedItem.PyHome()));
        localSettings.Values().Insert(L"sitePackages", box_value(m_sharedItem.SitePackages()));
    }

    void ConfigPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        auto window = e.Parameter().try_as<Window>();
        this->window = window;
    }

    void ConfigPage::InitializeLanguageSettings()
    {
        auto supportedLanguages = Windows::Media::Ocr::OcrEngine::AvailableRecognizerLanguages();
        hstring text = L"";
        OCR::implementation::LanguageItem::RegisterDependencyProperty();
        for (auto supported_language : supportedLanguages)
        {
            auto languageTag = supported_language.LanguageTag();
            auto displayName = supported_language.DisplayName();
            text = text + languageTag + L" " + displayName + L"\n";
            OCR::LanguageItem item = make<OCR::implementation::LanguageItem>();
            item.DisplayName(displayName);
            item.LanguageTag(languageTag);
            sourceComboBox().Items().Append(item);
            targetComboBox().Items().Append(item);
        }
        WindowHelper::OpenMessageWindow(text);
    }
}
