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

using namespace std;

typedef vector<vector<int>> puzzle_pieces;

//vars
int num_testcases, temp, res, n, r, c, p1, p2, p3, p4;
vector<vector<int>> testcase;
int size_temp;

//headers
void nothing () {}

void print_matrix_pieces(puzzle_pieces& pieces);
void rotate_piece_right(puzzle_pieces& pieces, int piece_index);
void rotate_piece_left(puzzle_pieces& pieces, int piece_index);


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

            puzzle_pieces pieces (n, vector<int>(4));

            for (int k = 0; k < n; k++) {
                cin >> p1 >> p2 >> p3 >> p4;
                pieces[k][0] = p1;
                pieces[k][1] = p2;
                pieces[k][2] = p3;
                pieces[k][3] = p4;
            }


            print_matrix_pieces(pieces);
            //rotate_piece_right(pieces, 0);
            //rotate_piece_left(pieces, 0);
            //print_matrix_pieces(pieces);

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

void rotate_piece_right(puzzle_pieces& pieces, int piece_index) {
    //cout << "\nROTATION RIGHT\n";
    int temp, last_index = 3;
    temp = pieces[piece_index][last_index];
    pieces[piece_index].pop_back();
    //cout << "temp= " << temp << "\n";
    pieces[piece_index].insert(pieces[piece_index].begin(), temp);
    //cout << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}

void rotate_piece_left(puzzle_pieces& pieces, int piece_index) {
    //cout << "\nROTATION LEFT\n";
    int temp;
    temp = pieces[piece_index][0];
    pieces[piece_index].erase(pieces[piece_index].begin());
    //cout << "temp= " << temp << "\n";
    pieces[piece_index].push_back(temp);
    // << pieces[piece_index][0] << " " << pieces[piece_index][1] << " " << pieces[piece_index][2] << " " << pieces[piece_index][3] << "\n";
}