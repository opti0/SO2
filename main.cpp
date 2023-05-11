#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <algorithm>
#include <windows.h>
#include <conio.h> // for Windows
#include <future>
#include "semafor.cpp"
const int BOARD_WIDTH = 20;
const int BOARD_HEIGHT = 20;
const int NUM_GHOSTS = 4;
const int GHOST_SPEED = 1;
const int PACMAN_SPEED = 1;
const int NUM_DOTS = 10;
const int SCORE_DOT = 10;
const int SCORE_GHOST = 100;
const int SCORE_PELLET = 50;
const int PELLET_DURATION = 10;
using namespace std;
int testInt=-1;

struct Ghost {
    int x, y;
    int direction;
};

Ghost ghosts[NUM_GHOSTS];
auto end_power_pellet_mode = []() {
    this_thread::sleep_for(chrono::seconds(PELLET_DURATION));
    // end power pellet mode
};
vector<string> board(BOARD_HEIGHT, string(BOARD_WIDTH, ' '));
mutex board_mutex;
Semafor semafor(false);
int pacman_x, pacman_y;
int score;
bool game_over;

void clear_screen() {
    system("cls");
}

bool still_points (){
    bool colectable_points = false;
    for (int i = 0; i<BOARD_WIDTH; i++)
        for (int j = 0; j<BOARD_WIDTH; j++)
        if (board [i][j]=='.') colectable_points = true;
    return colectable_points;

}

void print_board() {
    clear_screen();
    char pixel;
    for (int h = 0; h < BOARD_HEIGHT; h++) {
        for(int w=0;w<BOARD_WIDTH;w++){
            pixel=board[h][w];
            switch (pixel) {
                case 'P':
                    //Printing Pacman symbol in yellow color
                    printf("\x1b[33;40m%c\x1b[0m", pixel);
                    break;
                case '#':
                    //Printing board borders in blue color
                    printf("\x1b[94;104m%c\x1b[0m", pixel);
                    break;
                case 'O':
                    //Printing pellet in green color
                    printf("\x1b[32;40m%c\x1b[0m", pixel);
                    break;
                case 'G':
                    //Printing ghost in red color
                    printf("\x1b[91;40m%c\x1b[0m", pixel);
                    break;
                default:
                    cout<<pixel;
            }

        }
        cout<<endl;
    }
    cout << "Score: " << score << endl;
    cout<<"Moves: "<<testInt<<endl;
}

void update_board(int x, int y, char c) {
    board_mutex.lock();
    board[y][x] = c;
    board_mutex.unlock();
}

void remove_dot(int x, int y) {
    update_board(x, y, ' ');
    score += SCORE_DOT;
}

void place_pellet(){
    board[5][5]='O';
    board[15][15]='O';
    board[4][17]='O';
}

void remove_pellet(int x, int y) {
    update_board(x, y, ' ');
    score += SCORE_PELLET;
}

void remove_ghost(int x, int y) {
    update_board(x, y, ' ');
    score += SCORE_GHOST;
}

void move_pacman(int dx, int dy) {
    testInt++;
    int new_x = pacman_x + dx;
    int new_y = pacman_y + dy;
    update_board(pacman_x, pacman_y, ' ');
    if (new_x < 0 || new_x >= BOARD_WIDTH || new_y < 0 || new_y >= BOARD_HEIGHT) {
        update_board(pacman_x, pacman_y, 'P');
        return;
    }
    char c = board[new_y][new_x];
    if (c == '#'){
       update_board(pacman_x, pacman_y, 'P');
    }
    else if (c == ' ') {
        pacman_x = new_x;
        pacman_y = new_y;
        update_board(pacman_x, pacman_y, 'P');
    } else if (c == '.') {
        remove_dot(new_x, new_y);
        pacman_x = new_x;
        pacman_y = new_y;
        update_board(pacman_x, pacman_y, 'P');
    } else if (c == 'O') {
        remove_pellet(new_x, new_y);
        pacman_x = new_x;
        pacman_y = new_y;
        // start power pellet mode
        std::thread power_pellet_thread(end_power_pellet_mode);
        power_pellet_thread.detach();
        update_board(pacman_x, pacman_y, 'P');
    } else if (c == 'G') {
        update_board(pacman_x, pacman_y, 'X');
        //print_board();
        game_over = true;
    }

}

