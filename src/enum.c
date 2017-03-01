static void l_REG_UI_ENUM(lua_State *L, int idx)
{
  lua_pushnumber(L, uiDrawDefaultMiterLimit);
  lua_setfield(L, -2, "DrawDefaultMiterLimit");

#define REG_UI_ENUM(x)         \
  lua_pushstring(L, #x);       \
  lua_pushinteger(L, ui##x);   \
  lua_rawset(L, idx)

  REG_UI_ENUM(WindowResizeEdgeLeft);
  REG_UI_ENUM(WindowResizeEdgeTop);
  REG_UI_ENUM(WindowResizeEdgeRight);
  REG_UI_ENUM(WindowResizeEdgeBottom);
  REG_UI_ENUM(WindowResizeEdgeTopLeft);
  REG_UI_ENUM(WindowResizeEdgeTopRight);
  REG_UI_ENUM(WindowResizeEdgeBottomLeft);
  REG_UI_ENUM(WindowResizeEdgeBottomRight);

  REG_UI_ENUM(DrawBrushTypeSolid);
  REG_UI_ENUM(DrawBrushTypeLinearGradient);
  REG_UI_ENUM(DrawBrushTypeRadialGradient);
  REG_UI_ENUM(DrawBrushTypeImage);

  REG_UI_ENUM(DrawLineCapFlat);
  REG_UI_ENUM(DrawLineCapRound);
  REG_UI_ENUM(DrawLineCapSquare);

  REG_UI_ENUM(DrawLineJoinMiter);
  REG_UI_ENUM(DrawLineJoinRound);
  REG_UI_ENUM(DrawLineJoinBevel);

  REG_UI_ENUM(DrawFillModeWinding);
  REG_UI_ENUM(DrawFillModeAlternate);

  REG_UI_ENUM(DrawTextWeightThin);
  REG_UI_ENUM(DrawTextWeightUltraLight);
  REG_UI_ENUM(DrawTextWeightLight);
  REG_UI_ENUM(DrawTextWeightBook);
  REG_UI_ENUM(DrawTextWeightNormal);
  REG_UI_ENUM(DrawTextWeightMedium);
  REG_UI_ENUM(DrawTextWeightSemiBold);
  REG_UI_ENUM(DrawTextWeightBold);
  REG_UI_ENUM(DrawTextWeightUltraBold);
  REG_UI_ENUM(DrawTextWeightHeavy);
  REG_UI_ENUM(DrawTextWeightUltraHeavy);

  REG_UI_ENUM(DrawTextItalicNormal);
  REG_UI_ENUM(DrawTextItalicOblique);
  REG_UI_ENUM(DrawTextItalicItalic);

  REG_UI_ENUM(DrawTextStretchUltraCondensed);
  REG_UI_ENUM(DrawTextStretchExtraCondensed);
  REG_UI_ENUM(DrawTextStretchCondensed);
  REG_UI_ENUM(DrawTextStretchSemiCondensed);
  REG_UI_ENUM(DrawTextStretchNormal);
  REG_UI_ENUM(DrawTextStretchSemiExpanded);
  REG_UI_ENUM(DrawTextStretchExpanded);
  REG_UI_ENUM(DrawTextStretchExtraExpanded);
  REG_UI_ENUM(DrawTextStretchUltraExpanded);

  REG_UI_ENUM(ModifierCtrl);
  REG_UI_ENUM(ModifierAlt);
  REG_UI_ENUM(ModifierShift);
  REG_UI_ENUM(ModifierSuper);

  REG_UI_ENUM(ExtKeyEscape);
  REG_UI_ENUM(ExtKeyInsert);      // eqREG_UI_ENUM(valent to "Help" on Apple keyboards
  REG_UI_ENUM(ExtKeyDelete);
  REG_UI_ENUM(ExtKeyHome);
  REG_UI_ENUM(ExtKeyEnd);
  REG_UI_ENUM(ExtKeyPageUp);
  REG_UI_ENUM(ExtKeyPageDown);
  REG_UI_ENUM(ExtKeyUp);
  REG_UI_ENUM(ExtKeyDown);
  REG_UI_ENUM(ExtKeyLeft);
  REG_UI_ENUM(ExtKeyRight);
  REG_UI_ENUM(ExtKeyF1);      // F1..F12 are guaranteed to be consecutive
  REG_UI_ENUM(ExtKeyF2);
  REG_UI_ENUM(ExtKeyF3);
  REG_UI_ENUM(ExtKeyF4);
  REG_UI_ENUM(ExtKeyF5);
  REG_UI_ENUM(ExtKeyF6);
  REG_UI_ENUM(ExtKeyF7);
  REG_UI_ENUM(ExtKeyF8);
  REG_UI_ENUM(ExtKeyF9);
  REG_UI_ENUM(ExtKeyF10);
  REG_UI_ENUM(ExtKeyF11);
  REG_UI_ENUM(ExtKeyF12);
  REG_UI_ENUM(ExtKeyN0);      // numpad keys; independent of Num Lock state
  REG_UI_ENUM(ExtKeyN1);      // N0..N9 are guaranteed to be consecutive
  REG_UI_ENUM(ExtKeyN2);
  REG_UI_ENUM(ExtKeyN3);
  REG_UI_ENUM(ExtKeyN4);
  REG_UI_ENUM(ExtKeyN5);
  REG_UI_ENUM(ExtKeyN6);
  REG_UI_ENUM(ExtKeyN7);
  REG_UI_ENUM(ExtKeyN8);
  REG_UI_ENUM(ExtKeyN9);
  REG_UI_ENUM(ExtKeyNDot);
  REG_UI_ENUM(ExtKeyNEnter);
  REG_UI_ENUM(ExtKeyNAdd);
  REG_UI_ENUM(ExtKeyNSubtract);
  REG_UI_ENUM(ExtKeyNMultiply);
  REG_UI_ENUM(ExtKeyNDivide);

  REG_UI_ENUM(AlignFill);
  REG_UI_ENUM(AlignStart);
  REG_UI_ENUM(AlignCenter);
  REG_UI_ENUM(AlignEnd);

  REG_UI_ENUM(AtLeading);
  REG_UI_ENUM(AtTop);
  REG_UI_ENUM(AtTrailing);
  REG_UI_ENUM(AtBottom);
}
