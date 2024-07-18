@echo off
cls

SET includes=/external:W0 /MD /external:I "../ThirParty/include"
SET libs=/link /LIBPATH:"../ThirParty/lib" SDL2.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib user32.lib gdi32.lib 
SET initial=/nologo /std:c++20 /W0 /EHsc -Zi 

SET dll_link= /EXPORT:game_update /EXPORT:game_init /EXPORT:game_render /EXPORT:game_close

if not exist build mkdir build

pushd build 
cl %initial% %includes% ..\main.cpp %libs%
popd

pushd build
START main.exe
popd
