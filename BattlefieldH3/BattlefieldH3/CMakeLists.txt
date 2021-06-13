cmake_minimum_required(VERSION 3.19)
project(BattlefieldH3)

set(CMAKE_CXX_STANDARD 14)

#find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
#target_link_libraries(main PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

include_directories(.)
include_directories(Fonts)
include_directories(Textures)

add_executable(BattlefieldH3
        Fonts/bigfonts.ttf
        Fonts/micross.ttf
        Textures/Angel.png
        Textures/Archer.png
        Textures/background.jpg
        Textures/Cavalier.png
        Textures/CmBkDrMt.png
        "Textures/Creatures Portraits.png"
        Textures/Crusader.png
        Textures/Griffin.png
        Textures/Halberdier.png
        Textures/Marksman.png
        Textures/Monk.png
        Textures/ObDino3.png
        Textures/ObDRk01.png
        Textures/ObDRk02.png
        Textures/ObDRk03.png
        Textures/ObDRk04.png
        Textures/ObDtL02.png
        Textures/ObDtL03.png
        Textures/ObDtL04.png
        Textures/ObDtL06.png
        Textures/ObDtL10.png
        Textures/ObDtS04.png
        Textures/Pikeman.png
        "Textures/Royal Griffin.png"
        Textures/Swordsman.png
        Textures/Zealot.png
        AnimationComponent.cpp
        AnimationComponent.h
        BattlefieldH3.cpp
        Battlefiled.cpp
        Battlefiled.h
        BattleHandler.cpp
        BattleHandler.h
        BattlePathFinder.cpp
        BattlePathFinder.h
        BattleUnit.cpp
        BattleUnit.h
        Button.cpp
        Button.h
        Game.cpp
        Game.h
        GameConstants.cpp
        GameConstants.h
        Garrnison.cpp
        Garrnison.h
        Graphics.cpp
        Graphics.h
        GuiHandler.cpp
        GuiHandler.h
        InterfaceElem.cpp
        InterfaceElem.h
        MainMenu.h
        Monser.cpp
        Monser.h
        pch.cpp
        pch.h
        startMap.txt
        WindowObject.cpp
        WindowObject.h)
