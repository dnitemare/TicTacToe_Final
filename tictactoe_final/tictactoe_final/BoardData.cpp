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

        
        badmove.clear();
        goodmove.clear();
        bmatch.clear();
        umatch.clear();
        
        //reset the board
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = ' ';
            }
        }

        //reset the user moves.
        for (int i = 0; i < 9; i++) {
            usermove[i] = 0;
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
        if (usermove[i] == 0)
            continue;
        else
            outFile << usermove[i] << " ";
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
    }
    else if (x == 1) {
        if (y == 0)
           return 4;
        else if (y == 1)
            return 5;
        else if (y == 2)
            return 6;
    }
    else if (x == 2) {
        if (y == 0)
           return 7;
        else if (y == 1)
            return 8;
        else if (y == 2)
            return 9;
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
        usermove[countturns] = umove;
        
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
                    movedyet = getGoodMove();
                if (movedyet == 0)
                    movedyet = cornerMove();
                if (movedyet == 0)
                    randomMove();
            
            }
            
        //for all bots    
        umove = convertMove(locations[0], locations[1]);
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
    umatch.clear();
    bmatch.clear();
short int move1[2], move2[2], move3[2];
    
    
    
    //loop through winning 3 in a row list
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            if (j == 0) {
                move1[0] = LWINCOMBO[i][j][0];
                move1[1] = LWINCOMBO[i][j][1];
                }
            else if (j == 1) {
                move2[0] = LWINCOMBO[i][j][0];
                move2[1] = LWINCOMBO[i][j][1];
                }
            else if (j == 2) {
                move3[0] = LWINCOMBO[i][j][0];
                move3[1] = LWINCOMBO[i][j][1];
                }
            }  
        //if there is a bot piece in any winning location then
        if (board[ move1[0] ][ move1[1] ] == botside || board[ move2[0] ][ move2[1] ] == botside || board[ move3[0] ][ move3[1] ] == botside) {
            //if there are 2 blank spaces, mark the locations down.
            if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move2[0] ][ move2[1] ] == ' '){
                
                bmatch.push_back( convertMove(move1[0] , move1[1]) );
                bmatch.push_back( convertMove(move2[0] , move2[1]) );
                }
            else if (board[ move2[0] ][ move2[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                
                bmatch.push_back( convertMove(move2[0] , move2[1]) );
                bmatch.push_back( convertMove(move3[0] , move3[1]) );

                }
            else if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                
                bmatch.push_back( convertMove(move1[0] , move1[1]) );
                bmatch.push_back( convertMove(move3[0] , move3[1]) );

                }
        }//end of bot check

        //if there is a user piece in any winning location then
        if (board[ move1[0] ][ move1[1] ] == userside || board[ move2[0] ][ move2[1] ] == userside || board[ move3[0] ][ move3[1] ] == userside) {
                             
            //if there are 2 blank spaces, mark the locations down.
            if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move2[0] ][ move2[1] ] == ' '){
                
                umatch.push_back( convertMove(move1[0] , move1[1]) );
                umatch.push_back( convertMove(move2[0] , move2[1]) );
                }
            else if (board[ move2[0] ][ move2[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                
                umatch.push_back( convertMove(move2[0] , move2[1]) );
                umatch.push_back( convertMove(move3[0] , move3[1]) );

                }
            else if (board[ move1[0] ][ move1[1] ] == ' ' && board[ move3[0] ][ move3[1] ] == ' ') {
                
                umatch.push_back( convertMove(move1[0] , move1[1]) );
                umatch.push_back( convertMove(move3[0] , move3[1]) );

                }
        }//end of user check
        

    }//All the matches have been checked now 
    

}
//*******************************************************//
//                      Bot AI                          //
//*****************************************************//

//Function to add weight to the moves

