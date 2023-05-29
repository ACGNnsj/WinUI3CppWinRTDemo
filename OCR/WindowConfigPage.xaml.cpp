// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "WindowConfigPage.xaml.h"
#if __has_include("WindowConfigPage.g.cpp")
#include "WindowConfigPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OCR::implementation
{
    WindowConfigPage::WindowConfigPage()
    {
        m_sharedItem = SharedItem::Instance();
        InitializeComponent();
    }

    WindowConfigPage::WindowConfigPage(const Window& window)
    {
        m_sharedItem = SharedItem::Instance();
        this->outerWindow = window;
        InitializeComponent();
    }

    OCR::SharedItem WindowConfigPage::SharedItem()
    {
        return m_sharedItem;
    }

    void WindowConfigPage::Apply_Click(const IInspectable&, const RoutedEventArgs&)
    {
        auto width = m_sharedItem.Width();
        auto height = m_sharedItem.Height();
        auto x = m_sharedItem.X();
        auto y = m_sharedItem.Y();
        const HWND hWnd = WindowManager::mainWindowHandle;
        const auto appWindow = WindowHelper::GetAppWindow(hWnd);
        if (_isnan(width))
        {
            width = appWindow.Size().Width;
            WindowHelper::OpenMessageWindow(L"width is NaN");
        }
        if (_isnan(height))
        {
            height = appWindow.Size().Height;
            WindowHelper::OpenMessageWindow(L"height is NaN");
        }
        if (_isnan(x))
        {
            x = appWindow.Position().X;
            WindowHelper::OpenMessageWindow(L"x is NaN");
        }
        if (_isnan(y))
        {
            y = appWindow.Position().Y;
            WindowHelper::OpenMessageWindow(L"y is NaN");
        }
        appWindow.MoveAndResize({
            static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)
        });
    }

    void WindowConfigPage::Reset_Click(const IInspectable&, const RoutedEventArgs&)
    {
        const HWND hWnd = WindowManager::mainWindowHandle;
        const auto appWindow = WindowHelper::GetAppWindow(hWnd);
        appWindow.MoveAndResize(WindowManager::defaultWindowRect);
        m_sharedItem.X(WindowManager::defaultWindowRect.X);
        m_sharedItem.Y(WindowManager::defaultWindowRect.Y);
        m_sharedItem.Width(WindowManager::defaultWindowRect.Width);
        m_sharedItem.Height(WindowManager::defaultWindowRect.Height);
    }

    void WindowConfigPage::OnNavigatedTo(Navigation::NavigationEventArgs const& e)
    {
        const auto window = e.Parameter().try_as<Window>();
        outerWindow = window;
    }

    void WindowConfigPage::hotKeyTextBox_PreviewKeyDown(const IInspectable&, const Input::KeyRoutedEventArgs& e)
    {
        [[maybe_unused]] auto result = e.Handled();
        const auto key = e.Key();
        hstring text = L"";
        if (static auto resetKeys = {
            Windows::System::VirtualKey::Delete, Windows::System::VirtualKey::Back,
            Windows::System::VirtualKey::Escape, Windows::System::VirtualKey::None
        }; std::ranges::find(resetKeys, key) != resetKeys.end())
        {
            hotKeyTextBox().Text(text);
            return;
        }
        auto static control = Windows::System::VirtualKey::Control;
        auto static shift = Windows::System::VirtualKey::Shift;
        auto static alt = Windows::System::VirtualKey::Menu;
        auto static win = Windows::System::VirtualKey::LeftWindows;
        static auto leftControl = Windows::System::VirtualKey::LeftControl;
        static auto rightControl = Windows::System::VirtualKey::RightControl;
        static auto leftShift = Windows::System::VirtualKey::LeftShift;
        static auto rightShift = Windows::System::VirtualKey::RightShift;
        static auto leftAlt = Windows::System::VirtualKey::LeftMenu;
        static auto rightAlt = Windows::System::VirtualKey::RightMenu;
        static auto leftWin = Windows::System::VirtualKey::LeftWindows;
        static auto rightWin = Windows::System::VirtualKey::RightWindows;
        if (static auto completeModifiers = {
            leftControl, rightControl, leftShift, rightShift, leftAlt, rightAlt, leftWin, rightWin, control, shift, alt,
            win
        }; std::ranges::find(completeModifiers, key) != completeModifiers.end())
        {
            hotKeyTextBox().Text(text);
            return;
        }
        auto static modifiers = {control, shift, alt, win};
        auto static modifierFlagMap = std::map<Windows::System::VirtualKey, int>{
            {control, MOD_CONTROL}, {shift, MOD_SHIFT}, {alt, MOD_ALT}, {win, MOD_WIN}
        };
        static auto modifierMap = std::map<Windows::System::VirtualKey, std::wstring>{
            {control, L"Ctrl"}, {shift, L"Shift"}, {alt, L"Alt"}, {win, L"Win"}
        };
        static auto modifierMask = 0;
        for (auto modifier : modifiers)
        {
            if (const auto state = Microsoft::UI::Input::InputKeyboardSource::GetKeyStateForCurrentThread(modifier);
                state == Windows::UI::Core::CoreVirtualKeyStates::Down)
            {
                modifierMask |= modifierFlagMap[modifier];
                text = text + modifierMap[modifier] + L"+";
            }
        }
        text = text + to_hstring(static_cast<char16_t>(key));
        hotKeyTextBox().Text(text);
        WindowManager::modifierMask = modifierMask;
        WindowManager::hotkey = static_cast<UINT>(key);
    }

    void WindowConfigPage::ApplyHotkey(const IInspectable&, const RoutedEventArgs&)
    {
        UnregisterHotKey(WindowManager::mainWindowHandle, 1);
        RegisterHotKey(WindowManager::mainWindowHandle, 1, WindowManager::modifierMask, WindowManager::hotkey);
    }

    void WindowConfigPage::CleanWindow(const IInspectable&, const RoutedEventArgs&)
    {
        if (WindowManager::mainWindow)
        {
            if (const auto panel = WindowManager::mainWindow.Content().try_as<OCR::OverlayPanel>())
            {
                panel.CleanCanvas();
            }
            else
            {
                WindowHelper::OpenMessageWindow(L"panel is null");
            }
        }
        else
        {
            WindowHelper::OpenMessageWindow(L"mainWindow is null");
        }
    }
}
