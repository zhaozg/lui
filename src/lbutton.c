static int l_uiNewButton(lua_State *L)
{
  CREATE_OBJECT(Button, uiNewButton(luaL_checkstring(L, 1)));
  return 1;
}

static int l_uiButtonText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    char* text = uiButtonText(CAST_ARG(1, Button));
    lua_pushstring(L, text);
    return 1;
  }
  uiButtonSetText(CAST_ARG(1, Button), luaL_checkstring(L, 2));
  RETURN_SELF;
}

static void on_button_clicked(uiButton *b, void *data)
{
  callback(data, b);
}

static int l_uiButtonOnClicked(lua_State *L)
{
  uiButtonOnClicked(CAST_ARG(1, Button), on_button_clicked, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_Button[] =
{
  { "Text",                 l_uiButtonText },
  { "OnClicked",            l_uiButtonOnClicked },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
