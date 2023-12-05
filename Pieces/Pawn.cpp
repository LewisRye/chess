#include "../Board.hpp"
#include "../Move.hpp"
#include "../Square.hpp"
#include "Pawn.hpp"

// --------------------------------------------------------------------------------------------------------------------
Pawn::Pawn(PieceColour colour, PieceType name) : Piece(colour, name) {}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Move> Pawn::ListPseudoLegalMoves(Board *board)
{
  mPseudoLegalMoves.clear();
  int row = mPosition->GetRow();
  int col = mPosition->GetCol();
  int i;
  if (mColour == PieceColour::kWhite)
  {
    i = -1;
  }
  else
  {
    i = 1;
  }

  for (int j = 0; j < 2; j++) // normal move
  {
    if (j == 0 || !mHasMoved)
    {
      int nextRow = row + i * (j + 1);
      if (!board->InRange(nextRow, col))
      {
        continue;
      }
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(nextRow, col), *board);
      if (!IsBlockedByPiece(mv)) // not blocked by a piece
      {
        mPseudoLegalMoves.push_back(mv);
      }
      else
      {
        break;
      }
    }
  }

  for (int j = -1; j < 2; j++) // pawn takes diagonally
  {
    if (j == 0)
    {
      continue;
    }
    int nextRow = row + i;
    int nextCol = col + j;
    if (!board->InRange(nextRow, nextCol))
    {
      continue;
    }
    Move mv = Move(board->GetSquare(row, col), board->GetSquare(nextRow, nextCol), *board);
    if (IsBlockedByPiece(mv) && mv.mMovingPiece->GetColour() != mv.mKilledPiece->GetColour()) // there is a piece to take
    {
      mPseudoLegalMoves.push_back(mv);
    }
  }

  return mPseudoLegalMoves;
}

// --------------------------------------------------------------------------------------------------------------------
