/*
** Copyright (C) 2016-2018 Arseny Vakhrushev <arseny.vakhrushev@gmail.com>
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/

#pragma once

#include <lauxlib.h>
#include <mongoc.h>
#include <bson.h>

#ifndef _WIN32
#pragma GCC visibility push(hidden)
#endif

#define MODNAME "lua-mongo"
#define VERSION "1.2.0"

#define TYPE_BINARY "mongo.Binary"
#define TYPE_BSON "mongo.BSON"
#define TYPE_BULKOPERATION "mongo.BulkOperation"
#define TYPE_CLIENT "mongo.Client"
#define TYPE_COLLECTION "mongo.Collection"
#define TYPE_CURSOR "mongo.Cursor"
#define TYPE_DATABASE "mongo.Database"
#define TYPE_DATETIME "mongo.DateTime"
#define TYPE_DECIMAL128 "mongo.Decimal128"
#define TYPE_DOUBLE "mongo.Double"
#define TYPE_GRIDFS "mongo.GridFS"
#define TYPE_GRIDFSFILE "mongo.GridFSFile"
#define TYPE_GRIDFSFILELIST "mongo.GridFSFileList"
#define TYPE_INT32 "mongo.Int32"
#define TYPE_INT64 "mongo.Int64"
#define TYPE_JAVASCRIPT "mongo.Javascript"
#define TYPE_MAXKEY "mongo.MaxKey"
#define TYPE_MINKEY "mongo.MinKey"
#define TYPE_NULL "mongo.Null"
#define TYPE_OBJECTID "mongo.ObjectID"
#define TYPE_READPREFS "mongo.ReadPrefs"
#define TYPE_REGEX "mongo.Regex"
#define TYPE_TIMESTAMP "mongo.Timestamp"

extern char NEW_BINARY, NEW_DATETIME, NEW_DECIMAL128, NEW_JAVASCRIPT, NEW_REGEX, NEW_TIMESTAMP;
extern char GLOBAL_MAXKEY, GLOBAL_MINKEY, GLOBAL_NULL;

int newBinary(lua_State *L);
int newBSON(lua_State *L);
int newClient(lua_State *L);
int newDateTime(lua_State *L);
int newDecimal128(lua_State *L);
int newDouble(lua_State *L);
int newInt32(lua_State *L);
int newInt64(lua_State *L);
int newJavascript(lua_State *L);
int newObjectID(lua_State *L);
int newReadPrefs(lua_State *L);
int newRegex(lua_State *L);
int newTimestamp(lua_State *L);

void pushBSON(lua_State *L, const bson_t *bson, int hidx);
void pushBSONWithSteal(lua_State *L, bson_t *bson);
void pushBSONValue(lua_State *L, const bson_value_t *val);
void pushBSONField(lua_State *L, const bson_t *bson, const char *key, bool any);
void pushBulkOperation(lua_State *L, mongoc_bulk_operation_t *bulk, int pidx);
void pushCollection(lua_State *L, mongoc_collection_t *collection, bool ref, int pidx);
void pushCursor(lua_State *L, mongoc_cursor_t *cursor, int pidx);
void pushDatabase(lua_State *L, mongoc_database_t *database, int pidx);
void pushGridFS(lua_State *L, mongoc_gridfs_t *gridfs, int pidx);
void pushGridFSFile(lua_State *L, mongoc_gridfs_file_t *file, int pidx);
void pushGridFSFileList(lua_State *L, mongoc_gridfs_file_list_t *list, int pidx);
void pushMaxKey(lua_State *L);
void pushMinKey(lua_State *L);
void pushNull(lua_State *L);
void pushObjectID(lua_State *L, const bson_oid_t *oid);
void pushReadPrefs(lua_State *L, const mongoc_read_prefs_t *prefs);

int iterateCursor(lua_State *L, mongoc_cursor_t *cursor, int hidx);

bson_t *checkBSON(lua_State *L, int idx);
bson_t *testBSON(lua_State *L, int idx);
bson_t *castBSON(lua_State *L, int idx);
bson_t *toBSON(lua_State *L, int idx);

void toBSONValue(lua_State *L, int idx, bson_value_t *val);

bson_oid_t *checkObjectID(lua_State *L, int idx);
bson_oid_t *testObjectID(lua_State *L, int idx);

mongoc_bulk_operation_t *checkBulkOperation(lua_State *L, int idx);
mongoc_client_t *checkClient(lua_State *L, int idx);
mongoc_collection_t *checkCollection(lua_State *L, int idx);
mongoc_cursor_t *checkCursor(lua_State *L, int idx);
mongoc_database_t *checkDatabase(lua_State *L, int idx);
mongoc_gridfs_t *checkGridFS(lua_State *L, int idx);
mongoc_gridfs_file_t *checkGridFSFile(lua_State *L, int idx);
mongoc_gridfs_file_list_t *checkGridFSFileList(lua_State *L, int idx);
mongoc_read_prefs_t *checkReadPrefs(lua_State *L, int idx);
mongoc_read_prefs_t *toReadPrefs(lua_State *L, int idx);

int toInsertFlags(lua_State *L, int idx);
int toRemoveFlags(lua_State *L, int idx);
int toUpdateFlags(lua_State *L, int idx);

/* Utilities */

