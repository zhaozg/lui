/* uiAreaHandler */
static int l_uiAreaDrawParams_2_table(lua_State *L, uiAreaDrawParams *params)
{
  lua_newtable(L);
  CREATE_USER_OBJECT(DrawContext, params->Context);
  lua_setfield(L, -2, "Context");

#define PARAMS_SET_NUMBER(val, name)  \
  lua_pushnumber(L, val);             \
  lua_setfield(L, -2, #name)

  PARAMS_SET_NUMBER(params->AreaWidth, AreaWidth);
  PARAMS_SET_NUMBER(params->AreaHeight, AreaHeight);

  PARAMS_SET_NUMBER(params->ClipX, ClipX);
  PARAMS_SET_NUMBER(params->ClipY, ClipY);
  PARAMS_SET_NUMBER(params->ClipWidth, ClipWidth);
  PARAMS_SET_NUMBER(params->ClipHeight, ClipWidth);
#undef PARAMS_SET_NUMBER
  return 1;
}


//from: https://source.winehq.org/source/dlls/ntdll/string.c
 char *  WINE_i64toa(
     long long value, /* [I] Value to be converted */
     char *str,      /* [O] Destination for the converted value */
     int radix)      /* [I] Number base for conversion */
 {
     unsigned long long val;
     int negative;
     char buffer[65];
     char *pos;
     int digit;
 
     if (value < 0 && radix == 10) {
        negative = 1;
        val = -value;
     } else {
        negative = 0;
        val = value;
     } /* if */
 
     pos = &buffer[64];
     *pos = '\0';
 
     do {
       digit = val % radix;
       val = val / radix;
       if (digit < 10) {
           *--pos = '0' + digit;
       } else {
           *--pos = 'a' + digit - 10;
       } /* if */
     } while (val != 0L);
 
     if (negative) {
        *--pos = '-';
     } /* if */
 
     memcpy(str, pos, &buffer[64] - pos + 1);
     return str;
 }

#define lua_pushINT64(L,n)                           \
  if(n > 9007199254740992 || n < -9007199254740992){ \
    char buf[24];                                    \
    lua_pushstring(L, WINE_i64toa(n, buf, 10));          \
  }else{                                             \
    lua_pushnumber(L, (lua_Number)n);                \
  }

static int l_uiAreaMouseEvent_2_table(lua_State *L, uiAreaMouseEvent *evt)
{
  lua_newtable(L);

#define PARAMS_SET_VALUE(val, type)  \
  lua_push##type(L, evt->val);       \
  lua_setfield(L, -2, #val)

  PARAMS_SET_VALUE(X, number);
  PARAMS_SET_VALUE(Y, number);

  PARAMS_SET_VALUE(AreaWidth, number);
  PARAMS_SET_VALUE(AreaHeight, number);

  PARAMS_SET_VALUE(Down, integer);
  PARAMS_SET_VALUE(Up, integer);
  PARAMS_SET_VALUE(Count, integer);

  PARAMS_SET_VALUE(Modifiers, integer);

  lua_pushINT64(L, evt->Held1To64);
  lua_setfield(L, -2, "Held1To64");

#undef PARAMS_SET_VALUE
  return 1;
}

static int l_uiAreaKeyEvent_2_table(lua_State *L, uiAreaKeyEvent *evt)
{
  lua_newtable(L);

#define PARAMS_SET_VALUE(val, type)  \
  lua_push##type(L, evt->val);       \
  lua_setfield(L, -2, #val)

  PARAMS_SET_VALUE(Key, integer);
  PARAMS_SET_VALUE(ExtKey, integer);
  PARAMS_SET_VALUE(Modifier, integer);
  PARAMS_SET_VALUE(Modifiers, integer);
  PARAMS_SET_VALUE(Up, boolean);

#undef PARAMS_SET_VALUE
  return 1;
}

typedef struct
{
  uiAreaHandler handler;
  lua_State *L;
} luiAreaHandler;

static void l_HandlerDraw_cb(uiAreaHandler *handler, uiArea *area, uiAreaDrawParams *params)
{
  int err;
  luiAreaHandler *lh = (luiAreaHandler*)handler;
  lua_State *L = lh->L;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, handler);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "Draw");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "SELF");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  UI_CREATE_OBJECT_REF(Area, area);
  l_uiAreaDrawParams_2_table(L, params);

  /* Call function */
  if (lua_pcall(L, 3, 0, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
}

static void l_HandlerMouseEvent_cb(uiAreaHandler *handler, uiArea *area, uiAreaMouseEvent *evt)
{
  int err;
  luiAreaHandler *lh = (luiAreaHandler*)handler;
  lua_State *L = lh->L;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, handler);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "MouseEvent");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "SELF");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  UI_CREATE_OBJECT_REF(Area, area);
  l_uiAreaMouseEvent_2_table(L, evt);

  /* Call function */
  if (lua_pcall(L, 3, 0, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
}

static void l_HandlerMouseCrossed_cb(uiAreaHandler *handler, uiArea *area, int left)
{
  int err;
  luiAreaHandler *lh = (luiAreaHandler*)handler;
  lua_State *L = lh->L;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, handler);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "MouseCrossed");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "SELF");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  UI_CREATE_OBJECT_REF(Area, area);
  lua_pushinteger(L, left);

  /* Call function */
  if (lua_pcall(L, 3, 0, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
}

static void l_HandlerDragBroken_cb(uiAreaHandler *handler, uiArea *area)
{
  int err;
  luiAreaHandler *lh = (luiAreaHandler*)handler;
  lua_State *L = lh->L;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, handler);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "DragBroken");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "SELF");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  UI_CREATE_OBJECT_REF(Area, area);

  /* Call function */
  if (lua_pcall(L, 2, 0, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
}

static int l_HandlerKeyEvent_cb(uiAreaHandler *handler, uiArea *area, uiAreaKeyEvent *evt)
{
  int err, ret;
  luiAreaHandler *lh = (luiAreaHandler*)handler;
  lua_State *L = lh->L;

  /* Get the traceback function in case of error */
  lua_pushcfunction(L, traceback);
  err = lua_gettop(L);

  /* Find table with callback data in registry */
  lua_pushlightuserdata(L, handler);
  lua_gettable(L, LUA_REGISTRYINDEX);

  /* Get function, control userdata and callback data */
  luaL_checktype(L, -1, LUA_TTABLE);
  lua_getfield(L, -1, "KeyEvent");
  luaL_checktype(L, -1, LUA_TFUNCTION);
  lua_getfield(L, -2, "SELF");
  luaL_checktype(L, -1, LUA_TUSERDATA);
  UI_CREATE_OBJECT_REF(Area, area);
  l_uiAreaKeyEvent_2_table(L, evt);

  /* Call function */
  if (lua_pcall(L, 3, 1, err))
  {
    luaL_error(L, lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  else
  {
    if (lua_isboolean(L, -1))
      ret = lua_toboolean(L, -1);
    else
      ret = luaL_checkinteger(L, -1);
    lua_pop(L, 1);
  }
  /* Remove table with callback data in registry */
  lua_pop(L, 1);
  /* Remove the traceback function */
  lua_pop(L, 1);
  assert(err - 1 == lua_gettop(L));
  return ret;
}

static int l_uiDrawNewAreaHandler(lua_State *L)
{
  luiAreaHandler *lh;
  uiAreaHandler *handler;

  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  luaL_checktype(L, 3, LUA_TFUNCTION);
  luaL_checktype(L, 4, LUA_TFUNCTION);
  luaL_checktype(L, 5, LUA_TFUNCTION);

  lh = malloc(sizeof(luiAreaHandler));
  handler = &lh->handler;
  lh->L = L;

  CREATE_USER_OBJECT(AreaHandler, handler);

  /* Push registery key: userdata pointer to control */
  lua_pushlightuserdata(L, handler);
  lua_newtable(L);

  lua_pushvalue(L, 1);
  lua_setfield(L, -2, "Draw");
  lua_pushvalue(L, 2);
  lua_setfield(L, -2, "MouseEvent");
  lua_pushvalue(L, 3);
  lua_setfield(L, -2, "MouseCrossed");
  lua_pushvalue(L, 4);
  lua_setfield(L, -2, "DragBroken");
  lua_pushvalue(L, 5);
  lua_setfield(L, -2, "KeyEvent");
  lua_pushvalue(L, 6);
  lua_setfield(L, -2, "SELF");

  /* Store in registry */
  lua_settable(L, LUA_REGISTRYINDEX);

  handler->Draw = l_HandlerDraw_cb;
  handler->MouseEvent = l_HandlerMouseEvent_cb;
  handler->MouseCrossed = l_HandlerMouseCrossed_cb;
  handler->DragBroken = l_HandlerDragBroken_cb;
  handler->KeyEvent = l_HandlerKeyEvent_cb;

  return 1;
}

static int l_uiDrawFreeAreaHandler(lua_State *L)
{
  uiAreaHandler *handler = CHECK_USER_OBJECT(1, AreaHandler);
  luiAreaHandler *lh = (luiAreaHandler *)handler;
  lua_pushlightuserdata(L, handler);
  lua_pushnil(L);
  lua_settable(L, LUA_REGISTRYINDEX);
  free(lh);
  return 0;
}

static struct luaL_Reg meta_AreaHandler[] =
{
  { "Free",           l_uiDrawFreeAreaHandler },
  { "__gc",           l_uiDrawFreeAreaHandler },

  { NULL, NULL }
};

/** Path **/
static int l_uiDrawNewPath(lua_State *L)
{
  uiDrawPath *dp = uiDrawNewPath(luaL_checkinteger(L, 1));
  CREATE_USER_OBJECT(DrawPath, dp);
  return 1;
}

static int l_uiDrawFreePath(lua_State *L)
{
  uiDrawPath **p = (uiDrawPath**)lua_touserdata(L, 1);
  if (*p != NULL)
  {
    uiDrawFreePath(*p);
    *p = NULL;
  }
  return 0;
}

static int l_uiDrawPathNewFigure(lua_State *L)
{
  uiDrawPathNewFigure(CHECK_USER_OBJECT(1, DrawPath), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  UI_RETURN_SELF;
}

static int l_uiDrawPathNewFigureWithArc(lua_State *L)
{
  uiDrawPathNewFigureWithArc(CHECK_USER_OBJECT(1, DrawPath),
                             luaL_checknumber(L, 2),
                             luaL_checknumber(L, 3),
                             luaL_checknumber(L, 4),
                             luaL_checknumber(L, 5),
                             luaL_checknumber(L, 6),
                             luaL_checkboolean(L, 7));
  UI_RETURN_SELF;
}

static int l_uiDrawPathLineTo(lua_State *L)
{
  uiDrawPathLineTo(CHECK_USER_OBJECT(1, DrawPath), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  UI_RETURN_SELF;
}

static int l_uiDrawPathArcTo(lua_State *L)
{
  uiDrawPathArcTo(CHECK_USER_OBJECT(1, DrawPath),
                  luaL_checknumber(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checknumber(L, 4),
                  luaL_checknumber(L, 5),
                  luaL_checknumber(L, 6),
                  luaL_checkboolean(L, 7));
  UI_RETURN_SELF;
}

static int l_uiDrawPathBezierTo(lua_State *L)
{
  uiDrawPathBezierTo(CHECK_USER_OBJECT(1, DrawPath),
                     luaL_checknumber(L, 2),
                     luaL_checknumber(L, 3),
                     luaL_checknumber(L, 4),
                     luaL_checknumber(L, 5),
                     luaL_checknumber(L, 6),
                     luaL_checknumber(L, 7));
  UI_RETURN_SELF;
}

static int l_uiDrawPathCloseFigure(lua_State *L)
{
  uiDrawPathCloseFigure(CHECK_USER_OBJECT(1, DrawPath));
  UI_RETURN_SELF;
}

static int l_uiDrawPathAddRectangle(lua_State *L)
{
  uiDrawPathAddRectangle(CHECK_USER_OBJECT(1, DrawPath),
                         luaL_checknumber(L, 2),
                         luaL_checknumber(L, 3),
                         luaL_checknumber(L, 4),
                         luaL_checknumber(L, 5));
  UI_RETURN_SELF;
}

static int l_uiDrawPathEnd(lua_State *L)
{
  uiDrawPathEnd(CHECK_USER_OBJECT(1, DrawPath));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_DrawPath[] =
{
  { "NewFigure",        l_uiDrawPathNewFigure },
  { "NewFigureWithArc", l_uiDrawPathNewFigureWithArc },

  { "LineTo",           l_uiDrawPathLineTo },
  { "ArcTo",            l_uiDrawPathArcTo },
  { "BezierTo",         l_uiDrawPathBezierTo },
  { "CloseFigure",      l_uiDrawPathCloseFigure },
  { "AddRectangle",     l_uiDrawPathAddRectangle },
  { "End",              l_uiDrawPathEnd },
  { "Free",             l_uiDrawFreePath },
  { "__gc",             l_uiDrawFreePath },

  { NULL, NULL }
};

/** Matrix **/
static int l_uiDrawNewMatrix(lua_State *L)
{
  uiDrawMatrix *matrix = malloc(sizeof(uiDrawMatrix));
  memset(matrix, 0, sizeof(uiDrawMatrix));

  matrix->M11 = luaL_optnumber(L, 1, 0);
  matrix->M12 = luaL_optnumber(L, 2, 0);
  matrix->M21 = luaL_optnumber(L, 3, 0);
  matrix->M22 = luaL_optnumber(L, 4, 0);
  matrix->M31 = luaL_optnumber(L, 5, 0);
  matrix->M32 = luaL_optnumber(L, 6, 0);

  CREATE_USER_OBJECT(DrawMatrix, matrix);
  return 1;
}

static int l_uiDrawFreeMatrix(lua_State *L)
{
  free(CHECK_USER_OBJECT(1, DrawMatrix));
  return 0;
}
static int l_uiDrawMatrixSetIdentity(lua_State *L)
{
  uiDrawMatrixSetIdentity(CHECK_USER_OBJECT(1, DrawMatrix));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixTranslate(lua_State *L)
{
  uiDrawMatrixTranslate(CHECK_USER_OBJECT(1, DrawMatrix), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixScale(lua_State *L)
{
  uiDrawMatrixScale(CHECK_USER_OBJECT(1, DrawMatrix),
                    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixRotate(lua_State *L)
{
  uiDrawMatrixRotate(CHECK_USER_OBJECT(1, DrawMatrix),
                     luaL_checknumber(L, 2),
                     luaL_checknumber(L, 3),
                     luaL_checknumber(L, 4));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixSkew(lua_State *L)
{
  uiDrawMatrixSkew(CHECK_USER_OBJECT(1, DrawMatrix),
                   luaL_checknumber(L, 2), luaL_checknumber(L, 3),
                   luaL_checknumber(L, 4), luaL_checknumber(L, 5));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixMultiply(lua_State *L)
{
  uiDrawMatrixMultiply(CHECK_USER_OBJECT(1, DrawMatrix), CHECK_USER_OBJECT(2, DrawMatrix));
  UI_RETURN_SELF;
}

static int l_uiDrawMatrixInvertible(lua_State *L)
{
  lua_pushboolean(L, uiDrawMatrixInvertible(CHECK_USER_OBJECT(1, DrawMatrix)));
  return 1;
}

static int l_uiDrawMatrixInvert(lua_State *L)
{
  lua_pushboolean(L, uiDrawMatrixInvert(CHECK_USER_OBJECT(1, DrawMatrix)));
  return 1;
}

static int l_uiDrawMatrixTransformPoint(lua_State *L)
{
  double x, y;
  uiDrawMatrixTransformPoint(CHECK_USER_OBJECT(1, DrawMatrix), &x, &y);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

static int l_uiDrawMatrixTransformSize(lua_State *L)
{
  double x, y;
  uiDrawMatrixTransformSize(CHECK_USER_OBJECT(1, DrawMatrix), &x, &y);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

static struct luaL_Reg meta_DrawMatrix[] =
{
  { "SetIdentity",    l_uiDrawMatrixSetIdentity },
  { "Translate",      l_uiDrawMatrixTranslate },
  { "Scale",          l_uiDrawMatrixScale },
  { "Rotate",         l_uiDrawMatrixRotate },
  { "Skew",           l_uiDrawMatrixSkew },
  { "Multiply",       l_uiDrawMatrixMultiply },
  { "Invertible",     l_uiDrawMatrixInvertible },
  { "Invert",         l_uiDrawMatrixInvert },
  { "TransformPoint", l_uiDrawMatrixTransformPoint },
  { "TransformSize",  l_uiDrawMatrixTransformSize },
  { "Free",           l_uiDrawFreeMatrix },
  { "__gc",           l_uiDrawFreeMatrix },

  { NULL, NULL }
};

/* brush */
static int l_uiDrawNewBrush(lua_State *L)
{
  uiDrawBrush *brush = malloc(sizeof(uiDrawBrush));
  memset(brush, 0, sizeof(uiDrawBrush));
  CREATE_USER_OBJECT(DrawBrush, brush);
  return 1;
}

static int lui_uiDrawBrushType(lua_State *L)
{
  uiDrawBrush *brush = CHECK_USER_OBJECT(1, DrawBrush);
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, brush->Type);
    return 1;
  }
  brush->Type = luaL_checkinteger(L, 2);
  UI_RETURN_SELF;
}

static int lui_uiDrawBrushSolid(lua_State *L)
{
  uiDrawBrush *brush = CHECK_USER_OBJECT(1, DrawBrush);
  if (lua_isnone(L, 2))
  {
    lua_pushnumber(L, brush->R);
    lua_pushnumber(L, brush->G);
    lua_pushnumber(L, brush->B);
    lua_pushnumber(L, brush->A);
    return 4;
  }
  brush->R = luaL_checknumber(L, 2);
  brush->G = luaL_checknumber(L, 3);
  brush->B = luaL_checknumber(L, 4);
  brush->A = luaL_checknumber(L, 5);
  UI_RETURN_SELF;
}


static int lui_uiDrawBrushGradient(lua_State *L)
{
  uiDrawBrush *brush = CHECK_USER_OBJECT(1, DrawBrush);
  if (lua_isnone(L, 2))
  {
    lua_pushnumber(L, brush->X0);
    lua_pushnumber(L, brush->Y0);
    lua_pushnumber(L, brush->X1);
    lua_pushnumber(L, brush->Y1);
    lua_pushnumber(L, brush->OuterRadius);
    return 5;
  }
  brush->X0 = luaL_checknumber(L, 2);
  brush->Y0 = luaL_checknumber(L, 3);
  brush->X1 = luaL_checknumber(L, 4);
  brush->Y1 = luaL_checknumber(L, 5);
  brush->OuterRadius = luaL_checknumber(L, 6);
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_DrawBrush[] =
{
  { "Type",         lui_uiDrawBrushType },
  { "Solid",        lui_uiDrawBrushSolid },
  { "Gradient",     lui_uiDrawBrushGradient },

  { NULL, NULL }
};

/*
    struct uiDrawBrush {
    uiDrawBrushType Type;

    // gradient brushes
    double X0;    // linear: start X, radial: start X
    double Y0;    // linear: start Y, radial: start Y
    double X1;    // linear: end X, radial: outer circle center X
    double Y1;    // linear: end Y, radial: outer circle center Y
    double OuterRadius;   // radial gradients only
    uiDrawBrushGradientStop *Stops;
    size_t NumStops;
    // TODO extend mode
    // cairo: none, repeat, reflect, pad; no individual control
    // Direct2D: repeat, reflect, pad; no individual control
    // Core Graphics: none, pad; before and after individually
    // TODO cairo documentation is inconsistent about pad

    // TODO images

    // TODO transforms
  };
}
*/
/* StrokeParams */

static int l_uiDrawNewStrokeParams(lua_State *L)
{
  uiDrawStrokeParams *params = malloc(sizeof(uiDrawStrokeParams));
  memset(params, 0, sizeof(uiDrawStrokeParams));
  CREATE_USER_OBJECT(DrawStrokeParams, params);
  return 1;
}

static int l_uiDrawtrokeParamsCap(lua_State *L)
{
  uiDrawStrokeParams *params = CHECK_USER_OBJECT(1, DrawStrokeParams);
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, params->Cap);
    return 1;
  }
  params->Cap = luaL_checkinteger(L, 2);
  UI_RETURN_SELF;
}

static int l_uiDrawtrokeParamsJoin(lua_State *L)
{
  uiDrawStrokeParams *params = CHECK_USER_OBJECT(1, DrawStrokeParams);
  if (lua_isnone(L, 2))
  {
    lua_pushinteger(L, params->Join);
    return 1;
  }
  params->Join = luaL_checkinteger(L, 2);
  UI_RETURN_SELF;
}

static int l_uiDrawtrokeParamsThickness(lua_State *L)
{
  uiDrawStrokeParams *params = CHECK_USER_OBJECT(1, DrawStrokeParams);
  if (lua_isnone(L, 2))
  {
    lua_pushnumber(L, params->Thickness);
    return 1;
  }
  params->Thickness = luaL_checknumber(L, 2);
  UI_RETURN_SELF;
}

static int l_uiDrawtrokeParamsMiterLimit(lua_State *L)
{
  uiDrawStrokeParams *params = CHECK_USER_OBJECT(1, DrawStrokeParams);
  if (lua_isnone(L, 2))
  {
    lua_pushnumber(L, params->MiterLimit);
    return 1;
  }
  params->MiterLimit = luaL_checknumber(L, 2);
  UI_RETURN_SELF;
}

static int l_uiDrawtrokeParamsDashes(lua_State *L)
{
  luaL_error(L, "NYI");
  return 0;
}

static int l_uiDrawtrokeParamsDashPhase(lua_State *L)
{
  luaL_error(L, "NYI");
  return 0;
}

static struct luaL_Reg meta_DrawStrokeParams[] =
{
  { "Cap",         l_uiDrawtrokeParamsCap },
  { "Join",        l_uiDrawtrokeParamsJoin },
  { "Thickness",   l_uiDrawtrokeParamsThickness },
  { "MiterLimit",  l_uiDrawtrokeParamsMiterLimit },
  { "Dashes",      l_uiDrawtrokeParamsDashes },
  { "DashPhase",   l_uiDrawtrokeParamsDashPhase },

  { NULL, NULL }
};

static int l_uiDrawTransform(lua_State *L)
{
  uiDrawTransform(CHECK_USER_OBJECT(1, DrawContext), CHECK_USER_OBJECT(2, DrawMatrix));
  UI_RETURN_SELF;
}

static int l_uiDrawClip(lua_State *L)
{
  uiDrawClip(CHECK_USER_OBJECT(1, DrawContext), CHECK_USER_OBJECT(2, DrawPath));
  UI_RETURN_SELF;
}

static int l_uiDrawSave(lua_State *L)
{
  uiDrawSave(CHECK_USER_OBJECT(1, DrawContext));
  UI_RETURN_SELF;
}

static int l_uiDrawRestore(lua_State *L)
{
  uiDrawRestore(CHECK_USER_OBJECT(1, DrawContext));
  UI_RETURN_SELF;
}

static int l_uiDrawText(lua_State *L)
{
  uiDrawText(
    CHECK_USER_OBJECT(1, DrawContext),
    CHECK_USER_OBJECT(2, DrawTextLayout),
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4)
    );
  UI_RETURN_SELF;
}

static int l_uiDrawStroke(lua_State *L)
{
  uiDrawStroke(CHECK_USER_OBJECT(1, DrawContext), CHECK_USER_OBJECT(2, DrawPath), CHECK_USER_OBJECT(3, DrawBrush), CHECK_USER_OBJECT(4, DrawStrokeParams));
  UI_RETURN_SELF;
}

static int l_uiDrawFill(lua_State *L)
{
  uiDrawFill(CHECK_USER_OBJECT(1, DrawContext), CHECK_USER_OBJECT(2, DrawPath), CHECK_USER_OBJECT(3, DrawBrush));
  UI_RETURN_SELF;
}

static struct luaL_Reg meta_DrawContext[] =
{
  { "Transform",    l_uiDrawTransform },
  { "Clip",         l_uiDrawClip },
  { "Save",         l_uiDrawSave },
  { "Restore",      l_uiDrawRestore },
  { "Text",         l_uiDrawText },
  { "Stroke",       l_uiDrawStroke },
  { "Fill",         l_uiDrawFill },

  { NULL, NULL }
};

/* uiFontButton */
static int lui_getoptfield(lua_State*L, int idx, const char*name, int def) {
  lua_getfield(L, idx, name);
  def = luaL_optnumber(L, -1, def);
  lua_pop(L, 1);
  return def;
}

static int l_uiDrawFreeTextLayout(lua_State *L)
{
  uiDrawFreeTextLayout(CHECK_USER_OBJECT(1, DrawTextLayout));
  return 0;
}

static int l_uiDrawTextLayoutExtents(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    double width, height;
    uiDrawTextLayoutExtents(CHECK_USER_OBJECT(1, DrawTextLayout), &width, &height);

    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    return 2;
  }
  luaL_error(L, "NYI");
  return 0;
}

static struct luaL_Reg meta_DrawTextLayout[] =
{
  {  "Extents",   l_uiDrawTextLayoutExtents },
  {  "Free",      l_uiDrawFreeTextLayout },
  { "__gc",       l_uiDrawFreeTextLayout },

  { NULL, NULL }
};

static int l_uiDrawNewTextLayoutParams(lua_State *L)
{
  uiDrawTextLayoutParams *params = lua_newuserdata(L, sizeof(uiDrawTextLayoutParams));
  params->String = CHECK_USER_OBJECT(1, AttributedString);
  params->DefaultFont = CHECK_USER_OBJECT(2, FontDescriptor);
  params->Width = luaL_checknumber(L, 3);
  params->Align = luaL_checkinteger(L, 4);

  CREATE_USER_OBJECT(DrawTextLayoutParams, params);
  return 1;
}

static struct luaL_Reg meta_DrawTextLayoutParams[] =
{
  { NULL, NULL }
};

static int l_uiDrawNewTextLayout(lua_State *L)
{
  uiDrawTextLayoutParams *params = CHECK_USER_OBJECT(1, DrawTextLayoutParams);
  uiDrawTextLayout *layout = uiDrawNewTextLayout(params);
  CREATE_USER_OBJECT(DrawTextLayout, layout);
  return 1;
}

#define CREATE_DRAWMETA                     \
  CREATE_USER_META(AreaHandler)             \
  CREATE_USER_META(DrawBrush)               \
  CREATE_USER_META(DrawPath)                \
  CREATE_USER_META(DrawStrokeParams)        \
  CREATE_USER_META(DrawTextLayout)          \
  CREATE_USER_META(DrawTextLayoutParams)    \
  CREATE_USER_META(DrawContext)             \
  CREATE_USER_META(DrawMatrix)              \
  CREATE_USER_META(DrawBrush)               \
  CREATE_USER_META(DrawStrokeParams)
