#include "piece.hpp"

#include "tetromino_matrices.hpp"

Piece::Piece(int p_piece_type, int p_rotation) {
    piece_type = p_piece_type;
    rotation = p_rotation;
    r = 0;
    c = 0;
}

Piece::Piece (const Piece &p)
{
    piece_type = p.piece_type;
    rotation = p.rotation;
    r = p.r;
    c = p.c;
}

int Piece::getBlockType (int r_offset, int c_offset)
{
    return tetromino[piece_type][rotation][r_offset][c_offset];
}

int Piece::getInitialOffsetR ()
{
    return tetromino_initial_position[piece_type][rotation][0];
}

int Piece::getInitialOffsetC ()
{
    return tetromino_initial_position[piece_type][rotation][1];
}
