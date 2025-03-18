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

#ifndef GAME_H
#define GAME_H

#include <enemies.h>
#include <vector>
#include <functional>
#include <string>

namespace td
{
	class game_t
	{
	public:

		void render();

		void update();

	private:
		std::vector<enemy_instance_t> enemies;
	};

	class event_handler_t
	{
		// I don't want to keep the parameter as a void pointer, maybe it would be nice to have a gameObject class or sumthing,
		// or diverge into unique event_handlers types, idk
		std::function<void(void*)> callback;

		// I'm preparing for when a handler would contain more than just a regular std::function, so this class would contain that stuff
	public:
		explicit event_handler_t(std::function<void(void*)> handler) : callback(std::move(handler))
		{};

		void call(void* target) const
		{
			return callback(target);
		}

	private:
	};

	class event_t
	{
	public:
		void callHandler(void* target)
		{
			handler.call(target);
		}

	private:
		event_handler_t handler;
		std::string signature;
	};

	class event_scheduler_t
	{
	public:
		void add_event_listener(event_t ev, event_handler_t handler);

	private:
		void* target = nullptr;
	};
}

#endif //GAME_H
