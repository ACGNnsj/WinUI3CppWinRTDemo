// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "ConfigPage.g.h"

namespace winrt::OCR::implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage>
    {
        ConfigPage();
        ConfigPage(const Microsoft::UI::Xaml::Window& window);
        Microsoft::UI::Xaml::Window outerWindow{nullptr};

        // OCR::SharedItem SharedItem();
        winrt_property<OCR::SharedItem, false> SharedItem;
        // void SharedItem(OCR::SharedItem const& value);
        // Windows::Foundation::Collections::IObservableVector<OCR::LanguageItem> LanguageItems();
        winrt_property<Windows::Foundation::Collections::IObservableVector<OCR::LanguageItem>, false> LanguageItems;
        Windows::Foundation::IAsyncAction pyHomeButton_Click(Windows::Foundation::IInspectable const& sender,
                                                             Microsoft::UI::Xaml::RoutedEventArgs const& args);
        Windows::Foundation::IAsyncAction sitePackagesButton_Click(Windows::Foundation::IInspectable const& sender,
                                                                   Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void saveButton_Click(Windows::Foundation::IInspectable const& sender,
                              Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void applyButton_Click(Windows::Foundation::IInspectable const& sender,
                               Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

    private:
        // OCR::SharedItem& m_sharedItem;
        // Windows::Foundation::Collections::IObservableVector<OCR::LanguageItem> m_languageItems;
        void InitializeLanguageSettings();
    };
}

namespace winrt::OCR::factory_implementation
{
    struct ConfigPage : ConfigPageT<ConfigPage, implementation::ConfigPage>
    {
    };
}
