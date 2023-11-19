/*
** audio.h
**
** This file is part of mkxp.
**
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUDIO_H
#define AUDIO_H

/* Concerning the 'pos' parameter:
 *   RGSS3 actually doesn't specify a format for this,
 *   it's only implied that it is a numerical value
 *   (must be 0 on invalid cases), and it's not used for
 *   anything outside passing it back into bgm/bgs_play.
 *   We use this freedom to define pos to be a float,
 *   in seconds of playback. (RGSS3 seems to use large
 *   integers that _look_ like sample offsets but I can't
 *   quite make out their meaning yet) */
#include "audio.h"

#include <memory>

struct AlAudioPrivate;
struct RGSSThreadData;

class AlAudio : public Audio
{
public:
	void bgmPlay(const char *filename,
	             int volume = 100,
	             int pitch = 100,
	             float pos = 0,
                 int track = -127) override;
	void bgmStop(int track = -127) override;
	void bgmFade(int time, int track = -127) override;
    int bgmGetVolume(int track = -127) override;
    void bgmSetVolume(int volume = 100, int track = -127) override;

	void bgsPlay(const char *filename,
	             int volume = 100,
	             int pitch = 100,
	             float pos = 0) override;
	void bgsStop() override;
	void bgsFade(int time) override;

	void mePlay(const char *filename,
	            int volume = 100,
	            int pitch = 100) override;
	void meStop() override;
	void meFade(int time) override;

	void sePlay(const char *filename,
	            int volume = 100,
	            int pitch = 100) override;
	void seStop() override;

	void setupMidi() override;
	float bgmPos(int track = 0) override;
	float bgsPos() override;

	void reset() override;

private:
    explicit AlAudio(RGSSThreadData &rtData);
	~AlAudio() override;

	friend struct SharedStatePrivate;

	std::unique_ptr<AlAudioPrivate> p;
};

#endif // AUDIO_H
