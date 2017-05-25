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
#include <string>
#include <list>

#include "datatypes.h"

namespace sb
{
	namespace detail
	{
		template<typename T>
		void SetStrongShellPointer(T& var, detail::nodeshellPtr ptr)
		{
		}

		// Needed to propagate shell pointer through multidimentional arrays.
		template<typename T, int S>
		void SetStrongShellPointer(array<T, S>& var, detail::nodeshellPtr ptr)
		{
			var.strongPtrShell = ptr;
		}
	}

	template<typename T, int S>
	class array : public detail::typed_variable<static_cast<detail::node::DataType>(T::type), static_cast<detail::node::DataSize>(T::sizeM), static_cast<detail::node::DataSize>(T::sizeN)>
	{
	public:
		array(): strongPtrShell(new detail::nodeshell(src))
		{
			src->optype = detail::node::array_declaration;
			src->arraySize = S;
		}
		array(const std::string name) : name(name), strongPtrShell(new detail::nodeshell(src))
		{
			src->optype = detail::node::array_declaration;
			src->arraySize = S;
		}
		ivec1 length()
		{
			return S;
		}
		T& operator[](ivec1 i)
		{
			T* result = new T();

			SetStrongShellPointer(*result, strongPtrShell);

			result->src->optype = detail::node::arrayLookup;

			if (strongPtrShell->n != src)
			{
				detail::nodePtr junction(new detail::node());

				junction->optype = detail::node::dependency;
				junction->childs.push_back(strongPtrShell->n);
				junction->childs.push_back(src);

				result->src->childs.push_back(junction);
			}
			else
			{
				result->src->childs.push_back(src);
			}

			result->src->childs.push_back(i.src);
			
			shell = strongPtrShell;

			result->shell = shell;

			garbageVars.push_back(detail::varPtr(result));
			return *result;
		}

		array<T, S>& SetName(const std::string& name)
		{
			src->name = name;
			return *this;
		}

		detail::nodeshellPtr strongPtrShell;

	private:
		std::list<detail::varPtr> garbageVars; // destroyed when array is destoyed
		std::string name;
	};
}
