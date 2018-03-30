static int l_uiNewArea(lua_State *L)
{
  uiAreaHandler *ah = CHECK_USER_OBJECT(1, AreaHandler);
  uiArea *a = NULL;
  if (lua_gettop(L) > 3)
  {
    a = uiNewScrollingArea(ah, luaL_checkint(L, 2), luaL_checkint(L, 3));
  }
  else {
    a = uiNewArea(ah);
  }
  UI_CREATE_OBJECT(Area, a);
  lua_pushlightuserdata(L, a);
  lua_pushvalue(L, 1);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 1;
}

static int l_uiAreaSetSize(lua_State *L)
{
  uiAreaSetSize(UI_CHECK_OBJECT(1, Area), luaL_checkint(L, 2), luaL_checkint(L, 3));
  UI_RETURN_SELF;
}

static int l_uiAreaQueueRedrawAll(lua_State *L)
{
  uiAreaQueueRedrawAll(UI_CHECK_OBJECT(1, Area));
  UI_RETURN_SELF;
}

static int l_uiAreaScrollTo(lua_State *L)
{
  uiAreaScrollTo(UI_CHECK_OBJECT(1, Area),
                 luaL_checknumber(L, 2),
                 luaL_checknumber(L, 3),
                 luaL_checknumber(L, 4),
                 luaL_checknumber(L, 5));
  UI_RETURN_SELF;
}

static int l_uiAreaBeginUserWindowMove(lua_State *L)
{
  uiAreaBeginUserWindowMove(UI_CHECK_OBJECT(1, Area));
  UI_RETURN_SELF;
}

static int l_uiAreaBeginUserWindowResize(lua_State *L)
{
  uiAreaBeginUserWindowResize(UI_CHECK_OBJECT(1, Area), luaL_checkint(L, 2));
  UI_RETURN_SELF;
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
