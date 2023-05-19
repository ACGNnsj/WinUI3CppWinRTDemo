#include "pch.h"

namespace winrt::OCR
{
    hstring StringHelper::to_hex_digit(unsigned char value)
    {
        value &= 0xF;
        if (value < 10)
        {
            return hstring({static_cast<wchar_t>(L'0' + value)});
        }
        return hstring({static_cast<wchar_t>(L'A' + (value - 10))});
    }

    hstring StringHelper::to_hex_hstring(long long value)
    {
        hstring hex;
        for (int i = 0; i < 16; i++)
        {
            const char digit = value & 0xF;
            value >>= 4;
            hex = to_hex_digit(digit) + hex;
        }
        return L"0x" + hex;
    }

    hstring StringHelper::GetCurrentDirectory()
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileName(nullptr, exePath, MAX_PATH);
        wchar_t driveName[4];
        wchar_t subDir[MAX_PATH];
        wchar_t fileName[MAX_PATH];
        wchar_t externInfo[MAX_PATH];
        _wsplitpath_s(exePath, driveName, subDir, fileName, externInfo);
        hstring currentDirectory = driveName + hstring(subDir);
        return currentDirectory;
    }
}
