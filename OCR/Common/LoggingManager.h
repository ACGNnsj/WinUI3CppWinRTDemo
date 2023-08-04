#pragma once

namespace winrt::OCR
{
    class LoggingManager
    {
    public:
        static void Log(const hstring& message);
        static void LogError(const hstring& message);
        static  Windows::Foundation::IAsyncAction Initiate();

        class Logger
        {
        public:
            Windows::Foundation::Diagnostics::LoggingChannel m_channel{nullptr};
            Windows::Foundation::Diagnostics::LoggingSession m_session{nullptr};

        private:
            Windows::Storage::StorageFolder m_logFolder{nullptr};
            constexpr long long static TICKS_TO_DELETE = static_cast<long long>(15 * 24 * 60 * 60) * 10000000;
            Windows::Foundation::IAsyncAction RegisterUnhandledErrorHandler();
            Windows::Foundation::IAsyncAction Application_UnhandledException(
                Windows::Foundation::IInspectable const& sender,
                Microsoft::UI::Xaml::UnhandledExceptionEventArgs const& e) const;
            Windows::Foundation::IAsyncAction LogError(const hstring& message, const hresult& hr = -1) const;

        public:
            [[nodiscard]] const Windows::Foundation::Diagnostics::LoggingChannel& Channel() const;
            [[nodiscard]] const Windows::Foundation::Diagnostics::LoggingSession& Session() const;
            Windows::Foundation::IAsyncAction Initiate();
            static Logger& Instance();
            Windows::Foundation::IAsyncAction CleanUpFile() const;
        };
    };
}
