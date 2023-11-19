/*
** keybindings.cpp
**
** This file is part of mkxp.
**
** Copyright (C) 2014 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
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

#include "keybindings.h"

#include "config.h"
#include "util.h"

#include <stdio.h>

struct KbBindingData
{
	SDL_Scancode source;
	Input::ButtonCode target;

	void add(BDescVec &d) const
	{
		SourceDesc src;
		src.type = Key;
		src.d.scan = source;

		BindingDesc desc;
		desc.src = src;
		desc.target = target;

		d.push_back(desc);
	}
};

struct CtrlBindingData
{
    SDL_GameControllerButton source;
    Input::ButtonCode target;
    
    void add(BDescVec &d) const
    {
        SourceDesc src;
        src.type = CButton;
        src.d.cb = source;
        
        BindingDesc desc;
        desc.src = src;
        desc.target = target;
        
        d.push_back(desc);
    }
};

/* Common */
static const KbBindingData defaultKbBindings[] =
{
	{ SDL_SCANCODE_LEFT,   Input::ButtonCode::Left  },
	{ SDL_SCANCODE_RIGHT,  Input::ButtonCode::Right },
	{ SDL_SCANCODE_UP,     Input::ButtonCode::Up    },
	{ SDL_SCANCODE_DOWN,   Input::ButtonCode::Down  },
    
	{ SDL_SCANCODE_SPACE,  Input::ButtonCode::C     },
	{ SDL_SCANCODE_RETURN, Input::ButtonCode::C     },
	{ SDL_SCANCODE_ESCAPE, Input::ButtonCode::B     },
	{ SDL_SCANCODE_KP_0,   Input::ButtonCode::B     },
	{ SDL_SCANCODE_LSHIFT, Input::ButtonCode::A     },
	{ SDL_SCANCODE_X,      Input::ButtonCode::B     },
	{ SDL_SCANCODE_D,      Input::ButtonCode::Z     },
	{ SDL_SCANCODE_Q,      Input::ButtonCode::L     },
	{ SDL_SCANCODE_W,      Input::ButtonCode::R     },
	{ SDL_SCANCODE_A,      Input::ButtonCode::X     },
	{ SDL_SCANCODE_S,      Input::ButtonCode::Y     }
};

/* RGSS1 */
static const KbBindingData defaultKbBindings1[] =
{
	{ SDL_SCANCODE_Z,      Input::ButtonCode::A     },
	{ SDL_SCANCODE_C,      Input::ButtonCode::C     },
};

/* RGSS2 and higher */
static const KbBindingData defaultKbBindings2[] =
{
	{ SDL_SCANCODE_Z,      Input::ButtonCode::C     }
};

static elementsN(defaultKbBindings);
static elementsN(defaultKbBindings1);
static elementsN(defaultKbBindings2);

static const CtrlBindingData defaultCtrlBindings[] =
{
	{ SDL_CONTROLLER_BUTTON_X, Input::ButtonCode::A  },
	{ SDL_CONTROLLER_BUTTON_B, Input::ButtonCode::B  },
	{ SDL_CONTROLLER_BUTTON_A, Input::ButtonCode::C },
	{ SDL_CONTROLLER_BUTTON_Y, Input::ButtonCode::X  },
	{ SDL_CONTROLLER_BUTTON_LEFTSTICK, Input::ButtonCode::Y  },
	{ SDL_CONTROLLER_BUTTON_RIGHTSTICK, Input::ButtonCode::Z },
	{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER, Input::ButtonCode::L  },
	{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Input::ButtonCode::R  },
    
    { SDL_CONTROLLER_BUTTON_DPAD_UP, Input::ButtonCode::Up },
    { SDL_CONTROLLER_BUTTON_DPAD_DOWN, Input::ButtonCode::Down },
    { SDL_CONTROLLER_BUTTON_DPAD_LEFT, Input::ButtonCode::Left },
    { SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Input::ButtonCode::Right }
};

static elementsN(defaultCtrlBindings);

static void addAxisBinding(BDescVec &d, SDL_GameControllerAxis axis, AxisDir dir, Input::ButtonCode target)
{
	SourceDesc src;
	src.type = CAxis;
	src.d.ca.axis = axis;
	src.d.ca.dir = dir;

	BindingDesc desc;
	desc.src = src;
	desc.target = target;

	d.push_back(desc);
}

