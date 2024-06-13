#pragma once
#include <string>
#include <windows.h>
#include <iostream>
#include <locale>

namespace yunutyEngine
{
    // yunutyUtility에는 함수 정의, 혹은 functor만 들어간다.
    namespace yutility
    {
        inline std::wstring GetWString(const char* arg)
        {
            // Convert std::string to std::wstring using MultiByteToWideChar
            int wstrSize = MultiByteToWideChar(CP_UTF8, 0, arg, -1, nullptr, 0);
            std::wstring wstr(wstrSize - 1, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, arg, -1, &wstr[0], wstrSize);
            return wstr;
        }
        inline std::wstring GetWString(const std::string& arg) { return GetWString(arg.c_str()); }
        inline std::string GetString(const wchar_t* arg)
        {
            // Convert std::wstring to std::string using WideCharToMultiByte
            int strSize = WideCharToMultiByte(CP_UTF8, 0, arg, -1, nullptr, 0, nullptr, nullptr);
            std::string str(strSize - 1, '\0');
            WideCharToMultiByte(CP_UTF8, 0, arg, -1, &str[0], strSize, nullptr, nullptr);
            return str;
        }
        inline std::string GetString(const std::wstring& arg) { return GetString(arg.c_str()); }
    }
}
