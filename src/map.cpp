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
	path_segment_t::path_segment_t(const blt::gfx::curve2d_t& curve): m_bounding_box{get_bounding_box(curve, 32)}, m_curve{curve},
																	m_curve_length{curve.length(32)}
	{}

	bounding_box_t path_segment_t::get_bounding_box(const blt::gfx::curve2d_t& curve, const blt::i32 segments)
	{
		const auto lines = curve.to_lines(segments);
		auto min = lines.front().p1;
		auto max = lines.front().p1;
		for (const auto& line : lines)
		{
			if (line.p1 < min)
				min = line.p1;
			if (line.p2 < min)
				min = line.p2;
			if (line.p1 > max)
				max = line.p1;
			if (line.p2 > max)
				max = line.p2;
		}
		return bounding_box_t{min, max};
	}

	void map_t::update()
	{

	}

	blt::gfx::curve2d_mesh_data_t map_t::get_mesh_data(const float thickness) const
	{
		blt::gfx::curve2d_mesh_data_t mesh_data;
		for (const auto& segment : m_path_segments)
			mesh_data.with(segment.m_curve.to_mesh(32, thickness));
		return mesh_data;
	}
}
