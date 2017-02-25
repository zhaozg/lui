static int l_uiNewMenu(lua_State *L)
{
  CREATE_OBJECT(Menu, uiNewMenu(luaL_checkstring(L, 1)));
  return 1;
}

static int l_uiMenuAppendItem(lua_State *L)
{
  CREATE_OBJECT(MenuItem, uiMenuAppendItem(CAST_ARG(1, Menu), luaL_checkstring(L, 2)));
  return 1;
}

static int l_uiMenuAppendCheckItem(lua_State *L)
{
  CREATE_OBJECT(MenuItem, uiMenuAppendCheckItem(CAST_ARG(1, Menu), luaL_checkstring(L, 2)));
  return 1;
}

static int l_uiMenuAppendQuitItem(lua_State *L)
{
  CREATE_OBJECT(MenuItem, uiMenuAppendQuitItem(CAST_ARG(1, Menu)));
  return 1;
}

static int l_uiMenuAppendPreferencesItem(lua_State *L)
{
  CREATE_OBJECT(MenuItem, uiMenuAppendPreferencesItem(CAST_ARG(1, Menu)));
  return 1;
}

static int l_uiMenuAppendAboutItem(lua_State *L)
{
  CREATE_OBJECT(MenuItem, uiMenuAppendAboutItem(CAST_ARG(1, Menu)));
  return 1;
}

static int l_uiMenuAppendSeparator(lua_State *L)
{
  uiMenuAppendSeparator(CAST_ARG(1, Menu));
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
