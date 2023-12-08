#ifndef CHESSGRAPHICSSYSTEM_H
#define CHESSGRAPHICSSYSTEM_H

#include "../Pieces/Piece.hpp"
#include "../Move.hpp"

#include <string>
#include <SDL.h>

// Fwd Declare
class Board;
class ChessController;
class ChessTexture;

class ChessGraphicsSystem
{

    public:
        ChessGraphicsSystem(ChessController *controller);
        ~ChessGraphicsSystem();

        void Render(Board *board, std::vector<Move> movesToHighlight);
        void PlayAudio(std::string s);
        // Creates the game window, renders the game, and listens for input until exit
        void RunChessGame(Board *board);

    private:
        // Chess controller pointer to handle user input
        ChessController *mController = nullptr;

        // Textures for chess board and chess pieces sprite sheet
        ChessTexture *mChessPiecesSpriteSheetTexture = nullptr;
        ChessTexture *mChessBoardTexture = nullptr;

        //The window we'll be rendering to
        SDL_Window* mWindow = nullptr;

        //The window renderer
        SDL_Renderer* mRenderer = nullptr;

        // The bounding rectangles to use on the chess pieces sprite sheet
        SDL_Rect mChessPiecesTexturesBoundingRects[12];

        SDL_Point ConvertSDLCoordinatesToChessCoordinates(int sdlX, int sdlY);
        // Gets correct index of bounding rectangle to render given the color and type of the piece
        int GetChessFigureTextureBoundingRectIndex(PieceColour pieceColour, PieceType pieceType);
        // Checks if mouse click is inside the chess board or on the margins
        bool IsClickInsideBoard(int clickX, int clickY);
        // Loads all the required textures
        void LoadAllMedia();
        // Hues all legal move squares as blue given a selected piece
        void RenderGreenHighlightedMoves(std::vector<Move> movesToHighlight);
        // Helper function for RunChessGame to render the chess board given the current state 
        void RenderBoardState(Board *board);
};

#endif
