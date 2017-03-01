static int l_uiNewCombobox(lua_State *L)
{
  CREATE_OBJECT(Combobox, uiNewCombobox());
  return 1;
}

static int l_uiComboboxAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i++)
  {
    const char *text = luaL_checkstring(L, n);
    uiComboboxAppend(CAST_ARG(1, Combobox), text);
  }
  RETURN_SELF;
}

static int l_uiComboboxSelected(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiComboboxSelected(CAST_ARG(1, Combobox)));
    return 1;
  }
  uiComboboxSetSelected(CAST_ARG(1, Combobox), luaL_checkint(L, 2));
  RETURN_SELF;
}

static void on_combobox_selected(uiCombobox *c, void *data)
{
  callback(data, c);
}

static int l_uiComboboxOnSelected(lua_State *L)
{
  uiComboboxOnSelected(CAST_ARG(1, Combobox), on_combobox_selected, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_Combobox[] =
{
  { "Append",               l_uiComboboxAppend },
  { "Selected",             l_uiComboboxSelected },
  { "OnSelected",           l_uiComboboxOnSelected },

  CONTROL_COMMON_METAFIELD
  { NULL }
};

static int l_uiNewEditableCombobox(lua_State *L)
{
  CREATE_OBJECT(EditableCombobox, uiNewEditableCombobox());
  return 1;
}

static int l_uiEditableComboboxAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i++)
  {
    const char *text = luaL_checkstring(L, n);
    uiEditableComboboxAppend(CAST_ARG(1, EditableCombobox), text);
  }
  RETURN_SELF;
}

static int l_uiEditableComboboxText(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushstring(L, uiEditableComboboxText(CAST_ARG(1, EditableCombobox)));
    return 1;
  }
  uiEditableComboboxSetText(CAST_ARG(1, EditableCombobox), luaL_checkstring(L, 2));
  RETURN_SELF;
}

static void on_EditableCombobox_Changed(uiEditableCombobox *c, void *data)
{
  callback(data, c);
}

static int l_uiEditableComboboxOnChanged(lua_State *L)
{
  uiEditableComboboxOnChanged(CAST_ARG(1, EditableCombobox), on_EditableCombobox_Changed, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_EditableCombobox[] =
{
  { "Append",               l_uiEditableComboboxAppend },
  { "Text",                 l_uiEditableComboboxText },
  { "OnChanged",            l_uiEditableComboboxOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
