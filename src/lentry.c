static l_uiNewEntry(lua_State *L)
{
  uiEntry *e = uiNewEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  CREATE_OBJECT(Entry, e);
  return 1;
}

static l_uiNewPasswordEntry(lua_State *L)
{
  uiEntry *e = uiNewPasswordEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  CREATE_OBJECT(Entry, e);
  return 1;
}

static l_uiNewSearchEntry(lua_State *L)
{
  uiEntry *e = uiNewSearchEntry();
  if (!lua_isnoneornil(L, 1))
    uiEntrySetText(e, luaL_checkstring(L, 1));
  CREATE_OBJECT(Entry, e);
  return 1;
}

static l_uiEntryText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiEntryText(CAST_ARG(1, Entry)));
    return 1;
  }
  uiEntrySetText(CAST_ARG(1, Entry), luaL_checkstring(L, 2));
  RETURN_SELF;
}

static void on_entry_changed(uiEntry *b, void *data)
{
  callback(data, b);
}

static int l_uiEntryOnChanged(lua_State *L)
{
  uiEntryOnChanged(CAST_ARG(1, Entry), on_entry_changed, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static l_uiEntryReadOnly(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiEntryReadOnly(CAST_ARG(1, Entry)));
    return 1;
  }
  uiEntrySetReadOnly(CAST_ARG(1, Entry), luaL_checkboolean(L, 2));
  RETURN_SELF;
}

static struct luaL_Reg meta_Entry[] =
{
  { "Text",                 l_uiEntryText },
  { "OnChanged",            l_uiEntryOnChanged },
  { "ReadOnly",             l_uiEntryReadOnly },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
