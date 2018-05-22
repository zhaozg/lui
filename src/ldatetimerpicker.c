static int l_uiNewDateTimePicker(lua_State *L)
{
  UI_CREATE_OBJECT(DateTimePicker, uiNewDateTimePicker());
  return 1;
}

static int l_uiNewDatePicker(lua_State *L)
{
  UI_CREATE_OBJECT(DateTimePicker, uiNewDatePicker());
  return 1;
}

static int l_uiNewTimePicker(lua_State *L)
{
  UI_CREATE_OBJECT(DateTimePicker, uiNewTimePicker());
  return 1;
}

static struct luaL_Reg meta_DateTimePicker[] =
{
  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};

