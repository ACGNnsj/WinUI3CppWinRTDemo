#include "pch.h"

namespace winrt::OCR
{
    hstring TranslateHelper::Translate(const char* text, const char* from, const char* to)
    {
        const auto localSettings = Windows::Storage::ApplicationData::Current().LocalSettings();
        const auto pyHome = unbox_value_or(localSettings.Values().TryLookup(L"pyHome"), L"");
        const auto sitePackages = unbox_value_or(localSettings.Values().TryLookup(L"sitePackages"), L"");
        if (pyHome.empty() || sitePackages.empty())
        {
            WindowHelper::OpenMessageWindow(L"pyHome or sitePackages is empty");
            return L"";
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
        // PyObject* pName = PyUnicode_FromWideChar(L"add", -1);
        PyObject* pName = PyUnicode_FromWideChar(L"translate_wrapper", -1);
        if (pName == nullptr)
        {
            WindowHelper::OpenMessageWindow(L"pName is nullptr");
            return L"";
        }
        PyObject* pModule = PyImport_Import(pName);
        if (pModule == nullptr)
        {
            WindowHelper::OpenMessageWindow(L"pModule is nullptr");
            return L"";
        }
        PyObject* pDict = PyModule_GetDict(pModule);
        if (pDict == nullptr)
        {
            WindowHelper::OpenMessageWindow(L"pDict is nullptr");
            return L"";
        }
        // PyObject* pFunc = PyDict_GetItemString(pDict, "add");
        PyObject* pFunc = PyDict_GetItemString(pDict, "translate");
        if (pFunc == nullptr)
        {
            WindowHelper::OpenMessageWindow(L"pFunc is nullptr");
            return L"";
        }
        // PyObject* pResult = PyObject_CallFunction(pFunc, "ssii", "12345678ABCDEF123456789", "ABCDEF12345678ABCDEF12345678", 16, 10);
        // PyObject* pResult = PyObject_CallFunction(pFunc, "sss", "ja", "en", "おはようございます");
        /*PyObject* pResult = PyObject_CallFunctionObjArgs(pFunc, PyUnicode_FromWideChar(L"ja", -1),
                                                         PyUnicode_FromWideChar(L"en", -1),
                                                         PyUnicode_FromWideChar(L"おはようございます", -1));*/
        PyObject* pResult = PyObject_CallFunction(pFunc, "sss", from, to, text);
        if (pResult == nullptr)
        {
            WindowHelper::OpenMessageWindow(L"pResult is nullptr");
            return L"";
        }
        auto pout = PyUnicode_AsWideCharString(pResult, nullptr);
        WindowHelper::OpenMessageWindow(hstring(L"result: ") + pout);
        Py_Finalize();
        return pout;
    }
}
