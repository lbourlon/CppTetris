#ifndef PIECE_H
#define PIECE_H

#include "raylib.h"

typedef enum {
    I, L, J, O, S, T, Z, MaxPiece,
} piece_type;


typedef struct {
    int row;
    int col;
} grid_pos;

class piece
{
private:
    int rotation; // 0..=3
    int initial_row;
    int initial_col;

    bool is_sliding;

    void inline handle_controls(double *fall_multiplier);
    void move_cuboids(int r, int c);
    bool will_collide(grid_pos next_cuboids_pos[4]);
    void rotate(bool rotate_clockwise);

public:
    Color color;
    piece_type type;
    bool is_active;
    double time_last_move;
    grid_pos piece_cuboids[4];

    piece(piece_type type);

    void setup_origin_cuboid();
    void setup_extra_cuboids_from_origin(grid_pos origin);
    void start_lifetime();
    void stop_lifetime();
    char type_as_char();
    void update_position();
    void print();
    void draw();
    void swap_properties(piece* other_piece);
    void draw_in_info(int global_row_offset);
};

#endif
