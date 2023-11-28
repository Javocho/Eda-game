#include "Player.hh"
#include <vector>
#include <list>
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Presi


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
	typedef vector<vector<char>> Matrix;
	map<int, queue<Dir>> pioneerPaths;

	
	void draw_table() {
		Matrix board(80, vector<char>(60, '-'));
		for (int i = 0; i < 80; i++) {
			for (int j = 0; j < 40; j++) {
				Cell c1 = cell(i,j,0);
				if (c1.type == Elevator) {
					board[i][j] = 'E';
				}
				else if (c1.type == Rock)
					board[i][j] == 'R';
			}
		}
	}

	Dir getDirection(const Pos& originalPos, const Pos& newPos) {
		if (originalPos.i > newPos.i) return Left;
		if (originalPos.i < newPos.i) return Right;
		if (originalPos.j > newPos.j) return Down;
		if (originalPos.j < newPos.j) return Top;
		return None;
	}

	// Function to check if a cell at the given position is passable.
	bool isPassable(const Pos& p) {
   		Cell c = cell(p);
		return  (pos_ok(p) && c.type != Rock);
		return false;
	}
	Pos updatePos(const Pos& pos, int newValue, int axis) {
		if (axis == 0) return Pos(newValue, pos.j, pos.k);
		else if (axis == 1) return Pos(pos.i, newValue, pos.k);
		else if (axis == 2) return Pos(pos.i, pos.j, newValue);
		return pos;
	}

	queue<Dir> nearElevator(const Pos& pos) {
		queue<Pos> Q;
 		queue<Dir> Qdir;
		vector<vector<bool>> visited(40, vector<bool>(80, false));
		Q.push(pos);
		visited[pos.i][pos.j] = true;

		while (not Q.empty()) {
			Pos current = Q.front();
			Cell c = cell(current);
			if (c.type == Elevator) return Qdir;
			Q.pop();
			
			/*vector<Pos> neighbors = {
            	updatePos(current, current.i + 1, 0), //right
				updatePos(current, current.j + 1, 1), //up
				updatePos(current, current.i - 1, 0), //left
				updatePos(current, current.j - 1, 1), //down
       		 };*/

        	/*for (Pos neighbor : neighbors) {
            	if (isPassable(neighbor) && not visited[neighbor.i][neighbor.j]) {
               		Q.push(neighbor);
                	visited[neighbor.i][neighbor.j] = true;
            	}
        	}*/
			Pos p = updatePos(current, current.i + 1, 0); //right
			if (isPassable(p) && not visited[p.i][p.j]) {
               		Q.push(p);
                	visited[p.i][p.j] = true;
					Qdir.push(Right);
			}
			cerr << "hola\n";

			p = updatePos(current, current.j + 1, 1); //up
			if (isPassable(p) && not visited[p.i][p.j]) {
               		Q.push(p);
                	visited[p.i][p.j] = true;
					Qdir.push(Top);
			}
			cerr << "qu-e\n";
			p = updatePos(current, current.i - 1, 0); //left
			if (isPassable(p) && not visited[p.i][p.j]) {
               		Q.push(p);
                	visited[p.i][p.j] = true;
					Qdir.push(Left);
			}
			cout << "tail";
			p = updatePos(current, current.j - 1, 1); //down
			if (isPassable(p) && not visited[p.i][p.j]) {
               		Q.push(p);
                	visited[p.i][p.j] = true;
					Qdir.push(Down);
			}

		}
		return Qdir;
	}

	list<Pos> nearGem(const Pos& pos) {
		queue<Pos> Q;
		list<Pos> Lpos;	
		vector<vector<bool>> visited(40, vector<bool>(80, false));
		Q.push(pos);
		visited[pos.i][pos.j] = true;

		while (not Q.empty()) {
			Pos current = Q.front();
			Cell c = cell(current);
			if (c.gem) return Lpos;
			Q.pop();
			Lpos.push_back(current);
			
			//updatePos(pos, pos.i + 1, 0);
			vector<Pos> neighbors = {
            	updatePos(current, current.i + 1, 0), //right
				updatePos(current, current.j + 1, 1), //up
				updatePos(current, current.i - 1, 0), //left
				updatePos(current, current.j - 1, 1), //down
       		 };

        	for (Pos neighbor : neighbors) {
            	if (isPassable(neighbor) && not visited[neighbor.i][neighbor.j]) {
               		Q.push(neighbor);
                	visited[neighbor.i][neighbor.j] = true;
            	}
        	}
		}
		Lpos.pop_front(); //borra primer elemento pos actual
		return Lpos;
	}

	 void printList(const list<Pos>& positions) {
        cerr << "List of Positions: ";
        for (const Pos& p : positions) {
            cerr << "(" << p.i << ", " << p.j << ", " << p.k << ") ";
        }
        cerr << endl;
    }

	void move_pioneers() {
		vector<int> P = pioneers(me());
		for (int id : P) {
			cerr << "id ";
			Unit pioneer = unit(id);
			if (pioneerPaths.find(id) == pioneerPaths.end()) {
				queue<Dir> lpath = nearElevator(pioneer.pos);
			}
			queue<Dir> pionPath = pioneerPaths.find(id)->second;
			//printList(lpos);
			if (pioneer.pos.k == 0) {
				//cerr << getDirection(pioneer.pos, lpos.front()) << endl;
				command(id, pionPath.front());
				pionPath.pop();
			}
			/*else {
				if (lpos.empty()) lpos = nearGem(pioneer.pos);
				else lpos.pop_front();
				command(id, getDirection(pioneer.pos, lpos.front()));
			}*/
		}
	}

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
	move_pioneers();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
