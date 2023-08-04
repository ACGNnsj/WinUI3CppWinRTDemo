#include "pch.h"

#include "App.xaml.h"

using namespace winrt::OCR;

void LoggingManager::Log(const hstring& message)
{
    Logger::Instance().Channel().LogMessage(message, Windows::Foundation::Diagnostics::LoggingLevel::Information);
}

void LoggingManager::LogError(const hstring& message)
{
    Logger::Instance().Channel().LogMessage(message, Windows::Foundation::Diagnostics::LoggingLevel::Error);
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Initiate()
{
    co_await Logger::Instance().Initiate();
    Logger::Instance().CleanUpFile();
    // auto logger = Logger::Instance();
    // auto addr = &logger;
    // auto another_logger = Logger::Instance();
    // auto another_addr = &another_logger;
    // if (addr == another_addr)
    // {
    //     WindowHelper::OpenMessageWindow(to_hstring(reinterpret_cast<std::uintptr_t>(addr)), L"Same instance");
    // }
    // else
    // {
    //     WindowHelper::OpenMessageWindow(
    //         to_hstring(reinterpret_cast<std::uintptr_t>(addr)) + L" " + to_hstring(
    //             reinterpret_cast<std::uintptr_t>(another_addr)), L"Different instance");
    // }
}

const winrt::Windows::Foundation::Diagnostics::LoggingChannel& LoggingManager::Logger::Channel() const
{
    return m_channel;
}

const winrt::Windows::Foundation::Diagnostics::LoggingSession& LoggingManager::Logger::Session() const
{
    return m_session;
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Logger::RegisterUnhandledErrorHandler()
{
    m_logFolder = co_await Windows::Storage::ApplicationData::Current().LocalFolder().CreateFolderAsync(
        L"Logs", Windows::Storage::CreationCollisionOption::OpenIfExists);
    [[maybe_unused]] auto token = Microsoft::UI::Xaml::Application::Current().UnhandledException({
        this, &Logger::Application_UnhandledException
    });
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Logger::Application_UnhandledException(
    Windows::Foundation::IInspectable const&,
    Microsoft::UI::Xaml::UnhandledExceptionEventArgs const& e) const
{
    co_await LogError(e.Message());
    // try
    // {
    //     Channel().LogMessage(L"Caught the exception");
    //     // e.Exception().Propagate();
    // }
    // catch (std::exception& ex)
    // {
    //     LogError(to_hstring(ex.what()));
    //     // throw; 
    // }catch (hresult_error& he)
    // {
    //     LogError(he.message());
    // }
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Logger::LogError(
    const hstring& message, const hresult& hr) const
{
    const auto logMessage = std::format(L"UnhandledError(HRESULT: {}): {}", hr.value, message);
    Channel().LogMessage(logMessage, Windows::Foundation::Diagnostics::LoggingLevel::Critical);
    //logChannel.LogMessage(string.Format("Effor Message: {0}", ex.Message));
    if (Session() != nullptr)
    {
        const Windows::Globalization::DateTimeFormatting::DateTimeFormatter formatter{L"shortdate longtime"};
        // const auto filename = formatter.Format(clock::now()) + to_hstring(".etl");
        const auto filename = std::format(L"{}.etl", formatter.Format(clock::now()).operator std::wstring_view());
        auto logFile = co_await Session()
            .SaveToFileAsync(m_logFolder, filename);
    }
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Logger::Initiate()
{
    m_channel = Windows::Foundation::Diagnostics::LoggingChannel(L"OCR Channel", nullptr);
    m_session = Windows::Foundation::Diagnostics::LoggingSession(L"OCR Session");
    m_session.AddLoggingChannel(m_channel);
    co_await RegisterUnhandledErrorHandler();
}

LoggingManager::Logger& LoggingManager::Logger::Instance()
{
    static auto instance = Logger();
    return instance;
}

winrt::Windows::Foundation::IAsyncAction LoggingManager::Logger::CleanUpFile() const
{
    try
    {
        for (const auto logFiles = co_await m_logFolder.GetFilesAsync(); auto&& logFile : logFiles)
        {
            const auto span = clock::now() - logFile.DateCreated();
            const auto ticks = span.count();
            auto t = clock::now().time_since_epoch();
            if (ticks > TICKS_TO_DELETE)
            {
                co_await logFile.DeleteAsync();
            }
        }
    }
    catch (std::exception& e)
    {
        Channel().LogMessage(L"Error while deleting log files: " + to_hstring(e.what()),
                             Windows::Foundation::Diagnostics::LoggingLevel::Error);
    }catch (hresult_error& he)
    {
        Channel().LogMessage(L"HRESULT Error while deleting log files: " + he.message(),
                             Windows::Foundation::Diagnostics::LoggingLevel::Error);
    }
    catch (...)
    {
        Channel().LogMessage(L"Non-standard error while deleting log files",
                             Windows::Foundation::Diagnostics::LoggingLevel::Error);
    }
}
