static int l_uiNewHorizontalSeparator(lua_State *L)
{
  CREATE_OBJECT(Separator, uiNewHorizontalSeparator());
  return 1;
}

static int l_uiNewVerticalSeparator(lua_State *L)
{
  CREATE_OBJECT(Separator, uiNewVerticalSeparator());
  return 1;
}

static struct luaL_Reg meta_Separator[] =
{
  CONTROL_COMMON_METAFIELD

  { NULL }
};
