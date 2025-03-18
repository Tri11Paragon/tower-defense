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
#include <map.h>

namespace td
{
	blt::vec2 curve_t::get_point(const float t) const
	{
		const auto t_inv = 1.0f - t;
		const auto t_inv_sq = t_inv * t_inv;
		const auto t_sq = t * t;
		return t_inv_sq * p0 + 2.0f * t_inv * t * p1 + t_sq * p2;
	}
}
