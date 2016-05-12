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

#ifndef _BEHAVIAC_REAL_BASETYPE_COMPOSE_H_INCLUDED
#define _BEHAVIAC_REAL_BASETYPE_COMPOSE_H_INCLUDED

#include "behaviac/base/meta/collection.h"

namespace behaviac
{
    namespace Meta
    {
        namespace Details
        {
            Inherit Implementation forward declaration
            template <   typename SequenceType,
                     template< typename Type, typename BaseType > class NodeType,
                     typename RootType = Empty >
            struct InheritImpl;
        }    namespace Details

                /// Inherit utility
                ///
                /// based on Boost::mpl::inherit
                ///
                /// Use this utility to create linear class hierarchy. You need to provide
                /// a list of type defined with the help of Type::Collection tool. Next, a node
                /// class containing two template parameters. The first parameter is the
                /// current Type (from those specified in Type::Collection). The second is the
                /// current base class that you must derived from. Finally, a Root class
                /// that is going to be the top class in the hierarchy.
                ///
                /// Ex:
                ///
                /// template< typename Type, typename Base >
                /// class Node : public Base
                /// {
                ///     virtual void Fun( Type * ) = 0;
                /// };
                ///
                /// class Root {};
                ///
                /// typedef typename Type::Collection< int, short, ( etc... ) > List;
                ///
                /// typedef Type::Inherit< List, Node, Root >::Result GeneratedClass;
                ///
                /// --------
                /// | Root |
                /// --------
                ///    ^
                ///    |
                /// ----------------------------------
                /// | Node< long, Root >             |
                /// |---------------------------------
                /// | virtual void Fun( long * ) = 0;|
                /// ----------------------------------
                ///    ^
                ///    |
                /// -----------------------------------------------------
                /// | InheritImpl< Sequence< long, Null >, Node, Root > |
                /// -----------------------------------------------------
                ///   ^
                ///   |
                /// --------------------------------------------------------------------
                /// | Node< short, InheritImpl< Sequence< long, Null >, Node, Root > > |
                /// |-------------------------------------------------------------------
                /// | virtual void Fun( short * ) = 0;                                 |
                /// --------------------------------------------------------------------
                ///    ^
                ///    |
                /// ------------
                /// | (etc...) |
                /// ------------
                ///    ^
                ///    |
                /// ------------------
                /// | GeneratedClass |
                /// ------------------

                template <   typename VectorType,
                template< typename Type, typename BaseType > class NodeType,
                typename RootType = Empty >
        struct Inherit
        {
            typedef typename VectorType::Type SequenceType;
            typedef typename Details::InheritImpl< SequenceType, NodeType, RootType > Result;
        };

        template <   template< typename Type, typename BaseType > class NodeType,
                 typename RootType >
        struct Inherit< Null, NodeType, RootType >
        {
            typedef RootType Result;
        };
    }
}

#include "behaviac/base/meta/inherit.hpp"

#endif
