/*
Rui Alexandre Tapadinhas - 2018283200
David Marcelino Mendes Palaio - 2018283864

Mooshak:
sub 1003 (130 pts - Limit Time Exceeded) without using unordered map
sub 1341 (100 pts - Limit Time Exceeded) using unordered map for sides
sub 1478 (70 pts - Limit Time Exceeded) using unordered map for sides and corners
*/
/*
g++ -std=c++17 -Wall -Wextra -O2 Project1_v2.cpp -lm
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <chrono>



using namespace std;
using namespace std::chrono;

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
vector<int> intersection(vector<int> &v1, vector<int> &v2);
bool check_pieces (puzzle_board& board, puzzle_pieces& pieces, int ind_piece_to_check, int size_cols, int x_pos, int y_pos, int direction);
bool solve_puzzle(puzzle_board& board, puzzle_pieces& pieces, bool* pieces_used, int x, int y, int size_row, int size_col, unordered_map<int, vector<int>> sides);


int main () {
    //input
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    //auto start = high_resolution_clock::now();

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
            unordered_map<int, vector<int>> sides; // example sides[int(color 1 * 1000 + color 2)] = {piece_index, rotation} : sides[1002] = {0, 0}


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
                // add sides to map
                /*if (k > 0) {
                    //vector<int> aux;
                    //for (int r = 0; r < 4; r++) {
                        //aux.push_back(k);
                        //aux.push_back(r);
                        sides[pieces[k][0]*1000+pieces[k][1]].push_back(k);
                        sides[pieces[k][1]*1000+pieces[k][2]].push_back(k);
                        sides[pieces[k][2]*1000+pieces[k][3]].push_back(k);
                        sides[pieces[k][3]*1000+pieces[k][0]].push_back(k);
                        // corners
                        sides[pieces[k][3]*1000000+pieces[k][0]*1000+pieces[k][1]].push_back(k);
                        sides[pieces[k][0]*1000000+pieces[k][1]*1000+pieces[k][2]].push_back(k);
                        sides[pieces[k][1]*1000000+pieces[k][2]*1000+pieces[k][3]].push_back(k);
                        sides[pieces[k][2]*1000000+pieces[k][3]*1000+pieces[k][0]].push_back(k);
                        //rotate_piece_right(pieces, k);
                        //aux.clear();
                    //}
                }*/
            }
            // count colors
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
                // add sides and corners to unordered map
                auto start = high_resolution_clock::now();
                for (int p = 1; p < n; p++) {
                    // sides
                    sides[pieces[p][0]*1000+pieces[p][1]].push_back(p);
                    sides[pieces[p][1]*1000+pieces[p][2]].push_back(p);
                    sides[pieces[p][2]*1000+pieces[p][3]].push_back(p);
                    sides[pieces[p][3]*1000+pieces[p][0]].push_back(p);
                    // corners
                    sides[pieces[p][3]*1000000+pieces[p][0]*1000+pieces[p][1]].push_back(p);
                    sides[pieces[p][0]*1000000+pieces[p][1]*1000+pieces[p][2]].push_back(p);
                    sides[pieces[p][1]*1000000+pieces[p][2]*1000+pieces[p][3]].push_back(p);
                    sides[pieces[p][2]*1000000+pieces[p][3]*1000+pieces[p][0]].push_back(p);
                }
                auto stop = high_resolution_clock::now(); 
                auto duration = duration_cast<microseconds>(stop - start); 

                cout << "store on unordered map: " << duration.count() << endl; 
                // print unordered map
                /*for (const pair<int, vector<int>>& tuple : sides) {
                    cout << "side: " << tuple.first << "\t";
                    for (int p = 0; p < (int)tuple.second.size(); p++) {
                        cout << "piece: " << tuple.second[p] << endl;// "\t" << "rotation: " << tuple.second[p][1] << endl;
                        if (p != (int)tuple.second.size()-1)
                            cout << "\t\t";
                    }
                }*/

                board[0] = pieces[0];
                pieces_used[0] = true;

                //cout << "TESTCASE " << i + 1 << endl << endl;

                if (c > 1) { // the board has more than 1 col in size
                    if (solve_puzzle(board, pieces, pieces_used, 1, 0, r, c, sides) == true) {
                        //print_matrix_board(board, r, c); // already printing on solve_puzzle function
                    }
                    else {
                        cout << "impossible puzzle!" << endl;
                    }
                }
                else { // the board only has 1 col of size
                    if (solve_puzzle(board, pieces, pieces_used, 0, 1, r, c, sides) == true) {
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
        /*auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start); 

        cout << "input file time: " << duration.count() << endl; */


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

vector<int> intersection(vector<int> &v1, vector<int> &v2){
    vector<int> v3;
    //auto start = high_resolution_clock::now(); 

    sort(v1.begin(), v1.end());
    // print list 1
    /*cout << "list 1: ";
    for (int i1 = 0; i1 < (int) v1.size(); i1++) {
        cout << v1[i1] << " ";
    }
    cout << endl;*/

    sort(v2.begin(), v2.end());

    // print list 2
    /*cout << "list 2: ";
    for (int i2 = 0; i2 < (int) v2.size(); i2++) {
        cout << v2[i2] << " ";
    }
    cout << endl;*/

    set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));

    // print interception list
    /*cout << "list 1 <> list 2: ";
    for (int i3 = 0; i3 < (int) v3.size(); i3++) {
        cout << v3[i3] << " ";
    }
    cout << endl;*/

    /*auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "intersection time: " << duration.count() << endl;*/

    return v3;
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

bool solve_puzzle(puzzle_board& board, puzzle_pieces& pieces, bool* pieces_used, int x, int y, int size_row, int size_col, unordered_map<int, vector<int>> sides)  {
    //int num_of_pieces = size_col * size_row; // not used
    cout << "call" << endl;
    cout << "x = " << x << "\ty = " << y << endl;
    //print_matrix_board(board, size_row, size_col);
    //vector<int> cur_piece = board[(y*size_col) + x]; // NOT USED

    if (y == size_row - 1 && x == size_col -1) { // last cell of board
        // check and then finish or go back
        // check with the surrounding pieces

        vector<int> possible_pieces;

        vector<int> piece_above; // = board[((y-1) * size_col) + x];
        //vector<int> inds_matching_side_above = sides[piece_above[3]*1000 + piece_above[2]];
        //cout << "above: " <<  inds_matching_side_above.size() << endl;

        vector<int> piece_previous; // = board[(y * size_col) + x - 1];
        //vector<int> inds_matching_side_previous = sides[piece_previous[1]*1000 + piece_previous[2]]; // index wrong probably
        //cout << "previous: " << inds_matching_side_previous.size() << endl;

        
        if (size_row > 1 && size_col > 1) {
            //cout << "size_row > 1 && size_col > 1" << endl;
            piece_above = board[((y-1) * size_col) + x];
            piece_previous = board[(y * size_col) + x - 1];
            vector<int> inds_matching_side_above = sides[piece_above[3]*1000 + piece_above[2]];
            vector<int> inds_matching_side_previous = sides[piece_previous[2]*1000 + piece_previous[1]];
            possible_pieces = intersection(inds_matching_side_above, inds_matching_side_previous);
        }
        else if (size_row == 1 && size_col == 1) {
            print_matrix_board(board, size_row, size_col);
            //cout << "PUZZLE SOLVED" << endl;
            return true;
        }
        else if (size_row == 1) {
            piece_previous = board[(y * size_col) + x - 1];
            vector<int> inds_matching_side_previous = sides[piece_previous[2]*1000 + piece_previous[1]];
            possible_pieces = inds_matching_side_previous;
        }
        else if (size_col == 1) {
            piece_above = board[((y-1) * size_col) + x];
            vector<int> inds_matching_side_above = sides[piece_above[3]*1000 + piece_above[2]];
            possible_pieces = inds_matching_side_above;
        }

        for (int ind_try = 0; ind_try < (int)possible_pieces.size(); ind_try++) {
            if (pieces_used[possible_pieces[ind_try]] == false) {
                //cout << "last piece pre check" << endl;
                for (int rot = 0; rot < 4; rot++) {
                    //cout << "last piece: before checking piece" << endl;
                    if (size_row > 1 && size_col > 1) {
                        //cout << "size_row > 1 && size_col > 1" << endl;
                        if (check_pieces(board, pieces, possible_pieces[ind_try], size_col, x, y, 0)) {
                            //cout << "check1" << endl;
                            if (check_pieces(board, pieces, possible_pieces[ind_try], size_col, x, y, 1)) {
                                //cout << "check2" << endl;
                                board[(y*size_col) + x] = pieces[possible_pieces[ind_try]];
                                pieces_used[possible_pieces[ind_try]] = true;
                                print_matrix_board(board, size_row, size_col);
                                //cout << "PUZZLE SOLVED" << endl;
                                return true;
                            }
                        }
                    }
                    // ERASE: else if condition not needed, copied up (before for (ind_try))
                    /*else if (size_row == 1 && size_col == 1) {
                        //cout << "size_row == 1 && size_col == 1" << endl;
                        print_matrix_board(board, size_row, size_col);
                        //cout << "PUZZLE SOLVED" << endl;
                        return true;
                    }*/
                    else if (size_row == 1) {
                        //cout << "size_row == 1" << endl;
                        if (check_pieces(board, pieces, possible_pieces[ind_try], size_col, x, y, 0)) {
                            //cout << "check3" << endl;
                            board[y*size_col + x] = pieces[possible_pieces[ind_try]];
                            pieces_used[possible_pieces[ind_try]] = true;
                            print_matrix_board(board, size_row, size_col);
                            //cout << "PUZZLE SOLVED" << endl;
                            return true;
                        }
                    }
                    else if (size_col == 1) {
                        //cout << "size_col == 1" << endl;
                        if (check_pieces(board, pieces, possible_pieces[ind_try], size_col, x, y, 1)) {
                            //cout << "check4" << endl;
                            board[y*size_col + x] = pieces[possible_pieces[ind_try]];
                            pieces_used[possible_pieces[ind_try]] = true;
                            print_matrix_board(board, size_row, size_col);
                            //cout << "PUZZLE SOLVED" << endl;
                            return true;
                        }
                    }
                    rotate_piece_right(pieces, possible_pieces[ind_try]);
                }
            }
        }
        //cout << "false1" << endl;
        return false;
    }
    if (y == 0) { // first line 

        //only where the side appears
        vector<int> piece_previous = board[(y * size_col) + x - 1];
        vector<int> inds_matching_side_previous = sides[piece_previous[2]*1000 + piece_previous[1]];

        if (x == 0) { // first cell of board
            cout << "impossible puzzle!" << endl;
            return false;
        }
        else { // x > 0
            // check only with the piece on the left
            for (int ind_try = 0; ind_try < (int)inds_matching_side_previous.size(); ind_try++) {
                if (pieces_used[inds_matching_side_previous[ind_try]] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, inds_matching_side_previous[ind_try], size_col, x, y, 0)) {
                            board[y*size_col + x] = pieces[inds_matching_side_previous[ind_try]];
                            pieces_used[inds_matching_side_previous[ind_try]] = true;
                            if (x == size_col-1) { // last col in line
                                if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col, sides) == false) {
                                    pieces_used[inds_matching_side_previous[ind_try]] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                            else { // not the last in the line
                                if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col, sides) == false) {
                                    pieces_used[inds_matching_side_previous[ind_try]] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                        }
                        rotate_piece_right(pieces, inds_matching_side_previous[ind_try]);
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
            vector<int> piece_above = board[((y-1) * size_col) + x];
            vector<int> inds_matching_side = sides[piece_above[3]*1000 + piece_above[2]];
            //cout << inds_matching_side.size() << endl;
            // print list of pieces that has the side pretended
            /*for (int inds = 0; inds < (int)inds_matching_side.size(); inds++) {
                cout << inds_matching_side[inds] << " ";
                if (inds == (int)inds_matching_side.size() - 1) {
                    cout << endl;
                }
            }*/
            for (int ind_try = 0; ind_try < (int)inds_matching_side.size(); ind_try++) {
                if (pieces_used[inds_matching_side[ind_try]] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, inds_matching_side[ind_try], size_col, x, y, 1)) {
                            board[y*size_col + x] = pieces[inds_matching_side[ind_try]];
                            pieces_used[inds_matching_side[ind_try]] = true;
                            if (x == size_col-1) { // last col in line
                                if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col, sides) == false) {
                                    pieces_used[inds_matching_side[ind_try]] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                            else {
                                if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col, sides) == false) {
                                    pieces_used[inds_matching_side[ind_try]] = false;
                                }
                                else {
                                    return true;
                                }
                            }
                        }
                        rotate_piece_right(pieces, inds_matching_side[ind_try]);
                    }
                }
            }
            //cout << "false3" << endl;
            return false;
        }
        else { // x > 0
            // check with the piece above and the piece on the left
            // TODO: NOT TESTE
            vector<int> piece_above = board[((y-1) * size_col) + x];
            //vector<int> inds_matching_side_above = sides[piece_above[3]*1000 + piece_above[2]];
            //cout << "above: " <<  inds_matching_side_above.size() << endl;

            vector<int> piece_previous = board[(y * size_col) + x - 1];
            //vector<int> inds_matching_side_previous = sides[piece_previous[2]*1000 + piece_previous[1]];
            //cout << "previous: " << inds_matching_side_previous.size() << endl;

            //vector<int> possible_pieces_interseption = intersection(inds_matching_side_above, inds_matching_side_previous);

            vector<int> possible_pieces_corner = sides[piece_previous[2]*1000000 + piece_previous[1]*1000 + piece_above[2]];
            // print list of pieces that has the side pretended
            /*for (int inds = 0; inds < (int)inds_matching_side.size(); inds++) {
                cout << inds_matching_side[inds] << " ";
                if (inds == (int)inds_matching_side.size() - 1) {
                    cout << endl;
                }
            }*/
            for (int ind_try = 0; ind_try < (int) possible_pieces_corner.size(); ind_try++) {
                if (pieces_used[possible_pieces_corner[ind_try]] == false) {
                    for (int rot = 0; rot < 4; rot++) {
                        if (check_pieces(board, pieces, possible_pieces_corner[ind_try], size_col, x, y, 0)) {
                            if (check_pieces(board, pieces, possible_pieces_corner[ind_try], size_col, x, y, 1)) {
                                board[y * size_col + x] = pieces[possible_pieces_corner[ind_try]];
                                pieces_used[possible_pieces_corner[ind_try]] = true;
                                if (x == size_col-1) { // last col in line
                                    if (solve_puzzle(board, pieces, pieces_used, 0, y+1, size_row, size_col, sides) == false) {
                                        pieces_used[possible_pieces_corner[ind_try]] = false;
                                    }
                                    else {
                                        return true;
                                    }
                                }
                                else {
                                    if (solve_puzzle(board, pieces, pieces_used, x+1, y, size_row, size_col, sides) == false) {
                                        pieces_used[possible_pieces_corner[ind_try]] = false;
                                    }
                                    else {
                                        return true;
                                    }
                                }
                            }
                        }
                        rotate_piece_right(pieces, possible_pieces_corner[ind_try]);
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
