static int l_uiNewGrid(lua_State *L)
{
  CREATE_OBJECT(Grid, uiNewGrid());
  return 1;
}

static int l_uiGridAppend(lua_State *L)
{
  int i;
  int n = lua_gettop(L);
  uiGrid *grid = CAST_ARG(1, Grid);

  for (i = 2; i <= n; i += 9)
  {
    int left = luaL_checkint(L, i + 1);
    int top = luaL_checkint(L, i + 2);
    int xspan = luaL_checkint(L, i + 3);
    int yspan = luaL_checkint(L, i + 4);
    int hexpand = luaL_checkboolean(L, i + 5);
    uiAlign halign = luaL_checkint(L, i + 6);
    int vexpand = luaL_checkboolean(L, i + 7);
    uiAlign valign = luaL_checkint(L, i + 8);
    uiGridAppend(grid, CAST_ARG(i, Control), left, top, xspan, yspan, hexpand, halign, vexpand, valign);

    //copy ltab.c, need check
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  RETURN_SELF;
}

static int l_uiGridInsertAt(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i += 10)
  {
    uiAt at = luaL_checkint(L, i + 3);
    int xspan = luaL_checkint(L, i + 4);
    int yspan = luaL_checkint(L, i + 5);
    int hexpand = luaL_checkboolean(L, i + 6);
    uiAlign halign = luaL_checkint(L, i + 7);
    int vexpand = luaL_checkboolean(L, i + 8);
    uiAlign valign = luaL_checkint(L, i + 9);
    uiGridInsertAt(CAST_ARG(1, Grid),
                   CAST_ARG(i, Control),
                   CAST_ARG(i + 1, Control),
                   at, xspan, yspan, hexpand, halign, vexpand, valign);

    //copy ltab.c, need check
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  RETURN_SELF;
}

static int l_uiGridPadded(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiGridPadded(CAST_ARG(1, Grid)));
    return 1;
  }
  uiGridSetPadded(CAST_ARG(1, Grid), luaL_checkboolean(L, 2));
  RETURN_SELF;
}

static struct luaL_Reg meta_Grid[] =
{
  { "Append",               l_uiGridAppend },
  { "InsertAt",             l_uiGridInsertAt },
  { "Padded",               l_uiGridPadded },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