BDescVec genDefaultBindings(const Config &conf)
{
	BDescVec d;

	for (size_t i = 0; i < defaultKbBindingsN; ++i)
		defaultKbBindings[i].add(d);

	if (conf.rgssVersion == 1)
		for (size_t i = 0; i < defaultKbBindings1N; ++i)
			defaultKbBindings1[i].add(d);
	else
		for (size_t i = 0; i < defaultKbBindings2N; ++i)
			defaultKbBindings2[i].add(d);

	for (size_t i = 0; i < defaultCtrlBindingsN; ++i)
		defaultCtrlBindings[i].add(d);

	addAxisBinding(d, SDL_CONTROLLER_AXIS_LEFTX, Negative, Input::ButtonCode::Left );
	addAxisBinding(d, SDL_CONTROLLER_AXIS_LEFTX, Positive, Input::ButtonCode::Right);
	addAxisBinding(d, SDL_CONTROLLER_AXIS_LEFTY, Negative, Input::ButtonCode::Up   );
	addAxisBinding(d, SDL_CONTROLLER_AXIS_LEFTY, Positive, Input::ButtonCode::Down );

	return d;
}

#define FORMAT_VER 3

struct Header
{
	uint32_t formVer;
	uint32_t rgssVer;
	uint32_t count;
};

static void buildPath(const std::string &dir, uint32_t rgssVersion,
                      char *out, size_t outSize)
{
	snprintf(out, outSize, "%skeybindings.mkxp%u", dir.c_str(), rgssVersion);
}

static bool writeBindings(const BDescVec &d, const std::string &dir,
                          uint32_t rgssVersion)
{
	if (dir.empty())
		return false;

	char path[1024];
	buildPath(dir, rgssVersion, path, sizeof(path));

	FILE *f = fopen(path, "wb");

	if (!f)
		return false;

	Header hd;
	hd.formVer = FORMAT_VER;
	hd.rgssVer = rgssVersion;
	hd.count = d.size();

	if (fwrite(&hd, sizeof(hd), 1, f) < 1)
	{
		fclose(f);
		return false;
	}

	if (fwrite(&d[0], sizeof(d[0]), hd.count, f) < hd.count)
	{
		fclose(f);
		return false;
	}

	fclose(f);
	return true;
}

void storeBindings(const BDescVec &d, const Config &conf)
{
    writeBindings(d, conf.customDataPath, conf.rgssVersion);
}

#define READ(ptr, size, n, f) if (fread(ptr, size, n, f) < n) return false

static bool verifyDesc(const BindingDesc &desc)
{
	const Input::ButtonCode codes[] =
	{
	    Input::ButtonCode::None,
	    Input::ButtonCode::Down, Input::ButtonCode::Left, Input::ButtonCode::Right, Input::ButtonCode::Up,
	    Input::ButtonCode::A, Input::ButtonCode::B, Input::ButtonCode::C,
	    Input::ButtonCode::X, Input::ButtonCode::Y, Input::ButtonCode::Z,
	    Input::ButtonCode::L, Input::ButtonCode::R,
	    Input::ButtonCode::Shift, Input::ButtonCode::Ctrl, Input::ButtonCode::Alt,
	    Input::ButtonCode::F5, Input::ButtonCode::F6, Input::ButtonCode::F7, Input::ButtonCode::F8, Input::ButtonCode::F9
	};

	elementsN(codes);
	size_t i;

	for (i = 0; i < codesN; ++i)
		if (desc.target == codes[i])
			break;

	if (i == codesN)
		return false;

	const SourceDesc &src = desc.src;

	switch (src.type)
	{
	case Invalid:
		return true;
	case Key:
		return src.d.scan < SDL_NUM_SCANCODES;
            
    case CButton:
        return true;

	case CAxis:
		return src.d.ca.dir == Negative || src.d.ca.dir == Positive;
	default:
		return false;
	}
}

static bool readBindings(BDescVec &out, const std::string &dir,
                         uint32_t rgssVersion)
{
	if (dir.empty())
		return false;

	char path[1024];
	buildPath(dir, rgssVersion, path, sizeof(path));

	FILE *f = fopen(path, "rb");

	if (!f)
		return false;

	Header hd;
	if (fread(&hd, sizeof(hd), 1, f) < 1)
	{
		fclose(f);
		return false;
	}

	if (hd.formVer != FORMAT_VER)
		return false;
	if (hd.rgssVer != rgssVersion)
		return false;
	/* Arbitrary max value */
	if (hd.count > 1024)
		return false;

	out.resize(hd.count);
	if (fread(&out[0], sizeof(out[0]), hd.count, f) < hd.count)
	{
		fclose(f);
		return false;
	}

	for (size_t i = 0; i < hd.count; ++i)
		if (!verifyDesc(out[i]))
			return false;

	return true;
}

BDescVec loadBindings(const Config &conf)
{
	BDescVec d;

	if (readBindings(d, conf.customDataPath, conf.rgssVersion))
		return d;

	return genDefaultBindings(conf);
}
