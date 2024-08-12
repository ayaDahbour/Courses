# Ai Project
# Aya Dahbour   1201738
# Alaa Shaheen  1200049
import time
import tkinter as tk
from tkinter import messagebox

# Function to check if a move is valid
def ValidMove(board, row, col):
    if 0 <= row < len(board) and 0 <= col < len(board[0]):
        # Check if the cell is empty
        if board[row][col] == ' ':
            # Check if the cell is adjacent to the wall or another brick
            if col == 0 or col == len(board[0]) - 1:  # Stacked directly on the left or right wall
                return True
            elif board[row][col - 1] != ' ' or board[row][
                col + 1] != ' ':  # Stacked to the left or right of another brick
                return True

    # If the move is not valid, return False
    return False


# Function to apply a move on the board
def ApplyMove(board, move, player_symbol):
    row, col = move
    board[row][col] = player_symbol


# Function to generate all possible moves
def GenerateMoves(board):
    moves = []
    for i in range(len(board)):
        for j in range(len(board[0])):
            if board[i][j] == ' ':
                moves.append((i, j))
    return moves


# Function to check if a player has won
def CheckWinner(board):
    # Check rows
    for row in board:
        for col in range(len(row) - 4):
            if row[col] != ' ' and row[col] == row[col + 1] == row[col + 2] == row[col + 3] == row[col + 4]:
                return row[col]

    # Check columns
    for col in range(len(board[0])):
        for row in range(len(board) - 4):
            if board[row][col] != ' ' and board[row][col] == board[row + 1][col] == board[row + 2][col] == \
                    board[row + 3][col] == board[row + 4][col]:
                return board[row][col]

    # Check diagonals (top-left to bottom-right)
    for row in range(len(board) - 4):
        for col in range(len(board[0]) - 4):
            if board[row][col] != ' ' and board[row][col] == board[row + 1][col + 1] == board[row + 2][col + 2] == \
                    board[row + 3][col + 3] == board[row + 4][col + 4]:
                return board[row][col]

    # Check diagonals (bottom-left to top-right)
    for row in range(len(board) - 1, 3, -1):
        for col in range(len(board[0]) - 4):
            if board[row][col] != ' ' and board[row][col] == board[row - 1][col + 1] == board[row - 2][col + 2] == \
                    board[row - 3][col + 3] == board[row - 4][col + 4]:
                return board[row][col]

    # Check for a tie
    for row in board:
        if ' ' in row:
            return None
    return 'Tie'


# Function to evaluate the board state
def EvaluateBoard(board):
    player1_wins = CountWinningPositions(board, '■')
    player2_wins = CountWinningPositions(board, '□')
    return player1_wins - player2_wins


# Function to count the number of winning positions for a player
def CountWinningPositions(board, player_symbol):
    count = 0

    # Check rows
    for row in board:
        for col in range(len(row) - 4):
            if row[col] == player_symbol and row[col] == row[col + 1] == row[col + 2] == row[col + 3] == row[col + 4]:
                count += 1

    # Check columns
    for col in range(len(board[0])):
        for row in range(len(board) - 4):
            if board[row][col] == player_symbol and board[row][col] == board[row + 1][col] == board[row + 2][col] == \
                    board[row + 3][col] == board[row + 4][col]:
                count += 1

    # Check diagonals (top-left to bottom-right)
    for row in range(len(board) - 4):
        for col in range(len(board[0]) - 4):
            if board[row][col] == player_symbol and board[row][col] == board[row + 1][col + 1] == board[row + 2][
                col + 2] == board[row + 3][col + 3] == board[row + 4][col + 4]:
                count += 1

    # Check diagonals (bottom-left to top-right)
    for row in range(len(board) - 1, 3, -1):
        for col in range(len(board[0]) - 4):
            if board[row][col] == player_symbol and board[row][col] == board[row - 1][col + 1] == board[row - 2][
                col + 2] == board[row - 3][col + 3] == board[row - 4][col + 4]:
                count += 1

    return count


# Function to make a move for the computer player
def MakeMove(board, computer_symbol):
    start_time = time.time()  # Get the current time
    best_move = None
    best_score = float('-inf')

    moves = GenerateMoves(board)
    for move in moves:
        ApplyMove(board, move, computer_symbol)
        score = minimax(board, 4, False)
        ApplyMove(board, move, ' ')
        if score > best_score:
            best_score = score
            best_move = move

        elapsed_time = time.time() - start_time
       # print("Elapsed time:", elapsed_time, "seconds")
        if elapsed_time >= 3.0:
            break  # Time limit exceeded, break the loop

    return best_move


