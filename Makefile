T=lui

PREFIX		?=/usr/local
CC		:= $(CROSS)$(CC)
AR		:= $(CROSS)$(AR)
LD		:= $(CROSS)$(LD)

#OS auto detect
ifneq (,$(TARGET_SYS))
SYS		:= $(TARGET_SYS)
else
SYS		:= $(shell gcc -dumpmachine)
endif

#Lua auto detect
ifneq (iOS,$SYS)
LUA_VERSION ?= $(shell pkg-config luajit --print-provides)
ifeq ($(LUA_VERSION),)
############ use lua
LUAV		?= $(shell lua -e "_,_,v=string.find(_VERSION,'Lua (.+)');print(v)")
LUA_CFLAGS	?= -I$(PREFIX)/include/lua$(LUAV)
LUA_LIBS	?= -L$(PREFIX)/lib 
LUA_LIBDIR	?= $(PREFIX)/lib/lua/$(LUAV)
else
############ use luajit
LUAV		?= $(shell luajit -e "_,_,v=string.find(_VERSION,'Lua (.+)');print(v)")
LUA_CFLAGS	?= $(shell pkg-config luajit --cflags)
LUA_LIBS	?= $(shell pkg-config luajit --libs)
LUA_LIBDIR	?= $(PREFIX)/lib/lua/$(LUAV)
endif
else
LUA_CFLAGS	?= $(shell pkg-config luajit --cflags)
LUA_LIBS	?= $(shell pkg-config luajit --libs)
endif

#libui auto detect
LIBUI_PREFIX	?= /usr/local
LIBUI_CFLAGS	?= -I$(LIBUI_PREFIX)/include
LIBUI_LIBS	?= -L$(LIBUI_PREFIX)/lib -lui

ifneq (, $(findstring linux, $(SYS)))
# Do linux things
CFLAGS		 = -fpic 
LDFLAGS		 = -Wl,--no-undefined -fpic -lrt -ldl -lm
endif
ifneq (, $(findstring apple, $(SYS)))
# Do darwin things
CFLAGS		 = -fPIC
LDFLAGS		 = -fPIC -undefined dynamic_lookup -ldl
MACOSX_DEPLOYMENT_TARGET="10.8"
CC := MACOSX_DEPLOYMENT_TARGET=${MACOSX_DEPLOYMENT_TARGET} $(CC)
endif
ifneq (, $(findstring mingw, $(SYS)))
# Do mingw things
CFLAGS		= -DLUA_LIB -DLUA_BUILD_AS_DLL -DWIN32_LEAN_AND_MEAN
endif
ifneq (, $(findstring cygwin, $(SYS)))
# Do cygwin things
CFLAGS		 = -fPIC 
endif
ifneq (, $(findstring iOS, $(SYS)))
# Do iOS things
CFLAGS           = -fPIC 
LDFLAGS		 = -fPIC -ldl
endif

#custom config
ifeq (.config, $(wildcard .config))
include .config
endif

LIBNAME= $T.so.$V
LUA_CFLAGS      += -Ideps/lua-compat-5.3
CFLAGS		+= $(LIBUI_CFLAGS) $(LUA_CFLAGS) $(TARGET_FLAGS)
LDFLAGS		+= -shared $(LIBUI_LIBS) $(LUA_LIBS)
# Compilation directives
WARN_MIN	 = -Wall -Wno-unused-value
WARN		 = -Wall
WARN_MOST	 = $(WARN) -W -Waggregate-return -Wcast-align -Wmissing-prototypes -Wnested-externs -Wshadow -Wwrite-strings -pedantic
CFLAGS		+= -g $(WARN_MIN) -DPTHREADS -Ideps -Ideps/compat -Ideps/auxiliar 

all: $T.so
	@echo "Target system: "$(SYS)

$T.so: src/*.c deps/auxiliar/auxiliar.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ src/lui.c deps/auxiliar/auxiliar.c

install: all
	mkdir -p $(LUA_LIBDIR)
	cp $T.so $(LUA_LIBDIR)

info:
	@echo "Target system: "$(SYS)
	@echo "CC:" $(CC)
	@echo "AR:" $(AR)
	@echo "PREFIX:" $(PREFIX)

clean:
	rm -rf $T.so*

# vim: ts=8 sw=8 noet
