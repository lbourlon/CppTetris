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
    double time_last_move;

    void inline handle_controls(double *fall_multiplier);
    void setup_cuboids();
    void move_cuboids(int r, int c);
    bool will_collide(grid_pos next_cuboids_pos[4]);
    void rotate(bool rotate_clockwise);

public:
    Color color;
    piece_type type;
    bool is_active;
    grid_pos piece_cuboids[4];

    piece(piece_type type);

    void start_lifetime();
    char type_as_char();
    void update_position();
    void print();
    void draw();
    void draw_in_info();
};

#endif
