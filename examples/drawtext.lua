local ui = require('lui')

local mainwin, area, handler, fontButton, alignment, attrstr

local function appendWithAttribute(what, attr, attr2)
	local start, ends

	start = attrstr:Len()
	ends = start + #what
	attrstr:AppendUnattributed(what)
	attrstr:SetAttribute(attr, start, ends)
	if (attr2 ) then
		attrstr:SetAttribute(attr2, start, ends)
  end
end

local function makeAttributedString()
	local attr, attr2, otf

	attrstr = ui.NewAttributedString([[
Drawing strings with libui is done with the attrstr: and uiDrawTextLayout objects.
attrstr: lets you have a variety of attributes: ]])

	attr = ui.NewAttribute("family", "Courier New")
	appendWithAttribute("font family", attr)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("size", 18)
	appendWithAttribute("font size", attr, NULL)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("weight", ui.TextWeightBold)
	appendWithAttribute("font weight", attr)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("italic", ui.TextItalicItalic)
	appendWithAttribute("font italicness", attr)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("stretch", ui.TextStretchCondensed)
	appendWithAttribute("font stretch", attr)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("color", 0.75, 0.25, 0.5, 0.75)
	appendWithAttribute("text color", attr)
	attrstr:AppendUnattributed(", ")

	attr = ui.NewAttribute("background", 0.5, 0.5, 0.25, 0.5)
	appendWithAttribute("text background color", attr)
	attrstr:AppendUnattributed(", ")


	attr = ui.NewAttribute("underline", ui.UnderlineSingle)
	appendWithAttribute("underline style", attr)
	attrstr:AppendUnattributed(", ")

	attrstr:AppendUnattributed("and ")
	attr = ui.NewAttribute("underline", ui.UnderlineDouble)
	attr2 = ui.NewAttribute("underlinecolor", ui.UnderlineColorCustom, 1.0, 0.0, 0.5, 1.0)
	appendWithAttribute("underline color", attr, attr2)
	attrstr:AppendUnattributed(". ")

	attrstr:AppendUnattributed("Furthermore, there are attributes allowing for ")
	attr = ui.NewAttribute("underline", ui.UnderlineSuggestion)
	attr2 = ui.NewAttribute("underlinecolor", ui.UnderlineColorSpelling, 0, 0, 0, 0)
	appendWithAttribute("special underlines for indicating spelling errors", attr, attr2)
	attrstr:AppendUnattributed(" (and other types of errors) ")

	attrstr:AppendUnattributed("and control over OpenType features such as ligatures (for instance, ")

  local l, i, g, a = string.byte('liga',1,4)
	otf = ui.NewOpenTypeFeatures()
	otf:Add(l, i, g, a, 0)
	attr = ui.NewAttribute('features', otf)
	appendWithAttribute("afford", attr)
	attrstr:AppendUnattributed(" vs. ")
	otf:Add(l, i, g, a, 1)
	attr = ui.NewAttribute('features', otf)
	appendWithAttribute("afford", attr)
	otf:Free()
	attrstr:AppendUnattributed(").\n")

	attrstr:AppendUnattributed("Use the controls opposite to the text to control properties of the text.")
end

function handlerDraw(a, area, p)
  local textLayout, defaultFont, params

  defaultFont = fontButton:Font()
  print(defaultFont)
  params = ui.DrawNewTextLayoutParams(attrstr, defaultFont, p.AreaWidth, alignment:Selected() )
	textLayout = ui.DrawNewTextLayout(params)
  p.Context:Text(textLayout, 0, 0)
  textLayout:Free()
  defaultFont:Free()
end

function handlerMouseEvent(a, area, e)
  print('MouseEvent')
end

function handlerMouseCrossed(ah, a, left)
  print('MouseCrossed')
end

function handlerDragBroken(ah, a)
  print('DragBroken')
end

function handlerKeyEvent(ah, a, e)
  print('KeyEvent')
	return 0
end

function onFontChanged(b, data)
	area:QueueRedrawAll()
end

function onComboboxSelected(b, data)
	area:QueueRedrawAll()
end

function onClosing(w, data)
	mainwin:Destroy()
	ui.Quit()
	return 0
end

function shouldQuit(data)
	mainwin:Destroy()
	return 1
end

function main()
  local err, hbox, vbox, form

  handler = ui.DrawNewAreaHandler(
	handlerDraw,
	handlerMouseEvent,
	handlerMouseCrossed,
	handlerDragBroken,
	handlerKeyEvent)

  ui.Init()
	ui.OnShouldQuit(shouldQuit)

	makeAttributedString()

	mainwin = ui.NewWindow("libui Text-Drawing Example", 640, 480, true)
    :Margined(true)
    :OnClosing(onClosing)

	hbox = ui.NewHorizontalBox()
    :Padded(true)
  mainwin:SetChild(hbox)

	vbox = ui.NewVerticalBox():Padded(true)
  hbox:Append(vbox, false)

	fontButton = ui.NewFontButton()
    :OnChanged(onFontChanged)
  vbox:Append(fontButton, false)

	form = ui.NewForm():Padded(true)
  --TODO on OS X if this is set to 1 then the window can't resize; does the form not have the concept of stretchy trailing space?
  vbox:Append(from, false)

	alignment = ui.NewCombobox()
	-- note that the items match with the values of the uiDrawTextAlign values
  alignment:Append("Left")
    :Append("Center")
    :Append("Right")
    :Selected(0)
    :OnSelected(onComboboxSelected)

	form:Append("Alignment", alignment, false)

	area = ui.NewArea(handler)
	hbox:Append(area, true)

	mainwin:Show()
	ui.Main()
  attrstr:Free()
	ui.Uninit()
	return 0
end

main()

