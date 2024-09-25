#ifndef PIECE_H
#define PIECE_H

#include "raylib.h"

typedef enum {
    I, L, J, O, S, T, Z
} piece_type;


typedef struct {
    int row;
    int col;
} grid_pos;

class piece
{
private:
    piece_type type;
    int rotation; // 0..=3

    bool is_active;
    bool is_sliding;

    Color color;
    grid_pos piece_cuboids[4];
    double time_last_move;

    void setup_cuboids();
    void move_cuboids(int r, int c);
    void rotate(bool rotate_clockwise);
public:
    piece(piece_type type);

    void update_position();
    void print();
    void draw();
};

#endif
