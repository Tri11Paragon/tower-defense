#pragma once
/*
 *  Copyright (C) 2024  Brett Terpstra
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

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <blt/math/vectors.h>
#include <blt/std/types.h>
#include <vector>

namespace td
{
	class bounding_box_t
	{
	public:
		bounding_box_t(const float xMin, const float yMin, const float xMax, const float yMax): m_min{xMin, yMin}, m_max{xMax, yMax}
		{}

		bounding_box_t(const blt::vec2& min, const blt::vec2& max): m_min{min}, m_max{max}
		{}

		[[nodiscard]] bool contains(const blt::vec2& point) const;

		[[nodiscard]] blt::vec2 get_center() const;

		[[nodiscard]] blt::vec2 get_size() const;

		[[nodiscard]] bool intersects(const bounding_box_t& other) const;

		[[nodiscard]] blt::vec2 get_min() const
		{
			return m_min;
		}

		[[nodiscard]] blt::vec2 get_max() const
		{
			return m_max;
		}

	private:
		blt::vec2 m_min, m_max;
	};

	class b_box_node_t
	{
	public:
		b_box_node_t(const blt::u8 parentIndex, bounding_box_t* bBox, std::vector<blt::u8> childIndices): parentIndx(parentIndex), bounds(bBox), childIndcs(childIndices)
		{}

		b_box_node_t(const blt::u8 parentIndex, bounding_box_t* bBox, const blt::u8 depth): parentIndx(parentIndex), bounds(bBox), depth(depth)
		{}

		void add_child_index(blt::u8 index)
		{
			childIndcs.push_back(index);
		}

		[[nodiscard]] std::vector<blt::u8> get_children() const
		{
			return childIndcs;
		}
		[[nodiscard]] blt::u8 get_depth()
		{
			return depth;
		}
		[[nodiscard]] bounding_box_t* get_bounds()
		{
			return bounds;
		}
	private:
		std::vector<blt::u8> childIndcs;
		blt::u8 parentIndx;
		blt::u8 depth;
		bounding_box_t* bounds;
	};

	class b_box_hierarchy_t
	{
	public:
		void add_node(bounding_box_t* bBox);
		blt::u8 size() {
			return nodeTree.size();
		}
		[[nodiscard]] b_box_node_t container_group(const blt::vec2& point) const;
		[[nodiscard]] std::vector<bounding_box_t*> contains(const blt::vec2& point) const;
		[[nodiscard]] std::vector<bounding_box_t*> intersections(const bounding_box_t& other) const;
	private:
		std::vector<b_box_node_t> nodeTree;
		blt::u8 search_children(bounding_box_t& other, blt::u8 startIndx);
	};
}

#endif //BOUNDING_BOX_H
