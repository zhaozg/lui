static int l_uiNewMultilineEntry(lua_State *L)
{
  uiMultilineEntry *ml = uiNewMultilineEntry();
  if (!lua_isnoneornil(L, 1))
    uiMultilineEntrySetText(ml, luaL_checkstring(L, 1));
  UI_CREATE_OBJECT(MulitlineEntry, ml);
  return 1;
}

static int l_uiNewNonWrappingMultilineEntry(lua_State *L)
{
  uiMultilineEntry *ml = uiNewNonWrappingMultilineEntry();
  if (!lua_isnoneornil(L, 1))
    uiMultilineEntrySetText(ml, luaL_checkstring(L, 1));
  UI_CREATE_OBJECT(MulitlineEntry, ml);
  return 1;
}

static int l_uiMultilineEntryText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiMultilineEntryText(UI_CHECK_OBJECT(1, MultilineEntry)));
    return 1;
  }
  uiMultilineEntrySetText(UI_CHECK_OBJECT(1, MultilineEntry), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

static int l_uiMultilineEntryAppend(lua_State *L)
{
  uiMultilineEntryAppend(UI_CHECK_OBJECT(1, MultilineEntry), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

static void on_multilineentry_changed(uiMultilineEntry *b, void *data)
{
  callback(data, b);
}

static int l_uiMultilineEntryOnChanged(lua_State *L)
{
  uiMultilineEntryOnChanged(UI_CHECK_OBJECT(1, MultilineEntry), on_multilineentry_changed, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static int l_uiMultilineEntryReadOnly(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiMultilineEntryReadOnly(UI_CHECK_OBJECT(1, MultilineEntry)));
    return 1;
  }
  uiMultilineEntrySetReadOnly(UI_CHECK_OBJECT(1, MultilineEntry), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_MultilineEntry[] =
{
  { "Text",                 l_uiMultilineEntryText },
  { "Append",               l_uiMultilineEntryAppend },
  { "OnChanged",            l_uiMultilineEntryOnChanged },
  { "ReadOnly",             l_uiMultilineEntryReadOnly },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
