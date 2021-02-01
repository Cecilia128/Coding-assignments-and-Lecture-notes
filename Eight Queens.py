#for row 1, pick one column randomly.
#For row from 2 to 8, first check which column is available.Then, pick one column randomly from available position.
#Look the chessboard as a 8*8 array.
# For column 9, print'| '.For other column, if it is in the list of position, print'|Q'. otherwise,print'| '.
import random
chessboard = [[y for y in range(8)] for x in range(8)] #look the chessboard as a 8*8 array.
position = [[] for row in range(8)] #where the queen is
def figure():            #generate figure     
    for row in range(8):
        for col in range(9):
            if col == 8: #built the right side bar '|'
                print('|', end='')
            else:
                if chessboard[row][col] in position[row]: #if is the queen's position, print'Q'
                    print('|Q', end='')
                else:
                    print('| ', end='')
        print('')
def find_position(): #find the queens' position
    row = 0
    while row < 8:
        if row == 0: #at the beginning, pick one column randomly
            col = random.choice(range(8))
            position[row].append(col)
            row += 1
            continue
        else:
            alternative = []
            for col in range(8):
                available = True
                for row2 in range(row): #check if this position is vailable
                    if col-row2-1 >= 0: 
                        if chessboard[row-row2-1][col] in position[row-row2-1] or chessboard[row-row2-1][col-row2-1] in position[row-row2-1]:
                            available = False
                    if col+row2+1 < 8:
                        if chessboard[row-row2-1][col] in position[row-row2-1] or chessboard[row-row2-1][col+row2+1] in position[row-row2-1]:
                            available = False
                if available: #if it's available, add to alternative choice.
                    alternative.append(col)     
            if len(alternative) == 0: #if this row has no solution, come back to row-1 to find a new solution
                row -= 1
                position[row] = []
            else: #if the row has solution, pick one randomly and go to next row to find solution
                col = random.choice(alternative)
                position[row].append(col)
                row += 1
find_position()
figure()


            