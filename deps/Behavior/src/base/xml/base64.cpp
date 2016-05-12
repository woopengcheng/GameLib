/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "behaviac/base/xml/base64.h"
namespace XmlBase64
{
    static const char index64ToChar[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const uint8_t charToIndex64[] =
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,						// Decimal  0 -  8
        0xFF, 0xFF,                                      					// Whitespace: Tab and Linefeed
        0xFF, 0xFF,                                      					// Decimal 11 - 12
        0xFF,                                         						// Whitespace: Carriage Return
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Decimal 14 - 26
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                             				// Decimal 27 - 31
        0xFF,                                         						// Whitespace: Space
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,             		// Decimal 33 - 42
        62,                                         						// Plus sign at decimal 43
        0xFF, 0xFF, 0xFF,                                   					// Decimal 44 - 46
        63,                                         						// Slash at decimal 47
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61,              						// Numbers zero through nine
        0xFF, 0xFF, 0xFF,														// Decimal 58 - 60
        0xFF,                                         						// Equals sign at decimal 61
        0xFF, 0xFF, 0xFF,                                   					// Decimal 62 - 64
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,            						// Letters 'A' through 'N'
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        						// Letters 'O' through 'Z'
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                  					// Decimal 91 - 96
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,     						// Letters 'a' through 'm'
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,     						// Letters 'n' through 'z'
        0xFF, 0xFF, 0xFF, 0xFF                             					// Decimal 123 - 126
    };

    //This is use to test the end of the buffer to decode.
    //= is a padding, 0 means we
    //are at the end of the buffer
    inline bool isEncoded(int8_t in)
    {
        return in != 0 && in != '=';
    }

    int32_t GetEncodedBuffSize(int32_t nbBytes)
    {
        //Always add the 2 for the size of the padding + 1 for the truncation error
        return ((nbBytes + 2) * 4 / 3) + 1;
    }

    //Encode 3 8 bits into 4 6 bits char
    inline void Encode3to4(int8_t in[3], int8_t* out, int8_t nb)
    {
        BEHAVIAC_ASSERT(nb >= 1);
        BEHAVIAC_ASSERT(nb <= 3);
        int8_t n0 = in[0];
        int8_t n1 = in[1];
        int8_t n2 = in[2];
        //6 more significatives bits byte 0
        (*out) = index64ToChar[(n0 >> 2)];
        //2 least significatives bits byte 0, 4 more significatives bits byte 1
        (*(out + 1)) = index64ToChar[((n0 & 0x03) << 4) | ((n1 & 0xF0) >> 4)];
        //4 bits least significatives byte 1, 2 bits more significatives byte 2
        (*(out + 2)) = (nb > 1) ? index64ToChar[((n1 & 0x0F) << 2) | ((n2 & 0xC0) >> 6)] : '=';
        //6 bits least significatives byte 3
        (*(out + 3)) = (nb > 2) ? index64ToChar[(n2 & 0x3F)] : '=';
    }

    //Decode 4 6 bits into 3 8 bits
    BEHAVIAC_FORCEINLINE void Decode4to3(int8_t in[4], int8_t* out)
    {
        uint8_t i0 = charToIndex64[in[0]];
        uint8_t i1 = charToIndex64[in[1]];
        uint8_t i2 = charToIndex64[in[2]];
        uint8_t i3 = charToIndex64[in[3]];
        //6 bits more significatives from byte 0 + 2 least significatives from byte 1
        *out = (i0 << 2) | (i1 >> 4);
        //4 bits least significatives from byte 1 + 4 bits  least significatives from byte 2
        (*(out + 1)) = (i2 != 0xFF) ? (i1 << 4) | (i2 >> 2) : '\0';
        //2 bits more significatives from byte 2, 6 bits least significatives from byte 3
        (*(out + 2)) = (i3 != 0xFF) ? (i2 << 6) | i3 : '\0';
    }

    //Encode the memory buffer, size must be at least 1,
    //client is responsible for freeing outBuff( BEHAVIAC_FREE )
    void EncodeBuff(const int8_t* inBuff, int8_t*& outBuff, int32_t nbBytes)
    {
        BEHAVIAC_ASSERT(inBuff != NULL);
        BEHAVIAC_ASSERT(nbBytes >= 1);
        int32_t buffSize = GetEncodedBuffSize(nbBytes);
        //Clients will free it
        outBuff = (int8_t*)BEHAVIAC_MALLOC_WITHTAG(buffSize, "Encode64 buff");
        int32_t indexOutBuff = 0;
        int8_t toEncode[3];
        int8_t nbToEncode = 0;

        //For all the buffer, encode by 3 bytes
        for (int32_t buffIndex = 0; buffIndex < nbBytes;)
        {
            int8_t encodeIndex = 0;

            for (; encodeIndex < 3 && buffIndex < nbBytes; ++encodeIndex, ++buffIndex)
            {
                toEncode[encodeIndex] = inBuff[buffIndex];
            }

            nbToEncode = encodeIndex;

            //Pad the buffer with 0s if needed, can be done once at the end
            for (; encodeIndex < 3 && encodeIndex > 0; ++encodeIndex)
            {
                toEncode[encodeIndex] = 0;
            }

            //Encode 3 bytes into 4 bytes in outBuff
            Encode3to4(toEncode, outBuff + indexOutBuff, nbToEncode);
            indexOutBuff += 4;
        }

        BEHAVIAC_ASSERT(indexOutBuff < buffSize);
        outBuff[indexOutBuff] = '\0';
    }

    //Encode the memory buffer, size must be at least 1,
    //client is responsible for freeing outBuff ( BEHAVIAC_FREE )
    void DecodeBuff(const int8_t* inBuff, int8_t*& outBuff)
    {
        BEHAVIAC_ASSERT(inBuff != NULL);
        BEHAVIAC_ASSERT(*inBuff != '\0');
        const int8_t* curPos = inBuff;
        //1 for the truncation error
        size_t buffSize = ((strlen((const char*)inBuff) * 3) / 4) + 3;
		size_t indexOutBuff = 0;
        //Clients will free it
        outBuff = (int8_t*)BEHAVIAC_MALLOC_WITHTAG(buffSize, "Decode64 buff");
        int8_t toDecode[4];
        //int8_t nbToDecode = 0;

        //Decode buff
        while (isEncoded(*curPos))
        {
            int8_t index = 0;

            for (; index < 4 && isEncoded(*(curPos)); ++index, ++curPos)
            {
                toDecode[index] = *curPos;
            }

            //nbToDecode = index;

            //Handle padding, can be done once
            for (; index < 4 && index > 0; ++index)
            {
                toDecode[index] = 0;
            }

            Decode4to3(toDecode, outBuff + indexOutBuff);
            indexOutBuff += 3;
        }

        BEHAVIAC_ASSERT(indexOutBuff < buffSize);
        outBuff[indexOutBuff] = '\0';
    }
} //namespace XmlBase64
