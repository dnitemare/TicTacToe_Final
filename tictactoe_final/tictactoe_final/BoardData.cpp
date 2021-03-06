#include "BoardData.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const unsigned short int LWINCOMBO[8][3][2] = { { /*Rows*/ {0,0},{0,1},{0,2} }, { {1,0},{1,1}, {1,2} }, { {2,0},{2,1},{2,2} },
                                    /*Columns*/ { {0,0},{1,0},{2,0} }, { {0,1},{1,1},{2,1} }, { {0,2},{1,2},{2,2} },
                                    /*Diagonals*/ { {0,0},{1,1},{2,2} }, { {0,2},{1,1},{2,0} } }; 
const unsigned short int MWINCOMBO[8][3] = { {1,2,3}, {4,5,6} , {7,8,9}, {1,4,7}, {2,5,8}, {3,6,9}, {1,5,9}, {3,5,7} };


//*******************************************************//
//                      Bot AI                          //
//*****************************************************//

//Function to look ahead and list the logical moves to make
bool BoardData::lookAhead() {
	bool badmove = 0;

    //do this logic when the user goes first and it's the bot's first turn.
    if (countturns == 1) {
        //user moves corner
        if (board[1][1] == ' ' && (board[0][0] == userside || board[0][2] == userside || 
            board[2][0] == userside || board[2][2] == userside)) {
            locations[0] = 1;
            locations[1] = 1;
            return 1;
            }
        //user does not move corner
        else {
            if (userpiece[0] == 2) {
                bool getrand = rand() % 2;
                if (getrand == 0)
                    convertMove(7);
                else
                    convertMove(9);
                return 1;
                }
            else if (userpiece[0] == 4) {
                bool getrand = rand() % 2;
                if (getrand == 0)
                    convertMove(3);
                else
                    convertMove(9);
                return 1;
                }
            else if (userpiece[0] == 6){
                bool getrand = rand() % 2;
                if (getrand == 0)
                    convertMove(1);
                else
                    convertMove(7);
                return 1;
                }
            else if (userpiece[0] == 8){
                bool getrand = rand() % 2;
                if (getrand == 0)
                    convertMove(1);
                else
                    convertMove(3);
                return 1;
                }
            else {
                 return cornerMove();
                }
        }
    }

    //do this logic any other time to check for a logical move
    //########################################################
    finalmove.clear();
    for (short int i = 0; i < botpiece.size(); i++) { //check each bot move

        //prioritize a bot's row setup for forcing the opponent's move.
        for (short int j = 0; j < ismatch.size(); j++) { //check each open space for 3 in row setup

            int umlocation = RELATIVEWIN[botpiece[i]][ismatch[j]]; //user move location

            //check if valid move
            if (umlocation != -1)
                convertMove(umlocation);

            if (umlocation != -1 && board[locations[0]][locations[1]] == ' ' ) {
                badmove = 0;
				short int tempBmove = ismatch[j];
				userpiece.push_back(umlocation);
                short int userwins = 0;
                
                //check if this move is bad
				for (short int k = 0; k < userpiece.size(); k++) { 
                    for (short int l = 0; l < userpiece.size(); l++) {
                        if (k < l) {
                            int movenum = RELATIVEWIN[userpiece[k] ][ userpiece[l] ];
                            convertMove(movenum);
                            if (movenum != -1 && board[locations[0]][locations[1]] == ' ' )
                                userwins++;
                            }
                        }
                            if (userwins >= 2) {
							badmove = 1;
							break;
							}
                        
                    } //end of bad move check
                userpiece.pop_back();
				if (badmove == 0)
					finalmove.push_back(tempBmove);
                }//end of valid move check   
            }//end of each open space
    }//end of bot move

    //if there is a logical move to make do so
    if (finalmove.size() != 0) {
        short int getrand = rand() % finalmove.size();
        convertMove(finalmove[getrand]);
        return 1;
        }
    else {
        return 0;
        }


}//end of function

//*******************************************************************//
//              Make a Move Function Here.                          //
//*****************************************************************//