def minimax(board, depth, maximizing_player, alpha=float('-inf'), beta=float('inf')):
    winner = CheckWinner(board)
    if depth == 0 or winner is not None or depth == 6:
        if winner == '■':
            return 100
        elif winner == '□':
            return -100
        else:
            return 0

    if maximizing_player:
        max_eval = float('-inf')
        moves = GenerateMoves(board)
        for move in moves:
            ApplyMove(board, move, '■')
            eval = minimax(board, depth - 1, False, alpha, beta)
            ApplyMove(board, move, ' ')
            max_eval = max(max_eval, eval)
            alpha = max(alpha, eval)
            if beta <= alpha:
                break
        return max_eval
    else:
        min_eval = float('inf')
        moves = GenerateMoves(board)
        for move in moves:
            ApplyMove(board, move, '□')
            eval = minimax(board, depth - 1, True, alpha, beta)
            ApplyMove(board, move, ' ')
            min_eval = min(min_eval, eval)
            beta = min(beta, eval)
            if beta <= alpha:
                break
        return min_eval


# Create the GUI
class MagneticCaveGameGUI:
    def __init__(self, window):
        self.mode = None

        while self.mode not in [1, 2, 3]:
            mode_input = input("Select mode:\n"
                               "1. Manual entry for ■'s moves & automatic moves for □.\n"
                               "2. Manual entry for □'s moves & automatic moves for ■.\n"
                               "3. Manual entry for both ■'s moves and □'s moves.\n"
                               "Enter the corresponding number: ")

            try:
                self.mode = int(mode_input)
            except ValueError:
                print("Invalid mode selection! Please enter a valid mode (1, 2, or 3).")

            if self.mode not in [1, 2, 3]:
                print("Invalid mode selection! Please enter a valid mode (1, 2, or 3).")

        self.window = window
        self.window.title("Magnetic Cave Game")
        self.window.configure(bg="pink")
        self.window.grid_rowconfigure(3, weight=2)
        self.window.grid_columnconfigure(0, weight=1)

        # Create the board frame with pink background
        self.board_frame = tk.Frame(self.window, bg="pink", width=300, height=300)
        self.board_frame.grid(row=0, column=0)
        self.board_buttons = []
        self.board = [[' ' for _ in range(8)] for _ in range(8)]
        # Create the message label with pink background and set font
        self.message_label = tk.Label(self.window, text="Welcome to Magnetic Cave Game!", font=("Arial", 24), bg="pink")
        self.message_label.grid(row=0, column=0)
        self.CreateBoard()
        self.is_first_player_turn = True  # Flag to track the first player's turn

    def MakeMove(self, row, col):
        if ValidMove(self.board, row, col):
            if self.mode == 1:  # Manual entry for '■' moves & automatic moves for '□'
                ApplyMove(self.board, (row, col), '■')
                self.board_buttons[row][col].config(text='■', state=tk.DISABLED)
            elif self.mode == 2:  # Manual entry for '□' moves & automatic moves for '■'
                ApplyMove(self.board, (row, col), '□')
                self.board_buttons[row][col].config(text='□', state=tk.DISABLED)
            elif self.mode == 3:  # Manual entry for both '■' and '□' moves
                if self.is_first_player_turn:
                    ApplyMove(self.board, (row, col), '■')
                    self.board_buttons[row][col].config(text='■', state=tk.DISABLED)
                    self.is_first_player_turn = False
                else:
                    ApplyMove(self.board, (row, col), '□')
                    self.board_buttons[row][col].config(text='□', state=tk.DISABLED)
                    self.is_first_player_turn = True

            winner = CheckWinner(self.board)
            if winner is None:
                if self.mode == 1:
                    move = MakeMove(self.board, '□')  # Automatic move for '□'
                    if move:
                        ApplyMove(self.board, move, '□')
                        self.board_buttons[move[0]][move[1]].config(text='□', state=tk.DISABLED)
                elif self.mode == 2:
                    move = MakeMove(self.board, '■')  # Automatic move for '■'
                    if move:
                        ApplyMove(self.board, move, '■')
                        self.board_buttons[move[0]][move[1]].config(text='■', state=tk.DISABLED)

                winner = CheckWinner(self.board)

            if winner is not None:
                self.show_winner_message(winner)
        else:
            messagebox.showerror("Invalid Move", "Please select a valid move.")

    def CreateBoard(self):
        self.board_frame = tk.Frame(self.window)
        self.board_frame.grid(row=2, column=0, padx=10, pady=5)

        for row in range(8):
            row_buttons = []
            for col in range(8):
                button = tk.Button(self.board_frame, text=' ', width=2,
                                   command=lambda r=row, c=col: self.MakeMove(r, c))
                button.grid(row=row, column=col)
                row_buttons.append(button)
            self.board_buttons.append(row_buttons)

    def show_winner_message(self, winner):
        if winner == '■':
            message = "Player ■ wins!"
        elif winner == '□':
            message = "Player □ wins!"
        else:
            message = "It's a tie!"
        messagebox.showinfo("Game Over", message)
        self.window.quit()


def main():
    # Create the main window
    root = tk.Tk()

    # Set the window size and position
    window_width = root.winfo_screenwidth()
    window_height = root.winfo_screenheight()
    root.geometry(f"{window_width}x{window_height}+0+0")

    # Create the GUI instance
    gui = MagneticCaveGameGUI(root)

    root.mainloop()


if __name__ == "__main__":
    main()
