#include "ChessGame.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include "Pieces/Piece.hpp"

#include <iostream>
#include <stdexcept>

// --------------------------------------------------------------------------------------------------------------------
ChessGame::ChessGame()
{
  mBoard = new Board();
}

// --------------------------------------------------------------------------------------------------------------------
ChessGame::~ChessGame()
{
  delete mBoard;
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGame::TakeTurn(Move mv)
{
  if (mState != ACTIVE)
  {
    throw std::logic_error("Game is already done");
  }
  mBoard->DoMove(mv);
  // white if black, black if white
  mPlayerInTurn = mPlayerInTurn == PieceColour::kWhite ? PieceColour::kBlack : PieceColour::kWhite;
  if (mBoard->IsCheckmated(mPlayerInTurn))
  {
    if (mPlayerInTurn == PieceColour::kWhite)
    {
      mState = BLACK_WIN;
      std::cout << "BLACK WIN";
    }
    else
    {
      mState = WHITE_WIN;
      std::cout << "WHITE WIN";
    }
  }
  else if (mBoard->IsStalemated(mPlayerInTurn))
  {
    mState = STALEMATE;
  }
}

// --------------------------------------------------------------------------------------------------------------------
std::vector<std::vector<std::vector<Move>>> ChessGame::GetLegalMoves()
{
  std::vector<std::vector<std::vector<Move>>> ret;
  for (int i = 0; i < 8; i++)
  {
    std::vector<std::vector<Move>> row;
    for (int j = 0; j < 8; j++)
    {
      row.emplace_back(std::vector<Move>{});
    }
    ret.push_back(row);
  }

  std::vector<Move> legalMoves = mBoard->ListLegalMoves(mPlayerInTurn);
  for (auto move : legalMoves)
  {

    int r = move.mStart->GetRow();
    int c = move.mStart->GetCol();

    ret[r][c].push_back(move);
  }

  return ret;
}

// --------------------------------------------------------------------------------------------------------------------
int ChessGame::GetState()
{
  return mState;
}

// --------------------------------------------------------------------------------------------------------------------
Board *ChessGame::GetBoard() {
  return mBoard;
}

// --------------------------------------------------------------------------------------------------------------------
