int LUI_EXP l_uiNewSpinbox(lua_State *L)
{
  UI_CREATE_OBJECT(Spinbox, uiNewSpinbox(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
  return 1;
}

int LUI_EXP l_uiSpinboxValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiSpinboxValue(UI_CHECK_OBJECT(1, Spinbox)));
    return 1;
  }
  uiSpinboxSetValue(UI_CHECK_OBJECT(1, Spinbox), luaL_checkinteger(L, 2));
  UI_RETURN_SELF;
}

void LUI_EXP on_spinbox_changed(uiSpinbox *b, void *data)
{
  callback(data, b);
}

int LUI_EXP l_uiSpinboxOnChanged(lua_State *L)
{
  uiSpinboxOnChanged(UI_CHECK_OBJECT(1, Spinbox), on_spinbox_changed, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Spinbox[] =
{
  { "Value",                l_uiSpinboxValue },
  { "OnChanged",            l_uiSpinboxOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
