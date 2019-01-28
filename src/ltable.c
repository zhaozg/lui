#include "lui.h"

// uiTableValue stores a value to be passed along uiTable and
// uiTableModel.
//
// uiTableValues are immutable and the uiTableModel and uiTable
// take ownership of the uiTableValue object once returned, copying
// its contents as necessary.

static int l_uiTableValueNew(lua_State *L)
{
  uiTableValue *v = NULL;
  int top = lua_gettop(L);
  if (top == 1)
  {
    int typ = lua_type(L, 1);
    switch (typ)
    {
    case LUA_TNUMBER:
    {
      int iv = lua_tointeger(L, 1);
      v = uiNewTableValueInt(iv);
      break;
    }
    case LUA_TSTRING:
    {
      const char *sv = lua_tostring(L, 1);
      v = uiNewTableValueString(sv);
      break;
    }
    case LUA_TUSERDATA:
    {
      uiImage *img = CHECK_USER_OBJECT(1, Image);
      v = uiNewTableValueImage(img);
      break;
    }
    default:
      luaL_error(L, "only support number, string, uiImage when only 1 arg");
      break;
    }
  }
  else if (top == 4)
  {
    double r = luaL_checknumber(L, 1);
    double g = luaL_checknumber(L, 2);
    double b = luaL_checknumber(L, 3);
    double a = luaL_checknumber(L, 4);
    v = uiNewTableValueColor(r, g, b, a);
  }
  CREATE_USER_OBJECT(TableValue, v);
  return 1;
}

static int l_uiTableValueFree(lua_State *L)
{
  uiTableValue *v = CHECK_USER_OBJECT(1, TableValue);
  uiFreeTableValue(v);
  return 0;
}

static int l_uiTableValueGetType(lua_State *L)
{
  const uiTableValue *v = CHECK_USER_OBJECT(1, TableValue);
  uiTableValueType typ = uiTableValueGetType(v);
  lua_pushinteger(L, typ);
  return 1;
}

static int l_uiTableValueValue(lua_State *L)
{
  int ret = 1;
  const uiTableValue *v = CHECK_USER_OBJECT(1, TableValue);
  uiTableValueType typ = uiTableValueGetType(v);
  switch (typ)
  {
  case uiTableValueTypeString:
  {
    const char *vs = uiTableValueString(v);
    lua_pushstring(L, vs);
    break;
  }
  case uiTableValueTypeInt:
  {
    int iv = uiTableValueInt(v);
    lua_pushinteger(L, iv);
    break;
  }
  case uiTableValueTypeColor:
  {
    double r, g, b, a;
    uiTableValueColor(v, &r, &g, &b, &a);
    ret = 4;
    break;
  }
  case uiTableValueTypeImage:
  {
    uiImage *img = uiTableValueImage(v);
    CREATE_USER_OBJECT(Image, img);
    break;
  }
  default:
    lua_pushnil(L);
  }
  return ret;
}

static struct luaL_Reg meta_TableValue[] =
{
  { "GetType", l_uiTableValueGetType },
  { "Value", l_uiTableValueValue },
  { "Free", l_uiTableValueFree },
  { "__gc", l_uiTableValueFree },

  { NULL, NULL}
};

static int l_uiNewTableModelHandler(lua_State *L)
{
  uiTableModelHandler *handler = (uiTableModelHandler *)malloc(sizeof(uiTableModelHandler));
  CREATE_USER_OBJECT(TableModelHandler, handler);
  return 1;
}

static int l_uiNewTableModel(lua_State *L)
{
  uiTableModelHandler *mh = CHECK_USER_OBJECT(1, TableModelHandler);
  uiTableModel *model = uiNewTableModel(mh);
  CREATE_USER_OBJECT(TableModel, model);
  return 1;
}

static int l_TableModleHandlerNumColumns(lua_State *L)
{
  uiTableModelHandler *h = CHECK_USER_OBJECT(1, TableModelHandler);
  uiTableModel *m = CHECK_USER_OBJECT(2, TableModel);
  int ret = h->NumColumns(h, m);
  lua_pushinteger(L, ret);
  return 1;
}

static int l_TableModleHandlerColumnType(lua_State *L)
{
  uiTableModelHandler *h = CHECK_USER_OBJECT(1, TableModelHandler);
  uiTableModel *m = CHECK_USER_OBJECT(2, TableModel);
  int col = luaL_checkinteger(L, 3);
  int ret = h->ColumnType(h, m, col);
  lua_pushinteger(L, ret);
  return 1;
}

