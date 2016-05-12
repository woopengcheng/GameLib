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

#ifndef BEHAVIAC_CORE_MD5_H
#define BEHAVIAC_CORE_MD5_H

#include "behaviac/base/base.h"

struct BEHAVIAC_API SMD5Digest
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(SMD5Digest);

public:
    uint8_t    m_digest[16];

protected:
    int cmp(SMD5Digest const& rhs) const
    {
        return memcmp(const_cast<uint8_t*>(m_digest),
                      const_cast<uint8_t*>(rhs.m_digest), sizeof(m_digest));
    }

public:
    bool operator ==(SMD5Digest const& rhs) const
    {
        return cmp(rhs) == 0;
    }
    bool operator !=(SMD5Digest const& rhs) const
    {
        return cmp(rhs) != 0;
    }
    bool operator < (SMD5Digest const& rhs) const
    {
        return cmp(rhs) < 0;
    }
    bool operator <=(SMD5Digest const& rhs) const
    {
        return cmp(rhs) <= 0;
    }
    bool operator > (SMD5Digest const& rhs) const
    {
        return cmp(rhs) > 0;
    }
    bool operator >=(SMD5Digest const& rhs) const
    {
        return cmp(rhs) >= 0;
    }
};

//--------------------------------------------------------------------------

class BEHAVIAC_API CMD5Hash
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMD5Hash);

public:
    CMD5Hash() { }

    void Start();
    void append(const uint8_t* data, uint32_t size);
    void Finish(SMD5Digest& digest);

    void append(SMD5Digest& digest)
    {
        append(digest.m_digest, sizeof(digest.m_digest));
    }

private:
    void ProcessBlock();

    union
    {
        uint8_t b[64];
        uint32_t w[16];
    }	m_buf;
    union
    {
        uint8_t b[16];
        uint32_t w[4];
    }	m_abcd;
    uint32_t									m_msgLength[2];
};

//--------------------------------------------------------------------------

#endif // #ifndef BEHAVIAC_CORE_MD5_H
