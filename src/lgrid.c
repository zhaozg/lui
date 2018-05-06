static int l_uiNewGrid(lua_State *L)
{
  UI_CREATE_OBJECT(Grid, uiNewGrid());
  return 1;
}

static int l_uiGridAppend(lua_State *L)
{
  int i;
  int n = lua_gettop(L);
  uiGrid *grid = UI_CHECK_OBJECT(1, Grid);

  for (i = 2; i <= n; i += 9)
  {
    int left = luaL_checkinteger(L, i + 1);
    int top = luaL_checkinteger(L, i + 2);
    int xspan = luaL_checkinteger(L, i + 3);
    int yspan = luaL_checkinteger(L, i + 4);
    int hexpand = luaL_checkboolean(L, i + 5);
    uiAlign halign = luaL_checkinteger(L, i + 6);
    int vexpand = luaL_checkboolean(L, i + 7);
    uiAlign valign = luaL_checkinteger(L, i + 8);
    uiGridAppend(grid, UI_CHECK_OBJECT(i, Control), left, top, xspan, yspan, hexpand, halign, vexpand, valign);

    //copy ltab.c, need check
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  UI_RETURN_SELF;
}

static int l_uiGridInsertAt(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i += 10)
  {
    uiAt at = luaL_checkinteger(L, i + 3);
    int xspan = luaL_checkinteger(L, i + 4);
    int yspan = luaL_checkinteger(L, i + 5);
    int hexpand = luaL_checkboolean(L, i + 6);
    uiAlign halign = luaL_checkinteger(L, i + 7);
    int vexpand = luaL_checkboolean(L, i + 8);
    uiAlign valign = luaL_checkinteger(L, i + 9);
    uiGridInsertAt(UI_CHECK_OBJECT(1, Grid),
                   UI_CHECK_OBJECT(i, Control),
                   UI_CHECK_OBJECT(i + 1, Control),
                   at, xspan, yspan, hexpand, halign, vexpand, valign);

    //copy ltab.c, need check
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  UI_RETURN_SELF;
}

static int l_uiGridPadded(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiGridPadded(UI_CHECK_OBJECT(1, Grid)));
    return 1;
  }
  uiGridSetPadded(UI_CHECK_OBJECT(1, Grid), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Grid[] =
{
  { "Append",               l_uiGridAppend },
  { "InsertAt",             l_uiGridInsertAt },
  { "Padded",               l_uiGridPadded },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
