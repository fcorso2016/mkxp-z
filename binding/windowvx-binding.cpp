/*
** window-binding.cpp
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

#include "binding-util.h"
#include "disposable-binding.h"
#include "viewportelement-binding.h"
#include "windowvx.h"

#include "bitmap.h"
#include "graphics.h"

#if RAPI_FULL > 187
DEF_TYPE_CUSTOMNAME(WindowVX, "Window");
#else
DEF_ALLOCFUNC(WindowVX);
#endif

void bitmapInitProps(Bitmap *b, VALUE self);

RB_METHOD(windowVXInitialize) {
  WindowVX *w;

    GFX_LOCK;
  if (rgssVer >= 3) {
    int x = 0;
      int y = 0;
      int width = 0;
      int height = 0;

    if (argc == 4)
      rb_get_args(argc, argv, "iiii", &x, &y, &width, &height RB_ARG_END);

    w = initInstance<WindowVX>(x, y, width, height);
  } else {
    w = viewportElementInitialize<WindowVX>(argc, argv, self);
  }

  setPrivateData(self, w);

  w->initDynAttribs();

  wrapProperty(self, &w->getCursorRect(), "cursor_rect", RectType);

  if (rgssVer >= 3)
    wrapProperty(self, &w->getTone(), "tone", ToneType);

    auto contents = initInstance<Bitmap>(1, 1);
  VALUE contentsObj = wrapObject(contents, BitmapType);
  bitmapInitProps(contents, contentsObj);
  rb_iv_set(self, "contents", contentsObj);

    GFX_UNLOCK;
  return self;
}

RB_METHOD(windowVXUpdate) {
  RB_UNUSED_PARAM

  WindowVX *w = getPrivateData<WindowVX>(self);

    GFX_LOCK;
  w->update();
    GFX_UNLOCK;

  return Qnil;
}

RB_METHOD(windowVXMove) {
  WindowVX *w = getPrivateData<WindowVX>(self);

  int x;
    int y;
    int width;
    int height;
  rb_get_args(argc, argv, "iiii", &x, &y, &width, &height RB_ARG_END);

    GFX_LOCK;
  w->move(x, y, width, height);
    GFX_UNLOCK;
    
  return Qnil;
}

RB_METHOD(windowVXIsOpen) {
  RB_UNUSED_PARAM

  const WindowVX *w = getPrivateData<WindowVX>(self);

  return rb_bool_new(w->isOpen());
}

RB_METHOD(windowVXIsClosed) {
  RB_UNUSED_PARAM

  const WindowVX *w = getPrivateData<WindowVX>(self);

  return rb_bool_new(w->isClosed());
}

DEF_GFX_PROP_OBJ_REF(WindowVX, Bitmap, Windowskin, "windowskin")
DEF_GFX_PROP_OBJ_REF(WindowVX, Bitmap, Contents, "contents")

DEF_GFX_PROP_OBJ_VAL(WindowVX, Rect, CursorRect, "cursor_rect")
DEF_GFX_PROP_OBJ_VAL(WindowVX, Tone, Tone, "tone")

DEF_GFX_PROP_I(WindowVX, X)
DEF_GFX_PROP_I(WindowVX, Y)
DEF_GFX_PROP_I(WindowVX, OX)
DEF_GFX_PROP_I(WindowVX, OY)
DEF_GFX_PROP_I(WindowVX, Width)
DEF_GFX_PROP_I(WindowVX, Height)
DEF_GFX_PROP_I(WindowVX, Padding)
DEF_GFX_PROP_I(WindowVX, PaddingBottom)
DEF_GFX_PROP_I(WindowVX, Opacity)
DEF_GFX_PROP_I(WindowVX, BackOpacity)
DEF_GFX_PROP_I(WindowVX, ContentsOpacity)
DEF_GFX_PROP_I(WindowVX, Openness)

DEF_GFX_PROP_B(WindowVX, Active)
DEF_GFX_PROP_B(WindowVX, ArrowsVisible)
DEF_GFX_PROP_B(WindowVX, Pause)

void windowVXBindingInit() {
  VALUE klass = rb_define_class("Window", rb_cObject);
#if RAPI_FULL > 187
  rb_define_alloc_func(klass, classAllocate<&WindowVXType>);
#else
  rb_define_alloc_func(klass, WindowVXAllocate);
#endif

  disposableBindingInit<WindowVX>(klass);
  viewportElementBindingInit<WindowVX>(klass);

  _rb_define_method(klass, "initialize", windowVXInitialize);
  _rb_define_method(klass, "update", windowVXUpdate);

  INIT_PROP_BIND(WindowVX, Windowskin, "windowskin")
  INIT_PROP_BIND(WindowVX, Contents, "contents")
  INIT_PROP_BIND(WindowVX, CursorRect, "cursor_rect")
  INIT_PROP_BIND(WindowVX, Active, "active")
  INIT_PROP_BIND(WindowVX, Pause, "pause")
  INIT_PROP_BIND(WindowVX, X, "x")
  INIT_PROP_BIND(WindowVX, Y, "y")
  INIT_PROP_BIND(WindowVX, Width, "width")
  INIT_PROP_BIND(WindowVX, Height, "height")
  INIT_PROP_BIND(WindowVX, OX, "ox")
  INIT_PROP_BIND(WindowVX, OY, "oy")
  INIT_PROP_BIND(WindowVX, Opacity, "opacity")
  INIT_PROP_BIND(WindowVX, BackOpacity, "back_opacity")
  INIT_PROP_BIND(WindowVX, ContentsOpacity, "contents_opacity")
  INIT_PROP_BIND(WindowVX, Openness, "openness")

  if (rgssVer >= 3) {
    _rb_define_method(klass, "move", windowVXMove);
    _rb_define_method(klass, "open?", windowVXIsOpen);
    _rb_define_method(klass, "close?", windowVXIsClosed);

    INIT_PROP_BIND(WindowVX, ArrowsVisible, "arrows_visible")
    INIT_PROP_BIND(WindowVX, Padding, "padding")
    INIT_PROP_BIND(WindowVX, PaddingBottom, "padding_bottom")
    INIT_PROP_BIND(WindowVX, Tone, "tone")
  }
}
