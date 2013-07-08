#ifndef BOARDDATA_H
#define BOARDDATA_H
#include <vector>
using std::vector;

class BoardData {
    public:
        //Variables
        char userside, botside;
        unsigned short int win, countturns;
        bool turn;
        
        
        //Function prototypes
        void printBoard();
        unsigned short int checkWin();
        void makeMove();
        void setSides();
        void resetData();
        void setDifficulty();
        bool cornerMove();
        void randomMove();
        void logWin();
        
        
        
        

    private:
        //Variables
        static const unsigned short int SIZE = 3;
        char board[SIZE][SIZE];
        unsigned short int mode;
        unsigned short int locations[2]; //this holds the final location to move to
        short int RELATIVEWIN[10][10];

        short int savemove[9]; //This is for the log file (all the user's/bot's moves)
        bool firstplayer; //this is for the log file (who moved first)

        vector<int> ismatch; //this is a list of all available moves that lead to a match
        vector<int> userpiece; //this is a list of user piece locations
        vector<int> botpiece; //this is a list of bot piece locations
        vector<int> finalmove; //this is a list of all the final moves after logic

        
        //Function Prototypes

        bool winMaker();
        int convertMove(int, int);
        void convertMove(int);
        void countMatches();
        bool lookAhead();
        bool killMultiples(int);
        

};

#endif