//---------------Function to make the move for the user    makeMove(board, userside, botside, turn);
//Warning: this is a monster of a function <.< it does most of the action
void BoardData::makeMove () {

    //Get the user's move and save it in umove
        unsigned short int umove = 0;
    
        //print the board numbers
    cout << "\t\t\t[ Tic-Tac-Toe ]\n\n";
    for (int i = 0; i < 9; i++) {
        if ( (i % 3) == 0)
            cout << "\n\n\t\t\t     ";
            
        cout << i + 1 << "  ";
    }
    //Begin the loop to get the move and modify the board appropriately (loops until valid choice is made)
    do {
    cout << "\n\n";
    if (turn == 0) {
        cout << "* " << userside << " * - ";
        cout << "choose a move. starting top left at 1, to bottom right at 9.  (1-9): ";
        cin >> setw(1) >> umove;
        savemove[countturns] = umove;
        
        convertMove(umove);

    }
    
    else { //bot's move
        cout << "* " << botside << " * - ";
        countMatches();

        if (mode == 1) {
            bool movedyet = 0;
            if (movedyet == 0)
                movedyet = winMaker();
            if (movedyet == 0)
                randomMove();
        }
        
        else if (mode == 2) {
            bool movedyet = 0;
            if (movedyet == 0)
                movedyet = winMaker();
            if (movedyet == 0)
                movedyet = cornerMove();
            if (movedyet == 0)
                randomMove();
            }


        else if (mode == 3) { //GURU MODE
            bool movedyet = 0;
            
                if (movedyet == 0)
                    movedyet = winMaker();
                //if (movedyet == 0)
                    //movedyet = matchMaker();
                if (movedyet == 0)
                    movedyet = lookAhead();
                if (movedyet == 0)
                    movedyet = cornerMove();
                if (movedyet == 0)
                    randomMove();
            
            }
            
        //for all bots
        umove = convertMove(locations[0], locations[1]);
        savemove[countturns] = umove;
        }
        
    
    
    //Print an error if they have done something wrong
    if ( (turn == 0 && (umove <= 0 || umove >= 10)) || (board[ locations[0] ][ locations[1] ] != ' ')) {
        cout << "Invalid choice. Please choose a number between 1 and 9.\n";
        }
    
    }//Evaluate if it is a valid move (loop till it is)
    while ((umove <= 0 || umove >= 10) || (board[ locations[0] ][ locations[1] ] != ' ') );
     
    //make the move (modify the board accordingly) now that the entry is valid
    if (turn == 0)
        board[ locations[0] ][ locations[1] ] = userside;
    else
        board[ locations[0] ][ locations[1] ] = botside;
    
    
    //Manage who's move it is.
    if (turn == 0)
        turn = 1;
    else
        turn = 0;
        
    //Clear the screen and print the modified board
    system("cls");
    printBoard();
    cout << "\n\n\n";
}


//-------------Function to set the sides. game.setSides();
void BoardData::setSides() {
        //Greet and get side
        cout << "Welcome to Tic-Tac-Toe! select your side (x/o): ";
        cin >> setw(1) >> userside;
        cout << "you chose: " << userside << "\n";

        if (userside == 'X')
            userside = 'x';
        else if (userside == 'O')
            userside = 'o';

        //Loop if invalid till valid
        while ( userside != 'x' && userside != 'o') {
            cout << "\nInvalid Entry! Please choose the letter x or the letter o: ";
            cin >> setw(1) >> userside;
            cout << "you chose: " << userside << "\n";
            }
            
    //Set the 2nd player's side
        if (userside == 'x' || userside == 'X')
            botside = 'o';
        else
            botside = 'x';


}

