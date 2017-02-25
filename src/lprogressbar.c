static int l_uiNewProgressBar(lua_State *L)
{
  CREATE_OBJECT(ProgressBar, uiNewProgressBar());
  return 1;
}

static int l_uiProgressBarValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiProgressBarValue(CAST_ARG(1, ProgressBar)));
  }
  uiProgressBarSetValue(CAST_ARG(1, ProgressBar), luaL_checkint(L, 2));
  RETURN_SELF;
}

static struct luaL_Reg meta_ProgressBar[] =
{
  { "Value",             l_uiProgressBarValue },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
