#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

using namespace std;

struct connect4 {

    connect4(string player1Name, string player2Name) {
        PlayerNames[0] = player1Name;
        PlayerNames[1] = player2Name;

        for (int i = 0; i < 7; i++) {
            ColumnMoves[i] = 6;
            for (int j = 0; j < 6; j++)
                Data[j][i] = 32; // 32 is code of space character
        }

        Result = 0;
        GameOver = false;
        TurningPlayer = 0;
        PlayerChar[0] = 'o';
        PlayerChar[1] = 'x';
        PlayerWinningChar[0] = 'O';
        PlayerWinningChar[1] = 'X';
    }


    //---------------------
    void SetColor(int ForgC)
    {
        WORD wColor;

        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        //We use csbi for the wAttributes word.
        if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
        {
            //Mask out all but the background attribute, and add in the foreground color
            wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
            SetConsoleTextAttribute(hStdOut, wColor);
        }
        return;
    }

    //-----------
    void ShowGame() {

        //Function to display the game status on screen;

        system("CLS");
        SetColor(10);

        //======== GAME TITLE BAR =====

        for (int i = 1; i <= 110; i++)
            cout << char(177);

        cout << "\n\t\t\t\t\t THE CONNECT 4 GAME\n";

        for (int i = 1; i <= 110; i++)
            cout << char(176);

        cout << "\n\n\n";

        //========== Printing Column Numbers ====================
        SetColor(14);
        cout << "\t\t\t\t  1   2   3   4   5   6   7\n";

        //=========== DISPLAYING THE MATRIX ======================
        SetColor(10);
        // top border
        cout << "\t\t\t\t" << char(201)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(209)
            << char(205) << char(205) << char(205) << char(187) << endl;

        for (int row = 0; row < 6; ++row) {
            cout << "\t\t\t\t" << char(186);
            for (int col = 0; col < 7; ++col) {
                cout << " " << Data[row][col] << " " << char(179);
            }
            cout << "\b" << char(186) << endl; // Remove last separator and add closing border
            if (row < 5) {
                cout << "\t\t\t\t" << char(199)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(197)
                    << char(196) << char(196) << char(196) << char(182) << endl;
            }
        }

        // bottom border
        cout << "\t\t\t\t" << char(200)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(207)
            << char(205) << char(205) << char(205) << char(188) << endl;

        //========== Printing Column Numbers ====================
        SetColor(14);
        cout << "\t\t\t\t  1   2   3   4   5   6   7\n";
    }

    //=======================
    int CheckHorizontal(int row, int col) {
        int rCount = 0; //right counter
        int lCount = 0; //left counter

        for (int i = 1; i < 4 && (col + i) < 7; i++) {
            if (Data[row][col] == Data[row][col + i]) {
                rCount++;
            }
            else {
                break;
            }
        }

        for (int i = 1; i < 4 && (col - i) >= 0; i++) {
            if (Data[row][col] == Data[row][col - i]) {
                lCount++;
            }
            else {
                break;
            }
        }

        if ((rCount + lCount) < 3) //Didn't Win in this move
            return 0;

        else //Won in this move -- Fill matrix with winning characters and return success
        {
            for (int i = 0; i <= rCount; i++)
                Data[row][col + i] = PlayerWinningChar[TurningPlayer];

            for (int i = 0; i <= lCount; i++)
                Data[row][col - i] = PlayerWinningChar[TurningPlayer];

            return 1;
        }
    }

    //=======================
    int CheckVertical(int row, int col) {
        int dCount = 0; //only down count used because current move always on top

        for (int i = 1; i < 4 && (row + i) < 6; i++) {
            if (Data[row][col] == Data[row + i][col]) {
                dCount++;
            }
            else {
                break;
            }
        }

        if (dCount < 3) //Didn't Win in this move
            return 0;

        else //Won in this move -- Fill matrix with winning characters and return success
        {
            for (int i = 0; i <= dCount; i++)
                Data[row + i][col] = PlayerWinningChar[TurningPlayer];

            return 1;
        }
    }

