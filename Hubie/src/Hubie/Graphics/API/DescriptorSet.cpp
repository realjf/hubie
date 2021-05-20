#include "hbpch.h"
#include "DescriptorSet.h"

namespace Hubie
{
	namespace Graphics
	{
		DescriptorSet* (*DescriptorSet::CreateFunc)(const DescriptorInfo&) = nullptr;

		DescriptorSet* DescriptorSet::Create(const DescriptorInfo& info)
		{
			HB_ASSERT(CreateFunc, "No DescriptorSet Create Function");

			return CreateFunc(info);
		}
	}
}
