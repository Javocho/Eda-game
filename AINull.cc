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
	// Pos updatePos(const Pos& pos, int newValue, int axis) {
	// 	if (axis == 0) return Pos(newValue, pos.j, pos.k);
	// 	else if (axis == 1) return Pos(pos.i, newValue, pos.k);
	// 	else if (axis == 2) return Pos(pos.i, pos.j, newValue);
	// 	return pos;
	// }

	// queue<Dir> nearElevator(const Pos& pos) {
	// 	queue<Pos> Q;
 	// 	queue<Dir> Qdir;
	// 	vector<vector<bool>> visited(40, vector<bool>(80, false));
	// 	Q.push(pos);
	// 	visited[pos.i][pos.j] = true;

	// 	while (not Q.empty()) {
	// 		Pos current = Q.front();
	// 		Q.pop();
	// 		Cell c = cell(current);
	// 		if (c.type == Elevator) return Qdir;
			
	// 		Pos p = updatePos(current, current.i + 1, 0); //right
	// 		if (isPassable(p) && not visited[p.i][p.j]) {
    //            		Q.push(p);
    //             	visited[p.i][p.j] = true;
	// 				Qdir.push(Right);
	// 		}
	// 		cerr << "hola\n";

	// 		p = updatePos(current, current.j + 1, 1); //up
	// 		if (isPassable(p) && not visited[p.i][p.j]) {
    //            		Q.push(p);
    //             	visited[p.i][p.j] = true;
	// 				Qdir.push(Top);
	// 		}
	// 		cerr << "qu-e\n";
	// 		p = updatePos(current, current.i - 1, 0); //left
	// 		if (isPassable(p) && not visited[p.i][p.j]) {
    //            		Q.push(p);
    //             	visited[p.i][p.j] = true;
	// 				Qdir.push(Left);
	// 		}
	// 		cout << "tail";
	// 		p = updatePos(current, current.j - 1, 1); //down
	// 		if (isPassable(p) && not visited[p.i][p.j]) {
    //            		Q.push(p);
    //             	visited[p.i][p.j] = true;
	// 				Qdir.push(Down);
	// 		}

	// 	}
	// 	return Qdir;
	// }

	// list<Pos> nearGem(const Pos& pos) {
	// 	queue<Pos> Q;
	// 	list<Pos> Lpos;	
	// 	vector<vector<bool>> visited(40, vector<bool>(80, false));
	// 	Q.push(pos);
	// 	visited[pos.i][pos.j] = true;

	// 	while (not Q.empty()) {
	// 		Pos current = Q.front();
	// 		Cell c = cell(current);
	// 		if (c.gem) return Lpos;
	// 		Q.pop();
	// 		Lpos.push_back(current);
			
	// 		//updatePos(pos, pos.i + 1, 0);
	// 		vector<Pos> neighbors = {
    //         	updatePos(current, current.i + 1, 0), //right
	// 			updatePos(current, current.j + 1, 1), //up
	// 			updatePos(current, current.i - 1, 0), //left
	// 			updatePos(current, current.j - 1, 1), //down
    //    		 };

    //     	for (Pos neighbor : neighbors) {
    //         	if (isPassable(neighbor) && not visited[neighbor.i][neighbor.j]) {
    //            		Q.push(neighbor);
    //             	visited[neighbor.i][neighbor.j] = true;
    //         	}
    //     	}
	// 	}
	// 	Lpos.pop_front(); //borra primer elemento pos actual
	// 	return Lpos;
	// }

	int distance (Pos p1, Pos p2) {
		return sqrt(pow(p2.i-p1.i,2)+pow(p1.j-p2.j,2));
	}

	bool imBeingStalked(Pos p, Unit &u) {
		vector<int> H = hellhounds();

		for (int id : H) {
			u = unit(id);
			if (distance(p, u.pos) < 6)
				return true;
		}

		for (int i = 0; i < 4; i++) {
			vector<int> F = furyans(i);
			for (int id : F) {
				u = unit(id);
				if (distance(u.pos, p) < 4) return true; //no comprueba vida
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
	//0. que se pueda pasar
	//6 si está en ascensor y quiere subir (temporal)
	//5 ascensor sin sol?
	//4. casillas enemigas sin enemigos
	//3. casillas blancas sin enemigos
	//2. casillas mías sin enemigos
	//1. enemigos
	//-1 por defecto
	//-2 ascensor con sol o hay alguien

	// priority_queue<pair<int, Dir>> invadeCells(const Pos& pos, vector<vector<bool>>& visitedConquering) {
	// 	priority_queue<pair<int, Dir>> q;
	// 	for (int i = 0; i < DirSize - 3; ++i) {
    //     // Access each direction using the enum values
    //     	Dir currentDir = static_cast<Dir>(i);
	// 		Pos newPos = pos + currentDir;
	// 		//cerr << newPos << endl;
	// 		auto p = make_pair(-1, currentDir);
	// 		if (isPassable(newPos)) {
	// 			p.first = 0;
	// 			Cell c = cell(newPos);
	// 			if (c.owner != me()) {
	// 				if (c.owner != -1) p.first = 4;
	// 				else p.first = 3;
	// 			}
	// 			// p.first += enemy_near(newPos);
	// 			// if (not visitedConquering[newPos.i][newPos.j]) { //cómo resuelvo bucles si todo alrededor está visitado??
	// 			// 	++p.first;
	// 			// 	visitedConquering[newPos.i][newPos.j] = true;
	// 			// }
	// 			//if (c.type == Elevator and not daylight(newPos)) p.first = 5;
	// 			/*else */if (c.type == Elevator) p.first = -2;
	// 			if (c.id != -1) p.first = -2;
	// 		}
	// 		q.push(p);
  	// 	}
	// 	return q;
	// }

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
		vector<vector<bool>> visitedConquering(40, vector<bool>(80, false)); //set de bools mejor e irlo reseteando de vez en cuando
		for (int id : P) {
			Unit u = unit(id);
			Unit u2;
			if (imBeingStalked(u.pos, u2))
				command(id, huir(u.pos, u2.pos));
			//cerr << invadeCells(u.pos, visitedConquering).top().second << endl;
			//command(id, invadeCells(u.pos, visitedConquering).top().second);
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

	void move_furyans() {
		vector<int> F = furyans(me());
		for (int id : F) {
			Unit u = unit(id);
			Unit u2;
			if (imBeingStalked(u.pos, u2)) {
				if (u2.type == Hellhound) command(id, huir(u.pos, u2.pos));
				else {
				//cerr << "hola?" << endl;
					Pos pfinal = kill_pioneers(u.pos, u.health);
					Dir dir = best_dir(u.pos, pfinal);
					command(id, dir);
				}
			}
			// 	else if (u2.type == Furyan and u2.health > u.health) command(id, huir(u.pos, u2.pos));
			// 	else if (u2.type == Furyan) {
			// 		command(id, best_dir(u.pos, u2.pos));
			// 	}
			// }
			else {
				//cerr << "hola?" << endl;
				Pos pfinal = kill_pioneers(u.pos, u.health);
				Dir dir = best_dir(u.pos, pfinal);
				command(id, dir);
			}
		}
	}
	// Dir escape_hellhounds() { //probar alrededor de cada hellhound markessi
	// 	vector<int> id_hellbounds = hellhounds();
	// 	for (int id : id_hellbounds) {
	// 		Unit u = unit(id);
	// 		Pos pos = u.pos;
	// 		for (int i = 0; i < DirSize - 3; i++) {
	// 			Dir currentDir = static_cast<Dir>(i);
	// 			Pos p = pos + currentDir;
	// 			Cell c = cell(p);
	// 			if (c.id != -1 and c.id == me())
	// 				return currentDir;
	// 		}
	// 	return None;
	// }

	//devuelve DIr y usa un queue<Pair<Pos, vector<Dir>>> y set visited 
		

	/*void bfs_furyans(const Pos& pos) {
		queue<Pos> Q;
		vector<vector<bool>> enc(40, vector<bool>(80, false));
		Q.push(pos);
		//mirar comentarios whats
		//al principio crear creo que era pair<Pos, vector<Dir>> y devolver vector<Dir y lista de visitados diferente
		while (not Q.empty()) {

		}
	}*/

	// void killPioneers() {
	// 	vector<int> F = furyans(me());
		

	// }
/*
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
			}
		}
	}

	*/

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
