// #define DEBUG
#include <cstdlib>
#include <cstring>

#include "raylib.h"
#include "grid.inl.h"
#include "piece.h"

char piece::type_as_char(){
    switch (type) {
        case I: return 'I'; break;
        case L: return 'L'; break;
        case J: return 'J'; break;
        case O: return 'O'; break;
        case S: return 'S'; break;
        case T: return 'T'; break;
        case Z: return 'Z'; break;
        case MaxPiece: return 'X'; break;
    }
}

piece::piece(piece_type pt) {
    type = pt;
    rotation = 0;

    is_active = true;
    is_sliding = false;
    setup_cuboids();
}

void piece::stop_lifetime() {
    rotation = 0; // fixes issues with I piece when swap
    is_active = false;
}


void piece::start_lifetime() {
    is_active = true;
    time_last_move = GetTime();
}

//void piece::replace_with(piece* other_piece) { }

void inline piece::handle_controls(double* fall_multiplier) {
    // MOVE RIGHT
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_L)) {
        move_cuboids(0, 1);
    }

    // MOVE LEFT
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_H)) {
        move_cuboids(0, -1);
    }

#ifndef DEBUG
    // Soft Drop
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J)) {
        *fall_multiplier = 6.0;
    }
#else
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_K)) {
        move_cuboids(1, 0);
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_J)) {
        move_cuboids(-1, 0);
    }
#endif

    // Hard Drop // Good Enough (TM)
    if (IsKeyReleased(KEY_SPACE)) {
        is_active = false;
        for(int i = 0; i <= ROWS; i++) move_cuboids(-1, 0);
    }

    // rotation horaire
    if (IsKeyPressed(KEY_F) ||
        IsKeyPressed(KEY_X) ||
        IsKeyPressed(KEY_U)) {
        rotate(true);
    }

    // rotation anti-horaire
    if (IsKeyPressed(KEY_D) ||
        IsKeyPressed(KEY_Z) ||
        IsKeyPressed(KEY_W) ||
        IsKeyPressed(KEY_RIGHT_CONTROL)) {
        rotate(false);
    }
}

void piece::update_position() {
    double current_time = GetTime();
    if (is_active == false){
        return;
    }

    double fall_multiplier = 1.0;
    handle_controls(&fall_multiplier);

#ifndef DEBUG
    for (int i = 0; i < 4; i++) {
        int new_r = piece_cuboids[i].row - 1;
        int col = piece_cuboids[i].col;
        bool touching_the_floor =  new_r == 0;
        bool above_empty_block = ColorToInt(piece_debris[new_r][col]) != ColorToInt(BLANK);
        if (touching_the_floor || above_empty_block) {
            is_sliding = true;
            break;
        } else {
            is_sliding = false;
        }
    }

    if (current_time - time_last_move  > (0.5 / fall_multiplier)) {
        time_last_move = current_time;
        #ifndef DEBUG
        move_cuboids(-1, 0); // Default gravity movement
        #endif
        if (is_sliding) {
            is_active = false;
        }
    }
#endif
}

bool piece::will_collide(grid_pos *next_cuboids_pos) {
    int new_r, new_c;
    for (int i = 0; i < 4; i++) {
        new_r = next_cuboids_pos[i].row;
        new_c = next_cuboids_pos[i].col;

        if (new_r < 0) return true;
        if (new_c > COLS - 1 || new_c < 0) return true;

        if ( ColorToInt(piece_debris[new_r][new_c]) != ColorToInt(BLANK) ) {
            return true;
        }
    }
    return false;
}

void piece::move_cuboids(int r, int c) {
    grid_pos next_position[4] = {0};
    for (int i = 0; i < 4; i++) {
        next_position[i].row = piece_cuboids[i].row + r;
        next_position[i].col = piece_cuboids[i].col + c;
    }
    if ( !will_collide(next_position) ) {
        memcpy(piece_cuboids, next_position, sizeof(piece_cuboids));
    }
}

void piece::draw() {
    for (int i = 0; i < 4; i++) {
        draw_square_in_grid(piece_cuboids[i].row, piece_cuboids[i].col, color);
    }
}

void piece::draw_in_info(int global_row_offset) {
    // if ( !is_active) {
    //     throw std::logic_error("Can only be called on inactive pieces");
    // }
    int row_offset = 0;
    int col_offset = (int) (grid_size * 2);
    if ( type == O );
    else if ( type == I )
        row_offset = (int) grid_size * 1 / 2;
    else
        col_offset = (int) (grid_size * 3 / 2);

    for (int i = 0; i < 4; i++) {
        draw_square_in_info(piece_cuboids[i].row - initial_row + global_row_offset,
                            piece_cuboids[i].col - 3,
                            color, row_offset, col_offset);
    }
}

