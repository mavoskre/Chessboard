#include "ChessEngine.h"
#include <iostream>
#include <vector>

int main() {
    ChessBoard game;
    std::vector<std::string> study = {"e2e4", "e7e5", "g1f3", "b8c6", "f1b5", "a7a6", "b5a4"};

    game.render("move_00.png"); // Starting position

    for (int i = 0; i < study.size(); i++) {
        if (game.move(study[i])) {
            game.render("move_" + std::to_string(i + 1) + ".png");
        }
    }

    std::cout << "Images generated. Creating GIF..." << std::endl;
    
    system("magick convert -delay 80 -loop 0 move_*.png chess_study.gif");

    std::cout << "Project Complete: chess_study.gif created." << std::endl;
    return 0;
}