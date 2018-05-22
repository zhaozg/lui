static int l_uiNewSlider(lua_State *L)
{
  UI_CREATE_OBJECT(Slider, uiNewSlider(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
  return 1;
}

static int l_uiSliderValue(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, uiSliderValue(UI_CHECK_OBJECT(1, Slider)));
    return 1;
  }
  uiSliderSetValue(UI_CHECK_OBJECT(1, Slider), luaL_checkinteger(L, 2));
  UI_RETURN_SELF;
}

static void on_slider_changed(uiSlider *b, void *data)
{
  callback(data, b);
}

static int l_uiSliderOnChanged(lua_State *L)
{
  uiSliderOnChanged(UI_CHECK_OBJECT(1, Slider), on_slider_changed, L);
  create_callback_data(L, 1);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_Slider[] =
{
  { "Value",                l_uiSliderValue },
  { "OnChanged",            l_uiSliderOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};
