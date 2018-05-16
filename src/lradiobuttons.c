int LUI_EXP l_uiNewRadioButtons(lua_State *L)
{
  UI_CREATE_OBJECT(RadioButtons, uiNewRadioButtons());
  return 1;
}

int LUI_EXP l_uiRadioButtonsAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i++)
  {
    const char *text = luaL_checkstring(L, i);
    uiRadioButtonsAppend(UI_CHECK_OBJECT(1, RadioButtons), text);
  }
  UI_RETURN_SELF;
}

int LUI_EXP l_uiRadioButtonsSelected(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiRadioButtonsSelected(UI_CHECK_OBJECT(1, RadioButtons)));
    return 1;
  }
  uiRadioButtonsSetSelected(UI_CHECK_OBJECT(1, RadioButtons), luaL_checkinteger(L, 2));;
  UI_RETURN_SELF;
}

void LUI_EXP on_radiobutton_selected(uiRadioButtons *b, void *data)
{
  callback(data, b);
}

int LUI_EXP l_uiRadioButtonsOnSelected(lua_State *L)
{
  uiRadioButtonsOnSelected(UI_CHECK_OBJECT(1, RadioButtons), on_radiobutton_selected, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_RadioButtons[] =
{
  { "Append",               l_uiRadioButtonsAppend },
  { "Selected",             l_uiRadioButtonsSelected },
  { "OnSelected",           l_uiRadioButtonsOnSelected },

  CONTROL_COMMON_METAFIELD
  { NULL }
};

