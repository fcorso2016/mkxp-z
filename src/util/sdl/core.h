// ====================================================================================================================
// Created by Retro & Chill on 11/18/2023.
// ------------------------------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the “Software”), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
// permit persons to whom the Software is furnished to do so.
// ====================================================================================================================
#pragma once

#include "raiiwrapper.h"

#include <SDL2/SDL.h>

namespace SDL2 {

    class Core : public RaiiWrapper {
    public:
        explicit Core(Uint32 flags) {
            if (SDL_Init(flags) >= 0)
                startupSucceeded();
            else
                startupFailed(std::string("Error initializing SDL: ") + SDL_GetError());
        }
        ~Core() {
            if (startedSuccessfully())
                SDL_Quit();
        }

        Core(const Core &) = delete;
        Core(Core&&) = delete;

        Core &operator=(const Core &) = delete;
        Core &operator=(Core &&) = delete;
    };

} // SDL2
