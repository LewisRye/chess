# Chess Game
This is the final project for the C++ Workshops. The idea is to create a chess game (excluding the moves empassant, castling, and pawn promotion).

## The Rules of Chess

Can look up the rules online, so the rules are briefly outlined here. For the sake of time and complexity, we excluding the moves empassant, castling, and pawn promotion. 

There are six types of pieces
- **King** moves 1 step in any direction
- **Queen** moves in any direction any distance, unless blocked by another piece
- **Bishop** moves along the diagonals any distance, unless blocked by another piece
- **Rook** moves vertically and horizontally any distance, unless blocked by another piece
- **Knight** moves in an L shape, can "jump" over pieces
- **Pawn** moves forward 1 step (or 2 on the first move), unless blocked by another piece

A piece takes another piece by moving to the location occupied by that piece. If a piece is able to take another piece, then we say it places that piece under attack.
- A piece can only take a peice of the opposite colour
- The piece that is taken is permanently removed from the board

The aim of the game is to place the opponent's King under attack such that the opponent cannot escape in one move. 
This is known as *checkmate*. Simply placing the King under attack whether escape is possible or not is known as *check*.

There are additional rules that govern piece movement:
- Pawn is the only piece whose standard move is different from its taking move. Pawns take diagonally. 
- If there are no legal moves but your King is not in check, then that is a *stalemate* and the game is a draw
- If your King is currently in check, a move must take it out of check to be legal
- If your King is not in check, a move is not legal if it puts your King in check 

That is, it's not legal to make a move such that after the move is completed, your King is in check.

## Starter code
We've provided some starter code for dealing with the GUI and taking in user input, so when the game first runs 
- The board initializes with the pieces of each side in their correct locations
- These pieces are displayed on the UI
- White player starts

<img width="400" alt="Screenshot 2023-11-30 at 4 10 02 PM" src="https://github.com/Wenfei134/chess/assets/60166421/975f892b-0e97-416b-a4ef-57c1f9437b05">
<img width="400" alt="Screenshot 2023-11-30 at 4 12 57 PM" src="https://github.com/Wenfei134/chess/assets/60166421/4df5dae5-5646-4223-8053-8a4315e0c255">

Additionally we handle user input
- When a user clicks on a piece of their colour the legal moves for that piece are displayed 
    - Currently only the Pawn piece will return a list of moves, every other peice returns an empty list so it looks like nothing happens, but the input is still being handled correctly
- If a legal move is clicked, tell the game to move the piece. 
    - Currently the move function does nothing, so the input is handled correctly but it looks like nothing happens
- If a different piece is clicked, the legal moves of that piece is shown 
- If any other click happens and a piece was selected, then the piece is deselected
- When a move happens, the turn switches to the opposite player

## Not implemented
- Get legal moves of each piece* (except Pawn which has been implemented)
- Moving a piece when a move happens 
- Undo a move (important for other functions)
- Determining if King is in check for each colour 
- Determining if the game is in stalemate 
- Determining if the game is in checkmate

*Legal moves is actually a bit difficult to determine in chess and is done in two phases, with pseudolegal moves that are then filtered to get the actual legal moves. We will explain this in the workshop.

## System Pre-requisites

### Supported
- Windows 64bit with MingW
- MacOS

For Windows if you followed the set up from the first workshop, you *should* have MingW.

If your system is supported, cloning the repository should be sufficient and no additional setup is required. Otherwise, continue reading.

### Not supported
- Linux
- Windows not MingW (Cygwin, MSYS, etc)
- Windows 32bit with MingW

If your system is not supported then you need to install SDL2 on your own. 

1. Follow this set up guide: [https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

2. Update the makefile for your OS so that the `INCLUDE_PATHS` and `LIBRARY_PATHS` point to where you downloaded SDL2. (If you're on Linux good luck, we do not have a Linux Makefile).

## Running the Game

In the `chess` directory run the command 

Unix: 

```make```

Followed by 

```./Chess```

Windows: 

```mingw32-make.exe```

Followed by

```.\Chess.exe```

## Common Setup Errors

- On windows there is no make
  - You might need to install make: [https://answers.microsoft.com/en-us/windows/forum/all/powershell-terminal-in-vs-code-make-the-term-make/74d69621-c91e-4929-83c2-2252f0371397](https://answers.microsoft.com/en-us/windows/forum/all/powershell-terminal-in-vs-code-make-the-term-make/74d69621-c91e-4929-83c2-2252f0371397)
- On Mac I get the error `fatal error: 'SDL.h' file not found` 
  - Every `#include <SDL.h>` needs to be replaced with `#include <SDL2/SDL.h>`
  - Every `#include <SDL_image.h>` needs to be replaced with `#include <SDL2_image/SDL_image.h>`
  - These are in `ChessTexture.cpp`, `ChessGraphicsSystem.cpp`, and `ChessGraphicsSystem.hpp`
