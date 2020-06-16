//
//  GameController.hpp
//  Game_Project_1
//
//  Created by Parth Patel on 3/10/19.
//

#ifndef GameController_hpp
#define GameController_hpp

#include <stdio.h>


class GameController {
public:
    enum class GameStates { MAIN_MENU, OPTIONS, GAME, GAME_OVER };
    GameStates gameState;
};

#endif /* GameController_hpp */
