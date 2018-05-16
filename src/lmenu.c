int LUI_EXP l_uiNewMenu(lua_State *L)
{
  UI_CREATE_OBJECT(Menu, uiNewMenu(luaL_checkstring(L, 1)));
  return 1;
}

int LUI_EXP l_uiMenuAppendItem(lua_State *L)
{
  UI_CREATE_OBJECT(MenuItem, uiMenuAppendItem(UI_CHECK_OBJECT(1, Menu), luaL_checkstring(L, 2)));
  return 1;
}

int LUI_EXP l_uiMenuAppendCheckItem(lua_State *L)
{
  UI_CREATE_OBJECT(MenuItem, uiMenuAppendCheckItem(UI_CHECK_OBJECT(1, Menu), luaL_checkstring(L, 2)));
  return 1;
}

int LUI_EXP l_uiMenuAppendQuitItem(lua_State *L)
{
  UI_CREATE_OBJECT(MenuItem, uiMenuAppendQuitItem(UI_CHECK_OBJECT(1, Menu)));
  return 1;
}

int LUI_EXP l_uiMenuAppendPreferencesItem(lua_State *L)
{
  UI_CREATE_OBJECT(MenuItem, uiMenuAppendPreferencesItem(UI_CHECK_OBJECT(1, Menu)));
  return 1;
}

int LUI_EXP l_uiMenuAppendAboutItem(lua_State *L)
{
  UI_CREATE_OBJECT(MenuItem, uiMenuAppendAboutItem(UI_CHECK_OBJECT(1, Menu)));
  return 1;
}

int LUI_EXP l_uiMenuAppendSeparator(lua_State *L)
{
  uiMenuAppendSeparator(UI_CHECK_OBJECT(1, Menu));
  return 0;
}

static struct luaL_Reg meta_Menu[] =
{
  { "AppendItem",              l_uiMenuAppendItem },
  { "AppendCheckItem",         l_uiMenuAppendCheckItem },
  { "AppendQuitItem",          l_uiMenuAppendQuitItem },
  { "AppendPreferencesItem",   l_uiMenuAppendPreferencesItem },
  { "AppendAboutItem",         l_uiMenuAppendAboutItem },
  { "AppendSeparator",         l_uiMenuAppendSeparator },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
