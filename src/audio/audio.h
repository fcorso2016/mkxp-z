// ====================================================================================================================
// Created by Retro & Chill on 11/18/2023.
// ------------------------------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the “Software”), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
// permit persons to whom the Software is furnished to do so.
// ====================================================================================================================

#pragma once


class Audio {
public:
    virtual ~Audio() = default;

    virtual void bgmPlay(const char *filename,
                 int volume = 100,
                 int pitch = 100,
                 float pos = 0,
                 int track = -127) = 0;

    virtual void bgmStop(int track = -127) = 0;

    virtual void bgmFade(int time, int track = -127) = 0;

    virtual int bgmGetVolume(int track = -127) = 0;

    virtual void bgmSetVolume(int volume = 100, int track = -127) = 0;

    virtual void bgsPlay(const char *filename,
                 int volume = 100,
                 int pitch = 100,
                 float pos = 0) = 0;

    virtual void bgsStop() = 0;

    virtual void bgsFade(int time) = 0;

    virtual void mePlay(const char *filename,
                int volume = 100,
                int pitch = 100) = 0;

    virtual void meStop() = 0;

    virtual void meFade(int time) = 0;

    virtual void sePlay(const char *filename,
                int volume = 100,
                int pitch = 100) = 0;

    virtual void seStop() = 0;

    virtual void setupMidi() = 0;

    virtual float bgmPos(int track = 0) = 0;

    virtual float bgsPos() = 0;

    virtual void reset() = 0;
};
