#!/usr/bin/env lua

local ui = require "lui"
local mle = nil

local function sayTime()
  local s = os.date()
  mle:Append(s..'\n')
  return false
end

local function onClosing(w, data)
  ui.Quit()
  return true
end

local function saySomething(b,data)
  mle:Append("Saying something\n");
end

function uiMain(void)
  local w,b,btn
  local ret, err = ui.Init()
  if not ret then
    error(err)
  end

  b = ui.NewVerticalBox():Padded(true)
  w = ui.NewWindow("Hello", 320, 240, false)
    :Margined(true)
    :SetChild(b)

  mle = ui.NewMultilineEntry():ReadOnly(true)

  btn = ui.NewButton("Say Something"):OnClicked(saySomething)
  b:Append(btn,false)
   :Append(mle,true)
  w:OnClosing(onClosing):Show()
  ui.Timer(1000, sayTime)
  ui.Main()
end

uiMain()