//---------------Function to reset the data values on new game. game.resetData();
void BoardData::resetData() {
            //Reset the counters and such
        //turn = rand() % 2; //Randomize who goes first
        turn = 0; //Test 1 person always moves first.  0 = user moves first. 1 = bot moves first
        firstplayer = turn;
        countturns = 0;
        win = 0;


        //set the relative wins START
        for (int i = 0; i < 10; i++) { //set all values to -1 first for spotting purposes.
            for (int j =0; j < 10; j++) {
                RELATIVEWIN[i][j] = -1;
                }
            }
        RELATIVEWIN[1][2] = 3;
        RELATIVEWIN[1][3] = 2;
        RELATIVEWIN[1][4] = 7;
        RELATIVEWIN[1][7] = 4;
        RELATIVEWIN[1][5] = 9;
        RELATIVEWIN[1][9] = 5;

        RELATIVEWIN[2][1] = 3;
        RELATIVEWIN[2][3] = 1;
        RELATIVEWIN[2][5] = 8;
        RELATIVEWIN[2][8] = 5;

        RELATIVEWIN[3][1] = 2;
        RELATIVEWIN[3][2] = 1;
        RELATIVEWIN[3][5] = 7;
        RELATIVEWIN[3][7] = 5;
        RELATIVEWIN[3][6] = 9;
        RELATIVEWIN[3][9] = 6;

        RELATIVEWIN[4][1] = 7;
        RELATIVEWIN[4][7] = 1;
        RELATIVEWIN[4][5] = 6;
        RELATIVEWIN[4][6] = 5;

        RELATIVEWIN[5][1] = 9;
        RELATIVEWIN[5][9] = 1;
        RELATIVEWIN[5][2] = 8;
        RELATIVEWIN[5][8] = 2;
        RELATIVEWIN[5][3] = 7;
        RELATIVEWIN[5][7] = 3;
        RELATIVEWIN[5][4] = 6;
        RELATIVEWIN[5][6] = 4;

        RELATIVEWIN[6][3] = 9;
        RELATIVEWIN[6][9] = 3;
        RELATIVEWIN[6][4] = 5;
        RELATIVEWIN[6][5] = 4;

        RELATIVEWIN[7][1] = 4;
        RELATIVEWIN[7][4] = 1;
        RELATIVEWIN[7][3] = 5;
        RELATIVEWIN[7][5] = 3;
        RELATIVEWIN[7][8] = 9;
        RELATIVEWIN[7][9] = 8;

        RELATIVEWIN[8][2] = 5;
        RELATIVEWIN[8][5] = 2;
        RELATIVEWIN[8][7] = 9;
        RELATIVEWIN[8][9] = 7;

        RELATIVEWIN[9][1] = 5;
        RELATIVEWIN[9][5] = 1;
        RELATIVEWIN[9][3] = 6;
        RELATIVEWIN[9][6] = 3;
        RELATIVEWIN[9][7] = 8;
        RELATIVEWIN[9][8] = 7;
//Relative win END
        
        ismatch.clear(); 
        userpiece.clear();
        botpiece.clear();
        
        //reset the board
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = ' ';
            }
        }

        //reset the user moves.
        for (int i = 0; i < 9; i++) {
            savemove[i] = 0;
        }
}
//---------------Function to set the Difficulty level   game.setDifficulty();
void BoardData::setDifficulty() {
    //Choose a difficulty level
        do {
            cout << "\nPlease choose your difficulty level.\n"
             << "\t1. Easy\n\t2.Skilled\n\t3.Guru\n\nChoice: ";
            cin >> mode;
            if (mode != 1 && mode != 2 && mode != 3)
                cout << "Invalid choice!...\n";
            //else if (mode == 2)
            //    cout << "this mode is still under construction. choose another one.\n";
            }while (mode != 1 && mode != 2 && mode != 3);
            
        //mode = 3;
}

//---------------Function to print the board   game.printBoard();
void BoardData::printBoard () {

        cout << "\t\t\t[ Tic-Tac-Toe ]\n\n";
    
        /*  old print method. very simplistic looking, decided to make it better
        for (int i = 0; i < SIZE; i++) {
            cout << "\n\t\t\t";
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j];
            }
        }*/
        cout << "\n\t\t\t  " << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << " ";  
        cout << "\n\t\t\t  " << "--- --- ---";
        cout << "\n\t\t\t  " << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << " "; 
        cout << "\n\t\t\t  " << "--- --- ---";
        cout << "\n\t\t\t  " << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << " "; 
        
        cout << "\n\n";
}
//-----------------Function to check if the game is over     checkWin(board, countturns, userside);
unsigned short int BoardData::checkWin() {
    unsigned short int winner = 0;
    
    //Check for a match with a loop. Set win as 1 if the user wins, 2 if bot wins, and 3 if it is a tie.
    for (int i = 0; i < SIZE; i++) {
        //check if it is a row matck like so: ---
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (userside == board[i][1])
                winner = 1;
            else
                winner = 2;
            return winner;
            }
        //check if it is a column match like so: |
        else if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (userside == board[1][i])
                winner = 1;
            else
                winner = 2;
            return winner;
        }
    }
        
    //Check if it is diagonal like so:  `.      ...ok never put that symbol in comments in devc++.... 
    //                                          F***.... i changed code over that... retest -.-
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[2][2] == board[1][1]) {
        if (userside == board[1][1])
            winner = 1;
        else
            winner = 2;
            
        return winner;
        }      
    //check if it is diagonal like so: /
    else if (winner == 0 && (board[1][1] != ' ' && board[1][1] == board[0][2] && board[0][2] == board[2][0]) ) {
        if (userside == board[1][1])
                winner = 1;
        else
                winner = 2;
                
        return winner;
            }
    //Check if it is a tie (full board, but no winners)
    else if (countturns >= 9) {
        winner = 3;
        return winner;
        }
    
    //return the winner!
    return winner;
    
    
}

