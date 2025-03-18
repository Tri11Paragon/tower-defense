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

#ifndef MAP_H
#define MAP_H

#include <blt/math/vectors.h>
#include <blt/gfx/renderer/batch_2d_renderer.h>
#include <fwddecl.h>
#include <enemies.h>

namespace td
{

	struct curve_mesh_data_t
	{
		struct line_vertex_t
		{
			blt::vec3 pos;
			blt::vec2 uv;
		};

		[[nodiscard]] std::unique_ptr<blt::gfx::vertex_array_t> to_vertex_array() const;
		void populate_vertex_array(blt::gfx::vertex_array_t& va) const;

		[[nodiscard]] std::vector<line_vertex_t> calculate_vertices() const;

		curve_mesh_data_t& with(const curve_mesh_data_t& mesh)
		{
			lines.insert(lines.end(), mesh.lines.begin(), mesh.lines.end());
			return *this;
		}

		std::vector<blt::gfx::line2d_t> lines;
	};

	class curve_t
	{
	public:
		curve_t(blt::vec2 p0, blt::vec2 p1, blt::vec2 p2);
		curve_t(blt::vec2 p0, blt::vec2 p1, blt::vec2 p2, blt::vec2 p3);

		[[nodiscard]] blt::vec2 get_point(float t) const;

		[[nodiscard]] std::vector<blt::gfx::line2d_t> to_lines(blt::i32 segments) const;

		[[nodiscard]] curve_mesh_data_t to_mesh(blt::i32 segments) const;
	private:
		blt::vec2 m_p0, m_p1, m_p2, m_p3;
	};

	class path_segment_t
	{
	public:

	private:

	};

	class map_t
	{
	public:

	private:

	};

}

#endif //MAP_H
