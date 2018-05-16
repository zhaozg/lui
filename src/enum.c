LUI_EXP void l_REG_UI_ENUM(lua_State *L, int idx)
{
  lua_pushnumber(L, uiDrawDefaultMiterLimit);
  lua_setfield(L, -2, "DrawDefaultMiterLimit");

#define REG_UI_ENUM(x)         \
  lua_pushstring(L, #x);       \
  lua_pushinteger(L, ui##x);   \
  lua_rawset(L, idx)

  REG_UI_ENUM(ForEachContinue);
  REG_UI_ENUM(ForEachStop);

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

  REG_UI_ENUM(AttributeTypeFamily);
  REG_UI_ENUM(AttributeTypeSize);
  REG_UI_ENUM(AttributeTypeWeight);
  REG_UI_ENUM(AttributeTypeItalic);
  REG_UI_ENUM(AttributeTypeStretch);
  REG_UI_ENUM(AttributeTypeColor);
  REG_UI_ENUM(AttributeTypeBackground);
  REG_UI_ENUM(AttributeTypeUnderline);
  REG_UI_ENUM(AttributeTypeUnderlineColor);
  REG_UI_ENUM(AttributeTypeFeatures);

  REG_UI_ENUM(TextWeightMinimum);
  REG_UI_ENUM(TextWeightThin);
  REG_UI_ENUM(TextWeightUltraLight);
  REG_UI_ENUM(TextWeightLight);
  REG_UI_ENUM(TextWeightBook);
  REG_UI_ENUM(TextWeightNormal);
  REG_UI_ENUM(TextWeightMedium);
  REG_UI_ENUM(TextWeightSemiBold);
  REG_UI_ENUM(TextWeightBold);
  REG_UI_ENUM(TextWeightUltraBold);
  REG_UI_ENUM(TextWeightHeavy);
  REG_UI_ENUM(TextWeightUltraHeavy);
  REG_UI_ENUM(TextWeightMaximum);

  REG_UI_ENUM(TextItalicNormal);
  REG_UI_ENUM(TextItalicOblique);
  REG_UI_ENUM(TextItalicItalic);

  REG_UI_ENUM(TextStretchUltraCondensed);
  REG_UI_ENUM(TextStretchExtraCondensed);
  REG_UI_ENUM(TextStretchCondensed);
  REG_UI_ENUM(TextStretchSemiCondensed);
  REG_UI_ENUM(TextStretchNormal);
  REG_UI_ENUM(TextStretchSemiExpanded);
  REG_UI_ENUM(TextStretchExpanded);
  REG_UI_ENUM(TextStretchExtraExpanded);
  REG_UI_ENUM(TextStretchUltraExpanded);

  REG_UI_ENUM(UnderlineNone);
  REG_UI_ENUM(UnderlineSingle);
  REG_UI_ENUM(UnderlineDouble);
  REG_UI_ENUM(UnderlineSuggestion);

  REG_UI_ENUM(UnderlineColorCustom);
  REG_UI_ENUM(UnderlineColorSpelling);
  REG_UI_ENUM(UnderlineColorGrammar);
  REG_UI_ENUM(UnderlineColorAuxiliary);		// for instance); the color used by smart replacements on macOS or in Microsoft Office

  REG_UI_ENUM(DrawTextAlignLeft);
  REG_UI_ENUM(DrawTextAlignCenter);
  REG_UI_ENUM(DrawTextAlignRight);

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
