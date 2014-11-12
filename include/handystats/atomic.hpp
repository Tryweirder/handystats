/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HANDYSTATS_ATOMIC_HPP_
#define HANDYSTATS_ATOMIC_HPP_

#include <handystats/common.h>

#if GCC_VERSION >= 40500
	#include <atomic>
#else
	#include <cstdatomic>

	template <typename T>
	void
	std::atomic<T*>::store(T* value, std::memory_order order) volatile {
		this->exchange(value, order);
	}
#endif

#endif // HANDYSTATS_ATOMIC_HPP_
