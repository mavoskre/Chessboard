#include "ChessEngine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <map>

std::map<char, int> pieceMap = {
    {'Q', 0}, {'K', 1}, {'R', 2}, {'N', 3}, {'B', 4}, {'P', 5}, 
    {'q', 0}, {'k', 1}, {'r', 2}, {'n', 3}, {'b', 4}, {'p', 5}  
};

ChessBoard::ChessBoard() {
    std::string start = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR";
    for (int i = 0; i < 64; i++) grid[i / 8][i % 8] = start[i];
}

bool ChessBoard::move(std::string m) {
    if (m.length() < 4) return false;
    int sc = m[0] - 'a', sr = 8 - (m[1] - '0');
    int ec = m[2] - 'a', er = 8 - (m[3] - '0');
    
    if (sr < 0 || sr > 7 || sc < 0 || sc > 7 || er < 0 || er > 7 || ec < 0 || ec > 7) return false;

    grid[er][ec] = grid[sr][sc];
    grid[sr][sc] = '.';
    return true;
}

void ChessBoard::render(std::string filename) {
    int sw, sh, sn;
    unsigned char* spriteData = stbi_load("ChessPiecesArray.png", &sw, &sh, &sn, 4);
    if (!spriteData) {
        std::cerr << "Error: Could not find ChessPiecesArray.png in the directory!\n";
        return;
    }

    int spriteW = sw / 6; 
    int spriteH = sh / 2; 

    std::vector<uint8_t> boardPixels(BOARD_SIZE * BOARD_SIZE * 4);

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            int r = y / TILE_SIZE, c = x / TILE_SIZE;
            int bIdx = (y * BOARD_SIZE + x) * 4;

            bool isLight = (r + c) % 2 == 0;
            boardPixels[bIdx]   = isLight ? 240 : 181; 
            boardPixels[bIdx+1] = isLight ? 217 : 136;
            boardPixels[bIdx+2] = isLight ? 181 : 99;
            boardPixels[bIdx+3] = 255;

            char p = grid[r][c];
            if (p != '.') {
                int spriteCol = pieceMap[p];
                int spriteRow = (p >= 'a') ? 0 : 1; 

                int lx = x % TILE_SIZE;
                int ly = y % TILE_SIZE;

                int sx = (spriteCol * spriteW) + (lx * spriteW / TILE_SIZE);
                int sy = (spriteRow * spriteH) + (ly * spriteH / TILE_SIZE);
                int sIdx = (sy * sw + sx) * 4;

                float alpha = spriteData[sIdx+3] / 255.0f;
                for (int i = 0; i < 3; i++) {
                    boardPixels[bIdx+i] = (uint8_t)(spriteData[sIdx+i] * alpha + boardPixels[bIdx+i] * (1.0f - alpha));
                }
            }
        }
    }

    stbi_write_png(filename.c_str(), BOARD_SIZE, BOARD_SIZE, 4, boardPixels.data(), BOARD_SIZE * 4);
    stbi_image_free(spriteData);
}