    //=======================
    int CheckRightDiagonal(int row, int col) {
        int urCount = 0; //upper-right counter
        int llCount = 0; //lower-left counter

        for (int i = 1; i < 4 && (row - i) >= 0 && (col + i) < 7; i++) {
            if (Data[row][col] == Data[row - i][col + i]) {
                urCount++;
            }
            else {
                break;
            }
        }

        for (int i = 1; i < 4 && (row + i) < 6 && (col - i) >= 0; i++) {
            if (Data[row][col] == Data[row + i][col - i]) {
                llCount++;
            }
            else {
                break;
            }
        }

        if ((llCount + urCount) < 3) //Didn't Win in this move
            return 0;

        else //Won in this move -- Fill matrix with winning characters and return success
        {
            for (int i = 0; i <= llCount; i++)
                Data[row + i][col - i] = PlayerWinningChar[TurningPlayer];

            for (int i = 0; i <= urCount; i++)
                Data[row - i][col + i] = PlayerWinningChar[TurningPlayer];

            return 1;
        }
    }

    //=======================
    int CheckLeftDiagonal(int row, int col) {
        int ulCount = 0; //upper-left counter
        int lrCount = 0; //lower-right counter

        for (int i = 1; i < 4 && (row - i) >= 0 && (col - i) >= 0; i++) {
            if (Data[row][col] == Data[row - i][col - i]) {
                ulCount++;
            }
            else {
                break;
            }
        }

        for (int i = 1; i < 4 && (row + i) < 6 && (col + i) < 7; i++) {
            if (Data[row][col] == Data[row + i][col + i]) {
                lrCount++;
            }
            else {
                break;
            }
        }

        if ((lrCount + ulCount) < 3) //Didn't Win in this move
            return 0;

        else //Won in this move -- Fill matrix with winning characters and return success
        {
            for (int i = 0; i <= lrCount; i++)
                Data[row + i][col + i] = PlayerWinningChar[TurningPlayer];

            for (int i = 0; i <= ulCount; i++)
                Data[row - i][col - i] = PlayerWinningChar[TurningPlayer];

            return 1;
        }
    }

    //=======================
    int MakeMove(int Column) {
        Column--; //changing from human index to array index

        int rowIndex = ColumnMoves[Column] - 1;
        int colIndex = Column;

        if (ColumnMoves[colIndex] == 0) //Selected column is full
            return 1;

        else {
            Data[rowIndex][colIndex] = PlayerChar[TurningPlayer]; //place the played letter in matrix

            isGameOver(rowIndex, colIndex); //checks for a possible result based on current move

            if (Result == 1 || Result == -1) // Current player wins
                return 2;
            else //next player's turn
            {
                TurningPlayer++;
                TurningPlayer %= 2;

                ColumnMoves[Column] = ColumnMoves[Column] - 1;

                return 0;
            }
        }
    }

    //===============
    void UserInput() {
        ShowGame();

        while (!GameOver) {
            SetColor(11);
            cout << "\n\n\tTurn for : ";

            SetColor(15);
            cout << PlayerNames[TurningPlayer];

            SetColor(11);
            cout << "\n\tEnter the number of the column where you want to place '" << PlayerChar[TurningPlayer] << "' : ";

            int selectedCol, moveResult;
            SetColor(14);
            cin >> selectedCol;

            if (selectedCol < 1 || selectedCol > 7) {
                SetColor(4);
                cout << "\n\tColumn number must be between 1 and 7. Press any key to try again..";
                ShowGame();
            }
            else {
                moveResult = MakeMove(selectedCol);

                if (moveResult == 0) //successful move is made
                    ShowGame();
                else {
                    if (moveResult == 1) //selected column full
                    {
                        SetColor(4);
                        cout << "\n\tThe column you selected is already full. Press any key to try again..";
                        ShowGame();
                    }
                    else //reaches here only if game is over
                    {
                        if (Result == 0) {
                            ShowGame();
                            SetColor(14);
                            cout << "\n\t\t\t\tGame is drawn. Thanks for playing.." << endl;
                            SetColor(15);
                        }
                        else {
                            ShowGame();
                            SetColor(14);
                            cout << "\n\t\t\t\tGAME OVER.. The Winner is " << PlayerNames[TurningPlayer] << " !!!" << endl;
                            SetColor(15);
                        }
                    }
                }
            }
        }
    }

private:
    void isGameOver(int row, int col) {
        //check if any 4 are in a row for current move
        if (CheckHorizontal(row, col) || CheckVertical(row, col) || CheckRightDiagonal(row, col) || CheckLeftDiagonal(row, col)) {
            GameOver = true;

            if (TurningPlayer == 0) //first player won
                Result = 1;
            else //first player lost
                Result = -1;
        }

        //check if matrix is full and game is drawn
        int fullColumns = 0;
        for (int i = 0; i < 7; i++) {
            if (ColumnMoves[i] == 0)
                fullColumns++;
        }
        if (fullColumns == 7) {
            Result = 0;
            GameOver = true;
        }
    }

