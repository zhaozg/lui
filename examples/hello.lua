#!/usr/bin/env lua

local ui = require "lui"
local function onClosing(w, data)
  ui.Quit()
  return true
end

function uiMain(void)
  --init
  local ret, err = ui.Init()
  if not ret then
    error(err)
  end

  local w = ui.NewWindow("Hello", 320, 240, false)
 -- w:OnClosing(onClosing)
  w:Show()
  ui.Main()
  ui.Uninit()
end

uiMain()
