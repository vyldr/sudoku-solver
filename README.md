# sudoku-solver

This program will solve any valid sudoku puzzle.  The program will take input from a text file specified on the command line.  The correct file format is 81 numbers, 0-9, separated by white space.  '0' represents an empty space in the puzzle.  

The program works by filling in each space that only has one possible number until all the remaining spaces have more than one option.  Then it will make a guess and recursively call solve().  If the guess is wrong, it will make a different guess.  If the guess was correct, a later instance of solve() will return the solved puzzle.
