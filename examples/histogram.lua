#!/usr/bin/env lua

local ui = require "lui"
local bit = require'bit'

local mainwin, histogram, handler, colorButton
local datapoints = {}
local currentPoint = nil

local xoffLeft    = 20      --/* histogram margins */
local yoffTop     = 20
local xoffRight   = 20
local yoffBottom  = 20
local pointRadius = 5

local function setSolidBrush(brush, color, alpha)
  local component, r,g,b

  r = bit.band(bit.rshift(color,16),0xFF)/255
  g = bit.band(bit.rshift(color,8),0xFF)/255
  b = bit.band(color, 0xFF)/255
  brush:Type(ui.DrawBrushTypeSolid)
       :Solid(r,g,b,alpha)
end

-- and some colors
-- names and values from https:--msdn.microsoft.com/en-us/library/windows/desktop/dd370907%28v=vs.85%29.aspx
local colorWhite      = 0xFFFFFF
local colorBlack      = 0x000000
local colorDodgerBlue = 0x1E90FF

local function pointLocations(width, height)
  local xincr, yincr;
  local i, n;
  local xs,ys = {},{}

  xincr = width / 9;    -- 10 - 1 to make the last point be at the end
  yincr = height / 100;

  for i = 1,10 do
    ---- get the value of the point
    n = datapoints[i]:Value()
    ---- because y=0 is the top but n=0 is the bottom, we need to flip
    n = 100 - n
    xs[i] = xincr * (i-1);
    ys[i] = yincr * n;
  end
  return xs,ys
end

local function constructGraph(width, height, extend)
  local xs,ys = pointLocations(width, height);
  local path = ui.DrawNewPath(ui.DrawFillModeWinding);

  path:NewFigure(xs[1], ys[1]);
  for i=1,10 do
    path:LineTo(xs[i], ys[i]);
  end
  path:LineTo(xs[10], ys[10]);

  if extend then
    path:LineTo(width, height);
    path:LineTo(0, height);
    path:CloseFigure();
  end

  path:End();
  return path;
end

local function graphSize(clientWidth, clientHeight)
  return clientWidth - xoffLeft - xoffRight,
	 clientHeight - yoffTop - yoffBottom
end

local function handlerDraw(a, area, p)
  local path,brush,sp,m,graphWidth, graphHeight,graphR, graphG, graphB, graphA

  -- fill the area with white
  brush = ui.DrawNewBrush()
  setSolidBrush(brush, colorWhite, 1.0)
  path = ui.DrawNewPath(ui.DrawFillModeWinding);
  path:AddRectangle(0, 0, p.AreaWidth, p.AreaHeight):End(path);
  p.Context:Fill(path, brush);
  path:Free()

  -- figure out dimensions
  graphWidth, graphHeight = graphSize(p.AreaWidth, p.AreaHeight);

  -- clear sp to avoid passing garbage to uiDrawStroke()
  -- for example, we don't use dashing
  sp = ui.DrawNewStrokeParams();

  -- make a stroke for both the axes and the histogram line
  sp:Cap(ui.DrawLineCapFlat)
  sp:Join(ui.DrawLineJoinMiter)
  sp:Thickness(2)
  sp:MiterLimit(ui.DrawDefaultMiterLimit)

  -- draw the axes
  brush = ui.DrawNewBrush()
  setSolidBrush(brush, colorBlack, 1.0)
  path = ui.DrawNewPath(ui.DrawFillModeWinding);
  path:NewFigure(xoffLeft, yoffTop)
      :LineTo(xoffLeft, yoffTop + graphHeight)
      :LineTo(xoffLeft + graphWidth, yoffTop + graphHeight)
      :End()
  p.Context:Stroke(path, brush, sp);
  path:Free()

  -- now transform the coordinate space so (0, 0) is the top-left corner of the graph

  m = ui.DrawNewMatrix()
  m:SetIdentity():Translate(xoffLeft, yoffTop);
  p.Context:Transform(m);

  -- now get the color for the graph itself and set up the brush
  graphR, graphG, graphB, graphA = colorButton:Color();
  brush:Type(ui.DrawBrushTypeSolid);
  -- we set brush->A below to different values for the fill and stroke

  -- now create the fill for the graph below the graph line

  path = constructGraph(graphWidth, graphHeight, true);
  brush:Solid(graphR,graphG,graphB,graphA / 2)
  p.Context:Fill(path, brush);
  path:Free()

  -- now draw the histogram line
  path = constructGraph(graphWidth, graphHeight, false);
  brush:Solid(graphR,graphG,graphB,graphA)
  p.Context:Stroke(path, brush, sp);
  path:Free()

  -- now draw the point being hovered over
  if (currentPoint) then
    local xs, ys = pointLocations(graphWidth, graphHeight);
    path = ui.DrawNewPath(ui.DrawFillModeWinding);
    path:NewFigureWithArc(
      xs[currentPoint], ys[currentPoint],
      pointRadius,
      0, 6.23,    -- TODO pi
      false);
    path:End()
    -- use the same brush as for the histogram lines
    p.Context:Fill(path, brush);
    path:Free()
    currentPoint = nil
  end
