/*
Rui Alexandre Tapadinhas - 2018283200
David Marcelino Mendes Palaio - 2018283864

Mooshak - sub 1003 (130 pts - Limit Time Exceeded)
*/
/*
g++ -std=c++17 -Wall -Wextra -O2 Project1_v2.cpp -lm
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define N_MAX 2500
#define R_MAX 50
#define C_MAX 50
#define COLOR_MAX 1000

typedef vector<vector<int>> puzzle_pieces;
typedef vector<vector<int>> puzzle_board;

// vars
int num_testcases, temp, res, n, r, c, p1, p2, p3, p4;
vector<vector<int>> testcase;
int size_temp;

// headers
void nothing () {}

void print_matrix_pieces(puzzle_pieces& pieces);
void print_matrix_board(puzzle_board& board, int rows, int cols);
void rotate_piece_right(puzzle_board& board, int piece_index);
void rotate_piece_left(puzzle_board& board, int piece_index);
bool check_pieces (puzzle_board& board, puzzle_pieces& pieces, int ind_piece_to_check, int size_cols, int x_pos, int y_pos, int direction);
bool solve_puzzle(puzzle_board& board, puzzle_pieces& pieces, bool* pieces_used, int x, int y, int size_row, int size_col);


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
            //vector<int> list_of_pieces; // list containing the indexes of the pieces being used on the board
            bool pieces_used[N_MAX]{false};
            int color_counter[COLOR_MAX]{0};


            for (int k = 0; k < n; k++) {
                cin >> p1 >> p2 >> p3 >> p4;
                pieces[k][0] = p1;
                pieces[k][1] = p2;
                pieces[k][2] = p3;
                pieces[k][3] = p4;
                // count colors
                color_counter[p1]++;
                color_counter[p2]++;
                color_counter[p3]++;
                color_counter[p4]++;
            }

            int counter_odd_color = 0;
            for (int color = 0; color < COLOR_MAX; color++) {
                if (color_counter[color] % 2 != 0) {
                    counter_odd_color++;
                }
            }
            if (counter_odd_color > 4) {
                cout << "impossible puzzle!" << endl;
            }
            else { // rest of the cases
                board[0] = pieces[0];
                pieces_used[0] = true;

                //cout << "TESTCASE " << i + 1 << endl << endl;

                if (c > 1) { // the board has more than 1 col in size
                    if (solve_puzzle(board, pieces, pieces_used, 1, 0, r, c) == true) {
                        //print_matrix_board(board, r, c); // already printing on solve_puzzle function
                    }
                    else {
                        cout << "impossible puzzle!" << endl;
                    }
                }
                else { // the board only has 1 col of size
                    if (solve_puzzle(board, pieces, pieces_used, 0, 1, r, c) == true) {
                        //print_matrix_board(board, r, c); // already printing on solve_puzzle function
                    }
                    else {
                        cout << "impossible puzzle!" << endl;
                    }
                }
            }

            pieces.clear();
            board.clear();
        }

        // size_temp = testcase.size();
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
    //cout << "BOARD:" << endl;
    for (int r = 0; r < rows; r++) {
        for (int c1 = 0; c1 < cols; c1++) {
            cout << board[(r * cols) + c1][0] << " ";
            cout << board[(r * cols) + c1][1];
            if (c1 != cols -1) {
                cout << "  ";
            }
        }
        cout << endl;
        for (int c2 = 0; c2 < cols; c2++) {
            cout << board[(r * cols) + c2][3] << " ";
            cout << board[(r * cols) + c2][2];
            if (c2 != cols -1) {
                cout << "  ";
            }
        }
        cout << endl;
        if (r != rows - 1) {
            cout << endl;
        }
    }
}

void rotate_piece_right(puzzle_pieces& pieces, int piece_index) {
    //cout << "\nROTATION RIGHT\n";
    int temp, last_index = 3;
    temp = pieces[piece_index][last_index];
    pieces[piece_index].pop_back();
    // cout << "temp= " << temp << "\n";
    pieces[piece_index].insert(pieces[piece_index].begin(), temp);
    // cout << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}

void rotate_piece_left(puzzle_board& board, int piece_index) {
    //cout << "\nROTATION LEFT\n";
    int temp;
    temp = board[piece_index][0];
    board[piece_index].erase(board[piece_index].begin());
    // cout << "temp= " << temp << "\n";
    board[piece_index].push_back(temp);
    // << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}

bool check_pieces (puzzle_board& board, puzzle_pieces& pieces, int ind_piece_to_check, int size_cols, int x_pos, int y_pos, int direction) {
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

    // int piece1 = (y_pos * cols) + x_pos;
    int piece1 = (y_pos * size_cols) + x_pos;
    int piece2; // = ind_piece_to_check;
    // cout << "pieces[" << ind_piece_to_check << "] = " << pieces[ind_piece_to_check][0] << " "  << pieces[ind_piece_to_check][1] << " " << pieces[ind_piece_to_check][2] << " " << pieces[ind_piece_to_check][3] << endl;

    if (direction == 0) { //left
        piece2 = piece1-1; //elem on the left of the piece1
        //cout << "board[" << piece2 << "] = " << board[piece2][0] << " "  << board[piece2][1] << " " << board[piece2][2] << " " << board[piece2][3] << endl;

        if (pieces[ind_piece_to_check][0] == board[piece2][1] && pieces[ind_piece_to_check][3] == board[piece2][2]) {
            //cout << "TRUE LEFT" << endl;
            return true;
        }
    }
    else if (direction == 1) { //up
        piece2 = piece1 - size_cols;  //elem from the line above
        //cout << "board[" << piece2 << "] = " << board[piece2][0] << " "  << board[piece2][1] << " " << board[piece2][2] << " " << board[piece2][3] << endl;

        if (pieces[ind_piece_to_check][0] == board[piece2][3] && pieces[ind_piece_to_check][1] == board[piece2][2]) {
            //cout << "TRUE UP" << endl;
            return true;
        }
    }
    else if (direction == 2) { //right
        //not needed and it's just switching the pieces and compare left (0)
    }
    else if (direction == 3) { //down
        //not needed and it's just switching the pieces and compare up (1)
    }
    //cout << "FALSE" << endl;
    return false;
}

bool solve_puzzle(puzzle_board& board, puzzle_pieces& pieces, bool* pieces_used, int x, int y, int size_row, int size_col) {
    int num_of_pieces = size_col * size_row;
    //cout << "call" << endl;
    //print_matrix_board(board, size_row, size_col);

    if (y == size_row - 1 && x == size_col -1) { // last cell of board
        // check and then finish or go back
        for (int ind_try = 0; ind_try < num_of_pieces; ind_try++) {
            if (pieces_used[ind_try] == false) {
                //cout << "last piece pre check" << endl;
                for (int rot = 0; rot < 4; rot++) {
                    //cout << "last piece: before checking piece" << endl;
                    if (size_row > 1 && size_col > 1) {
                        //cout << "size_row > 1 && size_col > 1" << endl;
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 0)) {
                            //cout << "check1" << endl;
                            if (check_pieces(board, pieces, ind_try, size_col, x, y, 1)) {
                                //cout << "check2" << endl;
                                board[(y*size_col) + x] = pieces[ind_try];
                                pieces_used[ind_try] = true;
                                print_matrix_board(board, size_row, size_col);
                                //cout << "PUZZLE SOLVED" << endl;
                                return true;
                            }
                        }
                    }
                    else if (size_row == 1 && size_col == 1) {
                        //cout << "size_row == 1 && size_col == 1" << endl;
                        print_matrix_board(board, size_row, size_col);
                        //cout << "PUZZLE SOLVED" << endl;
                        return true;
                    }
                    else if (size_row == 1) {
                        //cout << "size_row == 1" << endl;
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 0)) {
                            //cout << "check3" << endl;
                            board[y*size_col + x] = pieces[ind_try];
                            pieces_used[ind_try] = true;
                            print_matrix_board(board, size_row, size_col);
                            //cout << "PUZZLE SOLVED" << endl;
                            return true;
                        }
                    }
                    else if (size_col == 1) {
                        //cout << "size_col == 1" << endl;
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 1)) {
                            //cout << "check4" << endl;
                            board[y*size_col + x] = pieces[ind_try];
                            pieces_used[ind_try] = true;
                            print_matrix_board(board, size_row, size_col);
                            //cout << "PUZZLE SOLVED" << endl;
                            return true;
                        }
                    }
                    rotate_piece_right(pieces, ind_try);
                }
            }
        }
        //cout << "false1" << endl;
        return false;
    }
    if (y == 0) { // first line
        if (x == 0) { // first cell of board
            cout << "impossible puzzle!" << endl;
            return false;
        }
        else { // x > 0
            // check only with the piece on the left
            for (int ind_try = 0; ind_try < num_of_pieces; ind_try++) {
                if (pieces_used[ind_try] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 0)) {
                            board[y*size_col + x] = pieces[ind_try];
                            pieces_used[ind_try] = true;
                            if (x == size_col-1) { // last col in line
                                if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col) == false) {
                                    pieces_used[ind_try] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                            else { // not the last in the line
                                if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col) == false) {
                                    pieces_used[ind_try] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                        }
                        rotate_piece_right(pieces, ind_try);
                    }
                }
            }
            //cout << "false2" << endl;
            return false; // if none piece fits, returns false to try all other pieces in the previous positions
        }
    }
    else { // y > 0
        if (x == 0) { // first col of every line
            // check only with the piece above
            for (int ind_try = 0; ind_try < num_of_pieces; ind_try++) {
                if (pieces_used[ind_try] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 1)) {
                            board[y*size_col + x] = pieces[ind_try];
                            pieces_used[ind_try] = true;
                            if (x == size_col-1) { // last col in line
                                if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col) == false) {
                                    pieces_used[ind_try] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                            else {
                                if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col) == false) {
                                    pieces_used[ind_try] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                        }
                        rotate_piece_right(pieces, ind_try);
                    }
                }
            }
            //cout << "false3" << endl;
            return false;
        }
        else { // x > 0
            // check with the piece above and the piece on the left
            for (int ind_try = 0; ind_try < num_of_pieces; ind_try++) {
                if (pieces_used[ind_try] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, ind_try, size_col, x, y, 0)) {
                            if (check_pieces(board, pieces, ind_try, size_col, x, y, 1)) {
                                board[y * size_col + x] = pieces[ind_try];
                                pieces_used[ind_try] = true;
                                if (x == size_col-1) { // last col in line
                                    if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col) == false) {
                                        pieces_used[ind_try] = false;
                                    }
                                    else {
                                        return true;
                                    }
                                }
                                else {
                                    if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col) == false) {
                                        pieces_used[ind_try] = false;
                                    }
                                    else {
                                        return true;
                                    }
                                }
                            }
                        }
                        rotate_piece_right(pieces, ind_try);
                    }
                }
            }
            //cout << "false4" << endl;
            return false;
        }
    }
    //cout << "end of solve_puzzle function" << endl;
    return false;
}
