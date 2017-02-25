static int l_uiNewSlider(lua_State *L)
{
  CREATE_OBJECT(Slider, uiNewSlider(luaL_checkint(L, 1), luaL_checkint(L, 2)));
  return 1;
}

static int l_uiSliderValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiSliderValue(CAST_ARG(1, Slider)));
    return 1;
  }
  uiSliderSetValue(CAST_ARG(1, Slider), luaL_checkint(L, 2));
  RETURN_SELF;
}

static void on_slider_changed(uiSlider *b, void *data)
{
  callback(data, b);
}

static int l_uiSliderOnChanged(lua_State *L)
{
  uiSliderOnChanged(CAST_ARG(1, Slider), on_slider_changed, L);
  create_callback_data(L, 1);
  RETURN_SELF;
}

static struct luaL_Reg meta_Slider[] =
{
  { "Value",                l_uiSliderValue },
  { "OnChanged",            l_uiSliderOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL }
};
