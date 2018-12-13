#include "lui.h"

inline static int luaL_checkboolean(lua_State *L, int n)
{
  luaL_checktype(L, n, LUA_TBOOLEAN);
  return lua_toboolean(L, n);
}

/* make libui object felixble */
struct wrap
{
  uiControl *control;
  int ref;
};


#define UI_RETURN_SELF lua_pushvalue(L, 1); return 1;

#define UI_CREATE_META(t)                                   \
  luaL_newmetatable(L, "libui." #t);                        \
  lua_pushstring(L, "__index");                             \
  lua_newtable(L);                                          \
  luaL_setfuncs(L, meta_ ## t, 0);                          \
  lua_rawset(L, -3);                                        \
  lua_pushstring(L, "__gc");                                \
  lua_pushcfunction(L, l_uigc);                             \
  lua_rawset(L, -3);                                        \
  lua_pop(L, 1);

#define UI_CHECK_OBJECT(n, t)                               \
  ui ## t(((struct wrap *)lua_touserdata(L, n))->control)
//ui ## t(((struct wrap *)luaL_checkudata(L, n, "libui." #t))->control)

#define UI_CREATE_OBJECT(t, c)                              \
  struct wrap *w = lua_newuserdata(L, sizeof(struct wrap)); \
  w->control = uiControl(c);                                \
  w->ref = 0;                                               \
  luaL_setmetatable(L, "libui." #t);

#define UI_CREATE_OBJECT_REF(t, c)                          \
  struct wrap *w = lua_newuserdata(L, sizeof(struct wrap)); \
  w->control = uiControl(c);                                \
  w->ref = 1;                                               \
  luaL_setmetatable(L, "libui." #t);

#define UI_SET_GROUP(cls, grp)                              \
  auxiliar_add2group(L, "libui." #cls, "libui." #grp);

#define UI_CHECK_GROUP(n, t)                                \
  auxiliar_checkgroup(L, "libui." #t, n)

#define CREATE_USER_META(t)                                 \
  luaL_newmetatable(L, "libui.user." #t);                   \
  lua_pushstring(L, "__index");                             \
  lua_newtable(L);                                          \
  luaL_setfuncs(L, meta_ ## t, 0);                          \
  lua_rawset(L, -3);

#define CREATE_USER_OBJECT(t, c)                            \
  *(ui ## t**)lua_newuserdata(L, sizeof(ui ## t*)) = c;     \
  luaL_setmetatable(L, "libui.user." #t);

#define CHECK_USER_OBJECT(n, t) \
  *((ui ## t**)luaL_checkudata(L, n, "libui.user." #t ))

/* general libui callback  mechanism to lua */

static void create_callback_data(lua_State *L, int n)
{
  (void)n;

  /* Push registery key: userdata pointer to control */
  lua_pushlightuserdata(L, UI_CHECK_OBJECT(1, Control));

  /* Create table with callback data */
  lua_newtable(L);
  lua_pushvalue(L, 1);
  lua_setfield(L, -2, "udata");
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "fn");
  lua_pushvalue(L, 3);
  lua_setfield(L, -2, "data");

  /* Store in registry */
  lua_settable(L, LUA_REGISTRYINDEX);
}

static int traceback(lua_State *L)
{
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_pushglobaltable(L);
  lua_getfield(L, -1, "debug");
  lua_remove(L, -2);
  if (!lua_istable(L, -1))
  {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1))
  {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  return 1;
}

static int callback(lua_State *L, void *control)
{
  int ret = 0;
  int err = 0;
  uiWindow *win = NULL;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);
  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, control);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "fn");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "udata");
  win = UI_CHECK_OBJECT(-1, Window);
  luaL_checktype(L, -1, LUA_TUSERDATA);
  lua_getfield(L, -3, "data");

  /* Call function */
  if (lua_pcall(L, 2, 1, err))
  {
    uiMsgBoxError(win, "InnerException", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  else
  {
    if (lua_isboolean(L, -1))
      ret = lua_toboolean(L, -1);
    else if (!lua_isnoneornil(L, -1))
      ret = luaL_checkinteger(L, -1);
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  return ret;
}

/* all ui Control should not be gc, we need destroy it manually */
/* garbage collection for libui object */
static int l_uigc(lua_State *L)
{
  struct wrap *w = lua_touserdata(L, 1);
  uiControl *control = NULL, *parent = NULL;

  if(control && w->ref==0 )
  {
    uint32_t s = w->control->TypeSignature;
    printf("gc %p %c%c%c%c\n", w->control, s >> 24, s >> 16, s >> 8, s >> 0);

    control = w->control;
    parent = uiControlParent(control);

    if (parent)
    {
      if (parent->TypeSignature == 0x57696E64)
      {
        uiWindowSetChild(uiWindow(parent), NULL);
      }
      if (parent->TypeSignature == 0x47727062)
      {
        uiGroupSetChild((uiGroup*)uiWindow(parent), NULL);
      }
    }

    uiControlDestroy(control);
    w->control = NULL;
  }
  return 0;
}

/* real code */

#include "enum.c"
#include "lcontrol.c"

#include "larea.c"
#include "lattribute.c"
#include "lattributedstr.c"
#include "lbox.c"
#include "lbutton.c"
#include "lcheckbox.c"
#include "lcolorbutton.c"
#include "lcombobox.c"
#include "ldatetimerpicker.c"
#include "ldraw.c"
#include "lentry.c"
#include "lfontbutton.c"
#include "lform.c"
#include "lgrid.c"
#include "lgroup.c"
#include "limage.c"
#include "llabel.c"
#include "lmenu.c"
#include "lmenuitem.c"
#include "lmultiline.c"
#include "lprogressbar.c"
#include "lradiobuttons.c"
#include "lseparator.c"
#include "lslider.c"
#include "lspinbox.c"
#include "ltab.c"
#include "ltable.c"
#include "lwindow.c"

/* Various top level */

static int l_uiInit(lua_State *L)
{
  int ret = 1;
  uiInitOptions o;

  memset(&o, 0, sizeof(uiInitOptions));

  const char *err = uiInit(&o);
  if (err)
  {
    lua_pushnil(L);
    lua_pushstring(L, err);
    uiFreeInitError(err);
    ret = 2;
  }
  else
    lua_pushboolean(L, 1);
  return ret;
}

static int l_uiUninit(lua_State *L)
{
  uiUninit();
  return 0;
}

static int l_uiMain(lua_State *L)
{
  uiMain();
  return 0;
}

static int l_uiMainStep(lua_State *L)
{
  lua_pushboolean(L, uiMainStep(luaL_checkboolean(L, 1)));
  return 1;
}

static int l_uiMainSteps(lua_State *L)
{
  uiMainSteps();;
  return 0;
}

static int l_uiQuit(lua_State *L)
{
  uiQuit();
  return 0;
}

static int l_uiUserBugCannotSetParentOnToplevel(lua_State *L)
{
  uiUserBugCannotSetParentOnToplevel(luaL_checkstring(L, 1));
  return 0;
}

static void l_QueueMain(void *data)
{
  int err = 0;
  lua_State *L = data;

  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  lua_pushlightuserdata(L, l_QueueMain);
  lua_gettable(L, LUA_REGISTRYINDEX);
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "fn");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "data");

  if (lua_pcall(L, 1, 0, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  /* remove ref table*/
  lua_pop(L, 1);
  /* remove traceback function*/
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
}

static int l_uiQueueMain(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TFUNCTION);
  lua_pushlightuserdata(L, l_QueueMain);
  lua_newtable(L);
  lua_pushvalue(L, 1);
  lua_setfield(L, -2, "fn");
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "data");
  lua_settable(L, LUA_REGISTRYINDEX);
  uiQueueMain(l_QueueMain, L);
  return 0;
}

static int on_ShouldQuit(void *data)
{
  int ret = 0;
  int err = 0;
  lua_State *L = data;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);
  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, on_ShouldQuit);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "fn");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "data");

  /* Call function */
  if (lua_pcall(L, 1, 1, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  else
  {
    if (lua_isboolean(L, -1))
      ret = lua_toboolean(L, -1);
    else if (!lua_isnoneornil(L, -1))
      ret = luaL_checkinteger(L, -1);
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  return ret;
}

static int l_uiOnShouldQuit(lua_State *L)
{
  uiOnShouldQuit(on_ShouldQuit, L);

  /* Push registery key: userdata pointer to control */
  lua_pushlightuserdata(L, on_ShouldQuit);
  lua_newtable(L);
  lua_pushvalue(L, 1);
  lua_setfield(L, -2, "fn");
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "data");

  /* Store in registry */
  lua_settable(L, LUA_REGISTRYINDEX);
  return 0;
}

static int l_timer_cb(void *data);
static int l_uiTimer(lua_State *L);

static int l_timer_cb(void *data)
{
  int status = 0;
  lua_State *L = (lua_State*)data;

  lua_pushlightuserdata(L, l_uiTimer);
  lua_rawget(L, LUA_REGISTRYINDEX);
  lua_pushlightuserdata(L, l_timer_cb);
  lua_rawget(L, LUA_REGISTRYINDEX);

  status = lua_pcall(L, 1, 1, 0);
  if(status==0)
  {
    if (lua_isnumber(L, -1))
      status = lua_tointeger(L, -1);
    else
      status = lua_toboolean(L, -1);
    lua_pop(L, 1);
  }
  else
  {
    lua_pushfstring(L, "uiTimer callback fail: %s", lua_tostring(L, -1));
    lua_remove(L, -2);
    lua_error(L);
  }
  return status;
}

static int l_uiTimer(lua_State *L)
{
  int milliseconds = luaL_checkinteger(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  if(lua_isnone(L, 3))
    lua_pushnil(L);

  lua_pushlightuserdata(L, l_uiTimer);
  lua_pushvalue(L, 2);
  lua_rawset(L, LUA_REGISTRYINDEX);

  lua_pushlightuserdata(L, l_timer_cb);
  lua_pushvalue(L, 3);
  lua_rawset(L, LUA_REGISTRYINDEX);

  uiTimer(milliseconds, l_timer_cb, L);
  return 0;
}

static struct luaL_Reg lui_table[] =
{

  { "Init",                   l_uiInit },
  { "Uninit",                 l_uiUninit },
  { "Main",                   l_uiMain },
  { "MainStep",               l_uiMainStep },
  { "MainSteps",              l_uiMainSteps },
  { "Quit",                   l_uiQuit },
  { "QueueMain",              l_uiQueueMain },
  { "OnShouldQuit",           l_uiOnShouldQuit },
  {
    "UserBugCannotSetParentOnTopLevel",
    l_uiUserBugCannotSetParentOnToplevel
  },

  { "Timer",                  l_uiTimer },

  { "OpenFile",               l_uiOpenFile },
  { "SaveFile",               l_uiSaveFile },
  { "MsgBox",                 l_uiMsgBox },
  { "MsgBoxError",            l_uiMsgBoxError },

  { "NewArea",                l_uiNewArea },
  { "NewAttribute",           l_uiNewAttribute },
  { "NewAttributedString",    l_uiNewAttributedString },
  { "NewButton",              l_uiNewButton },
  { "NewCheckbox",            l_uiNewCheckbox },
  { "NewCombobox",            l_uiNewCombobox },
  { "NewColorButton",         l_uiNewColorButton },
  { "NewDateTimePicker",      l_uiNewDateTimePicker },
  { "NewDatePicker",          l_uiNewDatePicker },
  { "NewEditableCombobox",    l_uiNewEditableCombobox },
  { "NewEntry",               l_uiNewEntry },
  { "NewFontButton",          l_uiNewFontButton },
  { "NewForm",                l_uiNewForm },
  { "NewGrid",                l_uiNewGrid },
  { "NewGroup",               l_uiNewGroup },
  { "NewHorizontalBox",       l_uiNewHorizontalBox },
  { "NewHorizontalSeparator", l_uiNewHorizontalSeparator },
  { "NewLabel",               l_uiNewLabel },
  { "NewMenu",                l_uiNewMenu },
  { "NewMultilineEntry",      l_uiNewMultilineEntry },
  {
    "NewNonWrappingMultilineEntry",
    l_uiNewNonWrappingMultilineEntry
  },
  { "NewOpenTypeFeatures",    l_uiNewOpenTypeFeatures },
  { "NewPasswordEntry",       l_uiNewPasswordEntry },
  { "NewProgressBar",         l_uiNewProgressBar },
  { "NewRadioButtons",        l_uiNewRadioButtons },
  { "NewSearchEntry",         l_uiNewSearchEntry },
  { "NewSlider",              l_uiNewSlider },
  { "NewSpinbox",             l_uiNewSpinbox },
  { "NewTimePicker",          l_uiNewTimePicker },
  { "NewTab",                 l_uiNewTab },
  { "NewTable",               l_uiNewTable },
  { "NewTableValue",          l_uiTableValueNew },
  { "NewTableModelHandler",   l_uiNewTableModelHandler },
  {
    "NewTableTextColumnOptionalParams",
    l_uiNewTableTextColumnOptionalParams
  },
  { "NewVerticalBox",         l_uiNewVerticalBox },
  { "NewVerticalSeparator",   l_uiNewVerticalSeparator },
  { "NewWindow",              l_uiNewWindow },

  { "NewFontDescriptor",      l_uiNewFontDescriptor},

  { "DrawNewAreaHandler",     l_uiDrawNewAreaHandler },
  { "DrawNewBrush",           l_uiDrawNewBrush },
  { "DrawNewPath",            l_uiDrawNewPath },
  { "DrawNewMatrix",          l_uiDrawNewMatrix },
  { "DrawNewStrokeParams",    l_uiDrawNewStrokeParams },
  { "DrawNewTextLayout",      l_uiDrawNewTextLayout },
  { "DrawNewTextLayoutParams",l_uiDrawNewTextLayoutParams },

  { NULL, NULL }
};

LUA_API int luaopen_lui(lua_State *L)
{
  UI_CREATE_META(Control)

  UI_CREATE_META(Area)
  UI_SET_GROUP(Area, Control);
  UI_CREATE_META(Box)
  UI_SET_GROUP(Box, Control);
  UI_CREATE_META(Button)
  UI_SET_GROUP(Button, Control);
  UI_CREATE_META(Checkbox)
  UI_SET_GROUP(Checkbox, Control);
  UI_CREATE_META(Combobox)
  UI_SET_GROUP(Combobox, Control);
  UI_CREATE_META(ColorButton)
  UI_SET_GROUP(ColorButton, Control);
  UI_CREATE_META(DateTimePicker)
  UI_SET_GROUP(DateTimePicker, Control);
  UI_CREATE_META(EditableCombobox)
  UI_SET_GROUP(EditableCombobox, Control);
  UI_CREATE_META(Entry)
  UI_SET_GROUP(Entry, Control);
  UI_CREATE_META(FontButton)
  UI_SET_GROUP(FontButton, Control);
  UI_CREATE_META(Form)
  UI_SET_GROUP(Form, Control);
  UI_CREATE_META(Grid)
  UI_SET_GROUP(Grid, Control);
  UI_CREATE_META(Group)
  UI_SET_GROUP(Group, Control);
  UI_CREATE_META(Label)
  UI_SET_GROUP(Label, Control);
  UI_CREATE_META(MenuItem)
  UI_SET_GROUP(MenuItem, Control);
  UI_CREATE_META(Menu)
  UI_SET_GROUP(Menu, Control);
  UI_CREATE_META(MultilineEntry)
  UI_SET_GROUP(MultilineEntry, Control);
  UI_CREATE_META(ProgressBar)
  UI_SET_GROUP(ProgressBar, Control);
  UI_CREATE_META(RadioButtons)
  UI_SET_GROUP(RadioButtons, Control);
  UI_CREATE_META(Separator)
  UI_SET_GROUP(Separator, Control);
  UI_CREATE_META(Slider)
  UI_SET_GROUP(Slider, Control);
  UI_CREATE_META(Spinbox)
  UI_SET_GROUP(Spinbox, Control);
  UI_CREATE_META(Tab)
  UI_SET_GROUP(Tab, Control);
  UI_CREATE_META(Table)
  UI_SET_GROUP(Tabble, Control);
  UI_CREATE_META(Window)
  UI_SET_GROUP(Window, Control);

  CREATE_USER_META(Attribute)
  CREATE_USER_META(AttributedString)
  CREATE_USER_META(OpenTypeFeatures)
  CREATE_USER_META(FontDescriptor)

  /* draw, not finished */
  CREATE_DRAWMETA

  luaL_newlib(L, lui_table);
  l_REG_UI_ENUM(L, lua_gettop(L));

  return 1;
}
