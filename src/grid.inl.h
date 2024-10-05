#ifndef GRID_H
#define GRID_H

#include "piece.h"
#include "raylib.h"
#include <cstring>
#include <iostream>
#include <ostream>

const int ROWS = 20;
const int COLS = 10;
const int TOTAL_COORDINATES = ROWS * COLS;
const int PADDING = 10; // px

const int grid_width  = 500; // px
const int grid_size = static_cast<int>(grid_width / COLS);
const int grid_height = grid_size * ROWS;

constexpr int screen_width  = static_cast<int>(grid_width * 1.5) + (PADDING * 2);
constexpr int screen_height = static_cast<int>(grid_height * 1.0) + (PADDING * 2);

constexpr int sidebar_width_start = grid_width   + (PADDING * 2);
constexpr int sidebar_width_end   = screen_width - (PADDING);
constexpr int sidebar_width       = sidebar_width_end - sidebar_width_start;

extern Color piece_debris[ROWS][COLS];

extern void init_game_window();
extern void draw_grid();
extern void game_board_background();

#include "raylib.h"
#include <cstdio>
#include <stdexcept>

constexpr int __col_to_pix(int grid_pos) {
    if ( grid_pos > COLS) {
        printf("Expected grid_pos < %d, instead got grid_pos = %d\n", COLS, grid_pos);
        throw std::logic_error("grid_pos must be <= ROWS");
    }
    return grid_pos * grid_size + PADDING;
}

constexpr int __row_to_pix(int grid_pos) {
    if ( grid_pos > ROWS) {
        printf("Expected grid_pos < %d, instead got grid_pos = %d\n", ROWS, grid_pos);
        throw std::logic_error("grid_pos must be <= ROWS");
    }
    return grid_pos * grid_size + PADDING;
}

inline void draw_square_in_grid(int row, int col, Color color){
    DrawRectangle(__col_to_pix(col), __row_to_pix(row), grid_size, grid_size, color);
}

inline void draw_grid() {
    for (int r = 0; r < ROWS; r++){
        for (int c = 0; c < COLS; c++){
            draw_square_in_grid(r, c, piece_debris[r][c]);
            DrawRectangleLines(__col_to_pix(c), __row_to_pix(r), grid_size, grid_size, LIGHTGRAY);
        }
    }
    ClearBackground(GRAY);
}

/*----------------- INFO SIDE PANNEL -----------------*/
constexpr int __grid_pos_to_pix(int grid_pos) {
    return grid_pos * grid_size;
}

inline void draw_square_in_info(int row, int col, Color color){
    DrawRectangle(sidebar_width_start + __grid_pos_to_pix(col) + (int) (grid_size / 2 ),
                  __grid_pos_to_pix(row + 6), grid_size, grid_size, color);
}

inline void draw_info(char next, int score) {
    int fontsize = 50;
    DrawText(TextFormat("Score: %i", score), sidebar_width_start, screen_height / 20, fontsize, BLACK);

    fontsize = 30;
    int next_piece_background_pos = __grid_pos_to_pix(5);

    DrawText("Next Piece", sidebar_width_start, next_piece_background_pos - fontsize, fontsize, BLACK);
    DrawRectangle(sidebar_width_start, next_piece_background_pos, sidebar_width, grid_size * 3, BLACK);
}

/*---------------- NO REDEFINE ZONE ----------------*/
#endif // GRID_H

// #define GRID_H_IMPLEM
#ifdef GRID_H_IMPLEM

// GoodEnough (TM)
Color piece_debris[ROWS][COLS] = {0}; // BLANK color being {0, 0, 0, 0}

// GoodEnough (TM)
int check_completed_lines(){
    int score = 0;
    for (int r = ROWS-1; r >= 0; r--) {
        bool completed_line = true;

        for (int c = 0; c < COLS; c++) {
            if (ColorToInt(piece_debris[r][c]) == ColorToInt(BLANK)) {
                completed_line = false;
                break;
            }
        }

        if (!completed_line ) { continue; }
        score += 1;

        // Clear line and shift down
        memset(&piece_debris[r], 0, sizeof(BLANK) * COLS);
        for (int l = r; l >= 1; l--) {
            memmove(piece_debris[l], piece_debris[l-1], sizeof(BLANK) * COLS);
        }
    }
    return score == 4 ? 10 : score;
}

void init_game_window() {
    InitWindow(screen_width, screen_height, "Tetris++");
    // SetWindowState(FLAG_WINDOW_RESIZABLE); // maybe later
    SetTargetFPS(144);
}

void game_board_background() {
    DrawRectangle(PADDING, PADDING, grid_width, grid_height, BLACK);
}

#endif
