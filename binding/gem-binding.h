//
// Created by fcors on 11/8/2023.
//
#pragma once

#if defined(_WIN32)
    #define MKXPZ_GEM_EXPORT  __declspec(dllexport)
#else
    #if defined(__GNUC__) && __GNUC__ >= 4
        #define MKXPZ_GEM_EXPORT  __attribute__((visibility("default")))
    #else
        #define MKXPZ_GEM_EXPORT
    #endif
#endif

#include <memory>
#include <vector>
#include <string>
#include <thread>

struct ALCcontext;
struct RGSSThreadData;

using ALCcontextPtr = std::unique_ptr<ALCcontext, void (*)(ALCcontext *)>;

class GemBinding {
private:
    GemBinding();

    ~GemBinding();

public:
    static GemBinding &getInstance();

    void stopEventThread();

    void startEventThread(std::string &&windowName, std::vector<std::string> &&args, bool windowVisible);

private:
    void runEventThread();

public:
    inline bool isEventThreadKilled() const {
        return eventThreadKilled;
    }

    inline void setAlcContext(ALCcontextPtr &&ctx) {
        alcCtx = std::move(ctx);
    }

    inline void clearAlcContext() {
        alcCtx.reset();
    }

private:
    std::string rgssWindowName;
    std::vector<std::string> eventThreadArgs;
    bool showWindow = false;

    std::unique_ptr<std::jthread> eventThread;
    ALCcontextPtr alcCtx;
    bool eventThreadKilled = false;
};