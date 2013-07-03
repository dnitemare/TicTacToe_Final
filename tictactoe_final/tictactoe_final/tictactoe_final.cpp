/*  Tic-Tac-Toe program version 3.0 Final
    by:     Ruben Morgan
    date:   06/11/2013
    
    notes:  this program was created so i could practice using more advanced
            features, and to practice problem solving and general coding.
            
            it now has a bot that plays the user. :D
    
            
    last modified:  07/1/2013
    */
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>

#include "BoardData.h"

using namespace std;


//-------Main function, Ho~~
int main(int argc, char *argv[])
{


    char replay;
    
    //create the tic-tac-toe board data class
    BoardData game;
    
    srand(unsigned(time(0)));
    
    game.setSides();
    game.setDifficulty();

    //Now let's make this loopable
    do {
        
        game.resetData();
        
        
    
        //Get the party started by printing the board and getting the current user's move
        //cout << "\n\nValid entry! continuing.\n\n";
        system("cls");
        
        game.printBoard();
        cout << "\n\n";
    
        while ( game.win == 0) {
        game.makeMove();
        game.countturns++;
        game.win = game.checkWin();
        }
    
    
    
        cout << "Winner is: ";
        if (game.win == 1) {
            cout << game.userside << " (user wins).";
            game.logWin();
            }
        else if (game.win == 2)
            cout << game.botside << " (PC wins).";
        else
            cout << "noone! It's a tie... ";
        cout << " Feel free to play again. :)\n\n";
        do {
            cout << "Replay? (y/n): ";
            cin >> setw(1) >> replay;
            }//Loop logic for this sub do-while that will make sure it's a valid choice
            while( replay != 'y' && replay != 'Y' && replay != 'n' && replay != 'N');
        system("cls");
        }
        //Loop logic for the above main program's do-while)
        while (replay == 'y' || replay == 'Y');
        
    //End of loop. Exit time
    cout << "\n\nEnd of tic-tac-toe. Exiting successfully!\n";
    
        
    
    system("PAUSE");
    return EXIT_SUCCESS;
}