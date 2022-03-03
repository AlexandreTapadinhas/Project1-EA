/*
Rui Alexandre Tapadinhas - 2018283200
Mooshak - 
*/
/*
g++ -std=c++17 -Wall -Wextra -O2 Project1.cpp -lm
*/

#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

typedef vector<vector<int>> puzzle_pieces;
typedef vector<vector<int>> puzzle_board;

//vars
int num_testcases, temp, res, n, r, c, p1, p2, p3, p4;
vector<vector<int>> testcase;
int size_temp;

//headers
void nothing () {}

void print_matrix_pieces(puzzle_pieces& pieces);
void print_matrix_board(puzzle_board& board, int rows, int cols);
void rotate_piece_right(puzzle_board& board, int piece_index);
void rotate_piece_left(puzzle_board& board, int piece_index);
bool check_pieces (puzzle_board& board, puzzle_pieces& pieces, int piece1_board_coords[2], int piece2_board_coods[2], int side);
void solve_puzzle(puzzle_pieces& pieces, puzzle_board& board);


int main () {
    //input
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    
    while (cin >> num_testcases) {
        for (int i = 0; i < num_testcases; i++) {
            cin >> n >> r >> c;

            if (n != r*c) {
                cout << "Incorrect pieces number or board dimensions!\n";
                return -1;
            }

            puzzle_pieces pieces (n, vector<int>(4)); //matrix that stores a list of pieces in order of reading
            puzzle_board board (n, vector<int>(4)); //starts empty, pieces get copied into this list as its being solved

            for (int k = 0; k < n; k++) {
                cin >> p1 >> p2 >> p3 >> p4;
                pieces[k][0] = p1;
                pieces[k][1] = p2;
                pieces[k][2] = p3;
                pieces[k][3] = p4;
            }

            board[0] = pieces[0];

            rotate_piece_right(pieces, 0);

            print_matrix_pieces(pieces);
            print_matrix_pieces(board);
            //rotate_piece_right(pieces, 0);
            //rotate_piece_left(pieces, 0);
            //print_matrix_pieces(pieces);
            //solve_puzzle(pieces, board);

            pieces.clear();
        }

        //size_temp = testcase.size();


    }
    return 0;
}

void print_matrix_pieces(puzzle_pieces& pieces) {
    cout << "\n";
    size_temp = pieces.size();
    for (int i = 0; i < size_temp; i++) {
        cout << pieces[i][0] << " " << pieces[i][1] << " " << pieces[i][2] << " " << pieces[i][3] << "\n";
    }
    cout << "\n\n";
}

void print_matrix_board(puzzle_board& board, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c =0; c < cols; c++) {
            cout << board[r][c];
            if (c != cols -1)
                cout << " ";
        }
        cout << endl;
    }
}

void rotate_piece_right(puzzle_board& board, int piece_index) {
    //cout << "\nROTATION RIGHT\n";
    int temp, last_index = 3;
    temp = board[piece_index][last_index];
    board[piece_index].pop_back();
    //cout << "temp= " << temp << "\n";
    board[piece_index].insert(board[piece_index].begin(), temp);
    //cout << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}

void rotate_piece_left(puzzle_board& board, int piece_index) {
    //cout << "\nROTATION LEFT\n";
    int temp;
    temp = board[piece_index][0];
    board[piece_index].erase(board[piece_index].begin());
    //cout << "temp= " << temp << "\n";
    board[piece_index].push_back(temp);
    // << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}

bool check_pieces (puzzle_board& board, int cols, int x_pos, int y_pos, int direction) {
    /*
    DIRECTION:
        LEFT -> 0 
        UP -> 1
        RIGHT -> 2
        DOWN -> 3
    */

    /*
    piece1 is always the piece in the middle
    piece2 is always the piece around
            2
        2   1   2
            2
    */
    
    int piece1 = (y_pos * cols) + x_pos;
    int piece2;
    
    if (direction == 0) { //left
        piece2 = piece1-1; //elem on the left of the piece1

        if (board[piece1][0] == board[piece2][1] && board[piece1][3] == board[piece2][2]) {
            //TODO: finish comparison between sides
            return true;
        }
    }
    else if (direction == 1) { //up
        piece2 = piece1 - cols;  //elem from the line above
        if (board[piece1][0] == board[piece2][3] && board[piece1][1] == board[piece2][2]) {
            return true;
        }
    }
    else if (direction == 2) { //right
        //not needed and it's just switching the pieces and compare left (0)
    }
    else if (direction == 3) { //down
        //not needed and it's just switching the pieces and compare up (1)
    }
    return false;
}

void solve_puzzle(puzzle_pieces& pieces, puzzle_board& board){
    // not finished
    int num_of_pieces, board_size_row, board_size_col;
    cout << "pieces: " << (num_of_pieces = pieces.size()) << "\n";
    cout << "rows: " << (board_size_row = board.size()) << "\n";
    cout << "cols: " << (board_size_col = board[0].size()) << "\n";
    vector<int> list_of_pieces;



    bool found;
    print_matrix_pieces(board);

    for (int r = 0; r < board_size_row; r++) {
        for (int c = 0; c < board_size_col; c++) {
            for (int p = 0; p < num_of_pieces; p++) {
                //TODO: ifs para preencher a board
            }
        }
    }
    cout << "\nBOARD\n";
    print_matrix_board(board, board_size_row, board_size_col);
}

