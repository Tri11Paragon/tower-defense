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

#ifndef ENEMIES_H
#define ENEMIES_H

#include <fwddecl.h>
#include <string>
#include <vector>
#include <blt/std/types.h>
#include <bounding_box.h>

namespace td
{
	// define enemies here
	// if you add more you must register them.
	enum class enemy_id_t
	{
		TEST
	};

	struct enemy_instance_t
	{
		enemy_id_t id;
		float health_left;
		float percent_along_path;
	};

	class enemy_t
	{
	public:
		enemy_t(std::string texture_name, std::vector<enemy_id_t> children, damage_type_t damage_resistence, float health = 1, float damage = 1,
				float speed = 1.0f);

		enemy_t(std::string texture_name, std::vector<enemy_id_t> children);

		[[nodiscard]] float get_health() const
		{
			return m_health;
		}

		[[nodiscard]] damage_type_t get_damage_resistence() const
		{
			return m_damage_resistence;
		}

		[[nodiscard]] const std::vector<enemy_id_t>& get_children() const
		{
			return m_children;
		}

		[[nodiscard]] const std::string& get_texture_name() const
		{
			return m_texture_name;
		}

		[[nodiscard]] float get_damage() const
		{
			return m_damage;
		}

		[[nodiscard]] float get_speed() const
		{
			return m_speed;
		}

		enemy_t& set_health(const float value)
		{
			m_health = value;
			return *this;
		}

		enemy_t& set_damage_resistence(const damage_type_t value)
		{
			m_damage_resistence = value;
			return *this;
		}

		enemy_t& set_children(const std::vector<enemy_id_t>& value)
		{
			m_children = value;
			return *this;
		}

		enemy_t& set_texture_name(const std::string& value)
		{
			m_texture_name = value;
			return *this;
		}

		enemy_t& set_m_damage(const float m_damage)
		{
			this->m_damage = m_damage;
			return *this;
		}

		enemy_t& set_m_speed(const float m_speed)
		{
			this->m_speed = m_speed;
			return *this;
		}

	private:
		std::string m_texture_name;
		std::vector<enemy_id_t> m_children;
		damage_type_t m_damage_resistence = damage_type_t::BASE;
		float m_health = 1;
		float m_damage = 1;
		float m_speed = 1.0f;
	};

	class enemy_database_t
	{
	public:
		void add_enemy(enemy_id_t enemy_id, const enemy_t& enemy)
		{
			const auto index = static_cast<blt::i32>(enemy_id);
			if (static_cast<blt::i32>(enemies_registry.size()) <= index)
				enemies_registry.resize(index + 1, enemy_t{"no_enemy_texture", {}});
			enemies_registry[index] = enemy;
		}

	private:
		void register_entities();

		std::vector<enemy_t> enemies_registry;
	};
}

#endif //ENEMIES_H
