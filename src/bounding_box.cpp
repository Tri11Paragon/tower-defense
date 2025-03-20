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
}
