// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "ConfigPage.xaml.h"

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
        // m_sharedItem = SharedItem::Instance();
        SharedItem.value = SharedItem::Instance();
        InitializeComponent();
        InitializeLanguageSettings();
    }

    ConfigPage::ConfigPage(const Window& window)
    {
        this->outerWindow = window;
        // m_sharedItem = SharedItem::Instance();
        SharedItem.value = SharedItem::Instance();
        InitializeComponent();
        InitializeLanguageSettings();
        // this->DataContext(m_sharedItem);
    }

    // OCR::SharedItem ConfigPage::SharedItem()
    // {
    //     return m_sharedItem;
    // }

    // Windows::Foundation::Collections::IObservableVector<OCR::LanguageItem> ConfigPage::LanguageItems()
    // {
    //     return m_languageItems;
    // }

    Windows::Foundation::IAsyncAction ConfigPage::pyHomeButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        const auto picker = Windows::Storage::Pickers::FolderPicker();
        const HWND hWnd = WindowHelper::GetWindowHandle(outerWindow);
        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        const auto folder = co_await picker.PickSingleFolderAsync();
        const auto path = folder.Path();
        // WindowHelper::OpenMessageWindow(L"path: " + path);
        // m_sharedItem.PyHome(path);
        SharedItem.value.PyHome(path);
    }

    Windows::Foundation::IAsyncAction ConfigPage::sitePackagesButton_Click(
        Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        const auto picker = Windows::Storage::Pickers::FolderPicker();
        const HWND hWnd = WindowHelper::GetWindowHandle(outerWindow);
        picker.as<IInitializeWithWindow>()->Initialize(hWnd);
        const auto folder = co_await picker.PickSingleFolderAsync();
        SharedItem.value.SitePackages(folder.Path());
    }

    void ConfigPage::saveButton_Click(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        const auto localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
        localSettings.Values().Insert(L"pyHome", box_value(SharedItem.value.PyHome()));
        localSettings.Values().Insert(L"sitePackages", box_value(SharedItem.value.SitePackages()));
    }

    void ConfigPage::applyButton_Click(Windows::Foundation::IInspectable const&, RoutedEventArgs const&)
    {
        // auto selectedSourceLanguageItem = unbox_value_or<OCR::LanguageItem>(sourceComboBox().SelectedItem(), nullptr);
        auto selectedSourceLanguageTag = unbox_value_or(sourceComboBox().SelectedValue(), L"");
        WindowManager::sourceLanguageTag = selectedSourceLanguageTag;
        auto selectedTargetLanguageTag = unbox_value_or(targetComboBox().SelectedValue(), L"");
        WindowManager::targetLanguageTag = selectedTargetLanguageTag;
        // WindowHelper::OpenMessageWindow(WindowManager::sourceLanguageTag + L" " + WindowManager::targetLanguageTag);
    }

    void ConfigPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        const auto window = e.Parameter().try_as<Window>();
        this->outerWindow = window;
    }

    void ConfigPage::InitializeLanguageSettings()
    {
        const auto supportedLanguages = Windows::Media::Ocr::OcrEngine::AvailableRecognizerLanguages();
        // hstring text = L"";
        LanguageItem::RegisterDependencyProperty();
        // m_languageItems = single_threaded_observable_vector<OCR::LanguageItem>();
        LanguageItems.value = single_threaded_observable_vector<OCR::LanguageItem>();
        for (auto supported_language : supportedLanguages)
        {
            auto languageTag = supported_language.LanguageTag();
            auto displayName = supported_language.DisplayName();
            // text = text + languageTag + L" " + displayName + L"\n";
            OCR::LanguageItem item = make<OCR::implementation::LanguageItem>();
            item.DisplayName(displayName);
            item.LanguageTag(languageTag);
            LanguageItems.value.Append(item);
            // sourceComboBox().Items().Append(item);
            // targetComboBox().Items().Append(item);
            // sourceComboBox().Items().Append(box_value(languageTag));
            // targetComboBox().Items().Append(box_value(languageTag));
        }
        if (!WindowManager::sourceLanguageTag.empty())
        {
            // sourceComboBox().SelectedItem(box_value(WindowManager::sourceLanguageTag));
            sourceComboBox().SelectedValue(box_value(WindowManager::sourceLanguageTag));
        }
        else
        {
            sourceComboBox().SelectedIndex(0);
        }
        if (!WindowManager::targetLanguageTag.empty())
        {
            // targetComboBox().SelectedItem(box_value(WindowManager::targetLanguageTag));
            targetComboBox().SelectedValue(box_value(WindowManager::targetLanguageTag));
        }
        else
        {
            targetComboBox().SelectedIndex(0);
        }
        // WindowHelper::OpenMessageWindow(text);
    }
}
