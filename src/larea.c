static int l_uiNewArea(lua_State *L)
{
  uiAreaHandler *ah = CAST_DRAW_ARG(1, AreaHandler);
  uiArea *a = uiNewArea(ah);
  CREATE_OBJECT(Area, a);
  lua_pushlightuserdata(L, a);
  lua_pushvalue(L, 1);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 1;
}

static int l_uiAreaSetSize(lua_State *L)
{
  uiAreaSetSize(CAST_ARG(1, Area), luaL_checkint(L, 2), luaL_checkint(L, 3));
  RETURN_SELF;
}

static int l_uiAreaQueueRedrawAll(lua_State *L)
{
  uiAreaQueueRedrawAll(CAST_ARG(1, Area));
  RETURN_SELF;
}

static int l_uiAreaScrollTo(lua_State *L)
{
  uiAreaScrollTo(CAST_ARG(1, Area),
                 luaL_checknumber(L, 2),
                 luaL_checknumber(L, 3),
                 luaL_checknumber(L, 4),
                 luaL_checknumber(L, 5));
  RETURN_SELF;
}

static int l_uiAreaBeginUserWindowMove(lua_State *L)
{
  uiAreaBeginUserWindowMove(CAST_ARG(1, Area));
  RETURN_SELF;
}

static int l_uiAreaBeginUserWindowResize(lua_State *L)
{
  uiAreaBeginUserWindowResize(CAST_ARG(1, Area), luaL_checkint(L, 2));
  RETURN_SELF;
}

static struct luaL_Reg meta_Area[] =
{
  { "SetSize",                l_uiAreaSetSize },
  { "QueueRedrawAll",         l_uiAreaQueueRedrawAll },
  { "ScrollTo",               l_uiAreaScrollTo },
  { "BeginUserWindowMove",    l_uiAreaBeginUserWindowMove },
  { "BeginUserWindowResize",  l_uiAreaBeginUserWindowResize },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