static int l_TableModleHandlerNumRows(lua_State *L)
{
  uiTableModelHandler *h = CHECK_USER_OBJECT(1, TableModelHandler);
  uiTableModel *m = CHECK_USER_OBJECT(2, TableModel);
  int ret = h->NumRows(h, m);
  lua_pushinteger(L, ret);
  return 1;
}

static int l_TableModleHandlerCellValue(lua_State *L)
{
  uiTableModelHandler *h = CHECK_USER_OBJECT(1, TableModelHandler);
  uiTableModel *m = CHECK_USER_OBJECT(2, TableModel);
  int row = luaL_checkinteger(L, 3);
  int col = luaL_checkinteger(L, 4);
  uiTableValue *val;
  if (lua_isnone(L, 5)==0)
  {
    val = h->CellValue(h, m, row, col);
    CREATE_USER_OBJECT(TableValue, val);
  }
  else
  {
    val = CHECK_USER_OBJECT(5, TableValue);
    h->SetCellValue(h, m, row, col, val);
    lua_pushvalue(L, 5);
  }
  return 1;
}

static struct luaL_Reg meta_TableModelHandler[] =
{
  { "NewTableModel", l_uiNewTableModel },
  { "NumColumns",    l_TableModleHandlerNumColumns },
  { "ColumnType",    l_TableModleHandlerColumnType },
  { "NumRows",       l_TableModleHandlerNumRows },
  { "CellValue",     l_TableModleHandlerCellValue },

  { NULL, NULL }
};

static int l_uiFreeTableModel(lua_State *L)
{
  uiTableModel *m = CHECK_USER_OBJECT(1, TableModel);
  uiFreeTableModel(m);
  return 0;
}

static int l_uiTableModelRowInserted(lua_State *L)
{
  uiTableModel *m = CHECK_USER_OBJECT(1, TableModel);
  int newIndex = luaL_checkinteger(L, 2);
  uiTableModelRowInserted(m, newIndex);
  UI_RETURN_SELF;
}

static int l_uiTableModelRowChanged(lua_State *L)
{
  uiTableModel *m = CHECK_USER_OBJECT(1, TableModel);
  int index = luaL_checkinteger(L, 2);
  uiTableModelRowChanged(m, index);
  UI_RETURN_SELF;
}

static int l_uiTableModelRowDeleted(lua_State *L)
{
  uiTableModel *m = CHECK_USER_OBJECT(1, TableModel);
  int index = luaL_checkinteger(L, 2);
  uiTableModelRowDeleted(m, index);
  UI_RETURN_SELF;
}

static int l_uiNewTableParams(lua_State *L)
{
  uiTableModel *Model = CHECK_USER_OBJECT(1, TableModel);
  int RowBackgroundColorModelColumn = luaL_checkinteger(L, 1);
  uiTableParams *params = lua_newuserdata(L, sizeof(uiTableParams));
  params->Model = Model;
  params->RowBackgroundColorModelColumn = RowBackgroundColorModelColumn;
  //FIXME: set metatable
  return 1;
}

static struct luaL_Reg meta_TableModel[] =
{
  { "RowInserted", l_uiTableModelRowInserted },
  { "RowChanged", l_uiTableModelRowChanged },
  { "RowDeleted", l_uiTableModelRowDeleted },
  { "NewTableParams", l_uiNewTableParams },
  { "Free", l_uiFreeTableModel },
  { "__gc", l_uiFreeTableModel },

  { NULL, NULL}
};

// uiTableModelColumnNeverEditable and
// uiTableModelColumnAlwaysEditable are the value of an editable
// model column parameter to one of the uiTable create column
// functions; if used, that jparticular uiTable colum is not editable
// by the user and always editable by the user, respectively.


static int l_uiNewTableTextColumnOptionalParams(lua_State *L)
{
  int ColorModelColumn = luaL_checkinteger(L, 1);
  uiTableTextColumnOptionalParams *params = lua_newuserdata(L, sizeof(uiTableTextColumnOptionalParams));
  params->ColorModelColumn = ColorModelColumn;
  //FIXME: set metatable
  return 1;
}

