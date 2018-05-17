
// uiOpenTypeFeaturesForEachFunc is the type of the function
// invoked by uiOpenTypeFeaturesForEach() for every OpenType
// feature in otf. Refer to that function's documentation for more
// details.
typedef uiForEach(*uiOpenTypeFeaturesForEachFunc)(const uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t value, void *data);

static int l_uiNewOpenTypeFeatures(lua_State *L) {
  uiOpenTypeFeatures *f = uiNewOpenTypeFeatures();
  CREATE_USER_OBJECT(OpenTypeFeatures, f);
  return 1;
}

static int l_uiFreeOpenTypeFeatures(lua_State *L) {
  uiOpenTypeFeatures *f = CHECK_USER_OBJECT(1, OpenTypeFeatures);
  uiFreeOpenTypeFeatures(f);
  return 0;
}

static int l_uiOpenTypeFeaturesClone(lua_State *L) {
  uiOpenTypeFeatures *f = CHECK_USER_OBJECT(1, OpenTypeFeatures);
  uiOpenTypeFeatures *a = uiOpenTypeFeaturesClone(f);
  CREATE_USER_OBJECT(OpenTypeFeatures, a);
  return 1;
}

static int l_uiOpenTypeFeaturesAdd(lua_State *L) {
  uiOpenTypeFeatures *f = CHECK_USER_OBJECT(1, OpenTypeFeatures);
  char a = (char)luaL_checkinteger(L, 2);
  char b = (char)luaL_checkinteger(L, 3);
  char c = (char)luaL_checkinteger(L, 4);
  char d = (char)luaL_checkinteger(L, 5);
  uint32_t value = luaL_checkinteger(L, 6);
  uiOpenTypeFeaturesAdd(f, a, b, c, d, value);
  UI_RETURN_SELF;
}

static int l_uiOpenTypeFeaturesRemove(lua_State *L) {
  uiOpenTypeFeatures *f = CHECK_USER_OBJECT(1, OpenTypeFeatures);
  char a = (char)luaL_checkinteger(L, 2);
  char b = (char)luaL_checkinteger(L, 3);
  char c = (char)luaL_checkinteger(L, 4);
  char d = (char)luaL_checkinteger(L, 5);
  uiOpenTypeFeaturesRemove(f, a, b, c, d);
  UI_RETURN_SELF;
}

static int l_uiOpenTypeFeaturesGet(lua_State *L) {
  uiOpenTypeFeatures *f = CHECK_USER_OBJECT(1, OpenTypeFeatures);
  char a = (char)luaL_checkinteger(L, 2);
  char b = (char)luaL_checkinteger(L, 3);
  char c = (char)luaL_checkinteger(L, 4);
  char d = (char)luaL_checkinteger(L, 5);
  uint32_t value = 0;
  uiOpenTypeFeaturesGet(f, a, b, c, d, &value);
  return 0;
}

static int l_uiOpenTypeFeaturesForEach(lua_State *L) {
  //_UI_EXTERN void uiOpenTypeFeaturesForEach(const uiOpenTypeFeatures *otf, uiOpenTypeFeaturesForEachFunc f, void *data);
  return 0;
}

static struct luaL_Reg meta_OpenTypeFeatures[] =
{
  { "__gc",       l_uiFreeOpenTypeFeatures },
  { "Free",       l_uiFreeOpenTypeFeatures },
  { "Clone",      l_uiOpenTypeFeaturesClone },

  { "Add",        l_uiOpenTypeFeaturesAdd },
  { "Remove",     l_uiOpenTypeFeaturesRemove },
  { "Get",        l_uiOpenTypeFeaturesGet },
  { "ForEach",    l_uiOpenTypeFeaturesForEach },

  { NULL }
};

static int l_uiFreeAttribute(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  uiFreeAttribute(attr);
  return 0;
}

static int l_uiAttributeGetType(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushinteger(L, uiAttributeGetType(attr));
  return 1;
}

static int l_uiAttributeFamily(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushstring(L, uiAttributeFamily(attr));
  return 1;
}

static int l_uiAttributeSize(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushnumber(L, uiAttributeSize(attr));
  return 1;
}

static int l_uiAttributeWeight(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushinteger(L, uiAttributeWeight(attr));
  return 1;
}

static int l_uiAttributeItalic(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushinteger(L, uiAttributeItalic(attr));
  return 1;
}

static int l_uiAttributeStretch(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushinteger(L, uiAttributeStretch(attr));
  return 1;
}

static int l_uiAttributeColor(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  double r, g, b, a;
  uiAttributeColor(attr, &r, &g, &b, &a);
  lua_pushnumber(L, r);
  lua_pushnumber(L, g);
  lua_pushnumber(L, b);
  lua_pushnumber(L, a);
  return 4;
}


