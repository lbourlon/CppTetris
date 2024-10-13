#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <ostream>

#define GRID_H_IMPLEM
#include "grid.inl.h"

#include "raylib.h"
#include "rlgl.h"
#include "piece.h"
// #include "raymath.h"


static int score = 4;
int main() {
    init_game_window();
    // SetRandomSeed(0);

    piece_type rand_piece_type = (piece_type) GetRandomValue(0, MaxPiece - 1);
    piece *current_piece = new piece(rand_piece_type);
    current_piece->start_lifetime();

    rand_piece_type = (piece_type) GetRandomValue(0, MaxPiece - 1);
    piece *next_piece = new piece(rand_piece_type);

    // rand_piece_type = (piece_type) GetRandomValue(0, MaxPiece - 1);
    piece *stashed_piece = NULL;

    while (!WindowShouldClose()) {
        /* Handle new piece and cleanup of old */
        if (!current_piece->is_active) {
            rand_piece_type = (piece_type) GetRandomValue(0, MaxPiece - 1);
            for (int i = 0; i < 4; i++) {
                piece_debris[current_piece->piece_cuboids[i].row][current_piece->piece_cuboids[i].col] = current_piece->color;
            }

            delete current_piece;
            current_piece = next_piece;
            current_piece->start_lifetime();
            next_piece = new piece(rand_piece_type);
        }

        /* handle_stash_swap */
        if ( IsKeyPressed(KEY_S) ) {
            if (stashed_piece == nullptr) {
                stashed_piece = current_piece;
                current_piece = next_piece;
                next_piece = new piece((piece_type) GetRandomValue(0, MaxPiece - 1));
            } else {
                piece* curr_temp_ptr = current_piece;
                current_piece = stashed_piece;
                stashed_piece = curr_temp_ptr;
            }
            current_piece->swap_properties(stashed_piece);
        }

        current_piece->update_position();
        score += check_completed_lines();

        BeginDrawing();
        game_board_background();
        current_piece->draw();
        draw_grid();
        draw_info(next_piece, stashed_piece, score);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
