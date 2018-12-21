#!/usr/bin/env lua

local ui = require "lui"
local _,uv = pcall(require,'luv')
if not _ then
  print('not support thread here')
end

local e

local timeThread;

local function threadproc()
  local uv = require'luv'
  local ui = require'lui'

local function sayTime(time)
  print(time)
end

  for i=1,3 do
    uv.sleep(1000);
    local t = os.date()
    ui.QueueMain(sayTime,t)
  end
end

local function onClosing(w, data)
  if timeThread then
    timeThread:join()
  end
  ui.Quit()
  return true
end

local function saySomething(b,data)
  e:Append("Saying something\n");
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

  e = ui.NewMultilineEntry()
    :ReadOnly(true)

  btn = ui.NewButton("Say Something"):OnClicked(saySomething)
  b:Append(btn,false)
   :Append(e,true)
  w:OnClosing(onClosing):Show()
  timeThread = uv.new_thread(threadproc);
  ui.Main()
  ui.Uninit()
end

uiMain()
