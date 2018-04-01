import random
import copy

def printBoard(b):
    for x in range(b.gridSize):
        print("{} ".format(x), end = "")    
    print("|")
    print("-" * (b.gridSize*2 -1), "|")
    
    for i in range(b.gridSize):
        for j in b.grid[i]:
            print("{} ".format(j), end = "")
        print("| {}".format(i))


def getAdjacent(size, i, j):
    li = []
    for x in range(i-1, i+2):
        for y in range(j-1, j+2):
            if 0 <= x < size and 0 <= y < size and (x != i or y != j):
                li.append((x,y))

    return li

class Board:

    def __init__(self, bombs = 15, gridSize = 10):
        '''
        grid will hold key-value pairs where the key is either '*', 
        representing a bomb, or a number, representing the number
        of adjacent bombs
        '''
        assert(0 <= bombs < gridSize**2)
        self.grid = []
        self.gridSize = gridSize
        self.bombs = 0

        for i in range(0,gridSize):
            self.grid.append([0] * gridSize)

        # generate bomb locations
        while(self.bombs < bombs):
            i = random.randrange(0, gridSize)
            j = random.randrange(0, gridSize)
            if self.grid[i][j] != "*":
                self.grid[i][j] = "*"
                self.bombs += 1
                li = getAdjacent(self.gridSize, i, j)
                for tu in li:
                    if self.grid[tu[0]][tu[1]] != "*":
                        self.grid[tu[0]][tu[1]] += 1


    def getCell(self, i, j):
        return self.grid[i][j]

    def setCell(self, i, j, value):
        self.grid[i][j] = value