//---------------Function to log the user's wins--------//
void BoardData::logWin() {
    ofstream outFile;
    outFile.open("winnerLog.txt", ios::app);

    outFile << "Mode: " << mode << " FirstPlayer: " << firstplayer << " Moves: ";
    for (int i = 0; i < 9; i++) {
        if (savemove[i] == 0)
            continue;
        else
            outFile << savemove[i] << " ";
        }
    outFile << endl;

    

}

//-------Function to set the location to a move number and vice-versa
int BoardData::convertMove(int x, int y) {
    if (x == 0) {
        if (y == 0)
           return 1;
        else if (y == 1)
            return 2;
        else if (y == 2)
            return 3;
        else
            return 0;
    }
    else if (x == 1) {
        if (y == 0)
           return 4;
        else if (y == 1)
            return 5;
        else if (y == 2)
            return 6;
        else
            return 0;
    }
    else if (x == 2) {
        if (y == 0)
           return 7;
        else if (y == 1)
            return 8;
        else if (y == 2)
            return 9;
        else
            return 0;
    }

}

void BoardData::convertMove(int x) {
            //place the moves in the locations array so we can check if it's valid later
    if (x >= 1 && x <= 3) {
        locations[0] = 0;
        locations[1] = x - 1;
        }
    else if (x >= 4 && x <= 6) {
        locations[0] = 1;
        locations[1] = x - 4;
        }
    else if (x >= 7 && x <= 9) {
        locations[0] = 2;
        locations[1] = x - 7;
        }
    else {
        //invalid choice.
        }
}


