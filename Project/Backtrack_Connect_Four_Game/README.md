# Backtrack Connect Four Game

This project implements a Connect Four game with a backtracking algorithm for the PC's logic. The game is a classic two-player connection game in which players take turns dropping colored discs into a grid. The objective is to connect four of one's own discs in a row, either horizontally, vertically, or diagonally.

## Backtracking Algorithm

The PC opponent uses a backtracking algorithm to determine the optimal move. Backtracking is a problem-solving technique that incrementally builds candidates for solutions and abandons those candidates ("backtracks") as soon as it is determined that they cannot be extended to a valid solution.

### How Backtracking Works in Connect Four

1. **Recursive Exploration**:
   - The algorithm recursively explores all possible moves for the PC. For each move, it simulates the game state after the move and evaluates the outcome.
  
2. **Pruning**:
   - Moves that lead to unfavorable outcomes or do not contribute to a winning strategy are discarded (pruned). The algorithm prioritizes moves that bring the PC closer to winning or prevent the opponent from winning.

3. **Optimal Move Selection**:
   - The algorithm selects the move that maximizes the PC's chances of winning while minimizing the opponent's chances. It continues exploring until it finds the best possible move.

## Game Features

- **Two-Player Mode**: Play against the computer or another human player.
- **Intelligent PC Opponent**: The PC opponent uses a backtracking algorithm to play optimally.
- **User Interface**: Simple console-based interface for easy play.

## Objective

The goal of Connect Four is to be the first player to connect four of your own discs in a row. The connection can be horizontal, vertical, or diagonal. The game ends when one player achieves this goal or when the board is full with no possible moves left, resulting in a draw.

## Code Structure

The code is organized into the following components:

- **Game**: Manages the game state, including board updates and win conditions.
- **Player**: Handles player input and determines the player's moves.
- **AI**: Implements the backtracking algorithm for the computer's moves.
- **Board**: Represents the game board and contains methods for checking game status and updating the board.

## Getting Started

To get started with the Connect Four game:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your-username/connect-four.git
