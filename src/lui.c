#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>

#include "ui.h"


/* make lua version compat */

#if LUA_VERSION_NUM < 502
# define lua_rawlen lua_objlen
/* lua_...uservalue: Something very different, but it should get the job done */
# define lua_getuservalue lua_getfenv
# define lua_setuservalue lua_setfenv
# define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
# define luaL_setfuncs(L,l,n) (assert(n==0), luaL_register(L,NULL,l))
# define lua_resume(L,F,n) lua_resume(L,n)
# define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX)
#endif

inline static int luaL_checkboolean(lua_State *L, int n)
{
  luaL_checktype(L, n, LUA_TBOOLEAN);
  return lua_toboolean(L, n);
}

/* make libui object felixble */

struct wrap {
  uiControl *control;
};

#define CAST_ARG(n, type) ui ## type(((struct wrap *)lua_touserdata(L, n))->control)

#define RETURN_SELF lua_pushvalue(L, 1); return 1;

#define CREATE_META(n)                \
	luaL_newmetatable(L, "libui." #n);  \
	luaL_setfuncs(L, meta_ ## n, 0);

#define CREATE_OBJECT(t, c)                                 \
	struct wrap *w = lua_newuserdata(L, sizeof(struct wrap)); \
	w->control = uiControl(c);                                \
	lua_newtable(L);                                          \
	luaL_getmetatable(L, "libui." #t);                        \
	lua_setfield(L, -2, "__index");                           \
	lua_pushcfunction(L, l_uigc);                             \
	lua_setfield(L, -2, "__gc");                              \
	lua_setmetatable(L, -2);

#define CREATE_OBJECT_REF(t, c)                             \
	struct wrap *w = lua_newuserdata(L, sizeof(struct wrap)); \
	w->control = uiControl(c);                                \
	lua_newtable(L);                                          \
	luaL_getmetatable(L, "libui." #t);                        \
	lua_setfield(L, -2, "__index");                           \
	lua_setmetatable(L, -2);

/* draw releated macro */
#define CREATE_DRAW_META(n)                \
	luaL_newmetatable(L, "libui.draw." #n);  \
	luaL_setfuncs(L, meta_ ## n, 0);

#define CREATE_DRAW_OBJECT(t, c)                              \
	*(ui ## t**)lua_newuserdata(L, sizeof(ui ## t*)) = c;       \
	lua_newtable(L);                                            \
	luaL_getmetatable(L, "libui.draw." #t);                     \
	lua_setfield(L, -2, "__index");                             \
	lua_setmetatable(L, -2);

#define CAST_DRAW_ARG(n, type) *((ui ## type**)lua_touserdata(L, n))

/* general libui callback  mechanism to lua */

static void create_callback_data(lua_State *L, int n)
{
  /* Push registery key: userdata pointer to control */

  lua_pushlightuserdata(L, CAST_ARG(1, Control));

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

static int traceback(lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_pushglobaltable(L);
  lua_getfield(L, -1, "debug");
  lua_remove(L, -2);
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
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
  win = CAST_ARG(-1, Window);
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
      ret = luaL_checkint(L, -1);
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
  lua_pushnil(L);
  lua_pushlightuserdata(L, w->control);
  lua_settable(L, LUA_REGISTRYINDEX);
  return 0;


  uint32_t s = w->control->TypeSignature;
  printf("gc %p %c%c%c%c\n", w->control, s >> 24, s >> 16, s >> 8, s >> 0);

  uiControl *control = CAST_ARG(1, Control);
  uiControl *parent = uiControlParent(control);

  if (parent) {
    if (parent->TypeSignature == 0x57696E64) {
      //uiWindowSetChild(uiWindow(parent), NULL);
    }
    if (parent->TypeSignature == 0x47727062) {
      //uiGroupSetChild(uiWindow(parent), NULL);
    }
  }
  //uiControlDestroy(control);

  return 0;
}

/* real code */

#include "enum.c"
#include "lcontrol.c"

#include "larea.c"
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
#include "lwindow.c"

/* Various top level */

static int l_uiInit(lua_State *L)
{
  int ret = 1;
  uiInitOptions o;

  memset(&o, 0, sizeof(uiInitOptions));

  const char *err = uiInit(&o);
  if (err) {
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
  lua_pushboolean(L,uiMainStep(luaL_checkboolean(L, 1)));
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

static int l_uiQueueMain(lua_State *L)
{
  luaL_error(L, "NYI");
  //uiQueueMain();
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
      ret = luaL_checkint(L, -1);
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

static struct luaL_Reg lui_table[] = {

  { "Init",                   l_uiInit },
  { "Uninit",                 l_uiUninit },
  { "Main",                   l_uiMain },
  { "MainStep",               l_uiMainStep },
  { "MainSteps",              l_uiMainSteps },
  { "Quit",                   l_uiQuit },
  { "QueueMain",              l_uiQueueMain },
  { "OnShouldQuit",           l_uiOnShouldQuit },

  { "OpenFile",               l_uiOpenFile },
  { "SaveFile",               l_uiSaveFile },
  { "MsgBox",                 l_uiMsgBox },
  { "MsgBoxError",            l_uiMsgBoxError },

  { "NewArea",                l_uiNewArea },
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
  { "NewNonWrappingMultilineEntry",
                              l_uiNewNonWrappingMultilineEntry },
  { "NewPasswordEntry",       l_uiNewPasswordEntry },
  { "NewProgressBar",         l_uiNewProgressBar },
  { "NewRadioButtons",        l_uiNewRadioButtons },
  { "NewSearchEntry",         l_uiNewSearchEntry },
  { "NewSlider",              l_uiNewSlider },
  { "NewSpinbox",             l_uiNewSpinbox },
  { "NewTimePicker",          l_uiNewTimePicker },
  { "NewTab",                 l_uiNewTab },
  { "NewVerticalBox",         l_uiNewVerticalBox },
  { "NewVerticalSeparator",   l_uiNewVerticalSeparator },
  { "NewWindow",              l_uiNewWindow },

  /* draw, not finished */
  { "DrawNewAreaHandler",     l_uiDrawNewAreaHandler },
  { "DrawNewBrush",           l_uiDrawNewBrush }, 
  { "DrawNewPath",            l_uiDrawNewPath },
  { "DrawNewMatrix",          l_uiDrawNewMatrix },
  { "DrawNewStrokeParams",    l_uiDrawNewStrokeParams },
  { "DrawListFontFamilies",   l_uiDrawListFontFamilies },
  { "DrawLoadClosestFont",    l_uiDrawLoadClosestFont },
  { "DrawNewTextLayout",      l_uiDrawNewTextLayout },

  { NULL }
};

LUA_API int luaopen_lui(lua_State *L)
{
  CREATE_META(Area)
  CREATE_META(Box)
  CREATE_META(Button)
  CREATE_META(Checkbox)
  CREATE_META(Combobox)
  CREATE_META(ColorButton)
  CREATE_META(DateTimePicker)
  CREATE_META(EditableCombobox)  
  CREATE_META(Entry)
  CREATE_META(FontButton)
  CREATE_META(Form)
  CREATE_META(Grid)
  CREATE_META(Group)
  CREATE_META(Label)
  CREATE_META(MenuItem)
  CREATE_META(Menu)
  CREATE_META(MulitlineEntry)
  CREATE_META(ProgressBar)
  CREATE_META(RadioButtons)
  CREATE_META(Separator)
  CREATE_META(Slider)
  CREATE_META(Spinbox)
  CREATE_META(Tab)
  CREATE_META(Window)
  
  /* draw, not finished */
  CREATE_DRAWMETA

  luaL_newlib(L, lui_table);
  l_REG_UI_ENUM(L, lua_gettop(L));

  return 1;
}
