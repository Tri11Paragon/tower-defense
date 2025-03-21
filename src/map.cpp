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
#include <config.h>
#include <map.h>
#include <blt/gfx/window.h>

namespace td
{
	path_segment_t::path_segment_t(const blt::gfx::curve2d_t& curve): m_bounding_box{get_bounding_box(curve, PATH_UPDATE_SEGMENTS)}, m_curve{curve},
																	m_curve_length{curve.length(PATH_UPDATE_SEGMENTS)}
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

	float map_t::update()
	{
		float damage = 0;
		for (blt::size_t i = 0; i < m_path_segments.size(); ++i)
		{
			auto& segment = m_path_segments[i];
			const auto length = segment.m_curve_length;
			for (const auto [j, enemy] : blt::enumerate(segment.m_enemies))
			{
				if (!enemy.is_alive)
					continue;
				const auto& enemy_info = m_database->get(enemy.id);
				const auto movement = (enemy_info.get_speed() / length) * PATH_SPEED_MULTIPLIER * blt::gfx::getFrameDeltaSeconds();
				enemy.percent_along_path += static_cast<float>(movement);
				if (enemy.percent_along_path >= 1)
				{
					enemy.is_alive = false;
					segment.m_empty_indices.emplace_back(j);
					if (i != m_path_segments.size() - 1)
					{
						auto& next_segment = m_path_segments[i + 1];
						enemy_instance_t moved_enemy{enemy.id, enemy.health_left};
						next_segment.add_enemy(moved_enemy);
					} else
						damage += enemy_info.get_damage();
				}
			}
		}

		return damage;
	}

	void map_t::draw(blt::gfx::batch_renderer_2d& renderer)
	{
		// TODO: this is currently for debug
		const auto mesh_data = get_mesh_data(10);
		renderer.drawCurve(mesh_data, blt::make_color(0, 1, 0));
		for (const auto& segment : m_path_segments)
		{
			for (const auto& enemy : segment.m_enemies)
			{
				if (!enemy.is_alive)
					continue;
				const auto point = segment.m_curve.get_point(enemy.percent_along_path);
				constexpr blt::vec2f size{10, 10};
				renderer.drawRectangle(blt::gfx::rectangle2d_t{point, size}, blt::make_color(1, 0, 0), 1);
			}
		}
	}

	blt::gfx::curve2d_mesh_data_t map_t::get_mesh_data(const float thickness) const
	{
		blt::gfx::curve2d_mesh_data_t mesh_data;
		for (const auto& segment : m_path_segments)
			mesh_data.with(segment.m_curve.to_mesh(PATH_DRAW_SEGMENTS, thickness));
		return mesh_data;
	}
}