//--------------Function to check whether there is a 3 in a row win to be had....
bool BoardData::winMaker() {

//Check for three in a row first
    
    //******** loop for bot win check start
    for (int i = 0; i < SIZE; i++ ) {
    
    
    
        //check rows for bot win
            if (board[i][0] == botside && board[i][0] == board[i][1] && board[i][2] == ' ') {
                locations[0] = i;
                locations[1] = 2;
                //leave this function
                return 1;
                }
          else if (board[i][1] == botside && board[i][1] == board[i][2] && board[i][0] == ' ') {
                locations[0] = i;
                locations[1] = 0;
                //leave this function
                return 1;
                }
          else if (board[i][0] == botside && board[i][0] == board[i][2] && board[i][1] == ' ') {
                locations[0] = i;
                locations[1] = 1;
                //leave this function
                return 1;
                }
    //check columns for bot win
         else if (board[0][i] == botside && board[0][i] == board[1][i] && board[2][i] == ' ') {
                locations[0] = 2;
                locations[1] = i;
                //leave this function
                return 1;
                }
          else if (board[1][i] == botside && board[1][i] == board[2][i] && board[0][i] == ' ') {
                locations[0] = 0;
                locations[1] = i;
                //leave this function
                return 1;
                }
          else if (board[0][i] == botside && board[0][i] == board[2][i] && board[1][i] == ' ') {
                locations[0] = 1;
                locations[1] = i;
                //leave this function
                return 1;
                }
     //Check diagonally for bot win
        else if (board[0][0] == botside && board[0][0] == board[1][1] && board[2][2] == ' ') {
            locations[0] = 2;
            locations[1] = 2;
            //leave this function
            return 1;
            }
        else if (board[0][0] == botside && board[0][0] == board[2][2] && board[1][1] == ' ') {
            locations[0] = 1;
            locations[1] = 1;
            //leave this function
            return 1;
            }
        else if (board[1][1] == botside && board[1][1] == board[2][2] && board[0][0] == ' ') {
            locations[0] = 0;
            locations[1] = 0;
            //leave this function
            return 1;
            }
    //check the other diagonal for bot win
        else if (board[0][2] == botside && board[0][2] == board[1][1] && board[2][0] == ' ') {
            locations[0] = 2;
            locations[1] = 0;
            //leave this function
            return 1;
            }
        else if (board[0][2] == botside && board[0][2] == board[2][0] && board[1][1] == ' ') {
            locations[0] = 1;
            locations[1] = 1;
            //leave this function
            return 1;
            }
        else if (board[1][1] == botside && board[1][1] == board[2][0] && board[0][2] == ' ') {
            locations[0] = 0;
            locations[1] = 2;
            //leave this function
            return 1;
            }
        }//End loop for bot win check ***********
        
    //*************loop for user win check
    for (int i = 0; i < SIZE; i++ ) {
    //check rows for user win  
          if (board[i][0] == userside && board[i][0] == board[i][1] && board[i][2] == ' ') {
                locations[0] = i;
                locations[1] = 2;
                //leave this function
                return 1;
                }
          else if (board[i][1] == userside && board[i][1] == board[i][2] && board[i][0] == ' ') {
                locations[0] = i;
                locations[1] = 0;
                //leave this function
                return 1;
                }
          else if (board[i][0] == userside && board[i][0] == board[i][2] && board[i][1] == ' ') {
                locations[0] = i;
                locations[1] = 1;
                //leave this function
                return 1;
                }   
    //check columns for user win
         else if (board[0][i] == userside && board[0][i] == board[1][i] && board[2][i] == ' ') {
                locations[0] = 2;
                locations[1] = i;
                //leave this function
                return 1;
                }
          else if (board[1][i] == userside && board[1][i] == board[2][i] && board[0][i] == ' ') {
                locations[0] = 0;
                locations[1] = i;
                //leave this function
                return 1;
                }
          else if (board[0][i] == userside && board[0][i] == board[2][i] && board[1][i] == ' ') {
                locations[0] = 1;
                locations[1] = i;
                //leave this function
                return 1;
                }
                            
    //Check diagonally for user win
        else if (board[0][0] == userside && board[0][0] == board[1][1] && board[2][2] == ' ') {
            locations[0] = 2;
            locations[1] = 2;
            //leave this function
            return 1;
            }
        else if (board[0][0] == userside && board[0][0] == board[2][2] && board[1][1] == ' ') {
            locations[0] = 1;
            locations[1] = 1;
            //leave this function
            return 1;
            }
        else if (board[1][1] == userside && board[1][1] == board[2][2] && board[0][0] == ' ') {
            locations[0] = 0;
            locations[1] = 0;
            //leave this function
            return 1;
            }
    //check the other diagonal for user win
        else if (board[0][2] == userside && board[0][2] == board[1][1] && board[2][0] == ' ') {
            locations[0] = 2;
            locations[1] = 0;
            //leave this function
            return 1;
            }
        else if (board[0][2] == userside && board[0][2] == board[2][0] && board[1][1] == ' ') {
            locations[0] = 1;
            locations[1] = 1;
            //leave this function
            return 1;
            }
        else if (board[1][1] == userside && board[1][1] == board[2][0] && board[0][2] == ' ') {
            locations[0] = 0;
            locations[1] = 2;
            //leave this function
            return 1;
            }
            
        }//end loop for user win check *************

return 0;
}

//----------------------Function to make a random corner move-------------------//
bool BoardData::cornerMove() {

    //Make a random move to a corner position if any is open.
        if (board[0][0] == ' ' || board[0][2] == ' ' || board[2][0] == ' ' || board[2][2] == ' ') {
            do {
            int rnd1 = 0, rnd2 = 0;
            for (int i = 0; i < 2; i++) {//generate random places
                int getrnd = rand() % 2; // 0 or 1
                if (i == 0 && getrnd == 1)
                    rnd1 = 2;
                else if (i == 0 && getrnd == 0)
                    rnd1 = 0;
                else if (i == 1 && getrnd == 1)
                    rnd2 = 2;
                else if (i == 1 && getrnd == 0)
                    rnd2 = 0;
                 }   
                //set the generated random numbers
                locations[0] = rnd1;
                locations[1] = rnd2;
                }while(board[ locations[0] ][ locations[1]] != ' ');
                
                return 1;
            }
        return 0;

}

