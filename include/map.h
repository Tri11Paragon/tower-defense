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
#include <fwddecl.h>
#include <enemies.h>

namespace td
{

	class curve_t
	{
	public:
		blt::vec2 get_point(float t) const;
	private:
		blt::vec2 p0, p1, p2;
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
