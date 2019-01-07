static int l_uiControlDestroy(lua_State *L)
{
  struct wrap *w = NULL;
  uiControl *c = NULL;

  if(auxiliar_getclassudata(L, "libui.Control", 1))
    w = (struct wrap*)lua_touserdata(L, 1);
  else
    w = (struct wrap*)auxiliar_checkgroup(L, "libui.Control", 1);

  c = w->control;
  if(c!=NULL && w->ref)
    uiControlDestroy(c);
  w->control = NULL;

  lua_pushnil(L);
  lua_setmetatable(L, 1);
  return 0;
}

static uiControl* l_checkControl(lua_State *L, int i)
{
  struct wrap *w = NULL;

  if(auxiliar_getclassudata(L, "libui.Control", i))
    w = (struct wrap*)lua_touserdata(L, i);
  else
    w = (struct wrap*)auxiliar_checkgroup(L, "libui.Control", i);

  return w->control;
}

static int l_uiControlHandle(lua_State *L)
{
  lua_pushinteger(L, uiControlHandle(l_checkControl(L, 1)));
  return 1;
}

static int l_uiControlParent(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    UI_CREATE_OBJECT(Control, uiControlParent(l_checkControl(L, 1)));
    return 1;
  }
  uiControlSetParent(l_checkControl(L, 1), l_checkControl(L, 2));
  UI_RETURN_SELF;
}

static int l_uiControlToplevel(lua_State *L)
{
  lua_pushinteger(L, uiControlToplevel(l_checkControl(L, 1)));
  return 1;
}

static int l_uiControlVisible(lua_State *L)
{
  lua_pushboolean(L, uiControlVisible(l_checkControl(L, 1)));
  return 1;
}

static int l_uiControlShow(lua_State *L)
{
  uiControlShow(l_checkControl(L, 1));
  UI_RETURN_SELF;
}

static int l_uiControlHide(lua_State *L)
{
  uiControlHide(l_checkControl(L, 1));
  UI_RETURN_SELF;
}

static int l_uiControlEnabled(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabled(l_checkControl(L, 1)));
  return 1;
}

static int l_uiControlEnable(lua_State *L)
{
  uiControlEnable(l_checkControl(L, 1));
  UI_RETURN_SELF;
}
static int l_uiControlDisable(lua_State *L)
{
  uiControlDisable(l_checkControl(L, 1));
  UI_RETURN_SELF;
}

static int l_uiControlVerifySetParent(lua_State *L)
{
  uiControlVerifySetParent(l_checkControl(L, 1), l_checkControl(L, 2));
  UI_RETURN_SELF;
}

static int l_uiControlEnabledToUser(lua_State *L)
{
  lua_pushboolean(L, uiControlEnabledToUser(l_checkControl(L, 1)));
  return 1;
}

#define CONTROL_COMMON_METAFIELD                  \
{ "Destroy",          l_uiControlDestroy },       \
{ "Handle",           l_uiControlHandle },        \
{ "Parent",           l_uiControlParent },        \
{ "Toplevel",         l_uiControlToplevel },      \
{ "Visible",          l_uiControlVisible },       \
{ "Show",             l_uiControlShow },          \
{ "Hide",             l_uiControlHide },          \
{ "Enabled",          l_uiControlEnabled },       \
{ "Enable",           l_uiControlEnable },        \
{ "Disable",          l_uiControlDisable },       \
{ "EnabledToUser",    l_uiControlEnabledToUser }, \
{ "VerifySetParent",  l_uiControlVerifySetParent },

static struct luaL_Reg meta_Control[] =
{
  CONTROL_COMMON_METAFIELD

  {NULL, NULL}
};
