int LUI_EXP l_uiNewGroup(lua_State *L)
{
  UI_CREATE_OBJECT(Group, uiNewGroup(luaL_checkstring(L, 1)));
  return 1;
}

int LUI_EXP l_uiGroupTitle(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiGroupTitle(UI_CHECK_OBJECT(1, Group)));
    return 1;
  }
  uiGroupSetTitle(UI_CHECK_OBJECT(1, Group), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiGroupSetChild(lua_State *L)
{
  uiGroupSetChild(UI_CHECK_OBJECT(1, Group), UI_CHECK_OBJECT(2, Control));
  lua_getmetatable(L, 1);
  lua_pushvalue(L, 2);
  lua_pushboolean(L, 1);
  lua_settable(L, -3);
  UI_RETURN_SELF;
}

int LUI_EXP l_uiGroupMargined(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiGroupMargined(UI_CHECK_OBJECT(1, Group)));
    return 1;
  }
  uiGroupSetMargined(UI_CHECK_OBJECT(1, Group), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Group[] =
{
  { "Title",                l_uiGroupTitle },
  { "Margined",             l_uiGroupMargined },
  { "SetChild",             l_uiGroupSetChild },
  CONTROL_COMMON_METAFIELD
  { NULL }
};
