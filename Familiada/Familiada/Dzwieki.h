#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <vcclr.h>

#pragma comment(lib, "winmm.lib")

using namespace System;
using namespace Runtime::InteropServices;

ref class SoundManager
{
public:

    static void Play(String^ plik)
    {
        pin_ptr<const wchar_t> wch =
            PtrToStringChars(plik);

        PlaySoundW(
            wch,
            NULL,
            SND_FILENAME | SND_ASYNC
        );
    }
};