void move_ghosts() {
    Sleep(5000);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 3);
    while (!game_over) {
        semafor.pozyskaj();
        for(auto & ghost : ghosts) {
            int dx = 0, dy = 0;
            int r = dis(gen);
            if (r == 0) {
                dx = GHOST_SPEED;
            } else if (r == 1) {
                dx = -GHOST_SPEED;
            } else if (r == 2) {
                dy = GHOST_SPEED;
            } else if (r == 3) {
                dy = -GHOST_SPEED;
            }
            int new_x = std::max(0, std::min(BOARD_WIDTH - 1, ghost.x + dx));
            int new_y = std::max(0, std::min(BOARD_HEIGHT - 1, ghost.y + dy));
            if (board[new_y][new_x] != '#') {
                update_board(new_x, new_y, 'G');
                update_board(ghost.x, ghost.y, ' ');
                ghost.x = new_x;
                ghost.y = new_y;
                if (new_x == pacman_x && new_y == pacman_y) {
                    game_over = true;
                    board[new_y][new_x]='X';
                }
            }
        }
        print_board();
        semafor.zwolnij();
        Sleep(500);
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void game_loop() {
    while (!game_over && still_points()) {

        //print_board();
        int c = _getch(); // for Windows
        semafor.pozyskaj();
        if (c == 224) { // arrow key
            c = _getch(); // for Windows
            if (c == 72) { // up
                move_pacman(0, -PACMAN_SPEED);
            } else if (c == 80) { // down
                move_pacman(0, PACMAN_SPEED);
            } else if (c == 75) { // left
                move_pacman(-PACMAN_SPEED, 0);
            } else if (c == 77) { // right
                move_pacman(PACMAN_SPEED, 0);
            }
        }
        print_board();
        semafor.zwolnij();
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (game_over)
        cout<<"Game over!"<<endl;
    if (!still_points()){
      clear_screen();
      cout<<"Game over - YOU WON!"<<endl;
    }

}

void board_reloader(){
    while(!game_over) {
        print_board();
    }
}

int main() {

    // initialize game board and score
     srand(time(NULL));
  // Generate a random integer between 0 and 2
    int random_num = (rand() % 2);
    //choosing board
    switch (random_num){
    case 0:

        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (y == 0 || y == BOARD_HEIGHT - 1 || x == 0 || x == BOARD_WIDTH - 1) {
                    board[y][x] = '#';
            }
                else {
                    board[y][x] = '.';
                //score++;
            }
        }

    }
        //adding pellet
        place_pellet();
        ghosts[0].x = BOARD_WIDTH / 2 - 1;
        ghosts[0].y = BOARD_HEIGHT / 2 - 1;
        ghosts[1].x = BOARD_WIDTH / 2 + 1;
        ghosts[1].y = BOARD_HEIGHT / 2 - 1;
        ghosts[2].x = BOARD_WIDTH / 2 - 1;
        ghosts[2].y = BOARD_HEIGHT / 2 + 1;
        ghosts[3].x = BOARD_WIDTH / 2 + 1;
        ghosts[3].y = BOARD_HEIGHT / 2 + 1;
        for (auto & ghost : ghosts)
            update_board(ghost.x, ghost.y, 'G');
        break;

    case 1:
            board[0] = "###################";
            board[1] = "#..............O..#";
            board[2] = "#.###.###.###.###.#";
            board[3] = "#.###.###.###.###.#";
            board[4] = "#.###.###.###.###.#";
            board[5] = "#.................#";
            board[6] = "#.###.###.#.###.###";
            board[7] = "#.###.###.#.###.###";
            board[8] = "#.......#.#.......#";
            board[9] = "#####.#.#...#.#####";
            board[10]= "#..O..#.#..#.....##";
            board[11]= "#####.#.#...#.#####";
            board[12]= "#.......#.#......##";
            board[13]= "#.###.###.###.#####";
            board[14]= "#.###.###.###.#####";
            board[15]= "#.........#.....O.#";
            board[16]= "#.###.###.#####.###";
            board[17]= "#.###.###.#####.###";
            board[18]= "#.............O...#";
            board[19]= "###################";
            break;
    default:
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (y == 0 || y == BOARD_HEIGHT - 1 || x == 0 || x == BOARD_WIDTH - 1) {
                    board[y][x] = '#';
            }
                else {
                    board[y][x] = '.';
                //score++;
            }
        }

    }
        //adding pellet
        place_pellet();
        ghosts[0].x = BOARD_WIDTH / 2 - 1;
        ghosts[0].y = BOARD_HEIGHT / 2 - 1;
        ghosts[1].x = BOARD_WIDTH / 2 + 1;
        ghosts[1].y = BOARD_HEIGHT / 2 - 1;
        ghosts[2].x = BOARD_WIDTH / 2 - 1;
        ghosts[2].y = BOARD_HEIGHT / 2 + 1;
        ghosts[3].x = BOARD_WIDTH / 2 + 1;
        ghosts[3].y = BOARD_HEIGHT / 2 + 1;
        for (auto & ghost : ghosts)
            update_board(ghost.x, ghost.y, 'G');
        break;
    }

    // add Pacman and ghosts to board
    pacman_x = BOARD_WIDTH / 2;
    pacman_y = BOARD_HEIGHT / 2;
    update_board(pacman_x, pacman_y, 'P');
    print_board();
    semafor.zwolnij();
    // start game loop
    std::thread pacman_thread(game_loop);

    std::thread ghost_thread(move_ghosts);

    //std::thread board_thread(board_reloader);

    pacman_thread.join();
    ghost_thread.join();
    //board_thread.join();

    getch();
    return 0;
}
