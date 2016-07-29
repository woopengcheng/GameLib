/*
 * =====================================================================================
 *
 *      Filename:  pb.c
 *
 *      Description: protoc-gen-lua
 *      Google's Protocol Buffers project, ported to lua.
 *      https://code.google.com/p/protoc-gen-lua/
 *
 *      Copyright (c) 2010 , 鏋楀崜姣?(Zhuoyi Lin) netsnail@gmail.com
 *      All rights reserved.
 *
 *      Use, modification and distribution are subject to the "New BSD License"
 *      as listed at <url: http://www.opensource.org/licenses/bsd-license.php >.
 *
 *      Created:  2010骞?8鏈?2鏃?18鏃?4鍒?1绉? *
 *      Company:  NetEase
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <string.h>

#define LUA_LIB
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef _ALLBSD_SOURCE
#include <machine/endian.h>
#else
#include "endian.h"
#endif

#ifdef _WIN32
#define PRId64            "I64d"
#define PRIu64            "I64u"
#else
#define PRId64            "lld"
#define PRIu64            "llu"
#endif

//#if __BYTE_ORDER == __LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
//#endif

#define IOSTRING_META "protobuf.IOString"

#define checkiostring(L) \
    (IOString*) luaL_checkudata(L, 1, IOSTRING_META)

#define IOSTRING_BUF_LEN 65535

typedef struct{
    size_t size;
    char buf[IOSTRING_BUF_LEN];
} IOString;

enum FieldDescriptor {
    TYPE_DOUBLE         = 1,
    TYPE_FLOAT          = 2,
    TYPE_INT64          = 3,
    TYPE_UINT64         = 4,
    TYPE_INT32          = 5,
    TYPE_FIXED64        = 6,
    TYPE_FIXED32        = 7,
    TYPE_BOOL           = 8,
    TYPE_STRING         = 9,
    TYPE_GROUP          = 10,
    TYPE_MESSAGE        = 11,
    TYPE_BYTES          = 12,
    TYPE_UINT32         = 13,
    TYPE_ENUM           = 14,
    TYPE_SFIXED32       = 15,
    TYPE_SFIXED64       = 16,
    TYPE_SINT32         = 17,
    TYPE_SINT64         = 18,
    MAX_TYPE            = 18,

    //   Must be consistent with C++ FieldDescriptor::CppType enum in
    //   descriptor.h.
    //  
    CPPTYPE_INT32       = 1,
    CPPTYPE_INT64       = 2,
    CPPTYPE_UINT32      = 3,
    CPPTYPE_UINT64      = 4,
    CPPTYPE_DOUBLE      = 5,
    CPPTYPE_FLOAT       = 6,
    CPPTYPE_BOOL        = 7,
    CPPTYPE_ENUM        = 8,
    CPPTYPE_STRING      = 9,
    CPPTYPE_MESSAGE     = 10,
    MAX_CPPTYPE         = 10,

    //   Must be consistent with C++ FieldDescriptor::Label enum in
    //   descriptor.h.
    //  
    LABEL_OPTIONAL      = 1,
    LABEL_REQUIRED      = 2,
    LABEL_REPEATED      = 3,
    MAX_LABEL           = 3
};

static void pack_varint(luaL_Buffer *b, uint64_t value)
{
    if (value >= 0x80)
    {
        luaL_addchar(b, value | 0x80);
        value >>= 7;
        if (value >= 0x80)
        {
            luaL_addchar(b, value | 0x80);
            value >>= 7;
            if (value >= 0x80)
            {
                luaL_addchar(b, value | 0x80);
                value >>= 7;
                if (value >= 0x80)
                {
                    luaL_addchar(b, value | 0x80);
                    value >>= 7;
                    if (value >= 0x80)
                    {
                        luaL_addchar(b, value | 0x80);
                        value >>= 7;
                        if (value >= 0x80)
                        {
                            luaL_addchar(b, value | 0x80);
                            value >>= 7;
                            if (value >= 0x80)
                            {
                                luaL_addchar(b, value | 0x80);
                                value >>= 7;
                                if (value >= 0x80)
                                {
                                    luaL_addchar(b, value | 0x80);
                                    value >>= 7;
                                    if (value >= 0x80)
                                    {
                                        luaL_addchar(b, value | 0x80);
                                        value >>= 7;
                                    } 
                                } 
                            } 
                        } 
                    } 
                } 
            } 
        } 
    }
    luaL_addchar(b, value);
} 

static uint64_t check64BitStringValue(lua_State *L, int index)
{
	size_t len;
	char const* str = lua_tolstring(L, index, &len);
	if (len != 8)
	{
		luaL_error(L, "bad argument #1 to check64BitStringValue (8 bytes string expected, got len %d)", (int)len);
		return 0;
	}
	return *(uint64_t*)str;
}

//value at index is descriptor type, value at index+1 is the value
static uint64_t checkNumberValue(lua_State *L, int index)
{
	int field_type = luaL_checkinteger(L, index);
	int is64BitsStr;
	switch (field_type)
	{
    case CPPTYPE_INT64:
    case CPPTYPE_UINT64:
		is64BitsStr = 1;
		break;
	default:
		is64BitsStr = 0;
		break;
	}

	if (is64BitsStr)
	{
		return check64BitStringValue(L, index + 1);
	}
    else
	{
		lua_Number l_value = luaL_checknumber(L, index+1);
		return (uint64_t)l_value;
	}
}

static void push64BitString(lua_State *L, uint64_t value)
{
	lua_pushlstring(L, (char const*)&value, 8);
}

static void pushSignedNumberValue(lua_State *L, int field_type, int64_t value)
{
	switch (field_type)
	{
    case CPPTYPE_INT64:
    case CPPTYPE_UINT64:
		push64BitString(L, value);
		break;
	default:
		lua_pushnumber(L, (lua_Number)value);
		break;
	}
}

static void pushUnsignedNumberValue(lua_State *L, int field_type, uint64_t value)
{
	switch (field_type)
	{
    case CPPTYPE_INT64:
    case CPPTYPE_UINT64:
		push64BitString(L, value);
		break;
	default:
		lua_pushnumber(L, (lua_Number)(int64_t)value);
		break;
	}
}


static int varint_encoder(lua_State *L)
{
	uint64_t value = checkNumberValue(L, 2);

    luaL_Buffer b;
    luaL_buffinit(L, &b);
    
    pack_varint(&b, value);

    lua_settop(L, 1);
    luaL_pushresult(&b);
    lua_call(L, 1, 0);
    return 0;
}

static uint64_t getNumberValue(lua_State *L, int index)
{
	if (lua_type(L, index) == LUA_TSTRING)
	{
		return check64BitStringValue(L, index);
	}
	else
	{
		lua_Number l_value = luaL_checknumber(L, index);
		return (uint64_t)l_value;
	}
}

static int calc_varint_size(uint64_t value)
{
	if (value <= 0x7fLL)
		return 1;
	if (value <= 0x3fffLL)
		return 2;
	if (value <= 0x1fffffLL)
		return 3;
	if (value <= 0xfffffffLL)
		return 4;
	if (value <= 0x7ffffffffLL)
		return 5;
	if (value <= 0x3ffffffffffLL)
		return 6;
	if (value <= 0x1ffffffffffffLL)
		return 8;
	if (value <= 0xffffffffffffffLL)
		return 9;

	return 10;
}

static int varint_size(lua_State* L)
{
	uint64_t value = getNumberValue(L, 1);

	int len = calc_varint_size(value);
	lua_pushinteger(L, len);
	return 1;
}

static int signed_varint_size(lua_State* L)
{
	int64_t value = (int64_t)getNumberValue(L, 1);

	int len;
	if (value < 0)
		len = 10;
	else
		len = calc_varint_size((uint64_t)value);
	
	lua_pushinteger(L, len);
	return 1;
}


static int pack_fixed32(lua_State *L, uint8_t* value){
#ifdef IS_LITTLE_ENDIAN
    lua_pushlstring(L, (char*)value, 4);
#else
    uint32_t v = htole32(*(uint32_t*)value);
    lua_pushlstring(L, (char*)&v, 4);
#endif
    return 0;
}

static int pack_fixed64(lua_State *L, uint8_t* value){
#ifdef IS_LITTLE_ENDIAN
    lua_pushlstring(L, (char*)value, 8);
#else
    uint64_t v = htole64(*(uint64_t*)value);
    lua_pushlstring(L, (char*)&v, 8);
#endif
    return 0;
}

static int struct_pack(lua_State *L)
{
    uint8_t format = luaL_checkinteger(L, 2);
	uint64_t int64Strvalue;
	lua_Number value;

    switch(format){
        case 'q':
        case 'Q':
			int64Strvalue = check64BitStringValue(L, 3);
			break;
		default:
			value = luaL_checknumber(L, 3);
	}

    lua_settop(L, 1);

    switch(format){
        case 'i':
            {
                int32_t v = (int32_t)value;
                pack_fixed32(L, (uint8_t*)&v);
                break;
            }
        case 'q':
            {
                pack_fixed64(L, (uint8_t*)&int64Strvalue);
                break;
            }
        case 'f':
            {
                float v = (float)value;
                pack_fixed32(L, (uint8_t*)&v);
                break;
            }
        case 'd':
            {
                double v = (double)value;
                pack_fixed64(L, (uint8_t*)&v);
                break;
            }
        case 'I':
            {
                uint32_t v = (uint32_t)value;
                pack_fixed32(L, (uint8_t*)&v);
                break;
            }
        case 'Q':
            {
                pack_fixed64(L, (uint8_t*)&int64Strvalue);
                break;
            }
        default:
            luaL_error(L, "Unknown, format");
    }
    lua_call(L, 1, 0);
    return 0;
}

static size_t size_varint(const char* buffer, size_t len)
{
    size_t pos = 0;
    while(buffer[pos] & 0x80){
        ++pos;
        if(pos > len){
            return -1;
        }
    }
    return pos+1;
}

static uint64_t unpack_varint(const char* buffer, size_t len)
{
    uint64_t value = buffer[0] & 0x7f;
    size_t shift = 7;
    size_t pos=0;
    for(pos = 1; pos < len; ++pos)
    {
        value |= ((uint64_t)(buffer[pos] & 0x7f)) << shift;
        shift += 7;
    }
    return value;
}

static int varint_decoder(lua_State *L)
{
    size_t len;
    const char* buffer = luaL_checklstring(L, 1, &len);
    size_t pos = luaL_checkinteger(L, 2);
	int field_type = luaL_checkinteger(L, 3);
    
    buffer += pos;
    len = size_varint(buffer, len);
    if(len == -1){
        luaL_error(L, "error data %s, len:%d", buffer, len);
    }else{
		uint64_t value = unpack_varint(buffer, len);
		pushUnsignedNumberValue(L, field_type, value);
        lua_pushinteger(L, len + pos);
    }
    return 2;
}

static int signed_varint_decoder(lua_State *L)
{
    size_t len;
    const char* buffer = luaL_checklstring(L, 1, &len);
    size_t pos = luaL_checkinteger(L, 2);
	int field_type = luaL_checkinteger(L, 3);

    buffer += pos;
    len = size_varint(buffer, len);
    
    if(len == -1){
        luaL_error(L, "error data %s, len:%d", buffer, len);
    }else{
		pushSignedNumberValue(L, field_type, (int64_t)unpack_varint(buffer, len));
        lua_pushinteger(L, len + pos);
    }
    return 2;
}

static int zig_zag_encode32(lua_State *L)
{
    int32_t n = luaL_checkinteger(L, 1);
    uint32_t value = (n << 1) ^ (n >> 31);
    lua_pushinteger(L, value);
    return 1;
}

static int zig_zag_decode32(lua_State *L)
{
    uint32_t n = (uint32_t)luaL_checkinteger(L, 1);
    int32_t value = (n >> 1) ^ - (int32_t)(n & 1);
    lua_pushinteger(L, value);
    return 1;
}

static int zig_zag_encode64(lua_State *L)
{
    int64_t n = (int64_t)check64BitStringValue(L, 1);
    uint64_t value = (n << 1) ^ (n >> 63);
    push64BitString(L, value);
    return 1;
}

static int zig_zag_decode64(lua_State *L)
{
    uint64_t n = (uint64_t)check64BitStringValue(L, 1);
    int64_t value = (n >> 1) ^ - (int64_t)(n & 1);
    push64BitString(L, value);
    return 1;
}

static int read_tag(lua_State *L)
{
    size_t len;
    const char* buffer = luaL_checklstring(L, 1, &len);
    size_t pos = luaL_checkinteger(L, 2);
    
    buffer += pos;
    len = size_varint(buffer, len);
    if(len == -1){
        luaL_error(L, "error data %s, len:%d", buffer, len);
    }else{
        lua_pushlstring(L, buffer, len);
        lua_pushinteger(L, len + pos);
    }
    return 2;
}

static const uint8_t* unpack_fixed32(const uint8_t* buffer, uint8_t* cache)
{
#ifdef IS_LITTLE_ENDIAN
    return buffer;
#else
    *(uint32_t*)cache = le32toh(*(uint32_t*)buffer);
    return cache;
#endif
}

static const uint8_t* unpack_fixed64(const uint8_t* buffer, uint8_t* cache)
{
#ifdef IS_LITTLE_ENDIAN
    return buffer;
#else
    *(uint64_t*)cache = le64toh(*(uint64_t*)buffer);
    return cache;
#endif
}

static int uint64_to_string(lua_State *L)
{
	uint64_t value = check64BitStringValue(L, -1);

	char buffer[64];
	sprintf(buffer, "%" PRIu64 "", value);
	lua_pushstring(L, buffer);
	return 1;
}

static int number_to_int64(lua_State *L)
{
	double v = luaL_checknumber(L, 1);
	lua_pushlstring(L, (char const*)&v, 8);
	return 1;
}

static int int64_to_string(lua_State *L)
{
	int64_t value = (int64_t)check64BitStringValue(L, -1);

	char buffer[64];
	sprintf(buffer, "%" PRId64 "", value);
	lua_pushstring(L, buffer);
	return 1;
}

static int struct_unpack(lua_State *L)
{
    uint8_t format = luaL_checkinteger(L, 1);
    size_t len;
    uint8_t out[8];
    const uint8_t* buffer = (uint8_t*)luaL_checklstring(L, 2, &len);
    size_t pos = luaL_checkinteger(L, 3);

    buffer += pos;
    switch(format){
        case 'i':
            {
				int32_t v;
				memcpy(&v, unpack_fixed32(buffer, out), sizeof(v));
                lua_pushinteger(L, v);
                break;
            }
        case 'q':
            {
				uint64_t v;
				memcpy(&v, unpack_fixed64(buffer, out), sizeof(v));
				push64BitString(L, v);
                break;
            }
        case 'f':
            {
				float v;
				memcpy(&v, unpack_fixed32(buffer, out), sizeof(v));
                lua_pushnumber(L, (lua_Number)v);
                break;
            }
        case 'd':
            {
				double v;
				memcpy(&v, unpack_fixed64(buffer, out), sizeof(v));
                lua_pushnumber(L, (lua_Number)v);
                break;
            }
        case 'I':
            {
				uint32_t v;
				memcpy(&v, unpack_fixed32(buffer, out), sizeof(v));
                lua_pushnumber(L, v);
                break;
            }
        case 'Q':
            {
				uint64_t v;
				memcpy(&v, unpack_fixed64(buffer, out), sizeof(v));
				push64BitString(L, v);
                break;
            }
        default:
            luaL_error(L, "Unknown, format");
    }
    return 1;
}

static int iostring_new(lua_State* L)
{
    IOString* io = (IOString*)lua_newuserdata(L, sizeof(IOString));
    io->size = 0;

    luaL_getmetatable(L, IOSTRING_META);
    lua_setmetatable(L, -2); 
    return 1;
}

static int iostring_str(lua_State* L)
{
    IOString *io = checkiostring(L);
    lua_pushlstring(L, io->buf, io->size);
    return 1;
}

static int iostring_len(lua_State* L)
{
    IOString *io = checkiostring(L);
    lua_pushinteger(L, io->size);
    return 1;
}

static int iostring_write(lua_State* L)
{
    IOString *io = checkiostring(L);
    size_t size;
    const char* str = luaL_checklstring(L, 2, &size);
    if(io->size + size > IOSTRING_BUF_LEN){
        luaL_error(L, "Out of range");
    }
    memcpy(io->buf + io->size, str, size);
    io->size += size;
    return 0;
}

static int iostring_sub(lua_State* L)
{
    IOString *io = checkiostring(L);
    size_t begin = luaL_checkinteger(L, 2);
    size_t end = luaL_checkinteger(L, 3);

    if(begin > end || end > io->size)
    {
        luaL_error(L, "Out of range");
    }
    lua_pushlstring(L, io->buf + begin - 1, end - begin + 1);
    return 1;
}

static int iostring_clear(lua_State* L)
{
    IOString *io = checkiostring(L);
    io->size = 0; 
    return 0;
}

static const struct luaL_Reg _pb [] = {
	{"varint_encoder", varint_encoder},
    {"signed_varint_encoder", varint_encoder},
	{"varint_size", varint_size},
	{"signed_varint_size", signed_varint_size},
    {"read_tag", read_tag},
    {"struct_pack", struct_pack},
    {"struct_unpack", struct_unpack},
	{"number_to_int64", number_to_int64},
	{"uint64_to_string", uint64_to_string},
	{"int64_to_string", int64_to_string},
    {"varint_decoder", varint_decoder},
    {"signed_varint_decoder", signed_varint_decoder},
    {"zig_zag_decode32", zig_zag_decode32},
    {"zig_zag_encode32", zig_zag_encode32},
    {"zig_zag_decode64", zig_zag_decode64},
    {"zig_zag_encode64", zig_zag_encode64},
    {"new_iostring", iostring_new},
    {NULL, NULL}
};

static const struct luaL_Reg _c_iostring_m [] = {
    {"__tostring", iostring_str},
    {"__len", iostring_len},
    {"write", iostring_write},
    {"sub", iostring_sub},
    {"clear", iostring_clear},
    {NULL, NULL}
};

LUALIB_API int luaopen_pb (lua_State *L)
{
    luaL_newmetatable(L, IOSTRING_META);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

#if LUA_VERSION_NUM <= 501
	luaL_register(L, NULL, _c_iostring_m);
	luaL_register(L, "pb", _pb);
#else
	luaL_setfuncs(L, _c_iostring_m, 0);//lua5.2-->function name,只是注册，还没有创建table呢
	luaL_newlib(L, _pb);
	lua_pushvalue(L, -1);//copy to stack header
	lua_setglobal(L, "pb");
#endif
    return 1;
} 
