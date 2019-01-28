# lui, libui bare bindings for lua

[libui](https://github.com/andlabs/libui) bare bindings for
[luajit](http://luajit.org/) and [lua](http://www.lua.org/)
[5.1](http://www.lua.org/manual/5.1/manual.html)/
[5.2](http://www.lua.org/manual/5.2/manual.html)/
[5.3](http://www.lua.org/manual/5.3/manual.html).

This binding makes libui available to lua scripts. libui is easy binding and should usable from nearly any lua project.

## Notice

- Because lua is missing native threads support, so threads feature maybe lost.
- Because libui/lui is not mature enough, you maybe encounter strange bugs.

## Examples

Examples can be found in the [examples](examples) folders, those are translate from [examples](https://github.com/andlabs/libui/tree/master/examples).

## Buliding

### Checkout source

```shell
  git checkout  https://github.com/zhaozg/lui.git
  cd lui
  git submodule init
  git submodule update
```

### Building on linux or macOS

```shell
  cd lui
  make install
```

### Building on Windows

TODO

### APIs

- All top level functions are in the table returned by `require`

```lua
  ui = require'lui'  -- type(ui) is 'table'
```

- All module level uiXXXX() functions are accessible as ui.XXXX(), they are include
  - `Init`
  - `Uninit`
  - `Main`
  - `MainStep`
  - `MainSteps`
  - `timer`
  - `Quit`
  - `QueueMain`
  - `OnShouldQuit`
  - `OpenFile`
  - `SaveFile`
  - `MsgBox`
  - `MsgBoxError`.

```lua
  assert(ui.Init())
  ui.Quit()
  ui.Uninit()
```

- New objects are created with ui.NewXXX() or ui.DrawNewXXXX(), the are include
  - `NewArea`
  - `NewButton`
  - `NewCheckbox`
  - `NewCombobox`
  - `NewColorButton`
  - `NewDateTimePicker`
  - `NewDatePicker`
  - `NewEditableCombobox`
  - `NewEntry`
  - `NewFontButton`
  - `NewForm`
  - `NewGrid`
  - `NewGroup`
  - `NewHorizontalBox`
  - `NewHorizontalSeparator`
  - `NewLabel`
  - `NewMenu`
  - `NewMultilineEntry`
  - `NewNonWrappingMultilineEntry`
  - `NewPasswordEntry`
  - `NewProgressBar`
  - `NewRadioButtons`
  - `NewSearchEntry`
  - `NewSlider`
  - `NewSpinbox`
  - `NewTimePicker`
  - `NewTab`
  - `NewVerticalBox`
  - `NewVerticalSeparator`
  - `NewWindow`
  - `DrawNewAreaHandler`
  - `DrawNewBrush`
  - `DrawNewPath`
  - `DrawNewMatrix`
  - `DrawNewStrokeParams`
  - `DrawNewTextLayout`
  - `DrawListFontFamilies` (Special)
  - `DrawLoadClosestFont` (Special)

```lua
  wnd = ui.NewWindow(...) -- type(wnd) is userdata with metatable 'libui.Window'
```

- All object perperties like uiTypeValue() and uiTypeSetValue() are accessible as obj:Value()

```lua
  entry:Padded(true)  -- just call uiEntrySetPadded(entry,1) and return object itself
  entry:Padded()      -- just call uiEntryPadded(entry) and return boolean type padded value
```

- Object Append() methods take a variable number of arguments, allowing things

```lua
  `ui.NewRadioButtons():Append("One", "Two", "Three")
```

- All object functions like uiTypeMethod() are accessible as obj:Method() calls

```lua
  wnd:Show()
  wnd:OnClosing(onclosing_cb)
```

You need read [examples](examples) to be familiar with them.

## Thanks

- [Ico Doornekamp](https://github.com/zevv) lui basicly forked of [libuilua](https://github.com/zevv/libuilua) but updated.
- [Pietro Gagliardi](https://github.com/andlabs) author of [libui](https://github.com/andlabs/libui).
