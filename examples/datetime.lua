local ui = require'lui'

local dtboth, dtdate, dttime
local function timeFormat(d)
  local fmt
  if (d == dtboth) then
    fmt = "%c"
  elseif (d == dtdate) then
    fmt = "%x"
  elseif (d == dttime) then
    fmt = "%X"
  else
    fmt = "%c"
  end
  return fmt
end

local function onChanged(d, l)
  local tm = d:Time()
  local s = os.time(tm)
  s = os.date(timeFormat(d), s)
  l:Text(s)
end

local function onClicked(b, data)
  if (data) then
    data = os.time()
  end
  local tm = os.date(data)

  dtdate:SetTime(tm)
  dttime:SetTime(tm)
end

function onClosing(w, data)
  ui.Quit()
  return 1
end

function main()
  local err, w, g, l, b
  ui.Init()

  w = ui.NewWindow("Date / Time", 320, 240, false)
    :Margined(true)

  g = ui:NewGrid():Padded(true)
  w:SetChild(g)

  dtboth = ui.NewDateTimePicker()
  dtdate = ui.NewDatePicker()
  dttime = ui.NewTimePicker()

  g:Append(dtboth,
    0, 0, 2, 0,
    true, ui.AlignFill, false, ui.AlignFill)
  g:Append(dtdate,
    0, 1, 1, 1,
    true, ui.AlignFill, false, ui.AlignFill)
  g:Append(dttime,
    1, 1, 1, 1,
    true, ui.AlignFill, false, ui.AlignFill)

  l = ui.NewLabel("")
  g:Append(l,
    0, 2, 2, 1,
    true, ui.AlignFill, false, ui.AlignFill)
  dtboth:OnChanged(onChanged, l)

  l = ui.NewLabel("")
  g:Append(l,
    0, 3, 1, 1,
    true, ui.AlignCenter, false, ui.AlignFill)
  dtdate:OnChanged(onChanged, l)

  l = ui.NewLabel("")
  g:Append(l,
    1, 3, 1, 1,
    true, ui.AlignCenter, false, ui.AlignFill)
  dttime:OnChanged(onChanged, l)

  b = ui.NewButton("Now")
  b:OnClicked(onClicked, 1)
  g:Append(b,
    0, 4, 1, 1,
    true, ui.AlignFill, true, ui.AlignEnd)
  b = ui.NewButton("Unix epoch")
  b:OnClicked(onClicked, 0)
  g:Append(b,
    1, 4, 1, 1,
    true, ui.AlignFill, true, ui.AlignEnd)
  w:OnClosing(w, onClosing):Show()
  ui.Main()
  ui.Uninit()
  return 0
end

main()
