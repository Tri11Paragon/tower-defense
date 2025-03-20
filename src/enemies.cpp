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
#include <enemies.h>

td::enemy_t::enemy_t(std::string texture_name, std::vector<enemy_id_t> children, const damage_type_t damage_resistence, const float health,
					const float damage, const float speed): m_texture_name(std::move(texture_name)), m_children(std::move(children)),
																m_damage_resistence(damage_resistence), m_health(health), m_damage(damage),
																m_speed(speed)
{}

td::enemy_t::enemy_t(std::string texture_name, std::vector<enemy_id_t> children): m_texture_name(std::move(texture_name)),
																				m_children(std::move(children))
{}

void td::enemy_database_t::register_entities()
{
	add_enemy(enemy_id_t::TEST, enemy_t{"test", {}}.set_speed(10));
}
