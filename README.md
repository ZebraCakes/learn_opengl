TO BUILD:

You'll need Visual Studio. I use 2019, though I'd be surprised if this didn't compile on earlier versions.

run "git submodule --init --update to pull the appropriate copy of amp_lib"

create "build" folder in root.

Download and build glfw3.
    Copy glfw3.lib to build

From src, run build.bat in vs command prompt supplying the cpp file you wish to compile.