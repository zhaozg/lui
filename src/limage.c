static int l_uiNewImage(lua_State* L)
{
  double width = luaL_checknumber(L, 1);
  double height = luaL_checknumber(L, 2);
  uiImage* i = uiNewImage(width, height);
  UI_CREATE_OBJECT(Image, i);
  return 1;
}

static int l_uiImageAppend(lua_State* L)
{
  uiImage *i = UI_CHECK_OBJECT(1, Image);
  size_t size = 0;
  void* pixels = (void*)luaL_tolstring(L, 2, &size);
  int pixelWidth = (int)luaL_checkinteger(L, 3);
  int pixelHeight = (int)luaL_checkinteger(L, 4);
  int byteStride = (int)luaL_checkinteger(L, 5);

  luaL_argcheck(L, size>=pixelHeight * byteStride, 2,
                "pixels need more data");
  uiImageAppend(i, pixels, pixelWidth, pixelHeight, byteStride);
  UI_RETURN_SELF
}

static int l_uiImageFree(lua_State* L)
{
  uiFreeImage(UI_CHECK_OBJECT(1, Image));
  return 0;
}

static luaL_Reg meta_Image[] =
{
  { "Append",  l_uiImageAppend },
  { "Free",    l_uiImageFree },
  { "__gc",    l_uiImageFree },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