bool newType(lua_State *L, const char *tname, const luaL_Reg *funcs);
void setType(lua_State *L, const char *tname, const luaL_Reg *funcs);
void unsetType(lua_State *L);

void pushHandle(lua_State *L, void *ptr, int mode, int pidx);
int getHandleMode(lua_State *L, int idx);

void packParams(lua_State *L, int n);
int unpackParams(lua_State *L, int idx);

void checkStatus(lua_State *L, bool status, const bson_error_t *error);

int commandError(lua_State *L, const bson_error_t *error);
int commandStatus(lua_State *L, bool status, const bson_error_t *error);
int commandReply(lua_State *L, bool status, bson_t *reply, const bson_error_t *error);
int commandStrVec(lua_State *L, char **strv, const bson_error_t *error);

#define check(L, cond) (void)((cond) || luaL_error(L, "precondition '%s' failed at %s:%d", #cond, __FILE__, __LINE__))
#define argferror(L, idx, ...) (lua_pushfstring(L, __VA_ARGS__), luaL_argerror(L, idx, lua_tostring(L, -1)))
#define argfcheck(L, cond, idx, ...) (void)((cond) || argferror(L, idx, __VA_ARGS__))

#if LUA_VERSION_NUM >= 503 && LUA_MAXINTEGER >= INT64_MAX
#define pushInt64(L, n) lua_pushinteger(L, n)
#define toInt64(L, idx) lua_tointeger(L, idx)
#define checkInt64(L, idx) luaL_checkinteger(L, idx)
#else
#define pushInt64(L, n) lua_pushnumber(L, n)
#define toInt64(L, idx) lua_tonumber(L, idx)
#define checkInt64(L, idx) luaL_checknumber(L, idx)
#endif

#if LUA_VERSION_NUM >= 503 && LUA_MAXINTEGER >= INT32_MAX
#define pushInt32(L, n) lua_pushinteger(L, n)
#define toInt32(L, idx) lua_tointeger(L, idx)
#define checkInt32(L, idx) luaL_checkinteger(L, idx)
#else
#define pushInt32(L, n) lua_pushnumber(L, n)
#define toInt32(L, idx) lua_tonumber(L, idx)
#define checkInt32(L, idx) luaL_checknumber(L, idx)
#endif

#if LUA_VERSION_NUM < 502
#define lua_rawlen(L, idx) lua_objlen(L, idx)
#define lua_getuservalue(L, idx) lua_getfenv(L, idx)
#define lua_setuservalue(L, idx) lua_setfenv(L, idx)
#define lua_rawgetp(L, idx, ptr) (lua_pushlightuserdata(L, ptr), lua_rawget(L, idx))
#define lua_rawsetp(L, idx, ptr) (lua_pushlightuserdata(L, ptr), lua_insert(L, -2), lua_rawset(L, idx))
void *luaL_testudata(lua_State* L, int idx, const char *tname);
#endif

#ifndef _WIN32
#pragma GCC visibility pop
#endif
