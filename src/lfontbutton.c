static int l_uiNewFontButton(lua_State *L)
{
  CREATE_OBJECT(FontButton, uiNewFontButton());
  return 1;
}

static int l_uiFontButtonFont(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    uiDrawTextFont *font = uiFontButtonFont(CAST_ARG(1, FontButton));
    CREATE_OBJECT(DrawTextFont, font);
    return 1;
  }
  luaL_argcheck(L, !lua_isnone(L, 2), 2, "not accpet any more paramater");
  return 0;
}

static void on_fontbutton_changed(uiFontButton *b, void *data)
{
  callback(data, b);
}

static int l_uiFontButtonOnChanged(lua_State *L)
{
  uiFontButtonOnChanged(CAST_ARG(1, FontButton), on_fontbutton_changed, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_FontButton[] =
{
  { "Font",                 l_uiFontButtonFont },
  { "OnChanged",            l_uiFontButtonOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
