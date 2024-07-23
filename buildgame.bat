@echo off
cls

SET includes=/external:W0 /MD /external:I "../ThirParty/include"
SET libs=/link /LIBPATH:"../ThirParty/lib" SDL2.lib SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib user32.lib gdi32.lib 
SET initial=/nologo /std:c++20 /w /EHsc -Zi 

SET dll_link= /EXPORT:game_update_and_render 

if not exist build mkdir build

pushd build 
cl -FC %initial% %includes% ..\game.cpp -Fmgame.map -LD %libs% -opt:ref -incremental:no %dll_link%
popd

