#pragma once

#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace System;

ref class SoundManager
{
public:

    static void Play(String^ plik)
    {
        PlaySound(
            plik,
            NULL,
            SND_ASYNC | SND_FILENAME
        );
    }
};