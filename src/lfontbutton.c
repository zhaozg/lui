static int l_uiNewFontDescriptor(lua_State *L)
{
  uiFontDescriptor *desc = lua_newuserdata(L, sizeof(uiFontDescriptor));
  CREATE_USER_OBJECT(FontDescriptor, desc);
  return 1;
}

static int l_uiFontDescriptorFammily(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  lua_pushstring(L, desc->Family);
  return 1;
}

static int l_uiFontDescriptorSize(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  lua_pushnumber(L, desc->Size);
  return 1;
}

static int l_uiFontDescriptorWeight(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  lua_pushinteger(L, desc->Weight);
  return 1;
}

static int l_uiFontDescriptorItalic(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  lua_pushinteger(L, desc->Italic);
  return 1;
}

static int l_uiFontDescriptorStretch(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  lua_pushinteger(L, desc->Stretch);
  return 1;
}

static int l_uiFreeFontButtonFont(lua_State *L)
{
  uiFontDescriptor *desc = CHECK_USER_OBJECT(1, FontDescriptor);
  uiFreeFontButtonFont(desc);
  return 0;
}

static struct luaL_Reg meta_FontDescriptor[] =
{
  { "__gc",           l_uiFreeFontButtonFont },

  { "Free",           l_uiFreeFontButtonFont },
  { "Fammily",        l_uiFontDescriptorFammily },
  { "Size",           l_uiFontDescriptorSize },
  { "Weight",         l_uiFontDescriptorWeight },
  { "Italic",         l_uiFontDescriptorItalic },
  { "Stretch",        l_uiFontDescriptorStretch },

  { NULL, NULL }
};

static int l_uiNewFontButton(lua_State *L)
{
  UI_CREATE_OBJECT(FontButton, uiNewFontButton());
  return 1;
}

static int l_uiFontButtonFont(lua_State *L)
{
  uiFontButton *font = UI_CHECK_OBJECT(1, FontButton);
  uiFontDescriptor *desc = lua_newuserdata(L, sizeof(uiFontDescriptor));
  memset(desc, 0, sizeof(uiFontDescriptor));
  uiFontButtonFont(font, desc);
  CREATE_USER_OBJECT(FontDescriptor, desc);
  return 1;
}

static void on_fontbutton_changed(uiFontButton *b, void *data)
{
  callback(data, b, "changed");
}

static int l_uiFontButtonOnChanged(lua_State *L)
{
  uiFontButtonOnChanged(UI_CHECK_OBJECT(1, FontButton), on_fontbutton_changed, L);
  create_callback_data(L, 1, "changed");
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_FontButton[] =
{
  { "Font",                 l_uiFontButtonFont },
  { "OnChanged",            l_uiFontButtonOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
