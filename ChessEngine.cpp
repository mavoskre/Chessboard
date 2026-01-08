#include "ChessEngine.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <map>
#include <cmath>

std::map<char, int> pieceMap = {
    {'Q', 0}, {'K', 1}, {'R', 2}, {'N', 3}, {'B', 4}, {'P', 5}, 
    {'q', 0}, {'k', 1}, {'r', 2}, {'n', 3}, {'b', 4}, {'p', 5}  
};

ChessBoard::ChessBoard() {
    std::string start = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR";
    for (int i = 0; i < 64; i++) grid[i / 8][i % 8] = start[i];
}

bool ChessBoard::isPathClear(int sr, int sc, int er, int ec) {
    int dr = (er - sr == 0) ? 0 : (er - sr > 0 ? 1 : -1);
    int dc = (ec - sc == 0) ? 0 : (ec - sc > 0 ? 1 : -1);
    
    int currR = sr + dr;
    int currC = sc + dc;
    
    while (currR != er || currC != ec) {
        if (grid[currR][currC] != '.') return false;
        currR += dr;
        currC += dc;
    }
    return true;
}

bool ChessBoard::move(std::string m) {
    if (m.length() < 4) return false;
    int sc = m[0] - 'a', sr = 8 - (m[1] - '0');
    int ec = m[2] - 'a', er = 8 - (m[3] - '0');
    
    if (sr < 0 || sr > 7 || sc < 0 || sc > 7 || er < 0 || er > 7 || ec < 0 || ec > 7) return false;

    char piece = grid[sr][sc];
    char target = grid[er][ec];

    if (piece == '.') return false;

    if (target != '.') {
        bool isPieceWhite = (piece < 'a');
        bool isTargetWhite = (target < 'a');
        if (isPieceWhite == isTargetWhite) return false;
    }

    int rowDiff = std::abs(er - sr);
    int colDiff = std::abs(ec - sc);

switch (std::tolower(piece)) {
        case 'p': {
            int direction = (piece == 'P') ? -1 : 1;
            int startRow = (piece == 'P') ? 6 : 1;

            if (sc == ec && target == '.' && er - sr == direction) break;
        
            if (sc == ec && target == '.' && sr == startRow && er - sr == 2 * direction) {
                if (grid[sr + direction][sc] == '.') break;
            }
        
            if (colDiff == 1 && er - sr == direction && target != '.') break;

            return false;
        }
        case 'n':
            if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) return false;
            break;
        case 'r':
            if (sr != er && sc != ec) return false;
            if (!isPathClear(sr, sc, er, ec)) return false;
            break;
        case 'b':
            if (rowDiff != colDiff) return false;
            if (!isPathClear(sr, sc, er, ec)) return false;
            break;
        case 'q':
            if (!((sr == er || sc == ec) || (rowDiff == colDiff))) return false;
            if (!isPathClear(sr, sc, er, ec)) return false;
            break;
        case 'k':
            if (rowDiff > 1 || colDiff > 1) return false;
            break;
    }

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