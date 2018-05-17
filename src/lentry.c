static int l_uiNewEntry(lua_State *L)
{
  uiEntry *e = uiNewEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  UI_CREATE_OBJECT(Entry, e);
  return 1;
}

static int l_uiNewPasswordEntry(lua_State *L)
{
  uiEntry *e = uiNewPasswordEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  UI_CREATE_OBJECT(Entry, e);
  return 1;
}

static int l_uiNewSearchEntry(lua_State *L)
{
  uiEntry *e = uiNewSearchEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  UI_CREATE_OBJECT(Entry, e);
  return 1;
}

static int l_uiEntryText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiEntryText(UI_CHECK_OBJECT(1, Entry)));
    return 1;
  }
  uiEntrySetText(UI_CHECK_OBJECT(1, Entry), luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

static void on_entry_changed(uiEntry *b, void *data)
{
  callback(data, b);
}

static int l_uiEntryOnChanged(lua_State *L)
{
  uiEntryOnChanged(UI_CHECK_OBJECT(1, Entry), on_entry_changed, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static int l_uiEntryReadOnly(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiEntryReadOnly(UI_CHECK_OBJECT(1, Entry)));
    return 1;
  }
  uiEntrySetReadOnly(UI_CHECK_OBJECT(1, Entry), luaL_checkboolean(L, 2));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Entry[] =
{
  { "Text",                 l_uiEntryText },
  { "OnChanged",            l_uiEntryOnChanged },
  { "ReadOnly",             l_uiEntryReadOnly },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
