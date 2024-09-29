#ifndef GRID_H
#define GRID_H

#include "raylib.h"
const int ROWS = 20;
const int COLS = 10;
const int TOTAL_COORDINATES = ROWS * COLS;

static const int screenWidth  = 600;
static const int grid_size = static_cast<int>(screenWidth / COLS);
static const int screenHeight = grid_size * ROWS;
extern Color piece_debris[ROWS][COLS];

extern inline void init_game_window();
extern constexpr int row_to_pix(int grid_pos);
extern constexpr int col_to_pix(int grid_pos);
extern inline void draw_square_in_grid(int row, int col, Color color);
extern inline void draw_grid();

#endif // GRID_H

#ifdef GRID_H_IMPLEM
#include "raylib.h"
#include <cstdio>
#include <stdexcept>

// GoodEnough (TM)
Color piece_debris[ROWS][COLS];

inline void init_game_window() {
    InitWindow(screenWidth, screenHeight, "Tetris++");
    SetTargetFPS(120);
}

constexpr int row_to_pix(int grid_pos) {
    if ( grid_pos > ROWS) {
        printf("Expected grid_pos < %d, instead got grid_pos = %d\n", ROWS, grid_pos);
        throw std::logic_error("grid_pos must be <= ROWS");
    }
    return grid_pos * grid_size;
}


constexpr int col_to_pix(int grid_pos) {
    if ( grid_pos > COLS) {
        printf("Expected grid_pos < %d, instead got grid_pos = %d\n", COLS, grid_pos);
        throw std::logic_error("grid_pos must be <= ROWS");
    }
    return grid_pos * grid_size;
}

inline void draw_square_in_grid(int row, int col, Color color){
    DrawRectangle(col_to_pix(col), row_to_pix(row), grid_size, grid_size, color);
}

inline void draw_grid() {
    for (int r = 0; r < ROWS; r++){
        for (int c = 0; c < COLS; c++){
            draw_square_in_grid(r, c, piece_debris[r][c]);
            DrawRectangleLines(col_to_pix(c), row_to_pix(r), grid_size, grid_size, LIGHTGRAY);
        }
    }
}
#endif
