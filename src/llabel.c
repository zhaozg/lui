static int l_uiNewLabel(lua_State *L)
{
  UI_CREATE_OBJECT(Label, uiNewLabel(luaL_checkstring(L, 1)));
  return 1;
}

static int l_uiLabelText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiLabelText(UI_CHECK_OBJECT(1, Label)));
    return 1;
  }
  uiLabelSetText(UI_CHECK_OBJECT(1, Label), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}


static struct luaL_Reg meta_Label[] =
{
  { "Text",                 l_uiLabelText },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
