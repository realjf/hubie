project "freetype"
	kind "StaticLib"
	language "C"
    	staticruntime "on"
    	systemversion "latest"
	warnings "off"

	
	files {
     "freetype/include/config/ftconfig.h",
    "freetype/include/config/ftheader.h",
    "freetype/include/config/ftmodule.h",
    "freetype/include/config/ftoption.h",
    "freetype/include/config/ftstdlib.h",
    "freetype/include/ft2build.h",

    "freetype/src/autofit/autofit.c",
    "freetype/src/base/ftbase.c",
    "freetype/src/base/ftbbox.c",
    "freetype/src/base/ftbitmap.c",
    "freetype/src/base/ftdebug.c",
    "freetype/src/base/ftfstype.c",
    "freetype/src/base/ftgasp.c",
    "freetype/src/base/ftglyph.c",
    "freetype/src/base/ftgxval.c",
    "freetype/src/base/ftinit.c",
    "freetype/src/base/ftlcdfil.c",
    "freetype/src/base/ftmm.c",
    "freetype/src/base/ftotval.c",
    "freetype/src/base/ftpatent.c",
    "freetype/src/base/ftpfr.c",
    "freetype/src/base/ftstroke.c",
    "freetype/src/base/ftsynth.c",
    "freetype/src/base/ftsystem.c",
    "freetype/src/base/fttype1.c",
    "freetype/src/base/ftwinfnt.c",
    "freetype/src/bdf/bdf.c",
    "freetype/src/cache/ftcache.c",
    "freetype/src/cff/cff.c",
    "freetype/src/cid/type1cid.c",
    "freetype/src/gzip/ftgzip.c",
    "freetype/src/lzw/ftlzw.c",
    "freetype/src/pcf/pcf.c",
    "freetype/src/pfr/pfr.c",
    "freetype/src/psaux/psaux.c",
    "freetype/src/pshinter/pshinter.c",
    "freetype/src/psnames/psmodule.c",
    "freetype/src/raster/raster.c",
    "freetype/src/sfnt/sfnt.c",
    "freetype/src/smooth/smooth.c",
    "freetype/src/truetype/truetype.c",
    "freetype/src/type1/type1.c",
    "freetype/src/type42/type42.c",
    "freetype/src/winfonts/winfnt.c"
   }

buildoptions
{
	"-w"
}

   defines {
    "FT2_BUILD_LIBRARY",
   }

	sysincludedirs {
		"freetype/include"
    }
    
    includedirs {
		"freetype/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Production"
        runtime "Release"
        optimize "On"