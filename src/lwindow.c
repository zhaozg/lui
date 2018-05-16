int LUI_EXP onClosing(uiWindow *w, void *data)
{
  uiQuit();
  return 1;
}

int LUI_EXP l_uiNewWindow(lua_State *L)
{
  uiWindow *win = uiNewWindow(
                    luaL_checkstring(L, 1),
                    luaL_checkinteger(L, 2),
                    luaL_checkinteger(L, 3),
                    luaL_checkboolean(L, 4)
                  );
  uiWindowOnClosing(win, onClosing, NULL);
  UI_CREATE_OBJECT(Window, win);
  return 1;
}

int LUI_EXP l_uiWindowTitle(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiWindowTitle(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetTitle(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiWindowContentSize(lua_State *L)
{
  int width, height;
  if (lua_isnone(L, 2))
  {
    uiWindowContentSize(UI_CHECK_OBJECT(1, Window), &width, &height);
    lua_pushinteger(L, width);
    lua_pushinteger(L, height);
    return 2;
  }
  width = luaL_checkinteger(L, 2);
  height = luaL_checkinteger(L, 3);
  uiWindowSetContentSize(UI_CHECK_OBJECT(1, Window), width, height);
  UI_RETURN_SELF;
}

int LUI_EXP l_uiWindowFullscreen(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowFullscreen(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetFullscreen(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

void LUI_EXP Window_On_ContentSizeChanged(uiWindow *b, void *data)
{
  callback(data, b);
}

int LUI_EXP l_uiWindowOnContentSizeChanged(lua_State *L)
{
  uiWindowOnContentSizeChanged(UI_CHECK_OBJECT(1, Window), Window_On_ContentSizeChanged, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

int LUI_EXP Window_On_Closing(uiWindow *b, void *data)
{
  return callback(data, b);
}

int LUI_EXP l_uiWindowOnClosing(lua_State *L)
{
  uiWindowOnClosing(UI_CHECK_OBJECT(1, Window), Window_On_Closing, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

int LUI_EXP l_uiWindowBorderless(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowBorderless(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetBorderless(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiWindowSetChild(lua_State *L)
{
  uiWindowSetChild(UI_CHECK_OBJECT(1, Window), UI_CHECK_OBJECT(2, Control));
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_pushboolean(L, 1);
  lua_settable(L, -3);
  UI_RETURN_SELF;
}

int LUI_EXP l_uiWindowMargined(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiWindowMargined(UI_CHECK_OBJECT(1, Window)));
    return 1;
  }
  uiWindowSetMargined(UI_CHECK_OBJECT(1, Window), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiOpenFile(lua_State* L)
{
#ifdef LUI_MESSAGE_DIALOGS
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
#endif
  return 1;
}

int LUI_EXP l_uiSaveFile(lua_State* L)
{
#ifdef LUI_MESSAGE_DIALOGS
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
#endif
}

int LUI_EXP l_uiMsgBox(lua_State* L)
{
#ifdef LUI_MESSAGE_DIALOGS
  uiMsgBox(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2), luaL_checkstring(L, 3));
#endif
  return 0;
}

int LUI_EXP l_uiMsgBoxError(lua_State* L)
{
#ifdef LUI_MESSAGE_DIALOGS
  uiMsgBoxError(UI_CHECK_OBJECT(1, Window), luaL_checkstring(L, 2), luaL_checkstring(L, 3));
#endif
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
