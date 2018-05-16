int LUI_EXP l_uiNewAttributedString(lua_State *L)
{
  const char *initialString = luaL_checkstring(L, 1);
  uiAttributedString *s = uiNewAttributedString(initialString);
  CREATE_USER_OBJECT(AttributedString, s);
  return 1;
}

int LUI_EXP l_uiFreeAttributedString(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  uiFreeAttributedString(s);
  return 0;
}

int LUI_EXP l_uiAttributedStringString(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  int len = uiAttributedStringLen(s);
  lua_pushlstring(L, uiAttributedStringString(s), len);
  return 1;
}

int LUI_EXP l_uiAttributedStringLen(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  lua_pushinteger(L, uiAttributedStringLen(s));
  return 1;
}

int LUI_EXP l_uiAttributedStringAppendUnattributed(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  uiAttributedStringAppendUnattributed(s, luaL_checkstring(L, 2));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiAttributedStringInsertAtUnattributed(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  uiAttributedStringInsertAtUnattributed(s, luaL_checkstring(L, 2), luaL_checkinteger(L, 3));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiAttributedStringDelete(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  uiAttributedStringDelete(s, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiAttributedStringSetAttribute(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  uiAttribute *a = CHECK_USER_OBJECT(2, Attribute);
  uiAttributedStringSetAttribute(s, a, luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
  UI_RETURN_SELF;
}

int LUI_EXP l_uiAttributedStringNumGraphemes(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  size_t sz =  uiAttributedStringNumGraphemes(s);
  lua_pushinteger(L, sz);
  return 1;
}

int LUI_EXP l_uiAttributedStringByteIndexToGrapheme(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  size_t sz = uiAttributedStringByteIndexToGrapheme(s, (size_t)luaL_checkinteger(L, 2));
  lua_pushinteger(L, sz);
  return 1;
}

int LUI_EXP l_uiAttributedStringGraphemeToByteIndex(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  size_t sz = uiAttributedStringGraphemeToByteIndex(s, (size_t)luaL_checkinteger(L, 2));
  lua_pushinteger(L, sz);
  return 1;
}


typedef uiForEach(*uiAttributedStringForEachAttributeFunc)(const uiAttributedString *s, const uiAttribute *a, size_t start, size_t end, void *data);

int LUI_EXP l_uiAttributedStringForEachAttribute(lua_State *L)
{
  uiAttributedString *s = CHECK_USER_OBJECT(1, AttributedString);
  //_UI_EXTERN void uiAttributedStringForEachAttribute(const uiAttributedString *s, uiAttributedStringForEachAttributeFunc f, void *data);
  //uiAttributedStringForEachAttribute(s, (size_t)luaL_checkinteger(L, 2));
  //lua_pushinteger(L, sz);
  return 0;
}

static struct luaL_Reg meta_AttributedString[] =
{
  { "__gc",                 l_uiFreeAttributedString },
  { "__len",                l_uiAttributedStringLen },
  { "__tostring",           l_uiAttributedStringString },

  { "Free",                 l_uiFreeAttributedString },
  { "String",               l_uiAttributedStringString },
  { "Len",                  l_uiAttributedStringLen },
  { "AppendUnattributed",   l_uiAttributedStringAppendUnattributed },
  { "InsertAtUnattributed", l_uiAttributedStringInsertAtUnattributed},
  { "Delete",               l_uiAttributedStringDelete},
  { "SetAttribute",         l_uiAttributedStringSetAttribute},
  { "NumGraphemes",         l_uiAttributedStringNumGraphemes},
  { "ByteIndexToGrapheme",  l_uiAttributedStringByteIndexToGrapheme},
  { "GraphemeToByteIndex",  l_uiAttributedStringGraphemeToByteIndex},
  { "ForEach",              l_uiAttributedStringForEachAttribute},

  { NULL }
};

