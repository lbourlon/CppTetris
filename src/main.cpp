#include <cstdlib>
#include <ctime>
#include <time.h>
#include <cmath>
#include <cstdio>
#include <vector>

#include "piece.h"
#include "raylib.h"
#include "rlgl.h"
// #include "raymath.h"
#include "grid.inl.h"

static time_t g_start_time = 0;
static time_t g_current_time = 0;

int main() {
    init_game_window();

    srand(time(0));

    std::vector<piece> pieces;
    // pieces.push_back(piece(I));
    // pieces.push_back(piece(L));
    // pieces.push_back(piece(J));
    // pieces.push_back(piece(O));
    // pieces.push_back(piece(S));
    // pieces.push_back(piece(T));
    // pieces.push_back(piece(Z));

    piece current_piece = piece(I); // good
    // piece current_piece = piece(L); // good
    // piece current_piece = piece(J); // good
    // piece current_piece = piece(S); // good
    // piece current_piece = piece(Z); // good
    // piece current_piece = piece(T);    // good
    // piece current_piece = piece(O);    // good

    while (!WindowShouldClose()) {
        BeginDrawing();

        // DrawRectangle(grid_to_pix(1), grid_to_pix(2), grid_size, grid_size, RED);

        current_piece.update_position();
        current_piece.draw();

        // for (auto & element : pieces) {
        //     // element.update_position();
        //     element.draw();
        // }

        draw_grid();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
