#define CREATE_DRAW_META(n)                \
	luaL_newmetatable(L, "libui.draw." #n);  \
	luaL_setfuncs(L, meta_ ## n, 0);

#define CREATE_DRAW_OBJECT(t, c)                              \
	*(ui ## t**)lua_newuserdata(L, sizeof(ui ## t*)) = c;       \
	lua_newtable(L);                                            \
	luaL_getmetatable(L, "libui.draw." #t);                     \
	lua_setfield(L, -2, "__index");                             \
	lua_pushcfunction(L, l_uigc);                               \
	lua_setfield(L, -2, "__gc");                                \
	lua_setmetatable(L, -2);

#define CAST_DRAW_ARG(n, type) *((ui ## type**)lua_touserdata(L, n))

static int l_uiDrawNewPath(lua_State *L)
{
  uiDrawPath *dp = uiDrawNewPath(luaL_checkint(L, 1));
  CREATE_DRAW_OBJECT(DrawPath, dp);
  return 1;
}

static int l_uiDrawFreePath(lua_State *L)
{
  uiDrawFreePath(CAST_DRAW_ARG(1, DrawPath));
  return 0;
}

static int l_uiDrawPathNewFigure(lua_State *L)
{
  uiDrawPathNewFigure(CAST_DRAW_ARG(1, DrawPath), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  RETURN_SELF;
}

static int l_uiDrawPathNewFigureWithArc(lua_State *L)
{
  uiDrawPathNewFigureWithArc(CAST_DRAW_ARG(1, DrawPath), 
    luaL_checknumber(L, 2), 
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4),
    luaL_checknumber(L, 5),
    luaL_checknumber(L, 6),
    luaL_checkboolean(L, 7));
  RETURN_SELF;
}

static int l_uiDrawPathLineTo(lua_State *L)
{
  uiDrawPathLineTo(CAST_DRAW_ARG(1, DrawPath), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  RETURN_SELF;
}

static int l_uiDrawPathArcTo(lua_State *L)
{
  uiDrawPathArcTo(CAST_DRAW_ARG(1, DrawPath),
    luaL_checknumber(L, 2),
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4),
    luaL_checknumber(L, 5),
    luaL_checknumber(L, 6),
    luaL_checkboolean(L, 7));
  RETURN_SELF;
}

static int l_uiDrawPathBezierTo(lua_State *L)
{
  uiDrawPathBezierTo(CAST_DRAW_ARG(1, DrawPath),
    luaL_checknumber(L, 2),
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4),
    luaL_checknumber(L, 5),
    luaL_checknumber(L, 6),
    luaL_checknumber(L, 7));
  RETURN_SELF;
}

static int l_uiDrawPathCloseFigure(lua_State *L)
{
  uiDrawPathCloseFigure(CAST_DRAW_ARG(1, DrawPath));
  RETURN_SELF;
}

static int l_uiDrawPathAddRectangle(lua_State *L)
{
  uiDrawPathAddRectangle(CAST_DRAW_ARG(1, DrawPath),
    luaL_checknumber(L, 2),
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4),
    luaL_checknumber(L, 5));
  RETURN_SELF;
}

static int l_uiDrawPathEnd(lua_State *L)
{
  uiDrawPathEnd(CAST_DRAW_ARG(1, DrawPath));
  RETURN_SELF;
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

  { NULL }
};

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

  CREATE_DRAW_OBJECT(DrawMatrix, matrix);
  return 1;
}

static int l_uiDrawMatrixSetIdentity(lua_State *L)
{
  uiDrawMatrixSetIdentity(CAST_DRAW_ARG(1, DrawMatrix));
  RETURN_SELF;
}

static int l_uiDrawMatrixTranslate(lua_State *L)
{
  uiDrawMatrixTranslate(CAST_DRAW_ARG(1, DrawMatrix), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  RETURN_SELF;
}

static int l_uiDrawMatrixScale(lua_State *L)
{
  uiDrawMatrixScale(CAST_DRAW_ARG(1, DrawMatrix),
    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
  RETURN_SELF;
}

static int l_uiDrawMatrixRotate(lua_State *L)
{
  uiDrawMatrixRotate(CAST_DRAW_ARG(1, DrawMatrix),
    luaL_checknumber(L, 2), 
    luaL_checknumber(L, 3),
    luaL_checknumber(L, 4));
  RETURN_SELF;
}

static int l_uiDrawMatrixSkew(lua_State *L)
{
  uiDrawMatrixSkew(CAST_DRAW_ARG(1, DrawMatrix),
    luaL_checknumber(L, 2), luaL_checknumber(L, 3),
    luaL_checknumber(L, 4), luaL_checknumber(L, 5));
  RETURN_SELF;
}

static int l_uiDrawMatrixMultiply(lua_State *L)
{
  uiDrawMatrixMultiply(CAST_DRAW_ARG(1, DrawMatrix), CAST_DRAW_ARG(2, DrawMatrix));
  RETURN_SELF;
}

static int l_uiDrawMatrixInvertible(lua_State *L)
{
  lua_pushboolean(L, uiDrawMatrixInvertible(CAST_DRAW_ARG(1, DrawMatrix)));
  return 1;
}

static int l_uiDrawMatrixInvert(lua_State *L)
{
  lua_pushboolean(L, uiDrawMatrixInvert(CAST_DRAW_ARG(1, DrawMatrix)));
  return 1;
}

static int l_uiDrawMatrixTransformPoint(lua_State *L)
{
  double x, y;
  uiDrawMatrixTransformPoint(CAST_DRAW_ARG(1, DrawMatrix), &x, &y);
  lua_pushnumber(L, x);
  lua_pushnumber(L, y);
  return 2;
}

static int l_uiDrawMatrixTransformSize(lua_State *L)
{
  double x, y;
  uiDrawMatrixTransformSize(CAST_DRAW_ARG(1, DrawMatrix), &x, &y);
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

  { NULL }
};

static int l_uiDrawTransform(lua_State *L)
{
  uiDrawTransform(CAST_DRAW_ARG(1, DrawContext), CAST_DRAW_ARG(2, DrawMatrix));
  RETURN_SELF;
}

static int l_uiDrawClip(lua_State *L)
{
  uiDrawClip(CAST_DRAW_ARG(1, DrawContext), CAST_DRAW_ARG(2, DrawPath));
  RETURN_SELF;
}

static int l_uiDrawSave(lua_State *L)
{
  uiDrawSave(CAST_DRAW_ARG(1, DrawContext));
  RETURN_SELF;
}

static int l_uiDrawRestore(lua_State *L)
{
  uiDrawRestore(CAST_DRAW_ARG(1, DrawContext));
  RETURN_SELF;
}

static int l_uiDrawText(lua_State *L)
{
  uiDrawText(
    CAST_DRAW_ARG(1, DrawContext),
    luaL_checknumber(L, 2),
    luaL_checknumber(L, 3),
    CAST_DRAW_ARG(4, DrawTextLayout));
  RETURN_SELF;
}

static int l_uiDrawStroke(lua_State *L)
{
  uiDrawStroke(CAST_DRAW_ARG(1, DrawContext), CAST_DRAW_ARG(2, DrawPath), CAST_DRAW_ARG(3, DrawBrush), CAST_DRAW_ARG(4,DrawStrokeParams));
  RETURN_SELF;
}

static int l_uiDrawFill(lua_State *L)
{
  uiDrawFill(CAST_DRAW_ARG(1, DrawContext), CAST_DRAW_ARG(2, DrawPath), CAST_DRAW_ARG(3, DrawBrush));
  RETURN_SELF;
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
  

  { NULL }
};

static int l_uiDrawListFontFamilies(lua_State *L)
{
  CREATE_DRAW_OBJECT(DrawFontFamilies, uiDrawListFontFamilies());
  return 1;
}

static int l_uiDrawFontFamiliesNumFamilies(lua_State *L)
{
  lua_pushinteger(L, uiDrawFontFamiliesNumFamilies(CAST_DRAW_ARG(1, DrawFontFamilies)));
  return 1;
  return 1;
}

static int l_uiDrawFontFamiliesFamily(lua_State *L)
{
  lua_pushstring(L, uiDrawFontFamiliesFamily(CAST_DRAW_ARG(1, DrawFontFamilies), luaL_checkint(L, 2)));
  return 1;
}

static int l_uiDrawFreeFontFamilies(lua_State *L)
{
  uiDrawFreeFontFamilies(CAST_DRAW_ARG(1, DrawFontFamilies));
  return 0;
}

static struct luaL_Reg meta_DrawFontFamilies[] =
{
  { "NumFamilies",    l_uiDrawFontFamiliesNumFamilies },
  { "Family",         l_uiDrawFontFamiliesFamily },
  { "Free",           l_uiDrawFreeFontFamilies },

  { "__len",          l_uiDrawFontFamiliesNumFamilies },

  { NULL }
};

static int l_uiDrawLoadClosestFont(lua_State *L)
{
  CREATE_DRAW_OBJECT(DrawTextFont, uiDrawLoadClosestFont(CAST_DRAW_ARG(1, DrawTextFontDescriptor)));
  return 1;
}

static int l_uiDrawFreeTextFont(lua_State *L)
{
  uiDrawFreeTextFont(CAST_DRAW_ARG(1, DrawTextFont));
  return 0;
}

static int l_uiDrawTextFontHandle(lua_State *L)
{
  lua_pushinteger(L, uiDrawTextFontHandle(CAST_DRAW_ARG(1, DrawTextFont)));
  return 1;
}

static int l_uiDrawTextFontDescribe(lua_State *L)
{
  uiDrawTextFontDescribe(CAST_DRAW_ARG(1, DrawTextFont), CAST_DRAW_ARG(2, DrawTextFontDescriptor));
  RETURN_SELF;
}

static int l_uiDrawTextFontMetrics(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    uiDrawTextFontMetrics metrics;
    uiDrawTextFontGetMetrics(CAST_DRAW_ARG(1, DrawTextFont), &metrics);
    lua_pushnil(L);
    return 1;
  }
  luaL_error(L, "NYI");
  return 0;
}

static struct luaL_Reg meta_DrawTextFont[] =
{
  { "Free",    	  l_uiDrawFreeTextFont },
  { "Handle",     l_uiDrawTextFontHandle },
  { "Describe",   l_uiDrawTextFontDescribe },
  { "Metrics",    l_uiDrawTextFontMetrics },

  { NULL }
};

static int l_uiDrawNewTextLayout(lua_State *L)
{
  uiDrawTextLayout *layout = uiDrawNewTextLayout(
    luaL_checkstring(L, 1),
    CAST_DRAW_ARG(2, DrawTextFont),
    luaL_checknumber(L, 3));
  CREATE_DRAW_OBJECT(DrawTextLayout, layout);
  return 1;
}

static int l_uiDrawFreeTextLayout(lua_State *L)
{
  uiDrawFreeTextLayout(CAST_DRAW_ARG(1, DrawTextLayout));
  return 0;
}

static int l_uiDrawTextLayoutWidth(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    luaL_error(L, "NYI");
    return 0;
  }
  uiDrawTextLayoutSetWidth(CAST_DRAW_ARG(1, DrawTextLayout), luaL_checknumber(L, 2));
  RETURN_SELF;
}

static int l_uiDrawTextLayoutExtents(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    double width, height;
    uiDrawTextLayoutExtents(CAST_DRAW_ARG(1, DrawTextLayout), &width, &height);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    return 2;
  }
  luaL_error(L, "NYI");
  return 0;
}

