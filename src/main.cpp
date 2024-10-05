#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

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

    while (!WindowShouldClose()) {
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

        current_piece->update_position();
        score += check_completed_lines();

        BeginDrawing();
        game_board_background();
        current_piece->draw();
        draw_grid();

        draw_info(next_piece->type_as_char(), score);
        next_piece->draw_in_info();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
