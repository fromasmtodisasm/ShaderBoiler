// Copyright 2017 Stanislav Pidhorskyi. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include <memory>
#include <string>

#include "node.h"

namespace sb
{
	template<typename T, int S>
	class array;

	namespace detail
	{
		class variable
		{
		public:
			variable() : src(nodePtr(new node()))
			{}

			// Pointer to a node in compute graph
			nodePtr src;

			// This pointer is not null only for reference variables (output variable, return of subscript operator on array, etc.). 
			// It is a pointer to the record (which is a pointer to the value variable), which is used to replace the value node if the it was modified by the reference. 
			nodeshellWeakPtr shell;
		};

		typedef std::shared_ptr<variable> varPtr;

		template<node::DataType T, node::DataSize S, node::DataSize S2 = node::size1>
		class typed_variable : public variable
		{
			template<typename T, int S> friend class ::sb::array;
		public:
			typed_variable()
			{
				Init();
			}

			typed_variable(const std::string& name)
			{
				Init();
				src->name = name;
			}

			typed_variable(const std::string& name, node::OpType t)
			{
				Init();
				src->name = name;
				src->optype = t;
			}

		private:
			enum
			{
				type = T,
				sizeM = S,
				sizeN = S2
			};

			void Init()
			{
				src->datatype = T;
				src->datasize = S;
				src->datasize_secondary = S2;
			}
		};

		// Free function to access node from pointer to variable object. 
		// Needed to work objects of with declarated, but not defined classes derivated from typed_variable
		template<typename T>
		inline nodePtr GetPtr(T& x)
		{
			return x.src;
		}
	}
}
