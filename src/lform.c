static int l_uiNewForm(lua_State *L)
{
  CREATE_OBJECT(Form, uiNewForm());
  return 1;
}

static int l_uiFormAppend(lua_State *L)
{
  int n = lua_gettop(L);
  int i;
  for (i = 2; i <= n; i += 3)
  {
    uiFormAppend(CAST_ARG(1, Form), luaL_checkstring(L, i + 0), CAST_ARG(i + 1, Control), luaL_checkboolean(L, i + 2));
    //copy from ltab.c, need check
    lua_getmetatable(L, 1);
    lua_pushvalue(L, i + 1);
    luaL_ref(L, -2);
  }
  RETURN_SELF;
}

static int l_uiFormDelete(lua_State *L)
{
  uiFormDelete(CAST_ARG(1, Form), luaL_checkint(L, 2));
  RETURN_SELF;
}

static int l_uiFormPadded(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushboolean(L, uiFormPadded(CAST_ARG(1, Form)));
    return 1;
  }
  uiFormSetPadded(CAST_ARG(1, Form), luaL_checkboolean(L, 2));
  RETURN_SELF;
}

static struct luaL_Reg meta_Form[] =
{
  { "Append",               l_uiFormAppend },
  { "Delete",               l_uiFormDelete },
  { "Padded",               l_uiFormPadded },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
