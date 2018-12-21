#!/usr/bin/env lua

local ui = require "lui"

local function onClosing(w)
  ui.Quit()
  return true
end

local function onShouldQuit(mw)
  ui.ControlDestroy(mw)
  return true
end

local function makeBasicControlsPage()
  local vbox, hbox, group, entryForm

  vbox = ui.NewVerticalBox():Padded(true)
  hbox = ui.NewHorizontalBox():Padded(true)

  vbox:Append(hbox,false)
  hbox:Append(ui.NewButton("Button"),false)
      :Append(ui.NewCheckbox("Checkbox"),false)

  vbox:Append(ui.NewLabel("This is a label. Right now, labels can only span one line."),false)
      :Append(ui.NewHorizontalSeparator(),false)

  group = ui.NewGroup("Entries"):Margined(true)
  vbox:Append(group,true)

  entryForm = ui.NewForm():Padded(true)
  group:SetChild(entryForm)
  entryForm:Append('Entry',ui.NewEntry(),false)
           :Append('Password Entry', ui.NewPasswordEntry(), false)
           :Append('Search Entry', ui.NewSearchEntry(), false)
           :Append('Multiline Entry', ui.NewMultilineEntry(), true)
  return vbox
end

local spinbox, slider, pbar

local function onSpinboxChanged(s, data)
  slider:Value(s:Value())
  pbar:Value(s:Value())
end

local function onSliderChanged(s, data)
  spinbox:Value(s:Value())
  pbar:Value(s:Value())
end

local function makeNumbersPage()
  local hbox, group, vbox, ip, cbox, ecbox, rb

  hbox = ui.NewHorizontalBox():Padded(true)
  group = ui.NewGroup("Numbers"):Margined(true)
  hbox:Append(group,true)

  vbox = ui.NewVerticalBox():Padded(true)
  group:SetChild(vbox)

  spinbox = ui.NewSpinbox(0, 100):OnChanged(onSpinboxChanged)
  slider = ui.NewSlider(0, 100):OnChanged(onSliderChanged);
  pbar = ui.NewProgressBar();
  vbox:Append(spinbox)
      :Append(slider)
      :Append(pbar)

  ip = ui.NewProgressBar():Value(-1)
  vbox:Append(ip)

  group = ui.NewGroup("Lists"):Margined(true)
  hbox:Append(group)

  vbox = ui.NewVerticalBox():Padded(true)
  group:SetChild(vbox)

  cbox = ui.NewCombobox()
        :Append("Combobox Item 1")
        :Append("Combobox Item 2")
        :Append("Combobox Item 3")
  vbox:Append(cbox,false)

  ecbox = ui.NewEditableCombobox()
        :Append("Editable Item 1")
        :Append("Editable Item 2")
        :Append("Editable Item 3")
  vbox:Append(ecbox,false)

  rb = ui.NewRadioButtons()
        :Append("Radio Button 1")
        :Append("Radio Button 2")
        :Append("Radio Button 3")
  vbox:Append(rb, false)
  return hbox
end

local mainwin

local function onOpenFileClicked(b, entry)
  local filename = ui.OpenFile(mainwin)
  if (filename == nil) then
    entry:Text("(cancelled)");
    return;
  end
  entry:Text(filename)
end

local function onSaveFileClicked(b, entry)
  local filename = ui.SaveFile(mainwin)
  if (filename == nil) then
    entry:Text("(cancelled)")
    return
  end
  entry:Text(filename)
end

local function onMsgBoxClicked(b)
  ui.MsgBox(mainwin,
    "This is a normal message box.",
    "More detailed information can be shown here.")
end

local function onMsgBoxErrorClicked(b)
  ui.MsgBoxError(mainwin,
    "This message box describes an error.",
    "More detailed information can be shown here.")
end

local function makeDataChoosersPage()
  local hbox, vbox, grid, button, entry, msggrid

  hbox = ui.NewHorizontalBox():Padded(true)
  vbox = ui.NewVerticalBox():Padded(true)
  hbox:Append(vbox, false)

  vbox:Append(ui.NewDatePicker(),false)
      :Append(ui.NewTimePicker(),false)
      :Append(ui.NewDateTimePicker(), false)
      :Append(ui.NewFontButton(),false)
      :Append(ui.NewColorButton(),false)

  hbox:Append(ui.NewVerticalSeparator(),false)

  vbox = ui.NewVerticalBox()
        :Padded(true)
  hbox:Append(vbox, true)

  grid = ui.NewGrid():Padded(true)
  vbox:Append(grid,false)

  button = ui.NewButton("Open File")
  entry = ui.NewEntry():ReadOnly(true)
  button:OnClicked(onOpenFileClicked, entry)
  grid:Append(button,
    0, 0, 1, 1,
    false, ui.AlignFill, false, ui.AlignFill)
  grid:Append(entry,
    1, 0, 1, 1,
    true, ui.AlignFill, false, ui.AlignFill)

  button = ui.NewButton("Save File")
  entry = ui.NewEntry():ReadOnly(true)
  button:OnClicked(onSaveFileClicked, entry);
  grid:Append(button,
    0, 1, 1, 1,
    false, ui.AlignFill, false, ui.AlignFill)
  grid:Append(entry,
    1, 1, 1, 1,
    true, ui.AlignFill, false, ui.AlignFill)

  msggrid = ui.NewGrid():Padded(true)
  grid:Append(msggrid,
    0, 2, 2, 1,
    false, ui.AlignCenter, false, ui.AlignStart)

  button = ui.NewButton("Message Box"):OnClicked(onMsgBoxClicked)
  msggrid:Append(button,
    0, 0, 1, 1,
    false, ui.AlignFill, false, ui.AlignFill);
  button = ui.NewButton("Error Box"):OnClicked(onMsgBoxErrorClicked)
  msggrid:Append(button,
    1, 0, 1, 1,
    false, ui.AlignFill, false, ui.AlignFill);

  return hbox
end

local function uiMain()
  local ret,err = ui.Init();
  if (not ret) then
    io.stderr:write(string.format("error initializing libui: %s", err));
    return 1;
  end

  mainwin = ui.NewWindow("libui Control Gallery", 640, 480, true)
    :OnClosing(onClosing);
  ui.OnShouldQuit(onShouldQuit, mainwin);

  tab = ui.NewTab();
  mainwin:SetChild(tab):Margined(true)
  tab:Append("Basic Controls", makeBasicControlsPage()):Margined(0, true);
  tab:Append("Numbers and Lists", makeNumbersPage()):Margined(1, true)
  tab:Append("Data Choosers", makeDataChoosersPage()):Margined(2, true)

  mainwin:Show()
  ui.Main()
  ui.Uninit()

  return 0;
end

uiMain()