bool BoardData::getGoodMove() {
    badmove.clear();
    goodmove.clear();

    if (countturns == 1) { //if user moved first

        //if middle is open and user has made a corner move
        if (board[1][1] == ' ' && (board[0][0] == userside || board[0][2] == userside || board[2][0] == userside || board[2][2] == userside) ) {
            locations[0] = 1;
            locations[1] = 1;
            return 1;
            }
        else { //otherwise make a corner move
            return cornerMove();
            }
    }

    //Loop to find moves that will set the user up for a split
    for (int i = 0; i < umatch.size(); i++) {
        for (int j = 0; j < umatch.size(); j++) {
            if (i == j) {
                continue; //it will always be true otherwise... that's bad, mmk?
            }
            else {
                if (umatch[i] == umatch[j]) {
                    badmove.push_back(umatch[i]);
                }
            }
        }
    }//end of loop

        //Loop to find moves that will set the bot up for a split
    for (int i = 0; i < bmatch.size(); i++) {
        for (int j = 0; j < bmatch.size(); j++) {
            if (i == j) {
                continue; //it will always be true otherwise... that's bad, mmk?
            }
            else {
                if (bmatch[i] == bmatch[j]) {
                    goodmove.push_back(bmatch[i]);
                }
            }
        }
    }//end of loop

    //################################################################################//
    //                           Actual Move logic section                           //
    //##############################################################################//

    //Loop to get a random move that is NOT a bad move
        if (!badmove.empty()) { //if there is a bad move
            if (!goodmove.empty()) { //if there is a good move as well
                for (int i = 0; i < badmove.size();i++) {
                    for (int j = 0; j < goodmove.size();j++) {
                        if (badmove[i] == goodmove[j])
                            goodmove[j] = -1;//make the move invalid if it is not a good move.
                        }
                }
                //if there is still a valid good move to make do so
                for (int i = 0; i < goodmove.size(); i++) {
                    if (goodmove[i] != -1) {
                        convertMove(goodmove[i]);
                        return 1;
                        }
                }

            }//end of if good move as well
            //if there is no good move to make, make one that is not a bad move.
            int getrand = rand() % bmatch.size();
            bool skipme = 0;
            do {
                skipme = 0;
                getrand = rand() % bmatch.size();
                for (int i = 0; i < badmove.size(); i++) {
                    if (bmatch[getrand] == badmove[i]) {
                        skipme = 1;
                        break;
                    }
                }//end of bad move check
            } while(skipme == 1); //no good move to make end of loop
            convertMove(bmatch[getrand]);
            return 1;
        }//end of bad move

        else if (!goodmove.empty()) {// if there is no bad moves, but there is a good move, make a random good move.
                int getrand = rand() % goodmove.size();
                convertMove(goodmove[getrand]);
                return 1;
                }
        else
            return 0;

}



