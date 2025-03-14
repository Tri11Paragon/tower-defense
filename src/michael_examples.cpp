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

/*
 * Naming Conventions
 */
// most classes, functions, variable names, etc should use lower_snake_case
// enum / enum class should use lower_snake_case for the type name, then UPPER_SNAKE_CASE for elements
// class member (private) variables should be prefixed with m_
// prefer enum class defs over enum defs, they are namespaced.

void refs();

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
			// BLT_ASSERT(m_id > 0 && "id must be greater than 0 (zero is used to represent empty state)");
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

	BLT_TRACE("--{}--", "{Rule of 5}");

	// look at the output from this, you will understand what is going on.
	// we should talk about R and L value references right?
	// soon! (keep this concept in mind, basically R value references are temporary objects, L values point to real objects)
	{
		// scoped block means lifetimes of objects only exist for the lifetime of the block
		rule_of_5_t test1(1); // constructed
		rule_of_5_t test2(2); // constructed

		std::vector<rule_of_5_t> vector_of_rules; // vector is empty
		vector_of_rules.emplace_back(test1); // vector expands internally to size 1, object is copy constructed
		vector_of_rules.
		emplace_back(std::move(test2)); // vector needs to expand, this is moved, then (1) is also moved (that sounds suboptimal right?)

		// so test1, test2, and vector_of_rules will be destroyed here.

		// (hint) order of destruction
		// I (0) am deleted -> (1) that was moved when the vector expanded
		// I (1) am deleted -> (1) that is in the vector
		// I (2) am deleted -> (2) that is in the vector
		// I (0) am deleted -> test2 object
		// I (1) am deleted -> test1 object
		// you'll notice that destruction is in reverse order of declaration
	}

	BLT_TRACE("");
	BLT_TRACE("--{}--", "{Rule of 0}");

	/*
	 * You know from 2P95 that we can use new/delete to allocate memory.
	 * Don't do that. Fun fact: you basically never need to use new/delete anymore
	 * If you need a dynamic array (new[]/delete[]) then you can use a vector instead.
	 * Vectors automatically have copy/move semantics defined for elements inside,
	 * so when you use them inside a class def, that class automatically has its move and copy constructors filled out.
	 * What this means is, using standard headers, you never need to worry about manual memory management.
	 * This is called the rule of 0. You don't need to define copy/move/destructors when the underlying types handle it for you.
	 */

	struct rule_of_0_t
	{
		// this is a vector of rule_of_5_t, use it to show that copy and move are already made for us
		std::vector<rule_of_5_t> m_rules;
	};

	{
		rule_of_0_t test3;
		// emplace_back is used to construct an object in place, you pass this function arguments. If you want to pass an already existing object,
		// you can use .push_back(object) instead.
		test3.m_rules.emplace_back(1);
		test3.m_rules.emplace_back(2);
		test3.m_rules.emplace_back(3);

		// this will make a copy
		rule_of_0_t copy_of_3 = test3;
		// this will make a move
		const rule_of_0_t move_of_3 = std::move(test3);
		// all of which is implicitly defined, because std::vector has respective move and copy constructors defined.

		// this function acts as a black box to the compiler. Can prevent optimizations in cases like this.
		blt::black_box(move_of_3);
	}

	BLT_TRACE("");
	BLT_TRACE("--{}--", "{std::vector reserve()}");

	// now remember the problem of vectors allocating, then moving objects around internally as a result?
	// well you can manually allocate a number of elements in a vector.
	{
		std::vector<rule_of_5_t> vector_of_rules;
		vector_of_rules.reserve(100); // vector is empty, but can store 100 elements before reallocating

		BLT_ASSERT(vector_of_rules.empty());
		BLT_ASSERT(vector_of_rules.capacity() == 100);

		for (int i = 0; i < 10; ++i)
			vector_of_rules.emplace_back(i + 1);
		// you'll notice in the console there's no sign of reallocation - objects are only constructed - in place
	}

	// Ok. Cool. If you know the number of elements you can reserve and not have to reallocate. But you said std::vector can replace basically
	// any dynamic array. How about when I know the number of elements, do I always have to use .emplace_back()/.push_back()????
	// the answer is of course not!
	BLT_TRACE("");
	BLT_TRACE("--{}--", "{std::vector resize()}");
	{
		// you can directly ask a size in the constructor (as long as you are not using an integer type as T)
		// (I don't tend to use this and prefer calling the function directly)
		// If the type is not default constructible you must provide the type to fill the requested elements with.
		// in which case the type must be copy constructible
		std::vector<rule_of_5_t> vector_of_rules{10, rule_of_5_t{0}};
		// or manually call resize
		vector_of_rules.resize(100, rule_of_5_t{0}); // vector has 100 elements of rule_of_5_t{0}

		// welcome to our first use of a BLT iterator
		// this is called a structured binding. You can use it to unpack structs. enumerate returns a tuple of <size_t, T>
		for (const auto [i, v] : blt::enumerate(vector_of_rules))
			v = rule_of_5_t{static_cast<int>(i)};

		// What this effectively means is:
		for (const auto& enumerate_element : blt::enumerate(vector_of_rules))
		{
			auto& i = enumerate_element.first;
			auto& v = enumerate_element.second;

			blt::black_box(i);
			blt::black_box(v);
			blt::black_box(enumerate_element);
		}

		// Oh, right the fun of C++'s casting.
		// In C and Java you have the C-style cast (Type) variable.
		// This is not good practice in C++
		// In C++ we have: (C-style casts will do one of these in some order based on what the compiler thinks is best. Not a good idea)
		// this one is the one you basically always want:
		// static_cast<type>(expression) -> converts the expression to the type. works for static type conversions, eg: int to float
		// dynamic_cast<ptr_or_ref_type>(expression) -> is used for OOP stuff. Is used to convert base class to derived class (I avoid OOP)
		// -- Danger Land -
		// reinterpret_cast<ptr_type>(&expression) -> tells the compiler we would like to interpret the pointer as this type.
		//		This type of cast can be very dangerous, it is undefined behaviour in a lot of cases if you are not careful.
		//		I do not remember all the silly rules with it. Basically: it works how you think it does until it doesn't
		// const_cast<type>(expression) -> can be used to add or remove const from an expression
		//		this can also create terrible UB. (imagine you make something const, then cast away that const and modify the underlying variable.
		//		That is major UB and will cause major problems at some point.
	}

	BLT_TRACE("");
	BLT_TRACE("");

	// Ok I think that covers the basics of copy and move semantics, lets quickly touch on blt::iterators
	// Well I guess we should cover iterators period.
	// So in Java we have iterators. We used them in Advanced OOP class with the stream,map,filter,etc. Same concept here
	// the C++ standard library has a lot of functions which do similar things / operate on iterators.
	// It would be useful to learn about some of them, look on the Cppreference.
	// I don't care for them, personally, especially as a lot of the good ones are locked to standards I can't use (C++20)
	// And to be honest, the Rust way of handling iterators is much more appealing.

	std::vector some_data{412.f, 4123.f, 3214.f, 5231.f, 1235.4f};
	std::vector<std::string> other_data{"hello", "there", "silly", "billy", "uwu"};
	blt::hashmap_t<std::string, float> some_map;
	some_map.emplace("world", 123.f);
	some_map.emplace(":3", 456.f);
	some_map.emplace("bully!", 789.f);
	some_map.emplace("Hooray a todd episode!", 5922.f);
	some_map.emplace("a gun", 321.f);

	// I would read up on C++ iterators,
	// here's the basics

	// the begin iterator is where the container starts. All containers which are iterable have a .begin and .end function
	// well actually I think, it has to be accessible by std::begin and std::end but that's a little too complicated for this
	// (if you have a public .begin / .end, these functions are automatically defined)
	const auto begin = some_data.begin();
	// the end iterator is one past the end of the data
	// if you think about iterators as generalizations of pointers, this makes sense.
	// for (int i = 0; i < size(); i++)
	// for (auto* ptr = data(); ptr != (data() + size()); ptr++)
	// for (auto begin = begin(); begin != end(); ++begin)
	const auto end = some_data.end();

	// it's very rare you actually need something like this, but I figure it's good to touch on.
	for (auto it = begin; it != end; ++it)
		blt::black_box(*it);

	// you can directly range-for loop iterate on objects with .begin/.end defined.
	for (auto object : some_data)
		blt::black_box(object);

	// if the object is larger than 16 bytes (fits in 1-2 registers) please use a reference, or (if possible a const reference)
	// or if the object has copy semantics, you use a reference to avoid making unnecessary copies.
	for (const auto& other : other_data)
		blt::black_box(other);

	// blt offers many options for iteration
	// all iterator containers can run any other functions (zip can enumerate, etc)

	// you can directly supply iterators
	for (const auto& v : blt::iterate(begin, end))
		blt::black_box(v);

	// as said you can use any number of functions on any iterate method
	for (const auto& [i,v] : blt::iterate(begin, end).enumerate())
		blt::black_box(v);

	// the most useful of blt::iterators are of course the enumerate function, a shortcut for use in containers is blt::enumerate
	for (const auto& [i,v] : blt::enumerate(some_data))
		blt::black_box(v);

	// show off reference stuff
	refs();
}

void refs()
{}
