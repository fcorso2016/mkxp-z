// ====================================================================================================================
// Created by Retro & Chill on 11/18/2023.
// ------------------------------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the “Software”), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
// permit persons to whom the Software is furnished to do so.
// ====================================================================================================================
#pragma once

#include "graphics.h"

#include <memory>

class Scene;
class Bitmap;
class Disposable;
struct RGSSThreadData;
struct OpenGlGraphicsPrivate;
struct AtomicFlag;
struct THEORAPLAY_VideoFrame;
struct Movie;

class OpenGlGraphics : public Graphics
{
public:
    double getDelta() override;
    double lastUpdate() override;

    void update(bool checkForShutdown = true) override;
    void freeze() override;
    void transition(int duration = 8,
                    const char *filename = "",
                    int vague = 40) override;
    void frameReset() override;

    DECL_ATTR_OVRD( FrameRate,  int )
    DECL_ATTR_OVRD( FrameCount, int )
    DECL_ATTR_OVRD( Brightness, int )

    void wait(int duration) override;
    void fadeout(int duration) override;
    void fadein(int duration) override;

    Bitmap *snapToBitmap() override;

    int width() const override;
    int height() const override;
    int widthHires() const override;
    int heightHires() const override;
    bool isPingPongFramebufferActive() const override;
    int displayContentWidth() const override;
    int displayContentHeight() const override;
    int displayWidth() const override;
    int displayHeight() const override;
    void resizeScreen(int width, int height) override;
    void resizeWindow(int width, int height, bool center=false) override;
    void drawMovieFrame(const THEORAPLAY_VideoFrame* video, Bitmap *videoBitmap) override;
    bool updateMovieInput(Movie *movie) override;
    void playMovie(const char *filename, int volume, bool skippable) override;
    void screenshot(const char *filename) override;

    void reset() override;
    void center() override;

    /* Non-standard extension */
    DECL_ATTR_OVRD( Fullscreen, bool )
    DECL_ATTR_OVRD( ShowCursor, bool )
    DECL_ATTR_OVRD( Scale,    double )
    DECL_ATTR_OVRD( Frameskip, bool )
    DECL_ATTR_OVRD( FixedAspectRatio, bool )
    DECL_ATTR_OVRD( SmoothScaling, int )
    DECL_ATTR_OVRD( IntegerScaling, bool )
    DECL_ATTR_OVRD( LastMileScaling, bool )
    DECL_ATTR_OVRD( Threadsafe, bool )
    double averageFrameRate() override;

    /* <internal> */
    Scene *getScreen() const override;
    /* Repaint screen with static image until exitCond
     * is set. Observes reset flag on top of shutdown
     * if "checkReset" */
    void repaintWait(const AtomicFlag &exitCond,
                     bool checkReset = true) override;

    void lock(bool force = false) override;
    void unlock(bool force = false) override;

private:
    explicit OpenGlGraphics(RGSSThreadData *data);
    ~OpenGlGraphics() override;

    void addDisposable(Disposable *) override;
    void remDisposable(Disposable *) override;

    friend struct SharedStatePrivate;

    std::unique_ptr<OpenGlGraphicsPrivate> p;
};