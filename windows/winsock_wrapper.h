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

#include <winsock.h>

class WinSock : public RaiiWrapper {
public:
    explicit WinSock();
    ~WinSock();

    WinSock(const WinSock &) = delete;
    WinSock(WinSock&&) = delete;

    WinSock &operator=(const WinSock &) = delete;
    WinSock &operator=(WinSock &&) = delete;

private:
    WSAData wsadata = {0};
};
