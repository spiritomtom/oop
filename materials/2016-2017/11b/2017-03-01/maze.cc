#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

enum Direction {
	NONE = 0,
	UP = 1,
	LEFT = 1<<1,
	DOWN = 1<<2,
	RIGHT = 1 << 3
};
//     0000
//     0001 UP
//     0011 UP | LEFT

class Cell {
	friend class Board;
	const static int SIZE = 20;
	
	unsigned walls_;
	unsigned row_;
	unsigned col_;
	bool visitted_;
	
	const char* draw_wall(bool has_wall) const {
		return has_wall ? "rlineto": "rmoveto";
	}
	void draw(ostream& out) const {
		out << SIZE*(1+col()) << " " << SIZE*(1+row()) << " moveto" << endl;
		out << SIZE << " " << 0 << " " << draw_wall(has_wall(DOWN)) << endl;
		out << 0 << " " << SIZE << " " << draw_wall(has_wall(RIGHT)) << endl;
		out << -SIZE << " " << 0 << " " << draw_wall(has_wall(UP)) << endl;
		out << 0 << " " << -SIZE << " " << draw_wall(has_wall(LEFT)) << endl;
	}

public:
	Cell(unsigned r, unsigned c,
		unsigned walls=UP|LEFT|DOWN|RIGHT)
	: row_(r), col_(c), walls_(walls), visitted_(false)
	{}
	
	/*
		walls_: 01111
		dir:    00010
		&:      00010
		
		walls_: 00101
		dir:    00010
		&:      00000
	*/
	bool has_wall(Direction dir) const {
		return walls_ & dir;
	}
	
	/*
		walls_: 00100
		dir:    00010
		|:      00110
	*/
	Cell& set_wall(Direction dir) {
		walls_ |= dir;
		return *this;
	}
	/*
		walls_: 00101
		dir:    00100
		~dir:   11011
		&:      00001
		
		res:    00001
	*/
	Cell& unset_wall(Direction dir) {
		walls_ &= ~dir;
		return *this;
	}
	
	unsigned row() const {
		return row_;
	}
	
	unsigned col() const {
		return col_;
	}
	
	bool visitted() const {
		return visitted_;
	}
	
	Cell& mark_visitted() {
		visitted_ = true;
		return *this;
	}
};

ostream& operator<<(ostream& out, const Cell& cell) {
	out << "Cell(" << cell.row() << ", " << cell.col() << ")";
	return out;
}

class BoardError {};

class Board {
	unsigned width_;
	unsigned height_;
	vector<Cell> cells_;
	
	unsigned index(unsigned row, unsigned col) const {
		if(row >= width() || col >= height()) {
			throw BoardError();
		}
		return row * width() + col;
	}
	
public:
	
	Board(unsigned w, unsigned h)
	: width_(w), height_(h)
	{
		for(unsigned row=0; row<height(); ++row) {
			for(unsigned col=0; col<width(); ++col) {
				cells_.push_back(Cell(row,col));
			}
		}	
	}
	
	unsigned height() const {
		return height_;
	}
	
	unsigned width() const {
		return width_;
	}
	
	Cell& at(unsigned row, unsigned col) {
		return cells_[index(row, col)];
	}
	
	const Cell& at(unsigned row, unsigned col) const {
		return cells_[index(row, col)];
	}
	
	Cell& neighbour(const Cell& cell, Direction dir) {
		return cells_[nindex(cell.row(), cell.col(), dir)];
	}
	const Cell& neighbour(const Cell& cell, Direction dir) const {
		return cells_[nindex(cell.row(), cell.col(), dir)];	
	}
	
private:
	unsigned nindex(unsigned row, unsigned col, Direction dir) const {
		int nrow = dir==UP? row + 1: (dir==DOWN?row - 1: row);
		int ncol = dir==LEFT? col - 1: (dir==RIGHT? col + 1: col);
		if(nrow <0 || nrow >= height() || ncol < 0 || ncol >= width()) {
			throw BoardError();
		}
		return index(nrow, ncol);
	}
public:

	Cell& drill(Cell& cell, Direction dir) {
		Cell& ncell = neighbour(cell, dir);

		cell.unset_wall(dir);
		Direction oposite = oposite_direction(dir);
		ncell.unset_wall(oposite);
		
		return ncell;
	}

	static Direction oposite_direction(Direction dir) {
		switch(dir) {
		case UP:
			return DOWN;
		case DOWN:
			return UP;
		case LEFT:
			return RIGHT;
		case RIGHT:
			return LEFT;
		default:
			throw BoardError();
		}
	}

	void draw(ostream& out) const {
		out << "newpath" << endl;
		for(unsigned row=0; row < height(); ++row) {
			for(unsigned col=0; col < width(); ++ col) {
				at(row, col).draw(out);
			}
		}
		out << "1 setlinewidth" << endl;
		out << "stroke" << endl;
	}
	
	bool has_neighbour(const Cell& cell, Direction dir) const {
		if(dir == DOWN && cell.row() == 0) {
			return false;
		}
		if(dir == UP && cell.row() == height() -1 ) {
			return false;
		}
		if(dir == LEFT && cell.col() == 0) {
			return false;
		}
		if(dir == RIGHT && cell.col() == width() - 1) {
			return false;
		}
		return true;
	}
private:
	const static Direction DIRECTIONS[]; // = {UP,DOWN,LEFT,RIGHT};
public:
	bool has_unvisitted_neighbour(const Cell& cell) const {
		for(int i = 0; i<4; ++i) { 
			Direction dir=DIRECTIONS[i];
			if(has_neighbour(cell, dir)) {
				const Cell& ncell = neighbour(cell, dir);
				if(!ncell.visitted()) {
					return true;
				}
			}
		}
		return false;
	}
	
	Direction get_random_unvisitted_direction(const Cell& cell) {
		if(!has_unvisitted_neighbour(cell)) {
			throw BoardError();
		}
		while(true) {
			unsigned dindex = rand() % 4;
			Direction dir = DIRECTIONS[dindex];
			if(has_neighbour(cell, dir)) {
				Cell& ncell = neighbour(cell, dir);
				if(!ncell.visitted()) {
					return dir;
				}
			}
		}
	}
	
	void generate_maze(Cell& cell) {
		cell.mark_visitted();
		while(true) {
			if(!has_unvisitted_neighbour(cell)) {
				return;
			}
			Direction dir = get_random_unvisitted_direction(cell);
			Cell& ncell = drill(cell, dir);
			generate_maze(ncell);
		}
	}
};

const Direction Board::DIRECTIONS[] = {UP,DOWN,LEFT,RIGHT};


int main() {
	Board b(25, 25);
	Cell& cell = b.at(0,0);
	b.generate_maze(cell);
	
	b.draw(cout);
	
	return 0;
}

