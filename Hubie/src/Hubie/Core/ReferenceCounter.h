#pragma once
#include "Core.h"
#include <stdint.h>
#include <atomic>

namespace Hubie
{
	struct HB_EXPORT ReferenceCounter
	{
		std::atomic<int> count = 0;

	public:
		inline bool Ref()
		{
			count++;
			return count != 0;
		}

		inline int RefValue()
		{
			count++;
			return count;
		}

		inline bool Unref()
		{
			--count;
			bool del = count == 0;
			return del;
		}

		inline int Get() const
		{
			return count;
		}

		inline void Init(int p_value = 1)
		{
			count = p_value;
		}
	};
}
