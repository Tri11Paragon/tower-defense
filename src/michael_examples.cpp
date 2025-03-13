/*
 *  <Short Description>
 *  Copyright (C) 2025  Brett Terpstra
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// this is the dynamic array (ArrayList<> from Java) of C++
// Any time you think you might need new/delete, especially new[]/delete[], you probably want this.
#include <vector>

// usually this is the only logging header you need to manually bring in. status.h is also a good one
#include <blt/logging/logging.h>
// this is the base iterator header and includes all others. If you only need one type of iterator I would suggest manually importing those ones
// over brining in the dependency on all of them. This way compile times are lower
#include <blt/iterator/iterator.h>

// When using hashmaps, please use blt's internal hashmap file. This will attempt to use the most optimal hashmap.
// If the parallel-hashmaps submodule isn't included it will use the std::unordered_map
// this brings in blt::hashset_t<K> and blt::hashmap<K, V>
#include <blt/std/hashmap.h>

// this is also a very useful header. it defines rust style fixed size integer types. blt::i32
#include <blt/std/types.h>

#include <blt/std/assert.h>

void michael_examples()
{
	/*
	 * Let's start with some basic modern C++.
	 * You know from 2P95 that copy constructors are a thing, right?
	 */
	struct rule_of_3_t // as a side note, please denote classes with _t, this way when defined a variable you can do test1_t test1; without error.
	{
	public:
		// Earl never taught this, but this is called the rule of 0/3/5
		rule_of_3_t() = default;

		// if you have a copy constructor, copy assignment, or destructor defined, then you must have the other two as well.
		rule_of_3_t(const rule_of_3_t& copy)
		{
			// do copying
		}

		rule_of_3_t& operator=(const rule_of_3_t& copy)
		{
			// always check for self assignment
			if (this == &copy)
				return *this;
			// do copying but where it's assignment to an exiting valid object
			// Note: in this case you may have to cleanup old resources.
			return *this;
		}

		// In this case we now have completed the rule of 3.
		// you can use = default to make the compiler generate the default for this class or = delete; to disallow the usage of that function
		// copy constructor -> you can't copy the object
		// copy assignment -> you can't copy the object but only when assigning to another object (this is sometimes useful)
		// destructor -> can't delete the object
		~rule_of_3_t() = delete;
	};

	// quickly on move semantics.
	struct rule_of_5_t
	{
		// member initializer list constructor. Please use this style to init class variables
		// the initializer is run before any other code, thus doing it inline guarantees the class is in a valid state.
		explicit rule_of_5_t(const blt::i32 id): m_id(id)
		{
			BLT_ASSERT(m_id > 0 && "id must be greater than 0 (zero is used to represent empty state)");
			BLT_TRACE("I ({}) was constructed", m_id);
		}

		// when using either std::trivially_copyable_v<T> types or types with a copy constructor defined,
		// clion will warn that a default constructor can be used.
		// This is because the compiler can automatically generate a default constructor for you.
		// We define one here for education
		rule_of_5_t(const rule_of_5_t& copy): m_id(copy.m_id)
		{
			BLT_TRACE("I ({}) was copied constructed", m_id);
		}

		// moving an object is not required to make the old object invalid, but it is a good idea to do so.
		// if m_id was a pointer, it would need to be set to null, as the destructor is run on moved-from objects
		// meaning you would get a double free error
		rule_of_5_t(rule_of_5_t&& move) noexcept: m_id(std::exchange(move.m_id, 0))
		{
			BLT_TRACE("I ({}) was moved constructed", m_id);
		}

		rule_of_5_t& operator=(const rule_of_5_t& copy)
		{
			// always check for self assignment
			if (this == &copy)
				return *this;
			m_id = copy.m_id;
			BLT_TRACE("I ({}) was copied assigned", m_id);
			return *this;
		}

		rule_of_5_t& operator=(rule_of_5_t&& move) noexcept
		{
			// always check for self assignment
			if (this == &move)
				return *this;
			m_id = std::exchange(move.m_id, 0);
			BLT_TRACE("I ({}) was moved assigned", m_id);
			return *this;
		}

		~rule_of_5_t()
		{
			// take a look inside blt tests for examples on how to use the logging library
			BLT_TRACE("I ({}) am deleted", m_id);
		}

	private:
		// member (private) variables should be prefixed with m_
		blt::i32 m_id = 0;
	};

	// look at the output from this, you will understand what is going on.
	{
		// scoped block means lifetimes of objects only exist for the lifetime of the block
		rule_of_5_t test1(1); // constructed
		rule_of_5_t test2(2); // constructed

		std::vector<rule_of_5_t> vector_of_rules; // vector is empty
		vector_of_rules.emplace_back(test1); // vector expands internally to size 1, object is copy constructed
		vector_of_rules.emplace_back(std::move(test2)); // vector needs to expand, this is moved, then (1) is also moved (that sounds suboptimal right?)

		// so test1, test2, and vector_of_rules will be destroyed here.

		// (hint) order of destruction
		// I (0) am deleted -> (1) that was moved when the vector expanded
		// I (1) am deleted -> (1) that is in the vector
		// I (2) am deleted -> (2) that is in the vector
		// I (0) am deleted -> test2 object
		// I (1) am deleted -> test1 object
		// you'll notice that destruction is in reverse order of declaration
	}

	/*
	 * You know from 2P95 that we can use new/delete to allocate memory.
	 * Don't do that. Fun fact: you basically never need to use new/delete anymore
	 * If you need a dynamic array (new[]/delete[]) then you can use a vector instead.
	 * Vectors automatically have copy/move semantics defined for elements inside,
	 * so when you use them inside a class def, that class automatically has its move and copy constructors filled out.
	 * What this means is, using standard headers, you never need to worry about manual memory management.
	 * This is called the rule of 0. You don't need to define copy/move/destructors when the underlying types handle it for you.
	 */
}
