add_rules("mode.debug", "mode.release")

add_requires("glfw")

if is_plat("linux") then
    set_toolset("cc", "gcc")
    set_toolset("ar", "gcc")
    set_toolset("ld", "gcc")
    set_toolset("sh", "gcc")
elseif is_plat("macosx") then
    set_toolset("cc", "clang")
    set_toolset("ar", "clang")
    set_toolset("ld", "clang")
    set_toolset("sh", "clang")
elseif is_plat("mingw") then 
    set_toolset("cc", "i686-w64-mingw32-gcc")
    set_toolset("ar", "i686-w64-mingw32-gcc")
    set_toolset("ld", "i686-w64-mingw32-gcc")
    set_toolset("sh", "i686-w64-mingw32-gcc")
end


target("MandelbrotGPUView")
    set_languages("c99")
    set_warnings("pedantic")
    set_kind("binary")
    
    add_rules("utils.bin2c", {
        extensions = {".fs", ".vs"}
    })
    add_files("src/**.c", "deps_src/gl.c", "src/**.fs", "src/**.vs")
    add_includedirs("src", "deps")
    add_packages("glfw")
    -- set_rundir("$(projectdir)")
    if is_mode("debug") then 
        set_symbols("debug")
        set_optimize("none")
        add_defines("HE_DEBUG")
    elseif is_mode("release") then
        set_policy("build.optimization.lto", true)
        --set_strip(true)
        set_symbols("hidden")
        set_fpmodels("fast")
        set_optimize("aggressive")
    end 
    add_links("m")

    if is_plat("mingw") then
        add_ldflags("-mcrtdll=msvcrt-os", {force=true})
    end

    
    
