#ifndef BOARD_H
#define BOARD_H

#include "Move.hpp"
#include "Pieces/Piece.hpp"
#include "Square.hpp"

#include <memory>
#include <vector>

class Board
{
public:
  static const int ROWS = 8;
  static const int COLS = 8;
  static const int NUM_COLOURS = 2;
  static const int NUM_PIECE_TYPES = 6;

private:
  std::vector<std::vector<Square>> mBoard;
  std::vector<std::shared_ptr<Piece>> mPieces[NUM_COLOURS];
  std::vector<Move> mMovesPlayed;
  std::shared_ptr<Piece> mKings[NUM_COLOURS];
  Move GetLastMove();
  void AddPiece(PieceColour colour, PieceType piece, Square *sq);

public:
  Board();
  ~Board();
  void Empty();
  void DoMove(Move &mv);
  void UndoMove();
  std::vector<Move> ListLegalMoves(PieceColour colour);
  bool IsChecked(PieceColour colour);
  bool IsCheckmated(PieceColour colour);
  bool IsStalemated(PieceColour colour);

  // Returns a pointer to Square in board at row and col corresponding to coord; ex: "d4", "e4"
  Square *GetSquare(int row, int col);

  bool InRange(int row, int col);
  int GetRows();
  int GetCols();
};
#endif