end

local function inPoint(x, y, xtest, ytest)
  -- TODO switch to using a matrix
  x = x - xoffLeft;
  y = y - yoffTop;
  return (x >= xtest - pointRadius) and
    (x <= xtest + pointRadius) and
    (y >= ytest - pointRadius) and
    (y <= ytest + pointRadius);
end

local function handlerMouseEvent(a, area, e)
  local graphWidth, graphHeight
  local xs,ys = {}, {}

  graphWidth, graphHeight = graphSize(e.AreaWidth, e.AreaHeight);
  xs, ys = pointLocations(graphWidth, graphHeight);
  for i=1,10  do
    if (inPoint(e.X, e.Y, xs[i], ys[i])) then
        currentPoint = i
        break;
    end
  end

  -- TODO only redraw the relevant area
  histogram:QueueRedrawAll();
end

local function handlerMouseCrossed(ah, a, left)
  -- do nothing
end

local function handlerDragBroken(ah, a)
  -- do nothing
end

local function handlerKeyEvent(ah, a, e)
  -- reject all keys
  return 0;
end

local function  onDatapointChanged(s, data)
  histogram:QueueRedrawAll();
end

local function onColorChanged(b)
  histogram:QueueRedrawAll();
end

local function onClosing(w)
  mainwin:Destroy()
  ui.Quit()
  return 0
end

local function shouldQuit()
  ui.ControlDestroy(mainwin)
  return true
end

local function uiMain()
  local ret,err = ui.Init();
  if (not ret) then
    io.stderr:write(string.format("error initializing libui: %s", err));
    return 1;
  end

  local hbox,vbox,brush;

  handler = ui.DrawNewAreaHandler(
	handlerDraw,
	handlerMouseEvent,
	handlerMouseCrossed,
	handlerDragBroken,
	handlerKeyEvent)

  ui.OnShouldQuit(shouldQuit);

  mainwin = ui.NewWindow("libui Histogram Example", 640, 480, true)
    :Margined(true)
    :OnClosing(onClosing);

  hbox = ui.NewHorizontalBox():Padded(true)
  mainwin:SetChild(hbox)

  vbox = ui.NewVerticalBox():Padded(true)
  hbox:Append(vbox, 0)

  math.randomseed(os.time())
  for i=1,10 do
    datapoints[i] = ui.NewSpinbox(0, 100)
      :Value(math.random(0,100))
      :OnChanged(onDatapointChanged)
    vbox:Append(datapoints[i])
  end

  colorButton = ui.NewColorButton()
  ---- TODO inline these
  brush = ui.DrawNewBrush()
  setSolidBrush(brush, colorDodgerBlue, 1.0)
  colorButton:OnChanged(onColorChanged):Color(brush:Solid())
  vbox:Append(colorButton)

  histogram = ui.NewArea(handler)
  hbox:Append(histogram, true);
  mainwin:Show()
  ui.Main();

  ui.Uninit();
  return 0
end

uiMain()
