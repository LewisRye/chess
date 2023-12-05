#include "../Board.hpp"
#include "../Move.hpp"
#include "../Square.hpp"
#include "Bishop.hpp"

// --------------------------------------------------------------------------------------------------------------------
Bishop::Bishop(PieceColour colour, PieceType name) : Piece(colour, name) {}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Move> Bishop::ListPseudoLegalMoves(Board *board)
{
  mPseudoLegalMoves.clear();
  int row = mPosition->GetRow();
  int col = mPosition->GetCol();

  for (int j = -1; j < 2; j++) // up and left, down and right
  {
    if (j == 0) { continue; }
    for (int i = 1; i < 8; i++)
    {
      if (!board->InRange(row+(i*j), col+(i*j)))
      {
        continue;
      }
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(row+(i*j), col+(i*j)), *board);
      if (IsBlockedByPiece(mv) && mv.mMovingPiece->GetColour() != mv.mKilledPiece->GetColour()) // there is one to take
      {
        mPseudoLegalMoves.push_back(mv);
        break;
      }
      if (IsBlockedByPiece(mv)&& mv.mMovingPiece->GetColour() == mv.mKilledPiece->GetColour()) // blocked by same team
      {
        break;
      }
      mPseudoLegalMoves.push_back(mv);
    }
    for (int i = 1; i < 8; i++) // up and right, down and left
    {
      if (!board->InRange(row-(i*j), col+(i*j)))
      {
        continue;
      }
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(row-(i*j), col+(i*j)), *board);
      if (IsBlockedByPiece(mv) && mv.mMovingPiece->GetColour() != mv.mKilledPiece->GetColour()) // there is one to take
      {
        mPseudoLegalMoves.push_back(mv);
        break;
      }
      if (IsBlockedByPiece(mv)&& mv.mMovingPiece->GetColour() == mv.mKilledPiece->GetColour()) // blocked by same team
      {
        break;
      }
      mPseudoLegalMoves.push_back(mv);
    }
  }

  return mPseudoLegalMoves;
}

// --------------------------------------------------------------------------------------------------------------------
