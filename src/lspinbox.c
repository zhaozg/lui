static int l_uiNewSpinbox(lua_State *L)
{
  CREATE_OBJECT(Spinbox, uiNewSpinbox(luaL_checkint(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int l_uiSpinboxValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiSpinboxValue(CAST_ARG(1, Spinbox)));
    return 1;
  }
  uiSpinboxSetValue(CAST_ARG(1, Spinbox), luaL_checkint(L, 2));
  RETURN_SELF;
}

static void on_spinbox_changed(uiSpinbox *b, void *data)
{
  callback(data, b);
}

static int l_uiSpinboxOnChanged(lua_State *L)
{
  uiSpinboxOnChanged(CAST_ARG(1, Spinbox), on_spinbox_changed, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_Spinbox[] =
{
  { "Value",                l_uiSpinboxValue },
  { "OnChanged",            l_uiSpinboxOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
