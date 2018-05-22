static int l_uiNewColorButton(lua_State *L)
{
  UI_CREATE_OBJECT(ColorButton, uiNewColorButton());
  return 1;
}

static int l_uiColorButtonColor(lua_State *L)
{
  double r, g, b, a;
  if (lua_isnone(L, 2))
  {
    uiColorButtonColor(UI_CHECK_OBJECT(1, ColorButton), &r, &g, &b, &a);
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    lua_pushnumber(L, a);
    return 4;
  }
  uiColorButtonSetColor(UI_CHECK_OBJECT(1, ColorButton),
                        luaL_checknumber(L, 2),
                        luaL_checknumber(L, 3),
                        luaL_checknumber(L, 4),
                        luaL_checknumber(L, 5));
  UI_RETURN_SELF;
}

static void on_colorbutton_changed(uiColorButton *b, void *data)
{
  callback(data, b);
}

static int l_uiColorButtonOnChanged(lua_State *L)
{
  uiColorButtonOnChanged(UI_CHECK_OBJECT(1, ColorButton), on_colorbutton_changed, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_ColorButton[] =
{
  { "Color",                l_uiColorButtonColor },
  { "OnChanged",            l_uiColorButtonOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