    string PlayerNames[2]; // To Hold Name of Players Who are playing
    int TurningPlayer; // To save the players ID who is making a move; 0: First Player, 1: Second Player
    int ColumnMoves[7]; // An array to keep record of first empty cell in a column
    char PlayerChar[2]; // Characters to be shown for move
    char PlayerWinningChar[2]; // Character to be shown if player wins.
    char Data[6][7]; // Array to hold game data.
    bool GameOver; // If true it means the game is over.
    int Result; // Variable to hold result of game {0: Drawn, -1: First Player Lost, 1: First Player Won}
};
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD axis;
    axis.X = x;
    axis.Y = y;
    SetConsoleCursorPosition(handle, axis);
}
void WelcomeMessage()
{
    char ch;
    int x = 28;
    int y = 10;
    gotoxy(x, y);
    system("Color 0E");
    cout << "       ______                                |    |       ";
    gotoxy(x + 6, y + 1);
    cout << "|        ___  ___  ___   ___   ____  __|__  |___|";
    gotoxy(x + 6, y + 2);
    cout << "|       | o | |  ||   | |___| |        |        | "; 
    gotoxy(x + 6, y + 3);
    cout << "|______ |___| |  ||   | |____ |____    |        |  ";
    gotoxy(x + 6, y + 4);
    cout << "      ";
    gotoxy(x, y + 6);
    cout << "                 Press '1' to play game" << endl;
    cout << "                                             Press '2' to see instructions" << endl;
    cout << "                                             Press '0' to exit game" << endl;
    ch = _getch();
    if (ch == '1')
    {
        string player1Name, player2Name;
        // Clear the screen again
        system("CLS");

        cout << "Enter the name of Player 1: ";
        getline(cin, player1Name);

        cout << "Enter the name of Player 2: ";
        getline(cin, player2Name);

        // Clear the screen again
        system("CLS");


        // Create the connect4 object with player names
        connect4 game(player1Name, player2Name);

        // Start the game
        game.UserInput();
    }
    if (ch == '0')
    {
        exit(0);
    }

    if (ch == '2')
    {
        string player1Name, player2Name;
        system("cls");
        system("Color 06");
        gotoxy(45, 8);
        cout << "     I N S T R U C T I O N S :" << endl;
        cout << endl;
        cout << endl;

        cout << "           Players take turns to place their pieces ('o' or 'x') on the grid by entering the column number " << endl;
        cout << "              First player to connect four of their pieces horizontally, vertically, or diagonally" << endl;
        cout << "                                                    Play and enjoy." << endl;
        cout << endl;
        cout << endl;
        cout << "                                           Press '1' to play game" << endl;
        cout << "                                           Press '0' or any key to exit game" << endl;
        ch = _getch();

        if (ch == '1')
        {
            // Clear the screen again
            system("CLS");

            cout << "Enter the name of Player 1: ";
            getline(cin, player1Name);

            cout << "Enter the name of Player 2: ";
            getline(cin, player2Name);

            // Clear the screen again
            system("CLS");


            // Create the connect4 object with player names
            connect4 game(player1Name, player2Name);

            // Start the game
            game.UserInput();
        }
        if (ch == '0')
        {
            exit(0);
        }
    }
    if (ch == '0')
    {
        exit(0);
    }
}


int main() {

    // Clear the screen
    system("CLS");
    WelcomeMessage();
    return 0;
}
