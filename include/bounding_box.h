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
}

#endif //BOUNDING_BOX_H
