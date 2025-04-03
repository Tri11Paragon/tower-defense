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
#include <bounding_box.h>

namespace td
{
	bool bounding_box_t::contains(const blt::vec2& point) const
	{
		return point >= m_min && point <= m_max;
	}

	blt::vec2 bounding_box_t::get_center() const
	{
		const auto center = get_size() / 2.0f;
		return m_min + center;
	}

	blt::vec2 bounding_box_t::get_size() const
	{
		return m_max - m_min;
	}

	bool bounding_box_t::intersects(const bounding_box_t& other) const
	{
		return other.m_min <= m_max && other.m_max >= m_min;
	}

	blt::u8 b_box_hierarchy_t::search_children(bounding_box_t& other, blt::u8 startIndx) { // [NOT FINISHED]
		if (startIndx == 0)
		{
			for (blt::u8 i = startIndx; i < nodeTree.size(); i++)
			{
				if (nodeTree[i].get_bounds()->intersects(other)) return i;
			}
		}
		else
		{
			for (blt::u8 childIndex: nodeTree[startIndx].get_children()) {
				if (nodeTree[childIndex].get_bounds()->intersects(other)) return childIndex;
			}
		}
		return -1;
	}

	void b_box_hierarchy_t::add_node(bounding_box_t* bBox) { // [NOT FINISHED]
		if (nodeTree.size() == 0) return nodeTree.push_back(b_box_node_t(0, bBox, 0));

		blt::u8 parentIndex, i = -1;

		do
		{
			parentIndex = i;
		} while (i = search_children(*bBox, parentIndex) != -1);

		if (parentIndex != -1)
		{
			nodeTree.push_back(b_box_node_t(parentIndex, bBox, nodeTree[parentIndex].get_depth() + 1));
			nodeTree[parentIndex].add_child_index(nodeTree.size() - 1);
		}
	}
}
