#include "ChessEngine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>

ChessBoard::ChessBoard() {
    //
    //
    std::string start = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR";
    for (int i = 0; i < 64; i++) grid[i / 8][i % 8] = start[i];
}

bool ChessBoard::move(std::string m) {
    if (m.length() < 4) return false;
    int startCol = m[0] - 'a';
    int startRow = 8 - (m[1] - '0');
    int endCol = m[2] - 'a';
    int endRow = 8 - (m[3] - '0');

    grid[endRow][endCol] = grid[startRow][startCol];
    grid[startRow][startCol] = '.';
    return true;
}

void ChessBoard::render(std::string filename) {
    std::vector<uint8_t> pixels(BOARD_SIZE * BOARD_SIZE * CHANNELS);

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int r = y / TILE_SIZE;
            int c = x / TILE_SIZE;
            int idx = (y * BOARD_SIZE + x) * CHANNELS;

            bool isLight = (r + c) % 2 == 0;
            pixels[idx] = isLight ? 240 : 181;     
            pixels[idx+1] = isLight ? 217 : 136;   
            pixels[idx+2] = isLight ? 181 : 99;    

            char piece = grid[r][c];
            if (piece != '.') {
                uint8_t pColor = (piece < 'a') ? 255 : 30;
                if (x % TILE_SIZE > 15 && x % TILE_SIZE < 49 && y % TILE_SIZE > 15 && y % TILE_SIZE < 49) {
                    pixels[idx] = pixels[idx+1] = pixels[idx+2] = pColor;
                }
            }
        }
    }
    stbi_write_png(filename.c_str(), BOARD_SIZE, BOARD_SIZE, CHANNELS, pixels.data(), BOARD_SIZE * CHANNELS);
}