int LUI_EXP l_uiNewTab(lua_State *L)
{
  UI_CREATE_OBJECT(Tab, uiNewTab());
  return 1;
}

int LUI_EXP l_uiTabAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i += 2)
  {
    uiTabAppend(UI_CHECK_OBJECT(1, Tab), luaL_checkstring(L, i + 0), UI_CHECK_OBJECT(i + 1, Control));
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  UI_RETURN_SELF;
}

int LUI_EXP l_uiTabInsertAt(lua_State *L)
{
  uiTabInsertAt(UI_CHECK_OBJECT(1, Tab), luaL_checkstring(L, 2), luaL_checkinteger(L, 3), UI_CHECK_OBJECT(4, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiTabDelete(lua_State *L)
{
  uiTabDelete(UI_CHECK_OBJECT(1, Tab), luaL_checkinteger(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiTabNumPages(lua_State *L)
{
  lua_pushinteger(L, uiTabNumPages(UI_CHECK_OBJECT(1, Tab)));
  return 1;
}

int LUI_EXP L_uiTabMargined(lua_State *L)
{
  if (lua_isnone(L, 3))
  {
    lua_pushboolean(L, uiTabMargined(UI_CHECK_OBJECT(1, Tab), luaL_checkinteger(L, 2)));
    return 1;
  }
  uiTabSetMargined(UI_CHECK_OBJECT(1, Tab), luaL_checkinteger(L, 2), luaL_checkboolean(L, 3));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Tab[] =
{
  { "Append",               l_uiTabAppend },
  { "InsertAt",             l_uiTabInsertAt },
  { "Delete",               l_uiTabDelete },
  { "NumPages",             l_uiTabNumPages },
  { "Margined",             L_uiTabMargined },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
