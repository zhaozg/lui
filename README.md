lui
===

[libui](https://github.com/andlabs/libui) bare bindings for
[luajit](http://luajit.org/) and [lua](http://www.lua.org/)
[5.1](http://www.lua.org/manual/5.1/manual.html)/
[5.2](http://www.lua.org/manual/5.2/manual.html)/
[5.3](http://www.lua.org/manual/5.3/manual.html).

This binding makes libui available to lua scripts. libui is easy binding and should usable from nearly any lua project.

Notice
======

  - Because lua is missing native threads support, so threads feature maybe lost.
  
  - Because libui/lui is not mature enough, you maybe encounter strange bugs.

Examples
========

Examples can be found in the [examples](examples) folders, those are translate from [examples](https://github.com/andlabs/libui/tree/master/examples).

Buliding
========

Now, I not make any build scripts or Makefile, so you need to write it yourself.

On linux, assume cwd is luv folder.

```shell
gcc -shared -fms-extensions -DLUI_MESSAGE_DIALOGS="" -Isrc src/lui.c -I{LUAINCDIR} -I{LIBUIINCDIR} -lui -llua-5.3 -o lui.so
```

On Mac, 

```shell
gcc  -o lui.so -fms-extensions  -DLUI_MESSAGE_DIALOGS="" -shared  -framework Foundation -framework AppKit -fPIC -Isrc src/lui.c -I {LUAINCDIR}  -I {LIBUIINCDIR}  -L .  -lui -undefined dynamic_lookup
```

On Windows, (Example is shown below, modify it accordingly)

Note: you need to use "Developer Command Prompt for Visual Studio" to run any commands.

Complete Build Steps:

```
1. Install Git, cmake, Visual Studio 2017;
2. mkdir andlabs;
3. cd andlabs
4. git clone https://github.com/andlabs/libui
5. Run x64 Native VS 2017 tools
6. cd andlabs
7. mkdir buildx86_64
8. cd buildx86_64
9. cmake .. -DCMAKE_INSTALL_PREFIX=”C:\usr\lib\andlabs-libui”
-DBUILD_SHARED_LIBS=OFF
10. Open libui.sln with VS2017 and build static library.
11. Create new configuration x64 (copy settings from Win32).
12. Go to Linker setting and Change Additional options: /machinex64
13. Build and run cpp-multithread: SUCCESS
14. Download and compile Lua 5.3.4 lua-5.3.4.tar.gz
15. cd lua-5.3.4\src
16. cl /MD /O2 /c /DLUA_BUILD_AS_DLL *.c
17. ren lua.obj lua.o
18. ren luac.obj luac.o
19. link /DLL /IMPLIB:lua5.3.4.lib /OUT:lua5.3.4.dll *.obj
20. link /OUT:lua.exe lua.o lua5.3.4.lib
21. lib /OUT:lua5.3.4-static.lib *.obj
22. link /OUT:luac.exe luac.o lua5.3.4-static.lib
23. SUCCESS
24. Create new VS2017 project named zhaozg-lui
25. Append lui.c file in lui\src folder and select x64 configuration
26. Disable precompiled headers in project properties.
27. Resolve include directories.
28. Build.
29. SUCCESS (except uiMsgBoxError in lui.c lwindow.c and other defined calls with
LUI_MESSAGE_DIALOGS)
```

You can use the following command to do the final compile (Example is given below as it is easy to follow)


```shell
cl.exe src/lui.c -I..\lua-5.3.4\src -I..\libui /U"LUA_BUILD_AS_DLL" /RTC1 /GR /Gd /MDd /FC /U"_UI_STATIC" /D "_DEBUG" /D "ZHAOZGLUI_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "WIN32" /D "_UI_STATIC" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /LD /link /LIBPATH:..\libui\build\x64\Release /LIBPATH:..\lua-5.3.4\src "libui.lib" "Comctl32.lib" "UxTheme.lib" "d2d1.lib" "Dwrite.lib" "libui.lib" "lua5.3.4.lib" "user32.lib" "kernel32.lib" "gdi32.lib" "uxtheme.lib" "msimg32.lib" "comdlg32.lib" "dwrite.lib" "ole32.lib" "oleaut32.lib" "oleacc.lib" "uuid.lib" "winspool.lib" "advapi32.lib" "shell32.lib" "odbc32.lib" "odbccp32.lib" /DEBUG /DLL /MACHINE:X64 /TLBID:1 /SUBSYSTEM:CONSOLE /DYNAMICBASE /OUT:lui.dll
```

Note: MessageBox and ErroBox are not working in Windows(Any contributions here would be useful).

APIs
====

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
  - `DrawLoadClosestFont`  (Special)

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

Thanks
======

  - [Ico Doornekamp](https://github.com/zevv) [lui](https://github.com/zhaozg/lui) basicly forked of [libuilua](https://github.com/zevv/libuilua) but updated.
  
  - [Pietro Gagliardi](https://github.com/andlabs) author of [libui](https://github.com/andlabs/libui).
