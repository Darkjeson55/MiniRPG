@echo off
cls

SET includes=/external:W0 /MD /external:I "F:\vcpkg\vcpkg\installed\x64-windows\include"
SET libs=/link /LIBPATH:"F:\vcpkg\vcpkg\installed\x64-windows\lib\" SDL2.lib SDL2_image.lib
SET initial=/nologo /std:c++20 /W0 /EHsc -Zi 

SET dll_link= /EXPORT:game_update /EXPORT:game_init /EXPORT:game_render /EXPORT:game_close

if not exist build mkdir build

pushd build 
cl -FC %initial% %includes% ..\game.cpp -Fmgame.map -LD %libs% -opt:ref -incremental:no %dll_link%
popd