//--------------Function to make a random move---------------//
void BoardData::randomMove() {
    //Randomly move because no move was found to make...
            do {
                locations[0] = rand() % 3;
                locations[1] = rand() % 3;
                }while(board[ locations[0] ][ locations[1]] != ' '); 
}

//--------------Function to get all the possible matches
void BoardData::countMatches() {
    ismatch.clear();
    userpiece.clear();
    botpiece.clear();

short int move1[2], move2[2], move3[2];
    
    
    
    //loop through winning 3 in a row list
    for (int i = 0; i < 10; i++) { //10 instead of 8 to run all checks (otherwise we miss checking move 9 for user/bot
        
        

        
        if (i != 0)
            convertMove(i); //convert to locations only if it is not loop 0 (1-9)

        //check if it is (1-9) and user or bot piece
        if (i != 0 && board[locations[0]][locations[1]] == userside) {
            userpiece.push_back(i);
            }
        else if (i != 0 && board[locations[0]][locations[1]] == botside) {
            botpiece.push_back(i);
            }
        //check at every loop, 0-7 for empty spaces
        else {

            if (i >= 8)
                continue; //skip this if it is not a valid array location

            int temp[3];

            //set the moves
            move1[0] = LWINCOMBO[i][0][0];
            move1[1] = LWINCOMBO[i][0][1];
        
            move2[0] = LWINCOMBO[i][1][0];
            move2[1] = LWINCOMBO[i][1][1];
        
            move3[0] = LWINCOMBO[i][2][0];
            move3[1] = LWINCOMBO[i][2][1];

            //if all 3 spaces are blank, mark them down
            if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move2[0] ][ move2[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ' ) {
                //store the temporary board location for testing
                temp[0] = convertMove(move1[0] , move1[1]);
                temp[1] = convertMove(move2[0] , move2[1]);
                temp[2] = convertMove(move3[0] , move3[1]);

                //check for multiples and add first occurences
                for (int tmpi = 0; tmpi < 3; tmpi++) {
                    if ( killMultiples(temp[tmpi]) == 0)
                        ismatch.push_back(temp[tmpi]);
                    }
                }
        //else if there are 2 blank spaces, mark the locations down.
            else if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move2[0] ][ move2[1] ] == ' '){
                //store the temporary board location for testing
                temp[0] = convertMove(move1[0] , move1[1]);
                temp[1] = convertMove(move2[0] , move2[1]);

                //check for multiples and add first occurences
                for (int tmpi = 0; tmpi < 2; tmpi++) {
                    if ( killMultiples(temp[tmpi]) == 0)
                        ismatch.push_back(temp[tmpi]);
                    }
                }
            else if (board[ move2[0] ][ move2[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                //store the temporary board location for testing

                temp[0] = convertMove(move2[0] , move2[1]);
                temp[1] = convertMove(move3[0] , move3[1]);

                //check for multiples and add first occurences
                for (int tmpi = 0; tmpi < 2; tmpi++) {
                    if ( killMultiples(temp[tmpi]) == 0)
                        ismatch.push_back(temp[tmpi]);
                    }
                }
            else if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                //store the temporary board location for testing
                temp[0] = convertMove(move1[0] , move1[1]);
                temp[1] = convertMove(move3[0] , move3[1]);

                //check for multiples and add first occurences
                for (int tmpi = 0; tmpi < 2; tmpi++) {
                    if ( killMultiples(temp[tmpi]) == 0)
                        ismatch.push_back(temp[tmpi]);
                    }
                }
        }

    }//All the matches have been checked now 
    

}
//--------------Function to make sure there is no repeating numbers in the match vector
bool BoardData::killMultiples(int x) {
    for (short int i = 0; i < ismatch.size(); i++) {
        if (ismatch[i] == x)
            return 1; //do not add this
        }

    return 0; //add this
}
