/*
 * Project Title:
 * - Tesla
 * Description:
 * This a game where you have to navigate a 15 x 15 room to find Elon Musk's roadster
 * and escape in it. There are two other cars in the room that if you enter you lose
 * and Elon Musk is also in the room and if he caughts you, you also lose. The player
 * can only see the spaces right next to them because the room is dark.
 *
 * Developers:
 * - Peter Kosir - kosirpj@mail.uc.edu
 * - Andrew Sliper - sliperam@mail.uc.edu
 *
 * Help Received:
 * - Had email correspondence with professor Pratyusha about requirements
 *
 * Special Instructions:
 * - you must run the program at full screen to see the full array
 *
 * Developer comments:
 * Peter Kosir:
 * Wrote the logic portion of the program inlcuding all the objects and structures
 * and how the game works
 * Andrew Sliper:
 * Wrote the part of the program that dispays the board and the user interface
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

// This structure is supposed to represent the player icon including values for the
// the location, steps taken, how far the player is from the roadster, and number
// lives the players has left
struct player {
    vector <int> location;
    int steps;
    double HotCold;
    int lives;
};

// This structure holds arrays to be put into the display to represent different objects
struct pictures {
    string r[12];
    string R[12];
    string D[12];
    string d[12];
    string T[12];
    string t[12];
    string F[12];
    string I[12];
    string i[12];
    string f[12];
    string N[12];
    string n[12];
    string Y[12];
    string U[12];
    string u[12];
    string y[12];
    string G[12];
    string g[12];
    string E[12];
};

// This structure holds values for Elon such as his location, whether or not he is
// awake, and a value makes him pause a turn
struct elon {
    vector <int> location;
    string status;
    int count;
};

// This is the object that the whole program is built around
class Game{
  private:
      player Player;
      elon Elon;
      char board[15][15];
      int numSuccess;
      int numFails;
      vector <int> Roadster;
      bool cont;
      string winning;
      string disp[40][7];
      pictures pic;

  public:
      Game();
      void Show();
      void countStep();
      void hotCold();
      void setupGame();
      void playerMove(string);
      void play();
      void elonMove();
      bool keePlaying();
      void elonCheck();
      void lifeCheck();
      void initializeDisplay();
      void showDisplay();
      void displayUpdate();
      void showPic(int, int, char);
      void initializePic();
      void instructions();
};

// This is a constructor that initializes the object when it is created
Game::Game() {
    numSuccess = 0;
    numFails = 0;
    cont = true;
}

// Function creates the game board and randomizes the player and vehicles position
void Game::setupGame() {

    for (int n = 0; n < 15; n++) {
        for (int m = 0; m < 15; m++){
            board[n][m] = 'o';
        }
    }
    int x;
    int y;
    bool hit = false;
    srand((time(0)));
    x = (rand() % 11) + 3;
    y = (rand() % 12) + 1;
    board[x][y] = 'r';
    board[x-1][y] = 'R';
    board[x-2][y] = 'T';
    board[x][y+1] = 'd';
    board[x-1][y+1] = 'D';
    board[x-2][y+1] = 't';
    Roadster = {x-1,y};

    hit = false;
    while (hit == false) {
        hit = true;
        x = (rand() % 11) + 3;
        y = (rand() % 12) + 1;
        for (int r = -4; r <= 1; r++) {
            for (int c = -1; c <= 3; c++) {
                if (board[x + r][y + c] != 'o')
                    hit = false;
            }
        }
    }
    board[x][y] = 'i';
    board[x-1][y] = 'I';
    board[x-2][y] = 'F';
    board[x][y+1] = 'n';
    board[x-1][y+1] = 'N';
    board[x-2][y+1] = 'f';

    hit = false;
    while (hit == false) {
        hit = true;
        x = (rand() % 11) + 3;
        y = (rand() % 12) + 1;
        for (int r = -4; r <= 1; r++) {
            for (int c = -1; c <= 3; c++) {
                if (board[x + r][y + c] != 'o')
                    hit = false;
            }
        }
    }
    board[x][y] = 'u';
    board[x-1][y] = 'U';
    board[x-2][y] = 'Y';
    board[x][y+1] = 'g';
    board[x-1][y+1] = 'G';
    board[x-2][y+1] = 'y';

    hit = false;
    while (hit == false) {
        hit = true;
        x = (rand() % 11) + 3;
        y = (rand() % 12) + 1;
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                if (board[x + r][y + c] != 'o')
                    hit = false;
            }
        }
    }
    board[x][y] = 'P';
    Player.location = {x,y};
    Player.steps = 0;
    Player.lives = 2;

    hit = false;
    while (hit == false) {
        hit = true;
        x = (rand() % 11) + 3;
        y = (rand() % 12) + 1;
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                if (board[x + r][y + c] != 'o')
                    hit = false;
            }
        }
    }
    board[x][y] = 'E';
    Elon.location = {x,y};
    Elon.status = "Asleep";
    Elon.count = 0;
    hotCold();
}

// Prints 15x15 board that was used during testing
void Game::Show() {
    for (int n = 0; n < 15; n++) {
        for (int m = 0; m < 15; m++){
            cout << board[m][n];
        }
        cout << endl;
    }
}

// Counts the number of steps the player takes and displays it to the board
void Game::countStep() {
    Player.steps = Player.steps + 1;
    if (Player.steps < 10) {
        disp[18][5] = "          Steps: " + to_string(Player.steps) + "            ";
    }
    else if (Player.steps < 100){
        disp[18][5] = "          Steps: " + to_string(Player.steps) + "           ";
    }
    else {
        disp[18][5] = "          Steps: " + to_string(Player.steps) + "          ";
    }
}

// This function uses Pythagorean theorem to calculate the distance between
// the player and the roadster and displays it on the screen
void Game::hotCold() {
    double temp;
    int xp = Player.location.at(0);
    int yp = Player.location.at(1);
    int xr = Roadster.at(0);
    int yr = Roadster.at(1);
    temp = sqrt(pow(xp - xr, 2) + pow(yp - yr, 2));
    Player.HotCold = temp;
    if (temp < 2)
        disp[15][5] = "    Cold  OOOOOOOOOO  Hot     ";
    else if (temp < 4)
        disp[15][5] = "    Cold  OOOOOOOOO_  Hot     ";
    else if (temp < 6)
        disp[15][5] = "    Cold  OOOOOOOO__  Hot     ";
    else if (temp < 8)
        disp[15][5] = "    Cold  OOOOOOO___  Hot     ";
    else if (temp < 10)
        disp[15][5] = "    Cold  OOOOOO____  Hot     ";
    else if (temp < 12)
        disp[15][5] = "    Cold  OOOOO_____  Hot     ";
    else if (temp < 14)
        disp[15][5] = "    Cold  OOOO______  Hot     ";
    else if (temp < 16)
        disp[15][5] = "    Cold  OOO_______  Hot     ";
    else if (temp < 18)
        disp[15][5] = "    Cold  OO________  Hot     ";
    else if (temp < 20)
        disp[15][5] = "    Cold  O_________  Hot     ";
    else
        disp[15][5] = "    Cold  __________  Hot     ";
}

// This function takes in a string as a parameter and interrupts it to update the location of the player
void Game::playerMove(string input) {
    if (input == "w") {
        if ((board[Player.location.at(0)][Player.location.at(1) - 1] == 'o') && (Player.location.at(1) != 0)) {
            board[Player.location.at(0)][Player.location.at(1)] = 'o';
            Player.location.at(1) = Player.location.at(1) - 1;
            board[Player.location.at(0)][Player.location.at(1)] = 'P';
            disp[19][2] = "   O   >  O        O  <   O   ";
            countStep();
        }
    }
    else if (input == "a") {
        if ((board[Player.location.at(0) - 1][Player.location.at(1)] == 'o') && (Player.location.at(0) != 0)) {
            board[Player.location.at(0)][Player.location.at(1)] = 'o';
            Player.location.at(0) = Player.location.at(0) - 1;
            board[Player.location.at(0)][Player.location.at(1)] = 'P';
            disp[19][2] = "   O   > O        O   <   O   ";
            countStep();
        }
    }
    else if (input == "s") {
        if (((board[Player.location.at(0)][Player.location.at(1) + 1] == 'o') && (Player.location.at(1) != 14)) || (board[Player.location.at(0)][Player.location.at(1) + 1] == 'R') || (board[Player.location.at(0)][Player.location.at(1) + 1] == 'U') || (board[Player.location.at(0)][Player.location.at(1) + 1] == 'I')) {
            board[Player.location.at(0)][Player.location.at(1)] = 'o';
            Player.location.at(1) = Player.location.at(1) + 1;
            board[Player.location.at(0)][Player.location.at(1)] = 'P';
            disp[19][2] = "   O   >  O        O  <   O   ";
            countStep();
        }
    }
    else if (input == "d") {
        if ((board[Player.location.at(0) + 1][Player.location.at(1)] == 'o') && (Player.location.at(0) != 14)) {
            board[Player.location.at(0)][Player.location.at(1)] = 'o';
            Player.location.at(0) = Player.location.at(0) + 1;
            board[Player.location.at(0)][Player.location.at(1)] = 'P';
            disp[19][2] = "   O   >   O        O <   O   ";
            countStep();
        }
    }
    else if (input == "q") {
        cont = false;
        winning = "quit";
    }
}

// This function moves Elon randomly to an adjacent location if it not already occupied
void Game::elonMove() {
    int move;
    bool success;
    success = false;
    while (!success) {
        move = (rand() % 4);
        if (move == 0) {
            if ((board[Elon.location.at(0) + 1][Elon.location.at(1)] == 'o') && (Elon.location.at(0) != 14)) {
                board[Elon.location.at(0)][Elon.location.at(1)] = 'o';
                Elon.location.at(0) = Elon.location.at(0) + 1;
                board[Elon.location.at(0)][Elon.location.at(1)] = 'E';
                success = true;
            }
        }
        else if (move == 1) {
            if ((board[Elon.location.at(0) - 1][Elon.location.at(1)] == 'o') && (Elon.location.at(0) != 0)) {
                board[Elon.location.at(0)][Elon.location.at(1)] = 'o';
                Elon.location.at(0) = Elon.location.at(0) - 1;
                board[Elon.location.at(0)][Elon.location.at(1)] = 'E';
                success = true;
            }
        }
        else if (move == 2) {
            if ((board[Elon.location.at(0)][Elon.location.at(1) + 1] == 'o') && (Elon.location.at(1) != 14)) {
                board[Elon.location.at(0)][Elon.location.at(1)] = 'o';
                Elon.location.at(1) = Elon.location.at(1) + 1;
                board[Elon.location.at(0)][Elon.location.at(1)] = 'E';
                success = true;
            }
        }
        else if (move == 3) {
            if ((board[Elon.location.at(0)][Elon.location.at(1) - 1] == 'o') && (Elon.location.at(1) != 0)) {
                board[Elon.location.at(0)][Elon.location.at(1)] = 'o';
                Elon.location.at(1) = Elon.location.at(1) - 1;
                board[Elon.location.at(0)][Elon.location.at(1)] = 'E';
                success = true;
            }
        }
    }
}

// This function checks if the player has either won or lost the game it returns false is the game is over
bool Game::keePlaying() {
    if (winning == "quit") {
        return false;
    }
    else if (Player.lives == 0) {
        numFails = numFails + 1;
        winning = "lose";
        return false;
    }
    else if (board[Player.location.at(0) + 1][Player.location.at(1)] == 'u') {
        numFails = numFails + 1;
        winning = "lose";
        return false;
    }
    else if (board[Player.location.at(0) + 1][Player.location.at(1)] == 'i') {
        numFails = numFails + 1;
        winning = "lose";
        return false;
    }
    else if (board[Player.location.at(0) + 1][Player.location.at(1)] == 'r') {
        numSuccess = numSuccess + 1;
        winning = "win";
        return false;
    }
    return true;
}

// This function decides if the player wakes up Elon and then if Elon is awake, he moves
void Game::elonCheck() {
    if (Player.lives == 1) {
        Elon.count++;
    }
    if ((Elon.status == "Asleep") && (fabs(Player.location.at(0) - Elon.location.at(0)) < 3) && (fabs(Player.location.at(1) - Elon.location.at(1)) < 3)) {
        Elon.status = "Awake";
        disp[11][5] = " !!!!!!!!!!!!!!!!!!!!!!!!!!!! ";
        disp[12][5] = " !       Elon is Awake      ! ";
        disp[13][5] = " !!!!!!!!!!!!!!!!!!!!!!!!!!!! ";
    }
    else if ((Elon.status == "Awake") && (Elon.count != 1)){
        elonMove();
    }
}

// This Function subtracts a life if Elon becomes within 2 spaces of the players
void Game::lifeCheck() {
    if ((fabs(Player.location.at(0) - Elon.location.at(0)) < 2) && (fabs(Player.location.at(1) - Elon.location.at(1)) < 2)) {
        Player.lives = Player.lives - 1;
        if (Player.lives == 1) {
            disp[17][5] = "          Lives: 1            ";
        }
        else if (Player.lives == 0) {
            disp[17][5] = "          Lives: 0            ";
        }
    }
}

// This function outputs the story and instructions of the game at the beginning
void Game::instructions() {
    string temp;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "                                      TESLA                                      " << endl;
    cout << " The Story:                                                                      " << endl;
    cout << "   You find your way to SpaceX the night before the Roadster is mounted atop the " << endl;
    cout << "   rocket which is to send it into space.  The building is dark and the roadster " << endl;
    cout << "   is hidden somewhere in large open dark room (15x15).  In addition to the      " << endl;
    cout << "   roadster, two other cars (a Yugo and ford pinto) are being stored in this     " << endl;
    cout << "   facility as well.      " << endl << endl;
    cout << "   BTW: Elon Musk is sleeping somewhere in the same room and you have to avoid   " << endl;
    cout << "   waking him to find your way to the roadster. Once awoken he will randomly " << endl;
    cout << "   move through the building looking for you. If he lands next to you, you can " << endl;
    cout << "   get away from him only once. The second time you will be the Starman and " << endl;
    cout << "   blasted into space with his roadster. Once he is awake your only chance to " << endl;
    cout << "   avoid being Starman is to make your escape in his roadster. If you get into " << endl;
    cout << "   either of the other two cars you will be caught and become the Starman. " << endl << endl;
    cout << " How to Play:" << endl;
    cout << "   The goal for the user is to find the roadster before becoming the Star man." << endl;
    cout << "      w ~ up" << endl;
    cout << "      s ~ down" << endl;
    cout << "      a ~ left" << endl;
    cout << "      d ~ right" << endl;
    cout << "      q ~ quit" << endl;
    cout << "   Enter a car by pressing the direction of the driver's side door while next to it" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN ~ FULL SCREEN" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Press any key to begin" << endl;
    cin >> temp;
}

// This function combines all the other functions to play the game
void Game::play() {
    string input;
    bool check;
    instructions();
    while (cont) {
        input = "";
        check = true;
        setupGame();
        initializePic();
        initializeDisplay();
        hotCold();
        elonCheck();
        showDisplay();
        while (check) {
            cout << "What is your move???" << endl;
            cin >> input;
            if ((input == "a") || (input == "w") || (input == "s") || (input == "d") || (input == "q")) {
                playerMove(input);
                elonCheck();
                lifeCheck();
                hotCold();
                displayUpdate();
                check = keePlaying();
            }
            showDisplay();
        }
        if (winning == "win") {
            cout << "YOU WIN!!!" << endl;
            cout << "would you like to play again (y/n)?" << endl;
            cin >> input;
            if (input != "y")
                cont = false;
        }
        else if (winning == "lose") {
            cout << "you lose" << endl;
            cout << "would you like to play again (y/n)?" << endl;
            cin >> input;
            if (input != "y")
                cont = false;
        }
    }
    cout << "thanks for playing!!!" << endl;
}

// This function initializes the outsides and player displays of the game.
// In other words, the stationary objects on the screen
void Game::initializeDisplay() {
    for (int r = 0; r < 40; r++){
        disp[r][0] = "XX";
        disp[r][1] = "                              ";
        disp[r][2] = "                              ";
        disp[r][3] = "                              ";
        disp[r][4] = "XX";
        disp[r][5] = "                              ";
        disp[r][6] = "XX";
    }
    disp[0][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[0][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[0][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[0][5] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[1][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[1][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[1][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[1][5] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

    disp[38][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[38][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[38][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[38][5] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[39][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[39][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[39][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    disp[39][5] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

    disp[14][2] = "                              ";
    disp[15][2] = "      OOOOOOOOOOOOOOOOOO      ";
    disp[16][2] = "    OOOOOOOOOOOOOOOOOOOOOO    ";
    disp[17][2] = "   OOOOOOOOOOOOOOOOOOOOOOOO   ";
    disp[18][2] = "   OO    ___  __  ___    OO   ";
    disp[19][2] = "   O   >  O        O  <   O   ";
    disp[20][2] = "   O                      O   ";
    disp[21][2] = "   O     \\     o    /     O   ";
    disp[22][2] = "   OO     )    3   (     OO   ";
    disp[23][2] = "    OOO                OOO    ";
    disp[24][2] = "      OOOOOOOOOOOOOOOOOO      ";
    disp[25][2] = "                              ";

    disp[2][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[3][5] = " OOOOO  OOOO  OOO  O      O   ";
    disp[4][5] = "   O    O     O    O     O O  ";
    disp[5][5] = "   O    OOO   OOO  O    OOOOO ";
    disp[6][5] = "   O    O       O  O    O   O ";
    disp[7][5] = "   O    OOOO  OOO  OOO  O   O ";
    disp[8][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[9][5] = "  Peter Kosir & Andrew Sliper ";
    disp[10][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[11][5] = " zZzZzZzZzZzZzZzZzZzZzZzZzZzZ ";
    disp[12][5] = " Z      Elon is Asleep      z ";
    disp[13][5] = " zZzZzZzZzZzZzZzZzZzZzZzZzZzZ ";
    disp[14][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[15][5] = "    Cold  __________  Hot     ";
    disp[16][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[17][5] = "          Lives: 2            ";
    disp[18][5] = "          Steps: 0            ";
    disp[19][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

    if (numFails < 10) {
        disp[21][5] = "      Number of Fails: " + to_string(numFails) + "      ";
    }
    else if (numFails < 100){
        disp[21][5] = "      Number of Fails: " + to_string(numFails) + "     ";
    }
    if (numSuccess < 10) {
        disp[20][5] = "  Number of Successes: " + to_string(numSuccess) + "      ";
    }
    else if (numSuccess < 100){
        disp[20][5] = "  Number of Successes: " + to_string(numSuccess) + "     ";
    }
    disp[22][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[23][5] = " You must get to the Roadster ";
    disp[24][5] = " before Elon caughts you.     ";
    disp[25][5] = " Elon will also caught you if ";
    disp[26][5] = " you enter the wrong car.     ";
    disp[27][5] = " If you lose, Elon will make  ";
    disp[28][5] = " you the STARMAN and launch   ";
    disp[29][5] = " you into space. If you win   ";
    disp[30][5] = " then you get to escape with  ";
    disp[31][5] = " Elon's prized Roadster.      ";
    disp[32][5] = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    disp[33][5] = "           CONTROLS           ";
    disp[34][5] = "   a = Left        d = Right  ";
    disp[35][5] = "   w = Up          s = Down   ";
    disp[36][5] = "   q = Quit                   ";
}

// This function creates all the images to represent the different things in the game
void Game::initializePic() {
    pic.R[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.R[1] = "                              ";
    pic.R[2] = "OOOOO                         ";
    pic.R[3] = "    OO                       O";
    pic.R[4] = "     O                       O";
    pic.R[5] = "     O                       O";
    pic.R[6] = "     O                       O";
    pic.R[7] = "     O                       O";
    pic.R[8] = "     O                       O";
    pic.R[9] = "     O                       O";
    pic.R[10] = "     O                       O";
    pic.R[11] = "     OXXXXXXXXXXXXXXXXXXXXXXXO";

    pic.r[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";
    pic.r[1] = "                           OO ";
    pic.r[2] = "OOOOOOOOOOOOOOO              O";
    pic.r[3] = "O             O         OOO  O";
    pic.r[4] = "              O         O O  O";
    pic.r[5] = "              O         O O  O";
    pic.r[6] = "              O         O O  O";
    pic.r[7] = "              O         OOO  O";
    pic.r[8] = "              O              O";
    pic.r[9] = "              O         _    O";
    pic.r[10] = "              O        | |   O";
    pic.r[11] = "              OXXXXXXX |  --|O";

    pic.T[0] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.T[1] = "OO                            ";
    pic.T[2] = "O OOOOOOOO         OOOOOOOOOOO";
    pic.T[3] = "O O       O        O          ";
    pic.T[4] = "O O       O        O          ";
    pic.T[5] = "O O       O        O          ";
    pic.T[6] = "O O       O        O          ";
    pic.T[7] = "O O       O        O          ";
    pic.T[8] = "O O       O        O          ";
    pic.T[9] = "O O       O        O          ";
    pic.T[10] = "O O       O        O          ";
    pic.T[11] = "OXO       OXXXXXXXXO          ";

    pic.t[0] = "OXO       OXXXXXXXXO          ";
    pic.t[1] = "O O       O        O          ";
    pic.t[2] = "O O       O        O          ";
    pic.t[3] = "O O       O        O          ";
    pic.t[4] = "O O       O        O          ";
    pic.t[5] = "O O       O        O          ";
    pic.t[6] = "O O       O        O          ";
    pic.t[7] = "O O       O        O          ";
    pic.t[8] = "O O       O        O          ";
    pic.t[9] = "O OOOOOOOO         OOOOOOOOOOO";
    pic.t[10] = "OO                            ";
    pic.t[11] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.D[0] = "     OXXXXXXXXXXXXXXXXXXXXXXXO";
    pic.D[1] = "     O                       O";
    pic.D[2] = "     O                       O";
    pic.D[3] = "     O                       O";
    pic.D[4] = "     O                       O";
    pic.D[5] = "     O                       O";
    pic.D[6] = "     O                       O";
    pic.D[7] = "     O                       O";
    pic.D[8] = "    OO                       O";
    pic.D[9] = "OOOOO                         ";
    pic.D[10] = "                              ";
    pic.D[11] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.d[0] = "              OXXXXXXX |  --|O";
    pic.d[1] = "              O        |_|   O";
    pic.d[2] = "              O              O";
    pic.d[3] = "              O              O";
    pic.d[4] = "              O         OOO  O";
    pic.d[5] = "              O         O O  O";
    pic.d[6] = "              O         O O  O";
    pic.d[7] = "              O         O O  O";
    pic.d[8] = "O             O         OOO  O";
    pic.d[9] = "OOOOOOOOOOOOOOO              O";
    pic.d[10] = "                           OO ";
    pic.d[11] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";

    pic.F[0] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.F[1] = "OO                            ";
    pic.F[2] = "O OOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.F[3] = "O O                           ";
    pic.F[4] = "O O                           ";
    pic.F[5] = "O O                           ";
    pic.F[6] = "O O                           ";
    pic.F[7] = "O O                           ";
    pic.F[8] = "O O                           ";
    pic.F[9] = "O O                           ";
    pic.F[10] = "O O                           ";
    pic.F[11] = "O O                           ";

    pic.I[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.I[1] = "                              ";
    pic.I[2] = "OOOOO                 OOOOOOOO";
    pic.I[3] = "    OO                O       ";
    pic.I[4] = "     O                O       ";
    pic.I[5] = "     O                O       ";
    pic.I[6] = "     O                O       ";
    pic.I[7] = "     O                O       ";
    pic.I[8] = "     O                O       ";
    pic.I[9] = "     O                O       ";
    pic.I[10] = "     O                O       ";
    pic.I[11] = "     O                O       ";

    pic.i[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";
    pic.i[1] = "                           OO ";
    pic.i[2] = "OOOOOOOO                     O";
    pic.i[3] = "       O                OOO  O";
    pic.i[4] = "       O                O O  O";
    pic.i[5] = "       O                O O  O";
    pic.i[6] = "       O                O O  O";
    pic.i[7] = "       O                OOO  O";
    pic.i[8] = "       O                     O";
    pic.i[9] = "       O             _       O";
    pic.i[10] = "       O            | |  _   O";
    pic.i[11] = "       O            | |_| |_ O";

    pic.n[0] = "       O            |_______ O";
    pic.n[1] = "       O                     O";
    pic.n[2] = "       O                     O";
    pic.n[3] = "       O                     O";
    pic.n[4] = "       O                OOO  O";
    pic.n[5] = "       O                O O  O";
    pic.n[6] = "       O                O O  O";
    pic.n[7] = "       O                O O  O";
    pic.n[8] = "       O                OOO  O";
    pic.n[9] = "OOOOOOOO                     O";
    pic.n[10] = "                           OO ";
    pic.n[11] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";

    pic.N[0] = "     O                O       ";
    pic.N[1] = "     O                O       ";
    pic.N[2] = "     O                O       ";
    pic.N[3] = "     O                O       ";
    pic.N[4] = "     O                O       ";
    pic.N[5] = "     O                O       ";
    pic.N[6] = "     O                O       ";
    pic.N[7] = "     O                O       ";
    pic.N[8] = "    OO                O       ";
    pic.N[9] = "OOOOO                 OOOOOOOO";
    pic.N[10] = "                              ";
    pic.N[11] = "0OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.f[0] = "O O                           ";
    pic.f[1] = "O O                           ";
    pic.f[2] = "O O                           ";
    pic.f[3] = "O O                           ";
    pic.f[4] = "O O                           ";
    pic.f[5] = "O O                           ";
    pic.f[6] = "O O                           ";
    pic.f[7] = "O O                           ";
    pic.f[8] = "O O                           ";
    pic.f[9] = "O OOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.f[10] = "OO                            ";
    pic.f[11] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.Y[0] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.Y[1] = "OO                            ";
    pic.Y[2] = "O    OOOOOOOOOOOOOOOO         ";
    pic.Y[3] = "O    O              OO        ";
    pic.Y[4] = "O    O               O        ";
    pic.Y[5] = "O    O               O        ";
    pic.Y[6] = "O    O               O        ";
    pic.Y[7] = "O    O               O        ";
    pic.Y[8] = "O    O               O        ";
    pic.Y[9] = "O    O               O        ";
    pic.Y[10] = "O    O               O        ";
    pic.Y[11] = "O    O               O        ";

    pic.y[0] = "O    O               O        ";
    pic.y[1] = "O    O               O        ";
    pic.y[2] = "O    O               O        ";
    pic.y[3] = "O    O               O        ";
    pic.y[4] = "O    O               O        ";
    pic.y[5] = "O    O               O        ";
    pic.y[6] = "O    O               O        ";
    pic.y[7] = "O    O               O        ";
    pic.y[8] = "O    O              OO        ";
    pic.y[9] = "O    OOOOOOOOOOOOOOOO         ";
    pic.y[10] = "OO                            ";
    pic.y[11] = " OOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.U[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    pic.U[1] = "                              ";
    pic.U[2] = "                        OOOOOO";
    pic.U[3] = "                        O     ";
    pic.U[4] = "                        O     ";
    pic.U[5] = "                        O     ";
    pic.U[6] = "                        O     ";
    pic.U[7] = "                        O     ";
    pic.U[8] = "                        O     ";
    pic.U[9] = "                        O     ";
    pic.U[10] = "                        O     ";
    pic.U[11] = "                        O     ";

    pic.G[0] = "                        O     ";
    pic.G[1] = "                        O     ";
    pic.G[2] = "                        O     ";
    pic.G[3] = "                        O     ";
    pic.G[4] = "                        O     ";
    pic.G[5] = "                        O     ";
    pic.G[6] = "                        O     ";
    pic.G[7] = "                        O     ";
    pic.G[8] = "                        O     ";
    pic.G[9] = "                        OOOOOO";
    pic.G[10] = "                              ";
    pic.G[11] = "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    pic.u[0] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";
    pic.u[1] = "                           OO ";
    pic.u[2] = "OOOOOOOOOO                   O";
    pic.u[3] = "         O              OOO  O";
    pic.u[4] = "         O              O O  O";
    pic.u[5] = "         O              O O  O";
    pic.u[6] = "         O              O O  O";
    pic.u[7] = "         O              OOO  O";
    pic.u[8] = "         O                   O";
    pic.u[9] = "         O                   O";
    pic.u[10] = "         O             \\\\    O";
    pic.u[11] = "         O              \\\\--|O";

    pic.g[0] = "         O              //--|O";
    pic.g[1] = "         O             //    O";
    pic.g[2] = "         O                   O";
    pic.g[3] = "         O                   O";
    pic.g[4] = "         O              OOO  O";
    pic.g[5] = "         O              O O  O";
    pic.g[6] = "         O              O O  O";
    pic.g[7] = "         O              O O  O";
    pic.g[8] = "         O              OOO  O";
    pic.g[9] = "OOOOOOOOOO                   O";
    pic.g[10] = "                           OO ";
    pic.g[11] = "OOOOOOOOOOOOOOOOOOOOOOOOOOO   ";

    pic.E[0] = "                              ";
    pic.E[1] = "       OOOOOOOOOOOOOOOO       ";
    pic.E[2] = "     OOO              OOO     ";
    pic.E[3] = "   OOO    ELON  MUSK    OOO   ";
    pic.E[4] = "   OO     \\        /     OO   ";
    pic.E[5] = "   O      >\\      /<      O   ";
    pic.E[6] = "   O          __          O   ";
    pic.E[7] = "   O         /  \\         O   ";
    pic.E[8] = "   OO        \\__/        OO   ";
    pic.E[9] = "    OOO                OOO    ";
    pic.E[10] = "      OOOOOOOOOOOOOOOOOO      ";
    pic.E[11] = "                              ";
}

// This function displays the game board
void Game::showDisplay() {
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    for (int r = 0; r < 40; r++) {
        for (int c = 0; c < 7; c++) {
            cout << disp[r][c];
        }
        cout << endl;
    }
}

// This function takes in the location of a thing on the board using r and c as its
// relative location from the player and then based on the letter that represents
// the thing, updates the display to show the object
void Game::showPic(int r, int c, char let) {
    if (let == 'R') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.R[i];
        }
    }
    else if (let == 'r') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.r[i];
        }
    }
    else if (let == 'T') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.T[i];
        }
    }
    else if (let == 't') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.t[i];
        }
    }
    else if (let == 'D') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.D[i];
        }
    }
    else if (let == 'd') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.d[i];
        }
    }
    else if (let == 'F') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.F[i];
        }
    }
    else if (let == 'I') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.I[i];
        }
    }
    else if (let == 'i') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.i[i];
        }
    }
    else if (let == 'f') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.f[i];
        }
    }
    else if (let == 'N') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.N[i];
        }
    }
    else if (let == 'n') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.n[i];
        }
    }
    else if (let == 'N') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.N[i];
        }
    }
    else if (let == 'n') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.n[i];
        }
    }
    else if (let == 'Y') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.Y[i];
        }
    }
    else if (let == 'U') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.U[i];
        }
    }
    else if (let == 'u') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.u[i];
        }
    }
    else if (let == 'y') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.y[i];
        }
    }
    else if (let == 'G') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.G[i];
        }
    }
    else if (let == 'g') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.g[i];
        }
    }
    else if (let == 'E') {
        for (int i = 0; i < 12; i++) {
            disp[(12*r) + 2 + i][c + 1] = pic.E[i];
        }
    }
}

// This function updates the display in the 3x3, and has it ready to be shown on the screen
void Game::displayUpdate() {
    for (int i = 0; i < 36; i++) {
        disp[i + 2][1] = "                              ";
        disp[i + 2][3] = "                              ";
    }
    for (int i = 0; i < 12; i++) {
        disp[i + 2][2] = "                              ";
        disp[i + 26][2] = "                              ";
    }
    if (Player.location.at(0) == 0) {
        for (int i = 0; i < 36; i++) {
            disp[i + 2][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX||";
        }
    }
    if (Player.location.at(0) == 14) {
        for (int i = 0; i < 36; i++) {
            disp[i + 2][3] = "||XXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        }
    }
    if (Player.location.at(1) == 0) {
        for (int i = 0; i < 10; i++) {
            disp[i + 2][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            disp[i + 2][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            disp[i + 2][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        }
        disp[12][1] = "------------------------------";
        disp[12][2] = "------------------------------";
        disp[12][3] = "------------------------------";
        disp[13][1] = "------------------------------";
        disp[13][2] = "------------------------------";
        disp[13][3] = "------------------------------";
    }
    if (Player.location.at(1) == 14) {
        for (int i = 0; i < 10; i++) {
            disp[i + 28][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            disp[i + 28][2] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
            disp[i + 28][3] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        }
        disp[26][1] = "------------------------------";
        disp[26][2] = "------------------------------";
        disp[26][3] = "------------------------------";
        disp[27][1] = "------------------------------";
        disp[27][2] = "------------------------------";
        disp[27][3] = "------------------------------";
    }
    if ((Player.location.at(1) == 0) && (Player.location.at(0) == 0)) {
        disp[12][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX/";
        disp[13][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX//";
    }
    else if ((Player.location.at(1) == 0) && (Player.location.at(0) == 14)) {
        disp[12][3] = "\\XXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        disp[13][3] = "\\\\XXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    }
    else if ((Player.location.at(1) == 14) && (Player.location.at(0) == 14)) {
        disp[26][3] = "//XXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        disp[27][3] = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    }
    else if ((Player.location.at(1) == 14) && (Player.location.at(0) == 0)) {
        disp[26][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX\\\\";
        disp[27][1] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX\\";
    }

    if ((Player.location.at(1) != 0) && (Player.location.at(0) != 0) && (board[Player.location.at(0) - 1][Player.location.at(1) - 1] != 'o')) {
        showPic(0,0,board[Player.location.at(0) - 1][Player.location.at(1) - 1]);
    }
    if ((Player.location.at(1) != 0) && (board[Player.location.at(0)][Player.location.at(1) - 1] != 'o')) {
        showPic(0,1,board[Player.location.at(0)][Player.location.at(1) - 1]);
    }
    if ((Player.location.at(1) != 0) && (Player.location.at(0) != 14) && (board[Player.location.at(0) + 1][Player.location.at(1) - 1] != 'o')) {
        showPic(0,2,board[Player.location.at(0) + 1][Player.location.at(1) - 1]);
    }
    if ((Player.location.at(0) != 0) && (board[Player.location.at(0) - 1][Player.location.at(1)] != 'o')) {
        showPic(1,0,board[Player.location.at(0) - 1][Player.location.at(1)]);
    }
    if ((Player.location.at(0) != 14) && (board[Player.location.at(0) + 1][Player.location.at(1)] != 'o')) {
        showPic(1,2,board[Player.location.at(0) + 1][Player.location.at(1)]);
    }
    if ((Player.location.at(1) != 14) && (Player.location.at(0) != 0) && (board[Player.location.at(0) - 1][Player.location.at(1) + 1] != 'o')) {
        showPic(2,0,board[Player.location.at(0) - 1][Player.location.at(1) + 1]);
    }
    if ((Player.location.at(1) != 14) && (board[Player.location.at(0)][Player.location.at(1) + 1] != 'o')) {
        showPic(2,1,board[Player.location.at(0)][Player.location.at(1) + 1]);
    }
    if ((Player.location.at(1) != 14) && (Player.location.at(0) != 14) && (board[Player.location.at(0) + 1][Player.location.at(1) + 1] != 'o')) {
        showPic(2,2,board[Player.location.at(0) + 1][Player.location.at(1) + 1]);
    }

}

// This is the main where everything is executed
int main()
{
    Game game;
    game.play();

    return 0;
}
