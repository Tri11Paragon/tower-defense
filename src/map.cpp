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
	curve_t::curve_t(const blt::vec2 p0, const blt::vec2 p1, const blt::vec2 p2): m_p0(p0), m_p1(p1), m_p2(p1), m_p3(p2)
	{}

	curve_t::curve_t(const blt::vec2 p0, const blt::vec2 p1, const blt::vec2 p2, const blt::vec2 p3): m_p0(p0), m_p1(p1), m_p2(p2), m_p3(p3)
	{}

	blt::vec2 curve_t::get_point(const float t) const
	{
		const auto t_inv = 1.0f - t;
		const auto t_inv_sq = t_inv * t_inv;
		const auto t_inv_cub = t_inv_sq * t_inv;
		const auto t_sq = t * t;
		const auto t_cub = t_sq * t;
		return t_inv_cub * m_p0 + 3 * t_inv_sq * t * m_p1 + 3 * t_inv * t_sq * m_p2 + t_cub * m_p3;
	}

	std::vector<blt::gfx::line2d_t> curve_t::to_lines(const blt::i32 segments) const
	{
		std::vector<blt::gfx::line2d_t> lines;
		float t = 0;
		const float diff = 1.0f / static_cast<float>(segments);

		for (blt::i32 i = 0; i < segments; ++i)
		{
			auto begin = get_point(t);
			t += diff;
			auto end = get_point(t);

			lines.emplace_back(begin, end);
		}

		return lines;
	}

	std::unique_ptr<blt::gfx::vertex_array_t> curve_mesh_data_t::to_vertex_array() const
	{}

	void curve_mesh_data_t::populate_vertex_array(blt::gfx::vertex_array_t& va) const
	{}

	std::vector<curve_mesh_data_t::line_vertex_t> curve_mesh_data_t::calculate_vertices() const
	{
		std::vector<line_vertex_t> vertices;

		return vertices;
	}

	curve_mesh_data_t curve_t::to_mesh(const blt::i32 segments) const
	{
		curve_mesh_data_t mesh_data;
		mesh_data.lines = to_lines(segments);
		return mesh_data;
	}
}
