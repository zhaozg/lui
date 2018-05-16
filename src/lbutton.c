int LUI_EXP l_uiNewButton(lua_State *L)
{
  UI_CREATE_OBJECT(Button, uiNewButton(luaL_checkstring(L, 1)));
  return 1;
}

int LUI_EXP l_uiButtonText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    char* text = uiButtonText(UI_CHECK_OBJECT(1, Button));
    lua_pushstring(L, text);
    return 1;
  }
  uiButtonSetText(UI_CHECK_OBJECT(1, Button), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

void LUI_EXP on_button_clicked(uiButton *b, void *data)
{
  callback(data, b);
}

int LUI_EXP l_uiButtonOnClicked(lua_State *L)
{
  uiButtonOnClicked(UI_CHECK_OBJECT(1, Button), on_button_clicked, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Button[] =
{
  { "Text",                 l_uiButtonText },
  { "OnClicked",            l_uiButtonOnClicked },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
