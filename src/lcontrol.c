int LUI_EXP l_uiControlDestroy(lua_State *L)
{
  uiControl* c = UI_CHECK_OBJECT(1, Control);
  uiControlDestroy(c);

  // destroy registry meta table
  lua_pushlightuserdata(L, c);
  lua_pushnil(L);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 0;
}

int LUI_EXP l_uiControlHandle(lua_State *L)
{
  lua_pushinteger(L, uiControlHandle(UI_CHECK_OBJECT(1, Control)));
  return 1;
}

int LUI_EXP l_uiControlParent(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    UI_CREATE_OBJECT(Control, uiControlParent(UI_CHECK_OBJECT(1, Control)));
    return 1;
  }
  uiControlSetParent(UI_CHECK_OBJECT(1, Control), UI_CHECK_OBJECT(2, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiControlToplevel(lua_State *L)
{
  lua_pushinteger(L, uiControlToplevel(UI_CHECK_OBJECT(1, Control)));
  return 1;
}

int LUI_EXP l_uiControlVisible(lua_State *L)
{
  lua_pushboolean(L, uiControlVisible(UI_CHECK_OBJECT(1, Control)));
  return 1;
}

int LUI_EXP l_uiControlShow(lua_State *L)
{
  uiControlShow(UI_CHECK_OBJECT(1, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiControlHide(lua_State *L)
{
  uiControlHide(UI_CHECK_OBJECT(1, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiControlEnabled(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabled(UI_CHECK_OBJECT(1, Control)));
  return 1;
}

int LUI_EXP l_uiControlEnable(lua_State *L)
{
  uiControlEnable(UI_CHECK_OBJECT(1, Control));
  UI_RETURN_SELF;
}
int LUI_EXP l_uiControlDisable(lua_State *L)
{
  uiControlDisable(UI_CHECK_OBJECT(1, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiControlVerifySetParent(lua_State *L)
{
  uiControlVerifySetParent(UI_CHECK_OBJECT(1, Control), UI_CHECK_OBJECT(2, Control));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiControlEnabledToUser(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabledToUser(UI_CHECK_OBJECT(1, Control)));
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

