#if 0
static int l_uiNewImage(lua_State* L)
{
  uiImage* i = uiNewImage(NULL);
  if (lua_isstring(L, 1))
  {
    uiImageLoad(i, lua_tostring(L, 1));
  }
  CREATE_OBJECT(i, Image);
  return 1;
}

static int l_uiImageLoad(lua_State* L)
{
  lua_pushboolean(L, uiImageLoad(CAST_ARG(1, Image), luaL_checkstring(L, 2)));
  return 1;
}

static int l_uiImageValid(lua_State* L)
{
  lua_pushboolean(L, uiImageValid(CAST_ARG(1, Image)));
  return 1;
}

static int l_uiImageSize(lua_State* L)
{
  int width = -1, height = -1;

  uiImageSize(CAST_ARG(1, Image), &width, &height);
  lua_pushinteger(L, width);
  lua_pushinteger(L, height);
  return 2;
}

static int l_uiImageResize(lua_State* L)
{
  uiImage *n = uiImageResize(CAST_ARG(1, Image), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
  CREATE_OBJECT(n, Image)
  return 1;
}

static int l_uiImageDestroy(lua_State* L)
{
  uiImageDestroy(CAST_ARG(1, Image));
  return 0;
}


static luaL_Reg meta_image[] =
{
  { "Load",     l_uiImageLoad },
  { "Valid",    l_uiImageValid },
  { "Size",     l_uiImageSize },
  { "Resize",   l_uiImageResize },
  { "Destory",  l_uiImageDestroy },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
#endif
