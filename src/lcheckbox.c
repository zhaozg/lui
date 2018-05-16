int LUI_EXP l_uiNewCheckbox(lua_State *L)
{
  UI_CREATE_OBJECT(Checkbox, uiNewCheckbox(luaL_checkstring(L, 1)));
  return 1;
}

int LUI_EXP l_uiCheckboxText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiCheckboxText(UI_CHECK_OBJECT(1, Checkbox)));
    return 1;
  }
  uiCheckboxSetText(UI_CHECK_OBJECT(1, Checkbox), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

void LUI_EXP on_checkbox_toggled(uiCheckbox *c, void *data)
{
  callback(data, c);
}

int LUI_EXP l_uiCheckboxOnToggled(lua_State *L)
{
  uiCheckboxOnToggled(UI_CHECK_OBJECT(1, Checkbox), on_checkbox_toggled, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

int LUI_EXP l_uiCheckboxChecked(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiCheckboxChecked(UI_CHECK_OBJECT(1, Checkbox)));
    return 1;
  }
  uiCheckboxSetChecked(UI_CHECK_OBJECT(1, Checkbox), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Checkbox[] =
{
  { "Text",                 l_uiCheckboxText },
  { "OnToggled",            l_uiCheckboxOnToggled },
  { "Checked",              l_uiCheckboxChecked },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
