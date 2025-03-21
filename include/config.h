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

#ifndef CONFIG_H
#define CONFIG_H

#include <blt/std/types.h>

namespace td {
	// number of segments used to draw curves. The larger this number the more triangles are created.
	inline blt::i32 PATH_DRAW_SEGMENTS = 32;
	// number of segments used when creating static data, such as the length of a curve
	inline blt::i32 PATH_UPDATE_SEGMENTS = 64;

	inline float PATH_SPEED_MULTIPLIER = 2.0f;
}

#endif //CONFIG_H
