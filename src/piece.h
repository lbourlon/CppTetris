#ifndef PIECE_H
#define PIECE_H

#include "raylib.h"

typedef enum {
    I, L, J, O, S, T, Z, MaxPiece,
} piece_type;

char piece_type_to_string(piece_type type);

typedef struct {
    int row;
    int col;
} grid_pos;

class piece
{
private:
    int rotation; // 0..=3
    bool is_sliding;
    double time_last_move;

    void setup_cuboids();
    void move_cuboids(int r, int c);
    void rotate(bool rotate_clockwise);

public:
    Color color;
    piece_type type;
    bool is_active;
    grid_pos piece_cuboids[4];

    piece(piece_type type);
    void update_position();
    void print();
    void draw();
};

#endif
