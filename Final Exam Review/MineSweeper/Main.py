import GameBoard
def main():
    key = GameBoard.Board()
    board = GameBoard.Board(bombs = 0)

    # GameBoard.printBoard(board)

    while(not gameWon(key, board)):
        GameBoard.printBoard(board)

        # get guess
        g = getGuess(key.gridSize)
        i= g[0]
        j=g[1]

        f = input("enter f to place a flag: ")
        if(f in 'fF'):
            flag(i, j, board)
        else:
            guess(i,j,board,key)
    
    print("NICE WORK")
    print("---------")
    GameBoard.printBoard(key)

def flag(i, j, board):
    val = board.getCell(i,j)
    if(val not in  [0, "F"]):
        print("Cannot flag a revealed cell")
        return
    symbol = 'F'
    if(val == 'F'):
        symbol = 0
    board.setCell(i, j, symbol)

def guess(i, j, board, key):
        if board.getCell(i,j) == 'F':
            print("Cell is Flagged - Unflag to reveal")
        elif key.getCell(i,j) == "*":
            print("GAME OVER")
            print("---------")
            GameBoard.printBoard(key)
            exit()
        elif key.getCell(i,j) == 0:
            board.setCell(i, j, ".")
            li = GameBoard.getAdjacent(key.gridSize, i, j)
            for tu in li:
                if board.getCell(tu[0], tu[1]) == 0:
                    guess(tu[0], tu[1], board, key)
        else:
            # update board
            board.setCell(i, j, key.getCell(i, j)) 

def getGuess(size):
    i = -1
    j = -1
    while(i < 0 or size <= i or j < 0 or size <= j):
        print("Enter the coordinates of the cell to reveal")
        i = int(input("Row: "))
        j = int(input("Col: "))

    return (i, j)


def gameWon(key, board):
    for i in range(key.gridSize):
        for j in range(key.gridSize):
            if board.getCell(i, j) in [0,'F'] and key.getCell(i,j) != '*':
                return False; 

    return True

if __name__ == "__main__":
    main()