static int l_uiAttributeUnderlineColor(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  uiUnderlineColor u;
  double r, g, b, a;
  uiAttributeUnderlineColor(attr, &u, &r, &g, &b, &a);
  lua_pushnumber(L, u); 
  lua_pushnumber(L, r);
  lua_pushnumber(L, g);
  lua_pushnumber(L, b);
  lua_pushnumber(L, a);
  return 5;
}
static int l_uiAttributeUnderline(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  lua_pushinteger(L, uiAttributeUnderline(attr));
  return 1;
}

static int l_uiAttributeFeatures(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  const uiOpenTypeFeatures* features = uiAttributeFeatures(attr);
  CREATE_USER_OBJECT(OpenTypeFeatures, (uiOpenTypeFeatures*)features);
  return 1;
}

/* uiFontButtonDescriptor */
static int l_uiAttribute2table(lua_State *L, const uiAttribute *atr)
{
  lua_newtable(L);
  lua_pushinteger(L, uiAttributeGetType(atr));
  lua_setfield(L, -2, "type");
  lua_pushstring(L, uiAttributeFamily(atr));
  lua_setfield(L, -2, "family");
  lua_pushnumber(L, uiAttributeSize(atr));
  lua_setfield(L, -2, "size");
  lua_pushinteger(L, uiAttributeWeight(atr));
  lua_setfield(L, -2, "weight");
  lua_pushinteger(L, uiAttributeItalic(atr));
  lua_setfield(L, -2, "italic");
  lua_pushinteger(L, uiAttributeStretch(atr));
  lua_setfield(L, -2, "stretch");
  lua_pushinteger(L, uiAttributeUnderline(atr));
  lua_setfield(L, -2, "underline");

  //color
  //underlinecolor
  return 1;
}

static int l_uiAttribute2tableInfo(lua_State *L)
{
  uiAttribute* attr = CHECK_USER_OBJECT(1, Attribute);
  l_uiAttribute2table(L, attr);
  return 1;
}


static struct luaL_Reg meta_Attribute[] =
{
  { "__gc",       l_uiFreeAttribute },
  { "Free",       l_uiFreeAttribute },
  { "Type",       l_uiAttributeGetType },
  { "Family",     l_uiAttributeFamily },
  { "Size",       l_uiAttributeSize },
  { "Weight",     l_uiAttributeWeight },
  { "Italic",     l_uiAttributeItalic },
  { "Stretch",    l_uiAttributeStretch },
  { "Color",      l_uiAttributeColor },
  { "UnderlineColor",
                  l_uiAttributeUnderlineColor },
  { "Underline",  l_uiAttributeUnderline },
  { "Features",   l_uiAttributeFeatures },
 
  { "info",       l_uiAttribute2tableInfo},
  { NULL }
};

static int l_uiNewAttribute(lua_State *L)
{
  uiAttribute *attr = NULL;
  const char* item = luaL_checkstring(L, 1);
  if (strcmp(item, "family") == 0) {
    attr = uiNewFamilyAttribute(luaL_checkstring(L, 2));
  } else if (strcmp(item, "size") == 0) {
    attr = uiNewSizeAttribute(luaL_checknumber(L, 2));
  } else if (strcmp(item, "weight") == 0) {
    attr = uiNewWeightAttribute(luaL_checkinteger(L, 2));
  } else if (strcmp(item, "italic") == 0) {
    attr = uiNewItalicAttribute(luaL_checkinteger(L, 2));
  } else if (strcmp(item, "stretch") == 0) {
    attr = uiNewStretchAttribute(luaL_checkinteger(L, 2));
  } else if (strcmp(item, "color") == 0) {
    attr = uiNewColorAttribute(luaL_checknumber(L, 2),
      luaL_checknumber(L, 3),
      luaL_checknumber(L, 4),
      luaL_checknumber(L, 5));
  } else if (strcmp(item, "background") == 0) {
    attr = uiNewBackgroundAttribute(luaL_checknumber(L, 2),
      luaL_checknumber(L, 3),
      luaL_checknumber(L, 4),
      luaL_checknumber(L, 5));
  } else if (strcmp(item, "underline") == 0) {
    attr = uiNewUnderlineAttribute(luaL_checkinteger(L, 2));
  } else if (strcmp(item, "underlinecolor") == 0) {
    attr = uiNewUnderlineColorAttribute(luaL_checkinteger(L, 2),
      luaL_checknumber(L, 3),
      luaL_checknumber(L, 4),
      luaL_checknumber(L, 5),
      luaL_checknumber(L, 6));
  } else if (strcmp(item, "features") == 0) {
    const uiOpenTypeFeatures *features = CHECK_USER_OBJECT(2, OpenTypeFeatures);
    attr = uiNewFeaturesAttribute(features);
  }
    luaL_argerror(L, 1, "not support");

  CREATE_USER_OBJECT(Attribute, attr);
  return 1;
}
