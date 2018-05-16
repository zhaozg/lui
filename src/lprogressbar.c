int LUI_EXP l_uiNewProgressBar(lua_State *L)
{
  UI_CREATE_OBJECT(ProgressBar, uiNewProgressBar());
  return 1;
}

int LUI_EXP l_uiProgressBarValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiProgressBarValue(UI_CHECK_OBJECT(1, ProgressBar)));
  }
  uiProgressBarSetValue(UI_CHECK_OBJECT(1, ProgressBar), luaL_checkinteger(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_ProgressBar[] =
{
  { "Value",             l_uiProgressBarValue },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
