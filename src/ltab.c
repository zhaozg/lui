static int l_uiNewTab(lua_State *L)
{
  CREATE_OBJECT(Tab, uiNewTab());
  return 1;
}

static int l_uiTabAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i += 2)
  {
    uiTabAppend(CAST_ARG(1, Tab), luaL_checkstring(L, i + 0), CAST_ARG(i + 1, Control));
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  RETURN_SELF;
}

static int l_uiTabInsertAt(lua_State *L)
{
  uiTabInsertAt(CAST_ARG(1, Tab), luaL_checkstring(L, 2), luaL_checkint(L, 3), CAST_ARG(4, Control));
  RETURN_SELF;
}

static int l_uiTabDelete(lua_State *L)
{
  uiTabDelete(CAST_ARG(1, Tab), luaL_checkint(L, 2));
  RETURN_SELF;
}

static int l_uiTabNumPages(lua_State *L)
{
  lua_pushinteger(L, uiTabNumPages(CAST_ARG(1, Tab)));
  return 1;
}

static int L_uiTabMargined(lua_State *L)
{
  if (lua_isnone(L, 3))
  {
    lua_pushboolean(L, uiTabMargined(CAST_ARG(1, Tab), luaL_checkint(L, 2)));
    return 1;
  }
  uiTabSetMargined(CAST_ARG(1, Tab), luaL_checkint(L, 2), luaL_checkboolean(L, 3));
  RETURN_SELF;
}

static struct luaL_Reg meta_Tab[] =
{
  { "Append",               l_uiTabAppend }, 
  { "InsertAt",             l_uiTabInsertAt },
  { "Delete",               l_uiTabDelete },
  { "NumPages",             l_uiTabNumPages },
  { "Margined",             L_uiTabMargined },
  { "__len",                l_uiTabNumPages },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