static int l_uiDrawTextLayoutColor(lua_State *L)
{
  if (lua_isnone(L, 2))
  {
    luaL_error(L, "NYI");
    return 0;
  }
  uiDrawTextLayoutSetColor(
    CAST_DRAW_ARG(1, DrawTextLayout),
    luaL_checkint(L, 2),
    luaL_checkint(L, 3),
    luaL_checknumber(L, 4),
    luaL_checknumber(L, 5),
    luaL_checknumber(L, 6),
    luaL_checknumber(L, 7));
  RETURN_SELF;
}

static struct luaL_Reg meta_DrawTextLayout[] =
{
  { "Free",    	  l_uiDrawFreeTextLayout },
  { "Handle",     l_uiDrawTextLayoutWidth },
  { "Describe",   l_uiDrawTextLayoutExtents },
  { "Metrics",    l_uiDrawTextLayoutColor },

  { NULL }
};

static struct luaL_Reg meta_DrawTextFontDescriptor[] =
{
  { NULL }
};

static struct luaL_Reg meta_DrawBrush[] =
{
  { NULL }
};

static struct luaL_Reg meta_DrawStrokeParams[] =
{
  { NULL }
};

#define CREATE_DRAWMETA                     \
  CREATE_DRAW_META(DrawPath)                \
  CREATE_DRAW_META(DrawFontFamilies)        \
  CREATE_DRAW_META(DrawTextFont)            \
  CREATE_DRAW_META(DrawTextLayout)          \
  CREATE_DRAW_META(DrawContext)             \
  CREATE_DRAW_META(DrawMatrix)              \
  CREATE_DRAW_META(DrawTextFontDescriptor)  \
  CREATE_DRAW_META(DrawBrush)               \
  CREATE_DRAW_META(DrawStrokeParams)
