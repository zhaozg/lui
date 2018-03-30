static int onClosing(uiWindow *w, void *data)
{
  uiQuit();
  return 1;
}

static int l_uiNewWindow(lua_State *L)
{
  uiWindow *win = uiNewWindow(
                    luaL_checkstring(L, 1),
                    luaL_checkint(L, 2),
                    luaL_checkint(L, 3),
                    luaL_checkboolean(L, 4)
                  );
  uiWindowOnClosing(win, onClosing, NULL);
  UI_CREATE_OBJECT(Window, win);
  return 1;
}

static int l_uiWindowTitle(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiWindowTitle(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetTitle(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

static int l_uiWindowContentSize(lua_State *L)
{
  int width, height;
  if (lua_isnone(L, 2))
  {
    uiWindowContentSize(UI_CHECK_OBJECT(1, Window), &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
  }
  width = luaL_checkint(L, 2);
  height = luaL_checkint(L, 3);
  uiWindowSetContentSize(UI_CHECK_OBJECT(1, Window), width, height);
  UI_RETURN_SELF;
}

static int l_uiWindowFullscreen(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowFullscreen(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetFullscreen(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static void Window_On_ContentSizeChanged(uiWindow *b, void *data)
{
  callback(data, b);
}

static int l_uiWindowOnContentSizeChanged(lua_State *L)
{
  uiWindowOnContentSizeChanged(UI_CHECK_OBJECT(1, Window), Window_On_ContentSizeChanged, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static int Window_On_Closing(uiWindow *b, void *data)
{
  return callback(data, b);
}

static int l_uiWindowOnClosing(lua_State *L)
{
  uiWindowOnClosing(UI_CHECK_OBJECT(1, Window), Window_On_Closing, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static int l_uiWindowBorderless(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowBorderless(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetBorderless(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static int l_uiWindowSetChild(lua_State *L)
{
  uiWindowSetChild(UI_CHECK_OBJECT(1, Window), UI_CHECK_OBJECT(2, Control));
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_pushboolean(L, 1);
  lua_settable(L, -3);
  UI_RETURN_SELF;
}

static int l_uiWindowMargined(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowMargined(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetMargined(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static int l_uiOpenFile(lua_State* L)
{
  char* f = uiOpenFile(UI_CHECK_OBJECT(1, Window));
  if (f)
  {
    lua_pushstring(L, f);
    uiFreeText(f);
  }
  else
  {
    lua_pushnil(L);
  }
  return 1;
}

static int l_uiSaveFile(lua_State* L)
{
  char* f = uiSaveFile(UI_CHECK_OBJECT(1, Window));
  if (f)
  {
    lua_pushstring(L, f);
    uiFreeText(f);
  }
  else
  {
    lua_pushnil(L);
  }
  return 1;
}

static int l_uiMsgBox(lua_State* L)
{
  uiMsgBox(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2), luaL_checkstring(L, 3));
  return 0;
}

static int l_uiMsgBoxError(lua_State* L)
{
  uiMsgBoxError(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2), luaL_checkstring(L, 3));
  return 0;
}

static struct luaL_Reg meta_Window[] =
{
  { "Title",               l_uiWindowTitle },
  { "ContentSize",         l_uiWindowContentSize },
  { "Fullscreen",          l_uiWindowFullscreen },
  { "OnContentSizeChanged", l_uiWindowOnContentSizeChanged },
  { "OnClosing",           l_uiWindowOnClosing },
  { "Borderless",          l_uiWindowBorderless },
  { "SetChild",            l_uiWindowSetChild },
  { "Margined",            l_uiWindowMargined },

  { "OpenFile",            l_uiOpenFile },
  { "SaveFile",            l_uiSaveFile },
  { "MsgBox",              l_uiMsgBox },
  { "MsgBoxError",         l_uiMsgBoxError },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