//--------------Fuction to check if there is a win to be had at some point...
/*  old code
bool BoardData::matchMaker() {
    
    //do the logic to see what move is the best choice.
    if (isBmatch >= isUmatch) { //If the bot can make more matches this turn
        if ((isBmatch / 2) == 1) { //if there is only 1 match to make.
            int getrand = rand() % isBmatch;
            locations[0] = bmatch[getrand][0];
            locations[1] = bmatch[getrand][1];
            return 1;
            }
        else if ( (isBmatch /2) > 1) { //if there is more than 1 match to make.
            
            if ( (board[1][1] == ' ') && (countturns < 2) && 
                (board[0][0] == botside || board[0][2] == botside || board[2][0] == botside || board[2][2] == botside) ) {
                    //If the middle is open and we have a move to connect to already we either win or stalemate...
                    locations[0] = 1;
                    locations[1] = 1;
                    return 1; //move to the middle
                    }
                    
            else {
                //loop and make the move if it is a corner move (priority corner)
                for (int i = 0; i < isBmatch; i++) {
                    for (int j = 0; j < isBmatch; j++) {
                        if (i == j) {
                        continue; //it will always be true otherwise... that's bad, mmk?
                        }
                    else {
                        if (bmatch[i][0] == bmatch[j][0] && bmatch[i][1] == bmatch[j][1]) {

                            if ( (bmatch[i][0] == 0 && (bmatch[i][1] == 0 || bmatch[i][1] == 2) ) || 
                                (bmatch[i][0] == 2 && (bmatch[i][1] == 0 || bmatch[i][1] == 2) ) ) { 
                                    locations[0] = bmatch[i][0];
                                    locations[1] = bmatch[i][1];
                                    return 1; //this is our move
                                    }
                            else if ( (bmatch[j][0] == 0 && (bmatch[j][1] == 0 || bmatch[j][1] == 2) ) || 
                                (bmatch[j][0] == 2 && (bmatch[j][1] == 0 || bmatch[j][1] == 2) ) ) {
                                    locations[0] = bmatch[j][0];
                                    locations[1] = bmatch[j][1];
                                    return 1; //this is our move
                                    }
                            
                            
                
                            }
                        }
                    }
                }//end of loop

                //Loop again if no corner move was found
                for (int i = 0; i < isBmatch; i++) {
                    for (int j = 0; j < isBmatch; j++) {
                        if (i == j) {
                        continue; //it will always be true otherwise... that's bad, mmk?
                        }
                    else {
                        if (bmatch[i][0] == bmatch[j][0] && bmatch[i][1] == bmatch[j][1]) {
                            locations[0] = bmatch[i][0];
                            locations[1] = bmatch[i][1];
                            return 1; //this is our move
                            }
                    }
                }
            }//end of loop
        } //all the code in the else
    }
}


    else if (isBmatch < isUmatch) { //If the user can make more matches this turn
        if ((isUmatch / 2) == 1) { //if there is only 1 match to make.
            int getrand = rand() % isUmatch;
            locations[0] = umatch[getrand][0];
            locations[1] = umatch[getrand][1];
            return 1;
            }
        else if ( (isUmatch /2) > 1) { //if there is more than 1 match to make.

            
            if ( (board[1][1] == ' ') && (countturns < 2) && 
                    (board[0][0] == userside || board[0][2] == userside || board[2][0] == userside || board[2][2] == userside) ) {
                    //If the middle is open and the player has a move to connect to already block the split setup
                        locations[0] = 1;
                        locations[1] = 1;
                        return 1; //move to the middle
                    }
                    
            else {
                //loop and make the move if it is a corner move (priority corner)
                for (int i = 0; i < isUmatch; i++) {
                    for (int j = 0; j < isUmatch; j++) {
                        if (i == j) {
                        continue; //it will always be true otherwise... that's bad, mmk?
                        }
                        else {
                        if (umatch[i][0] == umatch[j][0] && umatch[i][1] == umatch[j][1]) {

                            if ( (umatch[i][0] == 0 && (umatch[i][1] == 0 || umatch[i][1] == 2) ) || 
                                (umatch[i][0] == 2 && (umatch[i][1] == 0 || umatch[i][1] == 2) ) ) { 
                                    locations[0] = umatch[i][0];
                                    locations[1] = umatch[i][1];
                                    return 1; //this is our move
                                    }
                            else if ( (umatch[j][0] == 0 && (umatch[j][1] == 0 || umatch[j][1] == 2) ) || 
                                (umatch[j][0] == 2 && (umatch[j][1] == 0 || umatch[j][1] == 2) ) ) {
                                    locations[0] = umatch[j][0];
                                    locations[1] = umatch[j][1];
                                    return 1; //this is our move
                                    }
                            
                            
                
                            }
                        }
                    }
                }//end of loop

                //Loop again if no corner move was found
                for (int i = 0; i < isUmatch; i++) {
                    for (int j = 0; j < isUmatch; j++) {
                        if (i == j) {
                        continue; //it will always be true otherwise... that's bad, mmk?
                        }
                    else {
                        if (umatch[i][0] == umatch[j][0] && umatch[i][1] == umatch[j][1]) {
                            locations[0] = umatch[i][0];
                            locations[1] = umatch[i][1];
                            return 1; //this is our move
                            }
                    }
                }//end of loop
            }
        } //all the code inside the else
    }
}



    return 0;      
        
             
}
*/