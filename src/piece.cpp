#include <cstdlib>
#include <iostream>

#include "piece.h"
#include "raylib.h"
#include "grid.inl.h"


piece::piece(piece_type pt) {
    type = pt;
    rotation = 0;

    is_active = true;
    is_sliding = false;
    setup_cuboids();
    double time_last_move = GetTime();

    piece::print();
}

void piece::print()
{
    std::cout << "Im a type : " << type  << " that is  active : " << is_active << std::endl;
    std::cout << "starting time : " << time_last_move  << std::endl;
    //std::cout << " my position is row : " << row << " | col : " << col << std::endl;
};


void piece::update_position() {
    double current_time = GetTime();
    if (is_active == false){
        return;
    }

    if (IsKeyPressed(KEY_RIGHT)) move_cuboids(0, 1);
    if (IsKeyPressed(KEY_LEFT))  move_cuboids(0, -1);
    if (IsKeyDown(KEY_DOWN))  move_cuboids(1, 0);

    if (IsKeyPressed('F')) rotate(true);
    if (IsKeyPressed('D')) rotate(false);

    if (current_time - time_last_move  > 0.8){
        is_active = is_sliding ? false : true;
        time_last_move = current_time;
        // move_cuboids(1, 0);
    }

    for (int i = 0; i < 4; i++) {
        if (piece_cuboids[i].row == ROWS - 1) {
            is_sliding = true;
        }
    }
}


void piece::move_cuboids(int r, int c) {
    for (int i = 0; i < 4; i++) {
        if (piece_cuboids[i].row + r > ROWS - 1 || piece_cuboids[i].row + r < 0) {
            return;
        }
        if (piece_cuboids[i].col + c > COLS - 1 || piece_cuboids[i].col + c < 0) {
            return;
        }
    }
    for (int i = 0; i < 4; i++) {
        piece_cuboids[i].row += r;
        piece_cuboids[i].col += c;
    }
}

void piece::draw() {
    for (int i = 0; i < 4; i++) {
        draw_square_in_grid(piece_cuboids[i].row, piece_cuboids[i].col, color);
    }
}

void piece::rotate(bool rotate_clockwise) {
    grid_pos *pc = piece_cuboids;
    if (!rotate_clockwise) {
        rotate(true);
        rotate(true);
        rotate(true);
        return;
    }
    switch (type) {
        case I:
            pc[1].row = pc[2].row; pc[1].col = pc[2].col;
            switch (rotation){
                case 0:
                    pc[0].row -= 1; pc[0].col += 2;
                    pc[2].row += 1; pc[2].col += 0;
                    pc[3].row += 2; pc[3].col -= 1;
                    break;
                case 1:
                    pc[0].row += 2; pc[0].col += 1;
                    pc[2].row += 0; pc[2].col -= 1;
                    pc[3].row -= 1; pc[3].col -= 2;
                    break;
                case 2:
                    pc[0].row += 1; pc[0].col -= 2;
                    pc[2].row -= 1; pc[2].col -= 0;
                    pc[3].row -= 2; pc[3].col += 1;
                    break;
                case 3:
                    pc[0].row -= 2; pc[0].col -= 1;
                    pc[2].row -= 0; pc[2].col += 1;
                    pc[3].row += 1; pc[3].col += 2;
                    break;
            }
            rotation = (rotation + 1 ) % 4;
            break;
        case O:
            break;
        case L:
        case J:
        case S:
        case T:
        case Z:
            // change to local base
            // for (int i = 1; i <= 3; i++) {
            //     int tmp = pc[i].col;
            //     pc[i].col = -(pc[i].row - pc[0].row) + pc[0].row;
            //     pc[i].row =  (      tmp - pc[0].col) + pc[0].col;
            // }
            for (int i = 1; i <= 3; i++) {
                int tmp = pc[i].col;
                pc[i].col = -(pc[i].row - pc[0].row) + pc[0].col;
                pc[i].row =  (      tmp - pc[0].col) + pc[0].row;
            }
            break;

    }
}


void piece::setup_cuboids() {
    int r = 4;
    int c = 4;

    switch (type) {
        case I:
            c = 3;
            color = BLUE;
            piece_cuboids[0] = {.row = r, .col = c};
            piece_cuboids[1] = {.row = r, .col = c + 1};
            piece_cuboids[2] = {.row = r, .col = c + 2};
            piece_cuboids[3] = {.row = r, .col = c + 3};
            break;
        case L:
            color = ORANGE;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r,     .col = c - 1};
            piece_cuboids[2] = {.row = r,     .col = c + 1};
            piece_cuboids[3] = {.row = r - 1, .col = c + 1};
            break;
        case J:
            color = BLUE;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r,     .col = c + 1};
            piece_cuboids[2] = {.row = r,     .col = c - 1};
            piece_cuboids[3] = {.row = r - 1, .col = c - 1};
            break;

        case O:
            color = YELLOW;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r,     .col = c + 1};
            piece_cuboids[2] = {.row = r + 1, .col = c};
            piece_cuboids[3] = {.row = r + 1, .col = c + 1};
            break;

        case S:
            color = GREEN;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r,     .col = c - 1};
            piece_cuboids[2] = {.row = r - 1, .col = c};
            piece_cuboids[3] = {.row = r - 1, .col = c + 1};
            break;


        case Z:
            color = RED;
            c = 4;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r,     .col = c + 1};
            piece_cuboids[2] = {.row = r - 1, .col = c};
            piece_cuboids[3] = {.row = r - 1, .col = c - 1};
            break;

        case T:
            c = 4;
            color = PURPLE;
            piece_cuboids[0] = {.row = r,     .col = c};
            piece_cuboids[1] = {.row = r - 1, .col = c};
            piece_cuboids[2] = {.row = r,     .col = c - 1};
            piece_cuboids[3] = {.row = r,     .col = c + 1};
            break;


        default:
          break;
    }
}

