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
        short int usermove[9]; //This is for the log file (all the user's moves)
        bool firstplayer; //this is for the log file (who moved first)

        vector<int> bmatch, umatch; //this is a list of all available moves that lead to a match
        vector<int> badmove; //this is a list of bad moves that lead to the user's advantage
        vector<int> goodmove; //this is a list of good moves that lead to the bot's advantage

        
        //Function Prototypes

        bool winMaker();
        //bool matchMaker();
        int convertMove(int, int);
        void convertMove(int);
        bool getGoodMove();
        void countMatches();
        

};

#endif