#include "../Board.hpp"
#include "../Move.hpp"
#include "../Square.hpp"
#include "Knight.hpp"

// --------------------------------------------------------------------------------------------------------------------
Knight::Knight(PieceColour colour, PieceType name) : Piece(colour, name) {}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Move> Knight::ListPseudoLegalMoves(Board *board)
{
  mPseudoLegalMoves.clear();
  int row = mPosition->GetRow();
  int col = mPosition->GetCol();


  int move[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                     {1, -2}, {1, 2}, {2, -1}, {2, 1}};

  for (int j = 0; j < 8; ++j)
  {
    int nextRow = row + move[j][0];
    int nextCol = col + move[j][1];

    if (board->InRange(nextRow, nextCol))
    {
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(nextRow, nextCol), *board);

      if (!IsBlockedByPiece(mv) || mColour != mv.mKilledPiece->GetColour())
      {
        mPseudoLegalMoves.push_back(mv);
      }
    }
  }

  return mPseudoLegalMoves;
}



// --------------------------------------------------------------------------------------------------------------------
