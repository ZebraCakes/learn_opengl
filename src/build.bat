@echo off

SET CommonCompilerFlags=/I ..\src\include -I ..\glfw\include -MDd -nologo -fp:fast -Od -WX -W4 -DAMP_INTERNAL=1 -DAMP_SLOW=1 -FC -Z7
SET CommonLinkerFlags=-incremental:no -opt:ref opengl32.lib user32.lib gdi32.lib shell32.lib glfw3.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

cl %CommonCompilerFlags% ..\src\%1 -link %CommonLinkerFlags%

IF EXIST ..\src\shaders xcopy ..\src\shaders shaders\ /E/H/y

popd