void piece::rotate(bool rotate_clockwise) {
    grid_pos *pc = piece_cuboids;
    // Good Enough (TM)
    if (!rotate_clockwise) {
        rotate(true);
        rotate(true);
        rotate(true);
        return;
    }
    grid_pos temp[4] = {0};
    memcpy(temp, pc, sizeof(piece_cuboids));

    switch (type) {
        case I:
            temp[1] = pc[2];
            switch (rotation){
                case 0:
                    temp[0].row = pc[0].row - 1; temp[0].col = pc[0].col + 2;
                    temp[2].row = pc[2].row + 1; temp[2].col = pc[2].col + 0;
                    temp[3].row = pc[3].row + 2; temp[3].col = pc[3].col - 1;
                    break;
                case 1:
                    temp[0].row = pc[0].row + 2; temp[0].col = pc[0].col + 1;
                    temp[2].row = pc[2].row + 0; temp[2].col = pc[2].col - 1;
                    temp[3].row = pc[3].row - 1; temp[3].col = pc[3].col - 2;
                    break;
                case 2:
                    temp[0].row = pc[0].row + 1; temp[0].col = pc[0].col - 2;
                    temp[2].row = pc[2].row - 1; temp[2].col = pc[2].col - 0;
                    temp[3].row = pc[3].row - 2; temp[3].col = pc[3].col + 1;
                    break;
                case 3:
                    temp[0].row = pc[0].row - 2; temp[0].col = pc[0].col - 1;
                    temp[2].row = pc[2].row - 0; temp[2].col = pc[2].col + 1;
                    temp[3].row = pc[3].row + 1; temp[3].col = pc[3].col + 2;
                    break;
            }
            break;
        case O:
        case MaxPiece:
            break;
        default:
            // Doing it the other way would've been more work, doing properly here
            // Change base, rotate and change back
            for (int i = 1; i <= 3; i++) {
                int tmp = pc[i].col;
                temp[i].col =  (pc[i].row - pc[0].row) + pc[0].col;
                temp[i].row = -(      tmp - pc[0].col) + pc[0].row;
            }
            break;
    }

    if ( !will_collide(temp) ) {
        memcpy(pc, temp, sizeof(piece_cuboids));
        rotation = (rotation + 1 ) % 4;
    } else {

    }
}


bool piece::swap_properties(piece* other_piece) {
    grid_pos backup[4] = {0};
    memcpy(backup, other_piece->piece_cuboids, sizeof(piece_cuboids));

    /* Take position from stashed piece */
    piece_cuboids[0].col = other_piece->piece_cuboids[0].col;
    piece_cuboids[0].row = other_piece->piece_cuboids[0].row;
    setup_extra_cuboids_from_origin(piece_cuboids[0]);

    if ( will_collide(piece_cuboids) ) {
        memcpy(piece_cuboids, backup, sizeof(piece_cuboids));
        return false;
    }

    other_piece->stop_lifetime();

    /* Set position of stashed piece to zero */
    /* This is done so that it's properly displayed in the sidebar */
    other_piece->setup_cuboids();
    time_last_move = other_piece->time_last_move;
    is_active = true;
    return true;
}

void piece::setup_cuboids() {
    initial_col = type != I ? 4 : 3;
#ifndef DEBUG
    initial_row = 20;
#else
    initial_row = 17;
#endif
    grid_pos origin = {.row = initial_row, .col = initial_col};
    setup_extra_cuboids_from_origin(origin);
}

void piece::setup_extra_cuboids_from_origin(grid_pos origin) {
    piece_cuboids[0] = origin;
    int origin_row = origin.row;
    int origin_col = origin.col;
    switch (type) {
        case I:
            color = BLUE;
            piece_cuboids[1] = {.row = origin_row, .col = origin_col + 1};
            piece_cuboids[2] = {.row = origin_row, .col = origin_col + 2};
            piece_cuboids[3] = {.row = origin_row, .col = origin_col + 3};
            break;
        case L:
            color = ORANGE;
            piece_cuboids[1] = {.row = origin_row,     .col = origin_col - 1};
            piece_cuboids[2] = {.row = origin_row,     .col = origin_col + 1};
            piece_cuboids[3] = {.row = origin_row + 1, .col = origin_col + 1};
            break;
        case J:
            color = BLUE;
            piece_cuboids[1] = {.row = origin_row,     .col = origin_col + 1};
            piece_cuboids[2] = {.row = origin_row,     .col = origin_col - 1};
            piece_cuboids[3] = {.row = origin_row + 1, .col = origin_col - 1};
            break;
        case O:
            color = YELLOW;
            piece_cuboids[1] = {.row = origin_row,     .col = origin_col + 1};
            piece_cuboids[2] = {.row = origin_row + 1, .col = origin_col};
            piece_cuboids[3] = {.row = origin_row + 1, .col = origin_col + 1};
            break;
        case S:
            color = GREEN;
            piece_cuboids[1] = {.row = origin_row,     .col = origin_col - 1};
            piece_cuboids[2] = {.row = origin_row + 1, .col = origin_col};
            piece_cuboids[3] = {.row = origin_row + 1, .col = origin_col + 1};
            break;
        case Z:
            color = RED;
            piece_cuboids[1] = {.row = origin_row,     .col = origin_col + 1};
            piece_cuboids[2] = {.row = origin_row + 1, .col = origin_col};
            piece_cuboids[3] = {.row = origin_row + 1, .col = origin_col - 1};
            break;
        case T:
            color = PURPLE;
            piece_cuboids[1] = {.row = origin_row + 1, .col = origin_col};
            piece_cuboids[2] = {.row = origin_row,     .col = origin_col - 1};
            piece_cuboids[3] = {.row = origin_row,     .col = origin_col + 1};
            break;
        default:
          break;
    }
}

