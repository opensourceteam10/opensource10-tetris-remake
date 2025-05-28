#include "board.hpp"
#include <algorithm>

/*
 * ====================================
 * Public methods start here
 * ====================================
 */

Board::Board ()
{
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            board_state[row][col] = BlockStatus::block_empty;
        }
    }
}

// Get the type of tetromino placed in that block
int Board::getTetromino (int r, int c)
{
    return int(board_state[r][c])-1;
}

// True if a block is unoccupied
bool Board::isBlockFree (int r, int c)
{
    return (board_state[r][c] == BlockStatus::block_empty) ? true : false;
}

// True if a certain move is possible
bool Board::isPositionLegal (Piece p)
{
    for (int row = p.r; row < p.r+config::matrix_blocks; row++)
    {
        for (int col = p.c; col < p.c+config::matrix_blocks; col++)
        {
            // If outside of playfield limits
            if (col < 0 || col > (config::playfield_width-1) || row > (config::playfield_height-1))
            {
                if (p.getBlockType(row-p.r, col-p.c) != 0)
                {
                    return false;
                }
            }

            // If a block collides with a filled block
            if (row >= 0)
            {
                if (p.getBlockType(row-p.r, col-p.c) != 0 && !isBlockFree(row, col))
                {
                    return false;
                }
            }
        }
    }

    // No collisions found
    return true;
}

// Stores a piece in the board
void Board::storePiece (Piece p)
{
    for (int row = p.r; row < p.r+config::matrix_blocks; row++)
    {
        for (int col = p.c; col < p.c+config::matrix_blocks; col++)
        {
            if (p.getBlockType(row-p.r, col-p.c) != 0)
            {
                board_state[row][col] = static_cast<BlockStatus>(1+p.piece_type);
            }
        }
    }
    pieces.push_back(p);
}

int Board::clearFullLines()
{
    int linesCleared = 0;
    
    // 아래부터 위로 검사하여 완성된 줄 찾기
    for (int row = config::playfield_height - 1; row >= 0; --row)
    {
        bool isLineFull = true;
        for (int col = 0; col < config::playfield_width; ++col)
        {
            if (isBlockFree(row, col))
            {
                isLineFull = false;
                break;
            }
        }

        if (isLineFull)
        {
            // 완성된 줄 삭제하고 위쪽 줄들을 아래로 이동
            deleteLine(row);
            linesCleared++;
            row++; // 같은 줄을 다시 검사 (위쪽 줄이 내려왔으므로)
        }
    }

    return linesCleared;
}

// True if the game has ended; Note: the row index starts from the top
bool Board::isGameOver ()
{
    bool game_over = false;
    for (int col = 0; col < config::playfield_width; col++)
    {
        if (board_state[0][col] != BlockStatus::block_empty || board_state[1][col] != BlockStatus::block_empty)
        {
            game_over = true;
            break;
        }
    }
    return game_over;
}

/*
 * ====================================
 * Private methods start here
 * ====================================
 */

// Clears a filled row and moves all other blocks properly
void Board::deleteLine (int r)
{
    // r번째 줄을 삭제하고 위쪽 줄들을 아래로 이동
    for (int row = r; row > 0; row--)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            board_state[row][col] = board_state[row-1][col];
        }
    }
    
    // 맨 위 줄을 빈 줄로 만들기
    for (int col = 0; col < config::playfield_width; col++)
    {
        board_state[0][col] = BlockStatus::block_empty;
    }
}