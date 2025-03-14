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

#ifndef FWDDECL_H
#define FWDDECL_H

#include <utility>
#include <blt/std/types.h>

namespace td
{
	enum class damage_type_t : blt::u8
	{
		BASE      = 0,
		WAG_TAIL  = 1,
		KISSES    = 2,
		HUGS	  = 4,
		CUDDLES   = 8,
		LUST	  = 16,
		LOVE      = 32 // if on enemy it means only love can damage, otherwise on towers love can damage all types
	};

	template<typename... Args>
	blt::u8 bitwise_or(Args&&... args)
	{
		return (static_cast<blt::u8>(std::forward<Args>(args)) | ...);
	}

	inline bool has_type(const blt::u8 val, damage_type_t type)
	{
		return (val & static_cast<blt::u8>(type)) != 0;
	}
}

#endif //FWDDECL_H
