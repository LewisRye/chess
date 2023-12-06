#include "../Board.hpp"
#include "../Move.hpp"
#include "../Square.hpp"
#include "King.hpp"

// --------------------------------------------------------------------------------------------------------------------
King::King(PieceColour colour, PieceType name) : Piece(colour, name) {}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Move> King::ListPseudoLegalMoves(Board *board)
{
  mPseudoLegalMoves.clear();
  int row = mPosition->GetRow();
  int col = mPosition->GetCol();

  int move[8][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}, {-1, 0}, {1, 0}, {0, 1}, {0, -1}};
  for (int j = 0; j < 8; ++j)
  {
    int nextRow = row + move[j][0];
    int nextCol = col + move[j][1];

    if (board->InRange(nextRow, nextCol))
    {
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(nextRow, nextCol), *board);

      if (!IsBlockedByPiece(mv) && mv.mMovingPiece->GetColour() != mv.mKilledPiece->GetColour())
      {
        mPseudoLegalMoves.push_back(mv);
      }
    }
  }
  return mPseudoLegalMoves;
}

// --------------------------------------------------------------------------------------------------------------------
