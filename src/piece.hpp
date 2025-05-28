#ifndef PIECE_HPP
#define PIECE_HPP

// Struct to hold a Tetris piece
struct Piece
{
public:
    Piece() : piece_type(0), rotation(0), r(0), c(0) {}  // 기본 생성자
    Piece (int p_piece_type, int p_rotation);       // Initialize a piece with these values
    Piece (const Piece &p);                         // Copy constructor
    int getBlockType (int r_offset, int c_offset);  // Returns the block type for a certain piece, rotation, and position
    int getInitialOffsetR ();                       // Returns the initial position offset for the row
    int getInitialOffsetC ();                       // Returns the initial position offset for the column

    int piece_type;                                 // Type of tetromino
    int rotation;                                   // One of four possible rotations
    int r;                                          // Row index
    int c;                                          // Column index
};



#endif // PIECE_HPP
