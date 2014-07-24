#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
using namespace std;
#define DEFAULT_ROW 19
#define DEFAULT_COL 19
#define BLACK 'X'
#define WHITE 'O'
void PrintError(char* msg);
bool ValidateRowCol(int val);
class GoGame
{
private:
	vector<vector<char> >board;
	vector<vector<char> >board_prev;
	vector<vector<char> >board_next;
	vector<vector<char> >board_temp;
	int row;
	int col;
	int step;
	vector<pair<int, int> >steps;
public:
	GoGame(){
		GoGame(DEFAULT_ROW, DEFAULT_COL);
	}
	GoGame(int row, int col){
		int i, j;
		this->row = row;
		this->col = col;
		this->step = 0;
		vector<char> tmp;
		for (i=0; i<row; i++){
			this->board.push_back(tmp);
			for (j=0; j<col; j++){
				this->board[i].push_back(' ');
			}
		}
		showBoard();
		// Start game
		while (1){
			if (isBlack()) printf("Player (Black): ");
			else printf("Player (White): ");
			int row, iRow, col;
			char cCol;
			scanf("%d, %c", &iRow, &cCol); // Input 0 to surrender
			row = iRow - 1;
			col = cCol - 'a';
			if (row == -1){
				// save steps to file and etc...
				printf("%s Win!\n", isBlack() ? "White" : "Black");
				break;
			}
			if (!isValidRow(row) || !isValidCol(col) || !isValidPos(row, col)){
				printf("(%d, %c) is an invalid row/column/position!\n", iRow, cCol);
				continue;
			}
			board_next = board; 
			// Put the stone on next temp board
			board_next[row][col] = isBlack() ? 'X' : 'O';
			// Calculate eaten stones
			processBoard(row, col);
			if (this->step > 3 && isSameBoard()){
				printf("Cannot do this! Put the stone in somewhere else!\n");
				continue;
			}
			board_prev = board;
			board = board_next;
			
			this->step ++;
			showBoard();
		}
	}
	bool isSameBoard(){
		int i, j, row = this->row, col = this->col;
		for (i=0; i<row; i++)
			for (j=0; j<col; j++)
				if (this->board_prev[i][j] != this->board_next[i][j]) return false;
		return true;
	}
	void processBoard(int r, int c){
		#define B this->board_next
		int i, j, row = this->row, col = this->col;
		vector<pair<int, int> > stones;
		char my_color, op_color;
		my_color = B[r][c];
		op_color = my_color == BLACK ? WHITE : BLACK;
		// Check up for oppo stones 
		if (r != 0 && B[r-1][c] == op_color) if (isStoneDead(r-1, c)) removeStones(r-1, c);
		// Check down for oppo stones 
		if (r != row-1 && B[r+1][c] == op_color) if (isStoneDead(r+1, c)) removeStones(r+1, c);
		// Check left for oppo stones 
		if (c != 0 && B[r][c-1] == op_color) if (isStoneDead(r, c-1)) removeStones(r, c-1);
		// Check right for oppo stones 
		if (c != col-1 && B[r][c+1] == op_color) if (isStoneDead(r, c+1)) removeStones(r, c+1);
		
		// Check myself on this position
		if (isStoneDead(r, c)) removeStones(r, c);
	}
	
	void removeStones(int r, int c){
		printf("Removing stone %d, %d...\n", r, c);
		#define B this->board_next
		int i, j, row = this->row, col = this->col;
		char my_color, op_color;
		my_color = B[r][c];
		op_color = my_color == BLACK ? WHITE : BLACK;
		stack<pair<int, int> > stones;
		stones.push(make_pair(r, c));
		while (!stones.empty()){
			int r1 = stones.top().first;
			int c1 = stones.top().second;
			int r2, c2;
			char color;
			stones.pop();
			B[r1][c1] = ' ';
			if (r1 != 0){
				r2 = r1-1; c2 = c1;
				color = B[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
				}
			}
			if (r1 != row-1){
				r2 = r1+1; c2 = c1;
				color = B[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
				}
			}
			if (c1 != 0){
				r2 = r1; c2 = c1-1;
				color = B[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
				}
			}
			if (c1 != col-1){
				r2 = r1; c2 = c1+1;
				color = B[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
				}
			}					
		}
	}
	
	bool isStoneDead(int r, int c){		
		#define B this->board_next
		#define T this->board_temp
		int i, j, row = this->row, col = this->col;
		char my_color, op_color;
		my_color = B[r][c];
		op_color = my_color == BLACK ? WHITE : BLACK;
		
		stack<pair<int, int> > stones;
		T = B;
		bool alive = false;
		stones.push(make_pair(r, c));
		T[r][c] = '*'; // Marked with '*' means searched area
		while (!stones.empty() && !alive){
			int r1 = stones.top().first;
			int c1 = stones.top().second;
			int r2, c2;
			char color;
			stones.pop();
			if (r1 != 0){
				r2 = r1-1; c2 = c1;
				color = T[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
					T[r2][c2] = '*';
				}
				if (color == ' '){
					alive = true;
					break;
				}
			}
			if (r1 != row-1){
				r2 = r1+1; c2 = c1;
				color = T[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
					T[r2][c2] = '*';
				}
				if (color == ' '){
					alive = true;
					break;
				}
			}
			if (c1 != 0){
				r2 = r1; c2 = c1-1;
				color = T[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
					T[r2][c2] = '*';
				}
				if (color == ' '){
					alive = true;
					break;
				}
			}
			if (c1 != col-1){
				r2 = r1; c2 = c1+1;
				color = T[r2][c2];
				if (color == my_color){
					stones.push(make_pair(r2, c2));
					T[r2][c2] = '*';
				}
				if (color == ' '){
					alive = true;
					break;
				}
			}			
		}
		printf("Stone %d, %d is %s.\n", r, c, alive ? "alive" : "dead");
		return !alive;
	}
	bool isValidRow(int row){
		return row >= 0 && row < this->row;
	}
	bool isValidCol(int col){
		return col >= 0 && col < this->col;
	}
	bool isValidPos(int row, int col){
		return this->board[row][col] == ' ';
	}
	void showBoard(){
		int i, j, row = this->row, col = this->col;
		printf("   ");
		for (i=0; i<col; i++){
			printf("%c ", 'a'+i);
		}
		printf("\n");
		for (i=0; i<row; i++){
			printf("%2d ", i+1);
			for (j=0; j<col; j++){
				printf("%c ", this->board[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
	bool isBlack(){
		return !(this->step&1);
	}
};
int main(int argc, char** argv)
{
	if (argc != 1 && argc != 3){
		PrintError("Wrong Arguments!\n");
		exit(0);
	}
	else if (argc == 1){
		GoGame* game1 = new GoGame();
	}
	else if (argc == 3){
		int row = atoi(argv[1]);
		int col = atoi(argv[2]);
		if (!ValidateRowCol(row) || !ValidateRowCol(col)){
			PrintError("Wrong Arguments!\n");
		}
		GoGame* game1 = new GoGame(row, col);
	}
	return 1;
}
void PrintError(char* msg){
	printf("Error: %s\n", msg);
}
bool ValidateRowCol(int val){
	return val >=3 && val <= 26;
}

