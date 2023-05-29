﻿#include "pch.h"

namespace winrt::OCR
{
    hstring TranslateHelper::Translate(const char* text, const char* from, const char* to)
    {
        static auto count = 0;
        if (count == 0)
        {
            const auto localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
            const auto pyHome = unbox_value_or(localSettings.Values().TryLookup(L"pyHome"), L"");
            const auto sitePackages = unbox_value_or(localSettings.Values().TryLookup(L"sitePackages"), L"");
            if (pyHome.empty() || sitePackages.empty())
            {
                return L"pyHome or sitePackages is empty";
            }
            const auto sysPath = std::format(L"{}translator;{}\\DLLS;{}\\lib;{};", StringHelper::GetCurrentDirectory(),
                                             pyHome, pyHome, sitePackages);
            Py_SetPath(sysPath.c_str());
            /*Py_SetPath(L"D:\\visualstudio\\OCRTranslator\\translator;"
                // "C:\\Users\\ootop\\AppData\\Local\\Programs\\Python\\Python310\\python310.zip;"
                "C:\\Users\\ootop\\AppData\\Local\\Programs\\Python\\Python310\\DLLs;"
                "C:\\Users\\ootop\\AppData\\Local\\Programs\\Python\\Python310\\lib;"
                // "C:\\Users\\ootop\\AppData\\Local\\Programs\\Python\\Python310;"
                // "D:\\visualstudio\\OCRTranslator\\translator\\.venv;"
                "D:\\visualstudio\\OCRTranslator\\translator\\.venv\\lib\\site-packages;");*/
            Py_Initialize();
        }
        const PyGILState_STATE gstate = PyGILState_Ensure();
        PyObject* pName = PyUnicode_FromWideChar(L"translate_wrapper", -1);
        if (pName == nullptr)
        {
            return L"pName is nullptr";
        }
        PyObject* pModule = PyImport_Import(pName);
        if (pModule == nullptr)
        {
            // WindowHelper::OpenMessageWindow(L"pModule is nullptr" /*+ to_hstring(sysPath.c_str())*/);
            return L"pModule is nullptr";
        }
        PyObject* pDict = PyModule_GetDict(pModule);
        if (pDict == nullptr)
        {
            return L"pDict is nullptr";
        }
        PyObject* pFunc = PyDict_GetItemString(pDict, "translate");
        if (pFunc == nullptr)
        {
            return L"pFunc is nullptr";
        }
        //PyObject* pResult = PyObject_CallFunction(pFunc, "sss", "ja", "en", "おはようございます");
        /*PyObject* pResult = PyObject_CallFunctionObjArgs(pFunc, PyUnicode_FromWideChar(L"ja", -1),
                                                         PyUnicode_FromWideChar(L"en", -1),
                                                         PyUnicode_FromWideChar(L"おはようございます", -1));*/
        PyObject* pResult = PyObject_CallFunction(pFunc, "sss", from, to, text);
        if (nullptr == pResult)
        {
            return L"pResult is nullptr\ntext: " + to_hstring(text);
        }
        // WindowHelper::OpenMessageWindow(L"from: " + to_hstring(from) + L" to: " + to_hstring(to));
        const auto pout = PyUnicode_AsWideCharString(pResult, nullptr);
        PyGILState_Release(gstate);
        ++count;
        return pout;
    }

    Windows::Foundation::IAsyncAction TranslateHelper::TranslateAsync(hstring text, hstring from, hstring to)
    {
        auto static count = 0;
        if (count == 0)
        {
            co_await resume_background();
        }
        static apartment_context worker_thread;
        if (count > 0)
        {
            co_await worker_thread;
        }
        const auto translatedText = Translate(to_string(text).c_str(), to_string(from).substr(0, 2).c_str(),
                                              to_string(to).substr(0, 2).c_str());
        ++count;
        WindowHelper::OpenMessageWindow(WindowManager::rawText + L"\n\n" + translatedText, L"Result");
    }

    void TranslateHelper::TranslateInNewThread()
    {
        auto t = std::jthread([]
        {
            WindowManager::isThreadRunning = true;
            const auto translatedText = Translate(to_string(WindowManager::rawText).c_str(),
                                                  to_string(WindowManager::sourceLanguageTag).substr(0, 2).
                                                  c_str(),
                                                  to_string(WindowManager::targetLanguageTag).substr(0, 2).
                                                  c_str());
            WindowHelper::OpenMessageWindow(WindowManager::rawText + L"\n\n" + translatedText, L"Result");
            WindowManager::isThreadRunning = false;
        });
        t.detach();
    }

    void TranslateHelper::CreateThread()
    {
        auto t = std::jthread([]
        {
            while (true)
            {
                if (WindowManager::hasTaskToRun)
                {
                    WindowManager::isThreadRunning = true;
                    const auto translatedText = Translate(to_string(WindowManager::rawText).c_str(),
                                                          to_string(WindowManager::sourceLanguageTag).substr(0, 2).
                                                          c_str(),
                                                          to_string(WindowManager::targetLanguageTag).substr(0, 2).
                                                          c_str());
                    WindowHelper::OpenMessageWindow(WindowManager::rawText + L"\n\n" + translatedText, L"Result");
                    WindowManager::isThreadRunning = false;
                    WindowManager::hasTaskToRun = false;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
        });
        t.detach();
    }
}
