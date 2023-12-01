#include "ChessGraphicsSystem.hpp"
#include "ChessTexture.hpp"
#include "../Board.hpp"
#include "../ChessController.hpp"
#include <SDL_image.h>

#include <stdexcept>

const int SCREEN_WIDTH = 504;
const int SCREEN_HEIGHT = 504;

const int OFFSET_FROM_EDGE_OF_BOARD = 28;
const int CHESS_TILE_SIZE = 56;

const char *CHESS_PIECES_FILEPATH = "Graphics/IMG/pieces.png";
const char *CHESS_BOARD_FILEPATH = "Graphics/IMG/board.png";
const char *CHESS_ICON_FILEPATH = "Graphics/IMG/icon.png";

// --------------------------------------------------------------------------------------------------------------------
ChessGraphicsSystem::ChessGraphicsSystem(ChessController *controller)
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
		throw std::runtime_error("SDL could not initialize! SDL Error: " + std::string(SDL_GetError()));
	}

    // Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }

    // Create window
    SDL_Surface* icon = IMG_Load(CHESS_ICON_FILEPATH);
    mWindow = SDL_CreateWindow(
        "HackSussex Chess",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    SDL_SetWindowIcon(mWindow, icon);

    if(!mWindow)
    {
        throw std::runtime_error("Window could not be created! SDL Error: " +  std::string(SDL_GetError()));
    }

    //Create renderer for window
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!mRenderer)
    {
        throw std::runtime_error("Renderer could not be created! SDL Error:" + std::string(SDL_GetError()));
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        throw std::runtime_error("SDL_image could not initialize! SDL_image Error:" + std::string(IMG_GetError()));
    }

    mChessBoardTexture = new ChessTexture(mRenderer);
    mChessPiecesSpriteSheetTexture = new ChessTexture(mRenderer);
    mController = controller;

    LoadAllMedia();
}

// --------------------------------------------------------------------------------------------------------------------
ChessGraphicsSystem::~ChessGraphicsSystem()
{
	// Destroy window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	mRenderer = nullptr;
    
    delete mChessBoardTexture;
    delete mChessPiecesSpriteSheetTexture;
    mChessBoardTexture = nullptr;
    mChessPiecesSpriteSheetTexture = nullptr;

    // Controller not deleted here, it is deleted at the destruction of the controller itself.

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

// --------------------------------------------------------------------------------------------------------------------
SDL_Point ChessGraphicsSystem::ConvertSDLCoordinatesToChessCoordinates(int sdlX, int sdlY)
{
    if (!IsClickInsideBoard(sdlX, sdlY))
    {
        return {-1, -1};
    }
    
    // X and Y are switched because of SDL coordinate system (X goes across rows, Y goes down columns)
    return {(sdlY - OFFSET_FROM_EDGE_OF_BOARD) / CHESS_TILE_SIZE,
            (sdlX - OFFSET_FROM_EDGE_OF_BOARD) / CHESS_TILE_SIZE};
}
// --------------------------------------------------------------------------------------------------------------------
int ChessGraphicsSystem::GetChessFigureTextureBoundingRectIndex(PieceColour pieceColour, PieceType pieceType)
{
    // In mChessPiecesTexturesBoundingRects, 0-5 are black rook, knight, bishop, queen, king, pawn respectively
    // 6 - 11 are their counterparts for white

    int dimPieceColour = (pieceColour == PieceColour::kBlack) ? 0 : 1;
    int dimPieceType = 0;
    switch (pieceType)
    {
        case PieceType::kRook:
            dimPieceType = 0;
            break;
        case PieceType::kKnight:
            dimPieceType = 1;
            break;
        case PieceType::kBishop:
            dimPieceType = 2;
            break;
        case PieceType::kQueen:
            dimPieceType = 3;
            break;
        case PieceType::kKing:
            dimPieceType = 4;
            break;
        case PieceType::kPawn:
            dimPieceType = 5;
            break;
        default:
            break;
    }
    return dimPieceColour*6 + dimPieceType;
}

// --------------------------------------------------------------------------------------------------------------------
bool ChessGraphicsSystem::IsClickInsideBoard(int clickX, int clickY)
{
    return (clickX > OFFSET_FROM_EDGE_OF_BOARD && clickX < SCREEN_WIDTH - OFFSET_FROM_EDGE_OF_BOARD && 
            clickY > OFFSET_FROM_EDGE_OF_BOARD && clickY < SCREEN_HEIGHT - OFFSET_FROM_EDGE_OF_BOARD);
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::LoadAllMedia()
{

	// Load pieces textures
	if (!mChessPiecesSpriteSheetTexture->LoadTextureFromFile(CHESS_PIECES_FILEPATH))
    {
		throw std::runtime_error("Failed to load chess pieces sprite sheet texture!");
	}

    for (int i = 0; i < 12; i++)
    {
        mChessPiecesTexturesBoundingRects[i].x = (i % 6) * CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].y = (i < 6) ? 0 : CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].w = CHESS_TILE_SIZE;
        mChessPiecesTexturesBoundingRects[i].h = CHESS_TILE_SIZE;
    }

	// Load board texture
	if (!mChessBoardTexture->LoadTextureFromFile(CHESS_BOARD_FILEPATH))
    {
		throw std::runtime_error("Failed to load board texture image!");
	}
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::Render(Board *board, std::vector<Move> movesToHighlight)
{
    // Clear screen
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    RenderBoardState(board);
    RenderBlueHighlightedMoves(movesToHighlight);

    // Update screen
    SDL_RenderPresent(mRenderer);
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::RenderBlueHighlightedMoves(std::vector<Move> movesToHighlight)
{
    int highlightSize = CHESS_TILE_SIZE / 4;

    SDL_SetRenderDrawColor(mRenderer, 0x0, 0xFF, 0x0, 0xFF);
    for (auto &move : movesToHighlight)
    {
        int moveX = OFFSET_FROM_EDGE_OF_BOARD + move.mEnd->GetCol() * CHESS_TILE_SIZE + CHESS_TILE_SIZE / 2 - highlightSize / 2;
        int moveY = OFFSET_FROM_EDGE_OF_BOARD + move.mEnd->GetRow() * CHESS_TILE_SIZE + CHESS_TILE_SIZE / 2 - highlightSize / 2;
        SDL_Rect rectToFill = {moveX, moveY, highlightSize, highlightSize};
        SDL_RenderFillRect(mRenderer, &rectToFill);
    }
    // Reset draw color
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

}
// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::RenderBoardState(Board *board)
{
    // Render board and pieces
    mChessBoardTexture->Render(0, 0);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square *currSquare = board->GetSquare(i, j);
            if (!currSquare->IsEmpty())
            {
                auto currPiece = currSquare->GetPiece();
                mChessPiecesSpriteSheetTexture->Render(
                    OFFSET_FROM_EDGE_OF_BOARD + i * CHESS_TILE_SIZE,
                    OFFSET_FROM_EDGE_OF_BOARD + j * CHESS_TILE_SIZE,
                    &mChessPiecesTexturesBoundingRects[
                        GetChessFigureTextureBoundingRectIndex(
                            currPiece->GetColour(),
                            currPiece->GetType()
                        )
                    ]
                );
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void ChessGraphicsSystem::RunChessGame(Board *board)
{
    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;
    Render(board, {});
    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e))
        {
            // User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            //Handle click events
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    int clickX, clickY;
                    SDL_GetMouseState(&clickX, &clickY);
                    SDL_Point chessSquare = ConvertSDLCoordinatesToChessCoordinates(clickX, clickY);
                    mController->HandleClick(chessSquare.x, chessSquare.y);
                }
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
