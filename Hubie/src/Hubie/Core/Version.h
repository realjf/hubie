#pragma once

namespace Hubie
{
	struct InternalVersion
	{
		int major = 0;
		int minor = 1;
		int patch = 0;
	};

	constexpr InternalVersion const HubieVersion = InternalVersion();

	struct Version
	{
		enum class Stage
		{
			Preview,
			Alpha,
			Beta,
			RC,
			Release
		};

		int year = 2021;
		int release = 0;
		Stage stage = Stage::Alpha;
		int rev = 3;
	};

	constexpr Version const version = Version();
}