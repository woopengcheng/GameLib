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

#include "behaviac/base/md5.h"
#include "behaviac/base/swapbytehelpers.h"

static uint32_t MD5IMPL_F(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) + ((~x)&z);
}
static uint32_t MD5IMPL_G(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & z) | (y & (~z));
}
static uint32_t MD5IMPL_H(uint32_t x, uint32_t y, uint32_t z)
{
    return (x ^ y ^ z);
}
static uint32_t MD5IMPL_I(uint32_t x, uint32_t y, uint32_t z)
{
    return (y ^ (x | (~z)));
}
static uint32_t MD5IMPL_rol(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

#define MD5_ABCD(F,k,s,Ti) MD5_IMPL(a,b,c,d,F,k,s,Ti)
#define MD5_DABC(F,k,s,Ti) MD5_IMPL(d,a,b,c,F,k,s,Ti)
#define MD5_CDAB(F,k,s,Ti) MD5_IMPL(c,d,a,b,F,k,s,Ti)
#define MD5_BCDA(F,k,s,Ti) MD5_IMPL(b,c,d,a,F,k,s,Ti)
#define MD5_IMPL(a,b,c,d,F,k,s,Ti) \
    a = MD5IMPL_rol(a + MD5IMPL_##F(b,c,d) + m_buf.w[k] + Ti, s) + b;

BEHAVIAC_FORCEINLINE /* only one call site */
void CMD5Hash::ProcessBlock()
{
    uint32_t a = m_abcd.w[0];
    uint32_t b = m_abcd.w[1];
    uint32_t c = m_abcd.w[2];
    uint32_t d = m_abcd.w[3];
    // Round 1
    MD5_ABCD(F, 0, 7, 0xD76AA478)  MD5_DABC(F, 1, 12, 0xE8C7B756)
    MD5_CDAB(F, 2, 17, 0x242070DB)  MD5_BCDA(F, 3, 22, 0xC1BDCEEE)
    MD5_ABCD(F, 4, 7, 0xF57C0FAF)  MD5_DABC(F, 5, 12, 0x4787C62A)
    MD5_CDAB(F, 6, 17, 0xA8304613)  MD5_BCDA(F, 7, 22, 0xFD469501)
    MD5_ABCD(F, 8, 7, 0x698098D8)  MD5_DABC(F, 9, 12, 0x8B44F7AF)
    MD5_CDAB(F, 10, 17, 0xFFFF5BB1)  MD5_BCDA(F, 11, 22, 0x895CD7BE)
    MD5_ABCD(F, 12, 7, 0x6B901122)  MD5_DABC(F, 13, 12, 0xFD987193)
    MD5_CDAB(F, 14, 17, 0xA679438E)  MD5_BCDA(F, 15, 22, 0x49B40821)
    // Round 2
    MD5_ABCD(G, 1, 5, 0xF61E2562)  MD5_DABC(G, 6, 9, 0xC040B340)
    MD5_CDAB(G, 11, 14, 0x265E5A51)  MD5_BCDA(G, 0, 20, 0xE9B6C7AA)
    MD5_ABCD(G, 5, 5, 0xD62F105D)  MD5_DABC(G, 10, 9, 0x02441453)
    MD5_CDAB(G, 15, 14, 0xD8A1E681)  MD5_BCDA(G, 4, 20, 0xE7D3FBC8)
    MD5_ABCD(G, 9, 5, 0x21E1CDE6)  MD5_DABC(G, 14, 9, 0xC33707D6)
    MD5_CDAB(G, 3, 14, 0xF4D50D87)  MD5_BCDA(G, 8, 20, 0x455A14ED)
    MD5_ABCD(G, 13, 5, 0xA9E3E905)  MD5_DABC(G, 2, 9, 0xFCEFA3F8)
    MD5_CDAB(G, 7, 14, 0x676F02D9)  MD5_BCDA(G, 12, 20, 0x8D2A4C8A)
    // Round 3
    MD5_ABCD(H, 5, 4, 0xFFFA3942)  MD5_DABC(H, 8, 11, 0x8771F681)
    MD5_CDAB(H, 11, 16, 0x6D9D6122)  MD5_BCDA(H, 14, 23, 0xFDE5380C)
    MD5_ABCD(H, 1, 4, 0xA4BEEA44)  MD5_DABC(H, 4, 11, 0x4BDECFA9)
    MD5_CDAB(H, 7, 16, 0xF6BB4B60)  MD5_BCDA(H, 10, 23, 0xBEBFBC70)
    MD5_ABCD(H, 13, 4, 0x289B7EC6)  MD5_DABC(H, 0, 11, 0xEAA127FA)
    MD5_CDAB(H, 3, 16, 0xD4EF3085)  MD5_BCDA(H, 6, 23, 0x04881D05)
    MD5_ABCD(H, 9, 4, 0xD9D4D039)  MD5_DABC(H, 12, 11, 0xE6DB99E5)
    MD5_CDAB(H, 15, 16, 0x1FA27CF8)  MD5_BCDA(H, 2, 23, 0xC4AC5665)
    // Round 4
    MD5_ABCD(I, 0, 6, 0xF4292244)  MD5_DABC(I, 7, 10, 0x432AFF97)
    MD5_CDAB(I, 14, 15, 0xAB9423A7)  MD5_BCDA(I, 5, 21, 0xFC93A039)
    MD5_ABCD(I, 12, 6, 0x655B59C3)  MD5_DABC(I, 3, 10, 0x8F0CCC92)
    MD5_CDAB(I, 10, 15, 0xFFEFF47D)  MD5_BCDA(I, 1, 21, 0x85845DD1)
    MD5_ABCD(I, 8, 6, 0x6FA87E4F)  MD5_DABC(I, 15, 10, 0xFE2CE6E0)
    MD5_CDAB(I, 6, 15, 0xA3014314)  MD5_BCDA(I, 13, 21, 0x4E0811A1)
    MD5_ABCD(I, 4, 6, 0xF7537E82)  MD5_DABC(I, 11, 10, 0xBD3AF235)
    MD5_CDAB(I, 2, 15, 0x2AD7D2BB)  MD5_BCDA(I, 9, 21, 0xEB86D391)
    m_abcd.w[0] += a;
    m_abcd.w[1] += b;
    m_abcd.w[2] += c;
    m_abcd.w[3] += d;
}

#undef MD5_ABCD
#undef MD5_DABC
#undef MD5_CDAB
#undef MD5_BCDA
#undef MD5_IMPL

//--------------------------------------------------------------------------

void CMD5Hash::Start()
{
    m_msgLength[0] = m_msgLength[1] = 0;
    m_abcd.w[0] = 0x67452301;
    m_abcd.w[1] = 0xEFCDAB89;
    m_abcd.w[2] = 0x98BADCFE;
    m_abcd.w[3] = 0x10325476;
}

//--------------------------------------------------------------------------

void CMD5Hash::append(const uint8_t* data, uint32_t dataSize)
{
    uint32_t bufFill = (m_msgLength[0] >> 3) & 63;

    while (dataSize > 0)
    {
        m_msgLength[0] += 8;
        m_buf.b[bufFill++] = *data++;
        --dataSize;

        if (bufFill >= 64)
        {
            if (m_msgLength[0] == 0)
            {
                m_msgLength[1]++;
            }

#if BEHAVIAC_BIGENDIAN

            for (uint32_t i = 0; i < 16; ++i)
            {
                SwapByte(m_buf.w[i]);
            }

#endif
            ProcessBlock();
            bufFill = 0;
        }
    }
}

//--------------------------------------------------------------------------

void CMD5Hash::Finish(SMD5Digest& digest)
{
    static const uint8_t padding[64] =
    {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    uint32_t count[2];
    count[0] = m_msgLength[0];
    count[1] = m_msgLength[1];
#if BEHAVIAC_BIGENDIAN
    SwapByte(count[0]);
    SwapByte(count[1]);
#endif
    uint32_t padLength = 64 - (((m_msgLength[0] >> 3) + 8) & 63);
    append(padding, padLength);
    append(reinterpret_cast<uint8_t*>(count), 8);
    BEHAVIAC_ASSERT((m_msgLength[0] & 63) == 0);
#if BEHAVIAC_BIGENDIAN
    SwapByte(m_abcd.w[0]);
    SwapByte(m_abcd.w[1]);
    SwapByte(m_abcd.w[2]);
    SwapByte(m_abcd.w[3]);
#endif

    for (uint32_t i = 0; i < 16; ++i)
    {
        digest.m_digest[i] = m_abcd.b[i];
    }

    // *** If you were going to use this hash function for anything
    // *** sensitive, you would want to clear its state here...
    //m_msgLength[0] = m_msgLength[1] = 0;
    //memset(&m_buf, 0, sizeof(m_buf));
    //memset(&m_abcd, 0, sizeof(m_abcd));
}

//-------------------------------------------------------------------------

#ifdef DEBUG_MD5HASH

// this test code is adapted from the appendix of RFC-1321.
static const char* const test[7 * 2] =
{
    "", "d41d8cd98f00b204e9800998ecf8427e",
    "a", "0cc175b9c0f1b6a831c399e269772661",
    "abc", "900150983cd24fb0d6963f7d28e17f72",
    "message digest", "f96b697d7cb7938d525a2f31aaf161d0",
    "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f",
    "12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a"
};

void TestMD5()
{
    int status = 0;

    for (int i = 0; i < sizeof(test) / sizeof(test[0]); i += 2)
    {
        CMD5Hash hash;
        SMD5Digest digest;
        hash.Start();
        uint32_t length = (uint32_t)strlen(test[i]);
        hash.append((const uint8_t*)test[i], length);
        hash.Finish(digest);
        char hex_output[16 * 2 + 1];

        for (int di = 0; di < 16; ++di)
        {
            string_sprintf(hex_output + di * 2, "%02x", digest.m_digest[di]);
        }

        if (strcmp(hex_output, test[i + 1]))
        {
            BEHAVIAC_LOGINFO("MD5 (\"%s\") = %s", test[i], hex_output);
            BEHAVIAC_LOGINFO("**** ERROR, should be: %s\n", test[i + 1]);
            status = 1;
        }
    }

    if (status == 0)
    {
        BEHAVIAC_LOGINFO("md5 self-test completed successfully.");
    }

    return status;
}
#endif  // DEBUG_MD5HASH

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
