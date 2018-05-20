#!/usr/bin/lua

local ui = require "lui"
local steps = true
ui.Init()

i=0
ui.Timer(1000, function(i) 
  print(i, os.date())
  return 1
end, i)

local spinbox, slider, progressbar

local function update(control)
  local v = control:Value()
  spinbox:Value(v)
  slider:Value(v)
  progressbar:Value(v)
end

spinbox = ui.NewSpinbox(0, 100):OnChanged(update)
slider = ui.NewSlider(0, 100):OnChanged(update)
progressbar = ui.NewProgressBar(0, 100)


local win = ui.NewWindow("Hello", 320, 200, false)
  :Margined(true)
  :SetChild(
    ui.NewVerticalBox()
    :Append(
      ui.NewHorizontalBox()
      :Append(
	ui.NewGroup("Basic Controls")
	:Margined(true)
	:SetChild(
          ui.NewVerticalBox()
	  :Padded(true)
	  :Append(
            ui.NewButton("Button"),
            ui.NewCheckbox("Checkbox"),
            ui.NewLabel("Label"),
            ui.NewHorizontalSeparator(),
            ui.NewDatePicker(),
            ui.NewDateTimePicker(),
            ui.NewTimePicker()
          )
        ),
	ui.NewVerticalBox()
	:Append(
          ui.NewGroup("Numbers")
	  :Margined(true)
	  :SetChild(
            ui.NewVerticalBox()
	      :Padded(true)
	      :Append(spinbox, slider, progressbar)
	  )
        )
	:Append(
          ui.NewGroup("Lists")
	  :Margined(true)
	  :SetChild(
            ui.NewVerticalBox()
	    :Padded(true)
	    :Append(
              ui.NewCombobox()
	      :Append(
                "Combobox Item 1",
                "Combobox Item 2",
                "Combobox Item 3"
              ),
              ui.NewEditableCombobox()
	      :Append(
                "Editable Item 1",
                "Editable Item 2",
                "Editable Item 3"
              ),
              ui.NewRadioButtons()
	      :Append(
                "Radio Button 1",
                "Radio Button 2",
                "Radio Button 3"
              )
            )
            :Append(
              ui.NewTab():Append(
                "Page 1", ui.NewHorizontalBox(),
                "Page 2", ui.NewHorizontalBox(),
                "Page 3", ui.NewHorizontalBox()
              ), true
            )
          ), true
        ), true
      ), true
    )
  )

win:Show()


if (not steps) then
  ui.Main()
else
  ui.MainSteps()
  while ui.MainStep(true) do
  end
end
