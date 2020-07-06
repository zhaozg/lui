#include "lui.h"

static lua_State* _main_L= NULL;

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

inline static void* check_object(lua_State *L, int n, const char* cls)
{
  void *p = luaL_testudata(L, n, cls);
  if (p==NULL)
  {
    p = auxiliar_checkgroup(L, cls, n);
  }

  return (((struct wrap*)p)->control);
}

#define UI_RETURN_SELF lua_pushvalue(L, 1); return 1;

#define UI_CREATE_META(t)                                   \
  luaL_newmetatable(L, "libui." #t);                        \
  lua_pushstring(L, "__index");                             \
  lua_newtable(L);                                          \
  lua_pushstring(L, "class");                               \
  lua_pushstring(L, "libui." #t);                           \
  lua_rawset(L, -3);                                        \
  luaL_setfuncs(L, meta_ ## t, 0);                          \
  lua_rawset(L, -3);                                        \
  lua_pushstring(L, "__gc");                                \
  lua_pushcfunction(L, l_uigc);                             \
  lua_rawset(L, -3);                                        \
  lua_pushstring(L, "__tostring");                          \
  lua_pushcfunction(L, auxiliar_tostring);                  \
  lua_rawset(L, -3);                                        \
  lua_pop(L, 1);

#define UI_CHECK_OBJECT(n, t)                               \
  check_object(L, n, "libui." #t)

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
  lua_pushstring(L, "class");                               \
  lua_pushstring(L, "libui.user." #t);                      \
  lua_rawset(L, -3);                                        \
  luaL_setfuncs(L, meta_ ## t, 0);                          \
  lua_rawset(L, -3);                                        \
  lua_pushstring(L, "__tostring");                          \
  lua_pushcfunction(L, auxiliar_tostring);                  \
  lua_rawset(L, -3);                                        \
  lua_pop(L, 1);

#define CREATE_USER_OBJECT(t, c)                            \
  *(ui ## t**)lua_newuserdata(L, sizeof(ui ## t*)) = c;     \
  luaL_setmetatable(L, "libui.user." #t);

#define CHECK_USER_OBJECT(n, t) \
  *((ui ## t**)luaL_checkudata(L, n, "libui.user." #t ))

/* general libui callback  mechanism to lua */
static void create_callback_data(lua_State *L, int n, const char *event)
{
  /* Push registery key: userdata pointer to control */
  void *control = UI_CHECK_OBJECT(1, Control);
  lua_pushlightuserdata(L, control);
  lua_rawget(L, LUA_REGISTRYINDEX);
  if (lua_isnil(L, -1))
  {
    lua_pop(L, 1);
    lua_pushlightuserdata(L, control);
    lua_newtable(L);
    lua_rawset(L, LUA_REGISTRYINDEX);
    lua_pushlightuserdata(L, control);
    lua_rawget(L, LUA_REGISTRYINDEX);
  }
  /* Create table with event/callback data */

  lua_pushstring(L, event);
  lua_newtable(L);

  lua_pushvalue(L, n);
  lua_setfield(L, -2, "udata");
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "fn");
  lua_pushvalue(L, 3);
  lua_setfield(L, -2, "data");

  lua_rawset(L, -3);
  /* Store in registry */
  lua_pop(L, 1);
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

static int callback(lua_State *L, void *control, const char *event)
{
  int ret = 0;
  int err = 0;

  /* Get the traceback function in case of error ,+1 */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);
  /* Find table with callback data in registry , +1 */
  lua_pushlightuserdata(L, control);
  lua_rawget(L, LUA_REGISTRYINDEX);
  luaL_checktype(L, -1, LUA_TTABLE);

  /* Find table with event, +1 */
  lua_pushstring(L, event);
  lua_rawget(L, -2);
  luaL_checktype(L, -1, LUA_TTABLE);

  /* Get function, control userdata and callback data, +3 */
  lua_getfield(L, -1, "fn");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "udata");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  lua_getfield(L, -3, "data");

  /* Call function */
  if (lua_pcall(L, 2, 1, err))
  {
    luaL_error(L, "InnerException: %s", lua_tostring(L, -1));
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

  /* Remove table with given event */
  /* Remove table with callback data in registry */
  /* Remove the traceback function */
  lua_pop(L, 3);
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
    /*
     * uint32_t s = w->control->TypeSignature;
     */

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

static int l_QueueDumpWriter(lua_State* L, const void* p, size_t sz, void* B)
{
  (void)L;
  luaL_addlstring((luaL_Buffer*) B, (const char*) p, sz);
  return 0;
}

static const char* l_QueueThreadDump(lua_State* L, int idx, size_t* l)
{
  if (lua_isstring(L, idx))
  {
    return lua_tolstring(L, idx, l);
  }
  else
  {
    const char* buff = NULL;
    int top = lua_gettop(L);
    luaL_Buffer b;
    int test_lua_dump;
    luaL_checktype(L, idx, LUA_TFUNCTION);
    lua_pushvalue(L, idx);
    luaL_buffinit(L, &b);
    test_lua_dump = (lua_dump(L, l_QueueDumpWriter, &b, 1) == 0);
    if (test_lua_dump)
    {
      luaL_pushresult(&b);
      buff = lua_tolstring(L, -1, l);
    }
    else
      luaL_error(L, "Error: unable to dump given function");
    lua_settop(L, top);

    return buff;
  }
}

typedef struct value_s
{
  int type;
  union
  {
    double n;
    int i;
    struct
    {
      int   l;
      char* p;
    } s;
    void* p;
  } v;
} Value;

#define MAX_ARGS_NUM 9
typedef struct QueueArg_s
{
  size_t sz;
  char *entry;
  lua_State *L;

  size_t argc;
  Value argv[MAX_ARGS_NUM];
} QueueArg;

// This must in main thread
static void l_QueueMain(void *data)
{
  int i;
  int top = 0;
  QueueArg *arg = (QueueArg*)data;
  lua_State *L = arg->L;

  top = lua_gettop(L);
  lua_pushcfunction(L, traceback);
  luaL_loadbuffer(L, arg->entry, arg->sz, "=queue");
  luaL_checktype(L, -1, LUA_TFUNCTION);

  for(i=0; i<arg->argc; i++)
  {
    Value *val = arg->argv+i;
    switch(val->type)
    {
    case LUA_TBOOLEAN:
      lua_pushboolean(L, val->v.i);
      break;
    case LUA_TNUMBER:
      lua_pushnumber(L, val->v.n);
      break;
    case LUA_TSTRING:
      lua_pushlstring(L, val->v.s.p, val->v.s.l);
      break;
    case LUA_TNIL:
      lua_pushnil(L);
      break;
    }
  }

  if (lua_pcall(L, arg->argc, 0, top+1))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  /* remove traceback function*/
  lua_pop(L, 1);
  luaL_argcheck(L, top  == lua_gettop(L), 1, "stack not balance");
}

static int l_uiQueueMain(lua_State *L)
{
  int i, top;
  QueueArg *arg = NULL;
  size_t sz;
  const char* p;

  // check main thread exist
  if (_main_L==NULL)
  {
    luaL_error(L, "Can't access main thread");
    return 0;
  }

  top = lua_gettop(L);
  luaL_checktype(L, 1, LUA_TFUNCTION);
  arg = malloc(sizeof(QueueArg));
  memset(arg, 0, sizeof(QueueArg));

  p = l_QueueThreadDump(L, 1, &sz);
  arg->entry = malloc(sz+1);
  memcpy(arg->entry, p, sz);
  arg->entry[sz] = '\0';
  arg->sz = sz;
  arg->L = _main_L;

  arg->argc = (top-1 > MAX_ARGS_NUM) ? MAX_ARGS_NUM : top - 1;
  for(i=0; i < arg->argc; i++)
  {
    int type = lua_type(L, i+2);
    Value *val = arg->argv+i;
    val->type = type;

    luaL_argcheck(L, type==LUA_TBOOLEAN || type==LUA_TNUMBER ||
                  type==LUA_TSTRING || type==LUA_TNIL,
                  i+2,
                  "only accpet nil, boolean, number or string value");

    if (val->type==LUA_TBOOLEAN)
    {
      val->v.i = lua_toboolean(L, i+2);
    }
    else if(val->type==LUA_TNUMBER)
    {
      val->v.n = lua_tonumber(L, i+2);
      val->type = LUA_TNUMBER;
    }
    else if(val->type==LUA_TSTRING)
    {
      p = lua_tolstring(L,i+2, &sz);
      val->v.s.l = sz;
      val->v.s.p = malloc(sz);
      memcpy(val->v.s.p, p, sz);
    }
    else
      val->type = LUA_TNIL;
  }

  uiQueueMain(l_QueueMain, arg);
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
  lua_pushlightuserdata(L, (void*)on_ShouldQuit);
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
  lua_pushlightuserdata(L, (void*)on_ShouldQuit);
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

  lua_pushlightuserdata(L, (void*)l_uiTimer);
  lua_rawget(L, LUA_REGISTRYINDEX);
  lua_pushlightuserdata(L, (void*)l_timer_cb);
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

  lua_pushlightuserdata(L, (void*)l_uiTimer);
  lua_pushvalue(L, 2);
  lua_rawset(L, LUA_REGISTRYINDEX);

  lua_pushlightuserdata(L, (void*)l_timer_cb);
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
  { "NewImage",               l_uiNewImage },
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
  UI_CREATE_META(Image)
  UI_SET_GROUP(Image, Control);
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
  UI_SET_GROUP(Table, Control);
  UI_CREATE_META(Window)
  UI_SET_GROUP(Window, Control);

  CREATE_USER_META(Attribute)
  CREATE_USER_META(AttributedString)
  CREATE_USER_META(FontDescriptor)
  CREATE_USER_META(OpenTypeFeatures)
  CREATE_USER_META(TableModel)
  CREATE_USER_META(TableModelHandler)
  CREATE_USER_META(TableValue)
  CREATE_USER_META(TableParams)

  /* draw, not finished */
  CREATE_DRAWMETA

  luaL_newlib(L, lui_table);
  l_REG_UI_ENUM(L, lua_gettop(L));

  // store as a global variable, to support multi-thread
  if (_main_L==NULL)
    _main_L = L;

  return 1;
}
