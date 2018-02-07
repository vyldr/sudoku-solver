#include <iostream>
#include <fstream>

/* This program will solve any valid sudoku puzzle.  The program will take
 * input from a text file specified on the command line.  The correct file
 * format is 81 numbers, 0-9, separated by white space.  '0' represents an
 * empty space in the puzzle.
 * 
 * The program works by filling in each space that only has one possible number
 * until all the remaining spaces have more than one option.  Then it will make
 * a guess and recursively call solve().  If the guess is wrong, it will make a
 * different guess.  If the guess was correct, a later instance of solve() will
 * return the solved puzzle.
 */

#include <iostream>
#include <fstream>

void readfile(int board[9][9], char filename[256], int boardDefault[9][9])
{
    std::ifstream input;
    input.open(filename);

    // Read the file
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            input >> board[i][j];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            input >> boardDefault[i][j];

    // Board for green numbers
    bool old = false;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (boardDefault[i][j])
                old = true;
    if (!old)
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            boardDefault[i][j] = board[i][j];

    input.close();
    return;
}

void draw(int board[9][9], int boardDefault[9][9])
{
    std::cout << "   A B C D E F G H I\n"; // Top coordinates
    for (int i = 0; i < 9; i++)
    {
        std::cout << (i + 1) << "  "; // Left coordinates
        for (int j = 0; j < 9; j++)
        {
            if ((j == 2) or (j == 5))
                if (!board[i][j])
                    std::cout << " " << "|"; // Divider
                else
                    if (boardDefault[i][j])
                        // "\E[22;32m" changes the text to green and "\E[0m" changes it back
                        std::cout << "\E[22;32m" << board[i][j] << "\E[0m" << "|";
                    else
                        std::cout << board[i][j] << "|";
            else 
                if (!board[i][j])
                {
                    std::cout << " ";
                    if (j != 8)
                        std::cout << " ";
                }
                else
                {
                    if (boardDefault[i][j])
                        std::cout << "\E[22;32m" << board[i][j] << "\E[0m";
                    else
                        std::cout << board[i][j];
                    if (j != 8)
                        std::cout << " ";
                }
        }
         std::cout << std::endl;
        if ((i == 2) or (i == 5))
           std::cout << "-  -----+-----+-----\n"; // Divider
    }
    std::cout << std::endl << std::flush;
}

void closeOptions(int board[9][9], bool option[9][9][10])
{
   // Close invalid options
   for (int i = 0; i < 9; i++)
      for (int j = 0; j < 9; j++)
      {
         // Row and column
         for (int n = 0; n < 9; n++)
         {
            option[i][j][board[i][n]] = false;
            option[i][j][board[n][j]] = false;
         }

         // Square
         for (int n = ((i) / 3 * 3); n < ((i) / 3 * 3 + 3); n++)
         {
            for (int m = ((j) / 3 * 3); m < ((j) / 3 * 3 + 3);
                  m++)
            {
               option[i][j][board[n][m]] = false;
            }
         }
      }
}

bool solve(int boardStart[9][9], bool optionStart[9][9][10], int rowGuess, 
   int colGuess, int guess, int depth, int original[9][9])
{
    int board[9][9];
    bool option[9][9][10]; 

    // Graph how many guesses were made
    /*std::cout << std::endl;
    for (int i = 0; i < depth; i++)
        std::cout << "+";
    /**/

    // Copy the arrays for the current instance of solve()
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            board[i][j] = boardStart[i][j];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 0; k < 10; k++)
                option[i][j][k] = optionStart[i][j][k];

    // Apply the guess
    if (guess != -1)
        board[rowGuess][colGuess] = guess;

    closeOptions(board, option);

    // Fill in easy spaces and change options
    int number;
    int changes; // To see if we are making any progress
    while(true)
    {
        changes = 0; // Count how many changes we make this time through
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
            {
                int choices = 0;
                for (int k = 1; k < 10; k++)
                    if (option[i][j][k])
                    {
                        choices += 1;
                        number = k;
                    }

                // Only one option left for this space
                if ((choices == 1) and (!board[i][j]))
                {
                    // Mark invalid options
                    for (int n = 0; n < 9; n++)
                    {
                        option[i][n][number] = false; // Row
                        option[n][j][number] = false; // Column
                    }

                    // 3*3 box
                    for (int n = ((i) / 3 * 3); n < ((i) / 3 * 3 + 3); n++)
                        for (int m = ((j) / 3 * 3); m < ((j) / 3 * 3 + 3); m++)
                            option[n][m][number] = false;
                    option[i][j][number] = true; // Change the correct one back to true
                    board[i][j] = number; // Make the change
                    changes += 1;
                }
        }
        if (!changes) // Nothing changed this time, all the easy ones are done
            break;
    }

    // Fix options for solved spaces
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j])
            {
                for (int n = 1; n < 10; n++)
                    option[i][j][n] = false;
                option[i][j][board[i][j]] = true;
            }

    // Check if board is valid
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
        {
            int choices = 0;
            for (int k = 1; k < 10; k++)
                if (option[i][j][k])
                    choices++;
            if (!choices)
                return false;
        }

    // Count empty spaces
    int spaces = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0)
                spaces++;
   
    // Guess
    if ((changes == 0) and (spaces))
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                for (int k = 1; k < 10; k++)
                    if ((option[i][j][k]) and (k != board[i][j]))
                    {
                        // Recursion!
                        bool correct = solve(board, option, i, j, k, (depth + 1), original);
                        if (correct) // The guess was correct
                            return true;
                        else // The guess was wrong.  Try again.
                            option[i][j][k] = false;
                    }
   
   
    // Check if board is filled
    bool done = true;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0) // A space is not filled
                done = false;

    // Finish
    if (done)
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                original[i][j] = board[i][j];
        return true;
    }
   
    return false; // This board has no solution
}

int main(int argc, char** argv)
{
    // Set up...
    int board[9][9] = {}; // An array that holds the board
    int boardDefault[9][9] = {}; // The starting value of the board, used for the green numbers
    bool option[9][9][10] = {}; // Keeps track of which numbers are available for each space

    // Start all the options at true
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k = 1; k < 10; k++)
              option[i][j][k] = true;


    // Read in the file
    readfile(board, argv[1], boardDefault); // If no board can be read, an empty board will be solved

    // Start the recursive solving function
    solve(board, option, 0, 0, -1, 1, board);

    // Finally display the solved board
    draw(board, boardDefault);
}