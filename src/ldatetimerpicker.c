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

/* copy from lua source */
/*
** {======================================================
** Time/Date operations
** { year=%Y, month=%m, day=%d, hour=%H, min=%M, sec=%S,
**   wday=%w+1, yday=%j, isdst=? }
** =======================================================
*/

static void setfield (lua_State *L, const char *key, int value) {
  lua_pushinteger(L, value);
  lua_setfield(L, -2, key);
}

static void setboolfield (lua_State *L, const char *key, int value) {
  if (value < 0)  /* undefined? */
    return;  /* does not set field */
  lua_pushboolean(L, value);
  lua_setfield(L, -2, key);
}

#include <time.h>
/*
** Set all fields from structure 'tm' in the table on top of the stack
*/
static void setallfields (lua_State *L, struct tm *stm) {
  setfield(L, "sec", stm->tm_sec);
  setfield(L, "min", stm->tm_min);
  setfield(L, "hour", stm->tm_hour);
  setfield(L, "day", stm->tm_mday);
  setfield(L, "month", stm->tm_mon + 1);
  setfield(L, "year", stm->tm_year + 1900);
//  setfield(L, "wday", stm->tm_wday + 1);
//  setfield(L, "yday", stm->tm_yday + 1);
  if(stm->tm_isdst!=-1)
    setboolfield(L, "isdst", stm->tm_isdst);
}


static int getboolfield (lua_State *L, const char *key) {
  int res;
  res = (lua_getfield(L, -1, key) == LUA_TNIL) ? -1 : lua_toboolean(L, -1);
  lua_pop(L, 1);
  return res;
}


/* maximum value for date fields (to avoid arithmetic overflows with 'int') */
#if !defined(L_MAXDATEFIELD)
#define L_MAXDATEFIELD	(INT_MAX / 2)
#endif

static int getfield (lua_State *L, const char *key, int d, int delta) {
  int isnum;
  int t = lua_getfield(L, -1, key);  /* get field and its type */
  lua_Integer res = lua_tointegerx(L, -1, &isnum);
  if (!isnum) {  /* field is not an integer? */
    if (t != LUA_TNIL)  /* some other value? */
      return luaL_error(L, "field '%s' is not an integer", key);
    else if (d < 0)  /* absent field; no default? */
      return luaL_error(L, "field '%s' missing in date table", key);
    res = d;
  }
  else {
    if (!(-L_MAXDATEFIELD <= res && res <= L_MAXDATEFIELD))
      return luaL_error(L, "field '%s' is out-of-bound", key);
    res -= delta;
  }
  lua_pop(L, 1);
  return (int)res;
}

// TODO document that tm_wday and tm_yday are undefined, and tm_isdst should be -1
static int l_uiDateTimePickerTime(lua_State *L)
{
  uiDateTimePicker *picker = UI_CHECK_OBJECT(1, DateTimePicker);
  struct tm time = {0};
  uiDateTimePickerTime(picker, &time);
  lua_createtable(L, 0, 9);
  setallfields(L, &time);
  return 1;
}

static int l_uiDateTimePickerSetTime(lua_State *L)
{
  uiDateTimePicker *picker = UI_CHECK_OBJECT(1, DateTimePicker);
  struct tm ts = {0};
  luaL_checktype(L, 2, LUA_TTABLE);
  lua_settop(L, 2);

  ts.tm_sec = getfield(L, "sec", 0, 0);
  ts.tm_min = getfield(L, "min", 0, 0);
  ts.tm_hour = getfield(L, "hour", 12, 0);
  ts.tm_mday = getfield(L, "day", -1, 0);
  ts.tm_mon = getfield(L, "month", -1, 1);
  ts.tm_year = getfield(L, "year", -1, 1900);
  ts.tm_isdst = getboolfield(L, "isdst");

  uiDateTimePickerSetTime(picker, &ts);
  UI_RETURN_SELF;
}

static void l_uiDateTimePickerCallback(uiDateTimePicker *picker, void* arg)
{
  lua_State *L = (lua_State *)arg;
  int ret = 0;

  lua_rawgetp(L, LUA_REGISTRYINDEX, l_uiDateTimePickerCallback);
  UI_CREATE_OBJECT_REF(DateTimePicker, picker);
  lua_rawgetp(L, LUA_REGISTRYINDEX, picker);
  ret = lua_pcall(L, 1, 2, 0);
  if(ret!=0)
  {
    lua_pushfstring(L, "Callback function for uiDateTimePickerOnChanged fail: %s", lua_tostring(L, -1));
    lua_remove(L, lua_absindex(L, -2));
    lua_error(L);
  }
}

static int l_uiDateTimePickerOnChanged(lua_State *L)
{
  uiDateTimePicker *picker = UI_CHECK_OBJECT(1, DateTimePicker);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_settop(L, 3);
  lua_pushvalue(L, 2);
  lua_rawsetp(L, LUA_REGISTRYINDEX, l_uiDateTimePickerCallback);
  lua_pushvalue(L, 3);
  lua_rawsetp(L, LUA_REGISTRYINDEX, picker);
 
  uiDateTimePickerOnChanged(picker, l_uiDateTimePickerCallback, L);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_DateTimePicker[] =
{
  { "Time",         l_uiDateTimePickerTime },
  { "SetTime",      l_uiDateTimePickerSetTime },
  { "OnChanged",    l_uiDateTimePickerOnChanged },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL }
};