static struct luaL_Reg meta_TableParams[] =
{
  { "NewTableParams", l_uiNewTableParams },
  { "", l_uiTableValueValue },
  { "Free", l_uiTableValueFree },
  { "__gc", l_uiTableValueFree },

  { NULL, NULL}
};

static int l_uiTableAppendTextColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int textModelColumn = luaL_checkinteger(L, 3);
  int textEditableModelColumn = luaL_checkinteger(L, 4);
  uiTableTextColumnOptionalParams *textParams =
    lua_isnoneornil(L, 5) ? NULL : CHECK_USER_OBJECT(5, TableTextColumnOptionalParams);

  uiTableAppendTextColumn(tab,
                          name,
                          textModelColumn,
                          textEditableModelColumn,
                          textParams);
  UI_RETURN_SELF;
}

static int l_uiTableAppendImageColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int imageModelColumn = luaL_checkinteger(L, 3);

  uiTableAppendImageColumn(tab, name, imageModelColumn);
  UI_RETURN_SELF;
}

static int l_uiTableAppendImageTextColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int imageModelColumn = luaL_checkinteger(L, 3);
  int textModelColumn = luaL_checkinteger(L, 4);
  int textEditableModelColumn = luaL_checkinteger(L, 5);
  uiTableTextColumnOptionalParams *textParams =
    lua_isnoneornil(L, 6) ? NULL : CHECK_USER_OBJECT(6, TableTextColumnOptionalParams);

  uiTableAppendImageTextColumn(tab,
                               name,
                               imageModelColumn,
                               textModelColumn,
                               textEditableModelColumn,
                               textParams);
  UI_RETURN_SELF;
}

static int l_uiTableAppendCheckboxColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int checkboxModelColumn = luaL_checkinteger(L, 3);
  int checkboxEditableModelColumn = luaL_checkinteger(L, 4);

  uiTableAppendCheckboxColumn(tab,
                              name,
                              checkboxModelColumn,
                              checkboxEditableModelColumn);

  UI_RETURN_SELF;
}

static int l_uiTableAppendCheckboxTextColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);

  int checkboxModelColumn = luaL_checkinteger(L, 3);
  int checkboxEditableModelColumn = luaL_checkinteger(L, 4);
  int textModelColumn = luaL_checkinteger(L, 5);
  int textEditableModelColumn = luaL_checkinteger(L, 6);
  uiTableTextColumnOptionalParams *textParams =
    lua_isnoneornil(L, 7) ? NULL : CHECK_USER_OBJECT(7, TableTextColumnOptionalParams);

  uiTableAppendCheckboxTextColumn(tab,
                                  name,
                                  checkboxModelColumn,
                                  checkboxEditableModelColumn,
                                  textModelColumn,
                                  textEditableModelColumn,
                                  textParams);
  UI_RETURN_SELF;
}

static int l_uiTableAppendProgressBarColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int column = luaL_checkinteger(L, 3);

  uiTableAppendProgressBarColumn(tab, name, column);
  UI_RETURN_SELF;
}

static int l_uiTableAppendButtonColumn(lua_State *L)
{
  uiTable *tab = UI_CHECK_OBJECT(1, Table);
  const char *name = luaL_checkstring(L, 2);
  int cols = luaL_checkinteger(L, 3);
  int clickableColumn = luaL_checkinteger(L, 4);

  uiTableAppendButtonColumn(tab, name, cols, clickableColumn);
  UI_RETURN_SELF;
}

static int l_uiNewTable(lua_State *L)
{
  uiTableParams *params = CHECK_USER_OBJECT(1, TableParams);
  uiTable *tab = uiNewTable(params);
  UI_CREATE_OBJECT(Table, tab);
  return 1;
}

static struct luaL_Reg meta_Table[] =
{
  { "AppendButtonColomn", l_uiTableAppendButtonColumn },
  { "AppendCheckboxColumn", l_uiTableAppendCheckboxColumn },
  { "AppendCheckboxTextColumn", l_uiTableAppendCheckboxTextColumn },
  { "AppendImangeColumn", l_uiTableAppendImageColumn },
  { "AppendImangeTextColumn", l_uiTableAppendImageTextColumn },
  { "AppendProgressBarColumn", l_uiTableAppendProgressBarColumn },
  { "AppendTextColumn", l_uiTableAppendTextColumn },

  CONTROL_COMMON_METAFIELD
  { NULL, NULL}
};
