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

#include <enemies.h>
#include <fwddecl.h>
#include <bounding_box.h>
#include <blt/gfx/renderer/batch_2d_renderer.h>
#include <blt/math/vectors.h>

namespace td
{
	class path_segment_t
	{
		friend map_t;

	public:
		explicit path_segment_t(const blt::gfx::curve2d_t& curve);

		[[nodiscard]] const bounding_box_t& get_bounding_box() const
		{
			return m_bounding_box;
		}

		void remove_enemy(const blt::size_t index)
		{
			m_enemies[index].is_alive = false;
			m_empty_indices.emplace_back(index);
		}

		void add_enemy(const enemy_instance_t& enemy)
		{
			if (!m_empty_indices.empty())
			{
				m_enemies[m_empty_indices.back()] = enemy;
				m_empty_indices.pop_back();
			} else
			{
				m_enemies.push_back(enemy);
			}
		}

	private:
		static bounding_box_t get_bounding_box(const blt::gfx::curve2d_t& curve, blt::i32 segments);

		bounding_box_t m_bounding_box;
		blt::gfx::curve2d_t m_curve;
		float m_curve_length;
		std::vector<enemy_instance_t> m_enemies;
		std::vector<blt::size_t> m_empty_indices;
	};

	class map_t
	{
	public:
		explicit map_t(const std::vector<path_segment_t>& path_segments, enemy_database_t& database): m_path_segments{path_segments},
																									m_database{&database}
		{}

		void spawn(const enemy_id_t id)
		{
			m_path_segments.front().add_enemy(enemy_instance_t{id, m_database->get(id).get_health()});
		}

		void draw(blt::gfx::batch_renderer_2d& renderer);

		float update();

		[[nodiscard]] blt::gfx::curve2d_mesh_data_t get_mesh_data(float thickness = 1) const;

	private:
		std::vector<path_segment_t> m_path_segments;
		enemy_database_t* m_database;
	};
}

#endif //MAP_H
