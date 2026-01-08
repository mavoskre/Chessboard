#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <string>
#include <vector>
#include <cstdint>

const int TILE_SIZE = 64;
const int BOARD_SIZE = TILE_SIZE * 8;
const int CHANNELS = 3;

class ChessBoard {
public:
    char grid[8][8];
    ChessBoard();
    bool move(std::string m);
    void render(std::string filename);
};

#endif