#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

#include "piece.h"
#include "raylib.h"
#include "rlgl.h"
// #include "raymath.h"
#define GRID_H_IMPLEM
#include "grid.inl.h"

int main() {
    init_game_window();

    SetRandomSeed(0);

    // piece current_piece = piece(I); // good
    // piece current_piece = piece(L); // good
    // piece current_piece = piece(J); // good
    // piece current_piece = piece(S); // good
    // piece current_piece = piece(Z); // good
    // piece current_piece = piece(T);    // good
    // piece current_piece = piece(O);    // good

    piece_type rand_piece_type = (piece_type) GetRandomValue(0, MaxPiece - 1);
    piece *current_piece = new piece(rand_piece_type);

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
            next_piece = new piece(rand_piece_type);
        }

        current_piece->update_position();

        BeginDrawing();
        current_piece->draw();

        draw_grid();
        DrawRectangle(8, 8, 250, 94, LIGHTGRAY);
        DrawText(TextFormat("Score: %i", 4), 10, 10, 20, BLACK);
        DrawText(TextFormat("Current Piece : %c", piece_type_to_string(current_piece->type)), 10, 30, 20, BLACK);
        DrawText(TextFormat("Next Piece    : %c", piece_type_to_string(next_piece->type)), 10, 50, 20, BLACK);

        ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
