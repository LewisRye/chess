#include "../Board.hpp"
#include "../Move.hpp"
#include "../Square.hpp"
#include "Rook.hpp"

// --------------------------------------------------------------------------------------------------------------------
Rook::Rook(PieceColour colour, PieceType name) : Piece(colour, name) {}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Move> Rook::ListPseudoLegalMoves(Board *board)
{
  mPseudoLegalMoves.clear();
  int row = mPosition->GetRow();
  int col = mPosition->GetCol();

  for (int i = -1; i < 2; i++) // positive --> negative
  {
    if (i == 0)
    {
      continue;
    }
    for (int j = 0; j < 8; j++) // normal move
    {

      int nextRow = row + i * (j + 1);
      if (!board->InRange(nextRow, col))
      {
        continue;
      }
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(nextRow, col), *board);
      if (IsBlockedByPiece(mv) && mColour != mv.mKilledPiece->GetColour()) // there is a piece to take
      {
        mPseudoLegalMoves.push_back(mv);
      }
      if (!IsBlockedByPiece(mv)) // not blocked by a piece
      {
        mPseudoLegalMoves.push_back(mv);
      }
      else
      {
        break;
      }
    }

    for (int j = 0; j < 8; j++) // normal move
    {

      int nextCol = col - i * (j + 1);
      if (!board->InRange(row, nextCol))
      {
        continue;
      }
      Move mv = Move(board->GetSquare(row, col), board->GetSquare(row, nextCol), *board);
      if (IsBlockedByPiece(mv) && mColour != mv.mKilledPiece->GetColour()) // there is a piece to take
      {
        mPseudoLegalMoves.push_back(mv);
      }
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
  return mPseudoLegalMoves;
}

// --------------------------------------------------------------------------------------------------------------------
