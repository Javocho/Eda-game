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
	
	// Function to check if a cell at the given position is passable.
	bool isPassable(const Pos& p) {
   		Cell c = cell(p);
		return  (pos_ok(p) && c.type != Rock);
		return false;
	}

	int distance (Pos p1, Pos p2) {
		return sqrt(pow(p2.i-p1.i,2)+pow(p1.j-p2.j,2));
	}

	bool imBeingStalked(Pos p, Unit &u) {
		vector<int> H = hellhounds();

		for (int id : H) {
			u = unit(id);
			if (distance(p, u.pos) < 6)
			{
				cerr << distance(p, u.pos);
				return true;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (i == me()) continue;
			vector<int> F = furyans(i);
			for (int id : F) {
				u = unit(id);
				if (distance(u.pos, p) < 4) return true;
			}
		}
		return false;
	}

	Dir huir(Pos InitialPos, Pos enemyPos) {
		priority_queue<pair<int, Dir>> prio;
		prio.push(make_pair(0, None));
		for (int i = 0; i < DirSize - 3; i++) {
			Dir dir = static_cast<Dir>(i);
			Pos newPos = InitialPos + dir; 
			Cell c1 = cell(newPos);
			if (isPassable(newPos) and c1.id == -1)
				prio.push(make_pair(distance(newPos, enemyPos), dir));
		}
		cerr << "huir " << prio.top().second << endl;
		return prio.top().second;
	}

	 void printList(const list<Pos>& positions) {
        cerr << "List of Positions: ";
        for (const Pos& p : positions) {
            cerr << "(" << p.i << ", " << p.j << ", " << p.k << ") ";
        }
        cerr << endl;
    }

	int enemy_near(const Pos &newPos) {
		int enemies = 0;
		for (int i = 0; i < DirSize - 3; i++) {
			Dir currentDir = static_cast<Dir>(i);
			Pos p = newPos + currentDir;
			Cell c = cell(p);
			if (c.id != -1 and c.id != me())
				enemies--;
		}
		return enemies;
	}

	Dir best_dir(const Pos& posInitial, const Pos& posFinal) {
		Dir best = None;
		int dist = distance(posInitial, posFinal);
		for (int i = 0; i < DirSize - 3; ++i) {
            Dir currentDir = static_cast<Dir>(i);
			Pos ptmp = posInitial+currentDir;
			if (isPassable(ptmp) and distance(ptmp, posFinal) < dist) {
				dist = distance(ptmp, posFinal);
				best = currentDir;
			} 
		}
		return best;
	}

	Pos invadeCells(Pos p) {
		set<Pos> visited;
		queue<Pos> Q;
		Q.push(p);
		while (not Q.empty()) {
			Pos currentPos = Q.front();
			Q.pop();
			Cell c = cell(currentPos);
			if (isPassable(currentPos) and c.id == -1 and c.owner != me() and c.type != Elevator)
				return currentPos;
			for (int i = 0; i < DirSize - 3; ++i) {
            	Dir currentDir = static_cast<Dir>(i);
            	Pos neighborPos = currentPos + currentDir;

            	if (isPassable(neighborPos) and visited.find(neighborPos) == visited.end()) {
                	visited.insert(neighborPos);
                	Q.push(neighborPos);
            	}
        	}
		}
		return p;
	}

	void move_pioneers() {
		vector<int> P = pioneers(me());
		for (int id : P) {
			Unit u = unit(id);
			Unit u2;
			if (imBeingStalked(u.pos, u2))
				command(id, huir(u.pos, u2.pos));
			else {
				Pos ptmp = invadeCells(u.pos);
				command(id, best_dir(u.pos, ptmp));
			}
		}
	}

	Pos kill_pioneers(const Pos& p, int health) {
		set<Pos> visited;
		queue<Pos> Q;
		Q.push(p);
		while (not Q.empty()) {
			Pos currentPos = Q.front();
			Q.pop();
			Cell c = cell(currentPos);
			if (c.id != -1 and c.id != me()) {
				Unit u = unit(c.id);
				if (u.type == Pioneer) return currentPos;
				else if (u.type == Furyan and health > u.health) return currentPos;
			}
			for (int i = 0; i < DirSize - 3; ++i) {
            	Dir currentDir = static_cast<Dir>(i);
            	Pos neighborPos = currentPos + currentDir;

            	if (isPassable(neighborPos) && visited.find(neighborPos) == visited.end()) {
                	visited.insert(neighborPos);
                	Q.push(neighborPos);
            	}
        	}
		}
		return p;
	}


	void move_furyans() {
		vector<int> F = furyans(me());
		for (int id : F) {
			Unit u = unit(id);
			Unit u2;
			if (imBeingStalked(u.pos, u2)) {
				//cerr << "perseguido" << endl;
				if (u2.type == Hellhound) command(id, huir(u.pos, u2.pos));
				else if (u2.type == Furyan and u2.health > u.health) command(id, huir(u.pos, u2.pos));
				else if (u2.type == Furyan) {
					command(id, best_dir(u.pos, u2.pos));
				}
			}
			else {
				Pos pfinal = kill_pioneers(u.pos, u.health);
				Dir dir = best_dir(u.pos, pfinal);
				cerr << "bestdir " << dir << endl;
				command(id, dir);
			}
		}
	}

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
	move_pioneers();
	move_furyans();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
