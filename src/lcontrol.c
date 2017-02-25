static int l_uiControlDestroy(lua_State *L)
{
  uiControl* c = CAST_ARG(1, Control);
  uiControlDestroy(c);

  // destroy registry meta table
  lua_pushlightuserdata(L, c);
  lua_pushnil(L);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 0;
}

static int l_uiControlHandle(lua_State *L)
{
  lua_pushinteger(L, uiControlHandle(CAST_ARG(1, Control)));
  return 1;
}

static int l_uiControlParent(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    CREATE_OBJECT(Control, uiControlParent(CAST_ARG(1, Control)));
    return 1;
  }
  uiControlSetParent(CAST_ARG(1, Control), CAST_ARG(2, Control));
  RETURN_SELF;
}

static int l_uiControlToplevel(lua_State *L)
{
  lua_pushinteger(L, uiControlToplevel(CAST_ARG(1, Control)));
  return 1;
}

static int l_uiControlVisible(lua_State *L)
{
  lua_pushboolean(L, uiControlVisible(CAST_ARG(1, Control)));
  return 1;
}

static int l_uiControlShow(lua_State *L)
{
  uiControlShow(CAST_ARG(1, Control));
  RETURN_SELF;
}

static int l_uiControlHide(lua_State *L)
{
  uiControlHide(CAST_ARG(1, Control));
  RETURN_SELF;
}

static int l_uiControlEnabled(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabled(CAST_ARG(1, Control)));
  return 1;
}

static int l_uiControlEnable(lua_State *L)
{
  uiControlEnable(CAST_ARG(1, Control));
  RETURN_SELF;
}
static int l_uiControlDisable(lua_State *L)
{
  uiControlDisable(CAST_ARG(1, Control));
  RETURN_SELF;
}

static int l_uiControlVerifySetParent(lua_State *L)
{
  uiControlVerifySetParent(CAST_ARG(1, Control), CAST_ARG(2, Control));
  RETURN_SELF;
}

static int l_uiControlEnabledToUser(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabledToUser(CAST_ARG(1, Control)));
  return 1;
}

#define CONTROL_COMMON_METAFIELD          \
{ "Destroy",          l_uiControlDestroy }, \
{ "Handle",           l_uiControlHandle },  \
{ "Parent",           l_uiControlParent },  \
{ "Toplevel",         l_uiControlToplevel },\
{ "Visible",          l_uiControlVisible }, \
{ "Show",             l_uiControlShow },    \
{ "Hide",             l_uiControlHide },    \
{ "Enabled",          l_uiControlEnabled }, \
{ "Enable",           l_uiControlEnable },  \
{ "Disable",          l_uiControlDisable }, \
{ "EnabledToUser",    l_uiControlEnabledToUser }, \
{ "VerifySetParent",  l_uiControlVerifySetParent },

