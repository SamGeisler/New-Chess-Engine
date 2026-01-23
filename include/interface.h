#pragma once

#include "game.h"
#include "move.h"

#include "SDL.h"

class Interface{
private:
    SDL_Window* window {NULL};
    SDL_Surface* screenSurface {NULL};
    SDL_Surface* currentSurface {NULL};
    SDL_Surface* numberSurface {NULL};
    SDL_Surface* pieceSurfaces[16];

    void loadPieceSurfaces();
    void drawBackground(uint64_t bb);
    SDL_Surface* loadSurface(const char * path);

    static constexpr int SWIDTH {1200};
    static constexpr int SHEIGHT {800};
    static constexpr bool FULLSCREEN {false};
    static constexpr uint32_t BGCOLOR {0x222222};
    static constexpr uint32_t LIGHT_SPACE_COLOR {0xEEEEEE};
    static constexpr uint32_t DARK_SPACE_COLOR {0x444444};
    static constexpr uint32_t LIGHT_ATTACKING {0xEE5555};
    static constexpr uint32_t DARK_ATTACKING {0xAA2222};
    static constexpr int SQUARE_WIDTH {90};
    constexpr int XMARG() { return (SWIDTH - SQUARE_WIDTH*8)/2; }
    constexpr int YMARG() { return (SHEIGHT - SQUARE_WIDTH*8)/2; }

public:
    Interface();
    Interface(int _playerColor, std::string_view _initBoard);
    void renderInit();
    void renderBoard(int holding, uint64_t bb);
    void renderQuit();

    Move handlePlayerInput( uint64_t renderBB );

    Game game;

};