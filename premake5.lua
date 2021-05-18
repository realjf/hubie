require ("Scripts/premake-defines")
require ("Scripts/premake-triggers")
require ("Scripts/premake-settings")

root_dir = os.getcwd()

Arch = ""

if _OPTIONS["arch"] then
    Arch = _OPTIONS["arch"]
else
    if _OPTIONS["os"] then
        _OPTIONS["arch"] = "arm"
        Arch = "arm"
    else
        _OPTIONS["arch"] = "x64"
        Arch = "x64"
    end
end

workspace ( settings.workspace_name )
location "build"
startproject "Sandbox"

    targetdir ("bin/%{outputdir}/")
    objdir ("bin-int/%{outputdir}/obj/")

    if Arch == "arm" then
        architecture = "ARM"
    elseif Arch == "x64" then
        architecture = "x64"
    elseif Arch == "x86" then
        architecture = "x86"
    end

    printf ("Arch = %s", Arch)

    configurations
    {
        "Debug",
        "Release",
        "Production",
    }

    platforms
    {
        "x64"
    }

    include "Hubie/premake5"
    include "Sandbox/premake5"
    -- include "Editor/premake5"
