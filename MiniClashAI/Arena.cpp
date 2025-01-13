#include "Arena.h"

Units* Arena::cells[8][5];
list<Units*> Arena::team1;
list<Units*> Arena::team2;
list<char> Arena::deck1;
list<char> Arena::deck2;
int Arena::elixirTeam1;
int Arena::elixirTeam2;


Arena::Arena() {
    // Initialize the arena with all cells set to nullptr (empty)
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 5; col++) {
            cells[row][col] = nullptr;
        }
    }
    team1 = list<Units*>{};
    team2 = list<Units*>{};
    deck1 = list<char>{};
    deck2 = list<char>{};
}

Arena::~Arena() {
    for (auto unit : team1)
        delete unit;
    for (auto unit : team2)
        delete unit;
        

    
}

void Arena::predictGame(char teamNumber) {
    ofstream file("gameInProgress.txt");
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 5; col++) {
            if (getCell(row, col) != nullptr) {
                char unit = getCell(row, col)->getSymbol();
                file << unit;
            }
            else {
                file << '-';
            }
        }
        file << endl;
    }
    file << '0';
    file.close();
    string command = "python NN.py P ";
    command += teamNumber;
    int returnValue = std::system(command.c_str());
    std::cout << endl;
}


void Arena::playTeam(char teamNumber) {
    list<Units*> team;
    int elixir;
    int ligne[4];

    if (teamNumber == '1') {
        team = team1;
        elixirTeam1 += 4;
        elixir = elixirTeam1;
        int temp[] = { 0, 1, 2, 3 };
        std::copy(std::begin(temp), std::end(temp), ligne);
    }
    else {
        team = team2;
        elixirTeam1 += 4;
        elixir = elixirTeam2;
        int temp[] = { 4, 5, 6, 7 };
        std::copy(std::begin(temp), std::end(temp), ligne);
    }
    
    char userInput;
    predictGame(teamNumber);
    std::cout << "Press m to modify troop placement of team " << teamNumber << " or X to quit" << endl << "Enter a character: ";
    cin >> userInput;

    while (userInput != 'X') {
        // Faire quelque chose ici si l'utilisateur entre la lettre 'm' pour modifier la case de la troupe
        if (userInput == 'm') {
            int i = 0;
            std::cout << "Choisissez un numero de troupe a deplacer." << endl;
            for (Units* unit : team) {
                std::cout << i << " : " << unit->getSymbol() << " | " << "ligne : " << unit->getRow() << " | colonne : " << unit->getCol() << endl;
                i++;
            }

            int unitToMove;
            cin >> unitToMove;
            while (unitToMove < 0 || unitToMove > team.size()) {
                std::cout << "Erreur d'entree reessayer : ";
                cin >> unitToMove;
            }

            int unitToRow;
            std::cout <<"Choisissez un numero de ligne ou deplacer la troupe." << endl;
            cin >> unitToRow;
            while (unitToRow < ligne[0] || unitToRow > ligne[3]) {
                std::cout << "Erreur d'entree reessayer : ";
                cin >> unitToRow;
            }

            int unitToCol;
            std::cout << "Choisissez un numero de colonne ou deplacer la troupe." << endl;
            cin >> unitToCol;
            while (unitToCol < 0 || unitToCol > 4 || cells[unitToRow][unitToCol] != nullptr) {
                std::cout << "Erreur d'entree reessayer : ";
                cin >> unitToCol;
            }

            auto it = team.begin();
            advance(it, unitToMove);
            swapCell((*it)->getRow(), (*it)->getCol(), unitToRow, unitToCol);
            (*it)->setRow(unitToRow);
            (*it)->setCol(unitToCol);
        }
        std::system("cls");
        print();
        std::cout << endl;
        predictGame(teamNumber);
        std::cout << "Press m to modify troop placement or X to quit" << endl << "Enter a character: ";
        cin >> userInput;
    }
    std::system("cls");
    print();
    std::cout << endl;
}


void Arena::initialisationEquipe() {
    deck1 = { 'k', 'a', 'b' };
    deck2 = { 'k', 'a', 'b' };
    elixirTeam1 = 2;
    elixirTeam2 = 2;
}




void Arena::randomPlacement(char teamNumber) {
    list<Units*> team;
    int elixir;
    int ligne[4];
    list<char> deck;
    int lowestElixir;

    if (teamNumber == '1') {
        team = team1;
        deck = deck1;
        elixirTeam1 += 4;
        elixir = elixirTeam1;
        int temp[] = { 0, 1, 2, 3 };
        std::copy(std::begin(temp), std::end(temp), ligne);
    }
    else {
        team = team2;
        deck = deck2;
        elixirTeam2 += 4;
        elixir = elixirTeam2;
        int temp[] = { 4, 5, 6, 7 };
        std::copy(std::begin(temp), std::end(temp), ligne);
    }

    lowestElixir = 10;
    for (char unit : deck) {
        if (unit == 'k' && lowestElixir > 3)
            lowestElixir = 3;
        if (unit == 'a' && lowestElixir > 3)
            lowestElixir = 3;
        if (unit == 'b' && lowestElixir > 3)
            lowestElixir = 3;
    }

    // Utilisation de std::chrono::high_resolution_clock pour obtenir une graine plus précise
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(static_cast<unsigned>(seed));

    while (elixir > lowestElixir && deck.size() != 0) {
        // Generate random row and column values
        int randomUnit = (rand() % deck.size());
        int randomRow = ligne[0] + (rand() % (ligne[3] - ligne[0] + 1));
        int randomCol = (rand() % 5);
        auto it = deck.begin();
        advance(it, randomUnit);
        char unit = *it;
        deck.remove(unit);

        if (cells[randomRow][randomCol] == nullptr) {
            if (unit == 'k') {
                Units* knight = new Knight(randomRow, randomCol, teamNumber);
                setUnits(knight);
            }
            else if (unit == 'a') {
                Units* archer = new Archer(randomRow, randomCol, teamNumber);
                setUnits(archer);
            }
            else if (unit == 'b') {
                Units* barbarian = new Barbarian(randomRow, randomCol, teamNumber);
                setUnits(barbarian);
            }
        }
    }
}


void Arena::setUnits(Units* units) {
    int row = units->getRow();
    int col = units->getCol();
    if (row < 0 || row > 7 || col < 0 || col > 4) {
        return ;
    }
    if (cells[row][col] == nullptr) {
        cells[row][col] = units;
        if (units->getTeamName() == '1') {
            team1.push_back(units);
        }
        else {
            team2.push_back(units);
        }
    }
}


Units* Arena::getCell(int row, int col) {
    if (row < 0 || row > 7 || col < 0 || col > 4) {
        return nullptr;
    }
    return cells[row][col];
}


void Arena::updateSide(list<Units*>& side) {
    for (Units* unit : side) {
        if (unit->getTarget() == nullptr || !unit->getTarget()->isAlive() || Arena::calculateDistance(unit->getRow(), unit->getCol(), unit->getTarget()->getRow(), unit->getTarget()->getCol()) > unit->getAttackRange() ){
            unit->setTarget(unit->findTarget());
            if (unit->getTarget() == nullptr)
                return;
        }
        if (unit->ultimate()){}

        else if (unit->attack()) {}

        else if (unit->move()) {}

    }
}


void Arena::removeUnits(Units* units) {
    cells[units->getRow()][units->getCol()] = nullptr;
    if (units->getTeamName() == '1') {
        team1.remove(units);
    }
    else {
        team2.remove(units);
    }
    //delete units;

}

void Arena::swapCell(int row1, int col1, int row2, int col2) {
    Units* temp = cells[row1][col1];
    cells[row1][col1] = cells[row2][col2];
    cells[row2][col2] = temp;
}


int Arena::calculateDistance(int row1, int col1, int row2, int col2) {
    int dx = row2 - row1;
    int dy = col2 - col1;
    return std::sqrt(dx * dx + dy * dy);
}


// Define a helper function to check whether a point is within the bounds of the maze.
bool Arena::isWithinBounds(const Point& p) {
    return p.row >= 0 && p.row < 8 && p.col >= 0 && p.col < 5;
}


vector<Point> Arena::findShortestPath(Point start, Point end) {
    // Vérifier si les points de départ et d'arrivée sont identiques
    if (start.row == end.row && start.col == end.col) {
        return {};  // Renvoyer un vecteur vide
    }

    // Dimensions de la grille (à adapter selon vos besoins)
    int numRows = 8;
    int numCols = 5;

    vector<vector<bool>> visited(8, vector<bool>(5, false));
    vector<Point> result = _findShortestPath(start, end, visited, 10, 0);
    if (result.size() == 0) {
        Point below = { start.row + 1 , start.col };
        Point upper = { start.row - 1 , start.col };
        if (isWithinBounds(below)) {
            visited.assign(8, vector<bool>(5, false));
            result = _findShortestPath(below, end, visited, 10, 1);
            result.push_back(below);
        }
        if ((result.size() == 0 || result.back().row != end.row)  && isWithinBounds(upper)) {
            visited.assign(8, vector<bool>(5, false));
            result = _findShortestPath(upper, end, visited, 10, 1);
            result.push_back(upper);
        }
    }


    reverse(result.begin(), result.end());
    return result;
}

vector<Point> Arena::_findShortestPath(Point start, Point end, vector<vector<bool>> visited, int maxPath, int currentLength) {
    if (start.row == end.row && start.col == end.col) {
        return { end };  // Renvoyer un vecteur vide
    }
    if (currentLength >= maxPath)
        return {};

    vector<int> dRows;
    vector<int> dCols;
    if (start.col > end.col) {
        if (start.row == end.row) {
            dRows = { 0 };
            dCols = { -1 };
        }
        else if (start.row > end.row) {
            dRows = { -1, 0 };
            dCols = { 0, -1 };
        }
        else {
            dRows = { 1, 0 };
            dCols = { 0, -1 };
        }
    }
    else {
        if (start.row == end.row) {
            dRows = { 0 };
            dCols = { 1 };
        }
        else if (start.row > end.row) {
            dRows = { -1, 0 };
            dCols = { 0, 1 };
        }
        else {
            dRows = { 1, 0 };
            dCols = { 0, 1 };
        }
    }

    vector<Point> BestMatchingPath = {};

    for (int i = 0; i < dRows.size(); i++) {
        int newRow = start.row + dRows[i];
        int newCol = start.col + dCols[i];
        Point nouveau = { newRow, newCol };

        if (isWithinBounds(nouveau) && cells[newRow][newCol] == nullptr && visited[newRow][newCol] == false) {
            vector<vector<bool>> copiedVisited(visited);
            copiedVisited[newRow][newCol] = true;
            vector<Point> result = _findShortestPath(nouveau, end, copiedVisited, 10, currentLength++);
            if (result.size() != 0 && result.front().col == end.col && result.front().row == end.row) {
                result.push_back(nouveau);
                return result;
            }
        }
        if (nouveau.row == end.row && nouveau.col == end.col) {
            return { end };  // Renvoyer un vecteur vide
        }
    }
    return BestMatchingPath;
}


void Arena::print() {
    list<Units*>::iterator unit1 = team1.begin();
    list<Units*>::iterator unit2 = team2.begin();
    cout << "0 1 2 3 4" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            Units* unit = getCell(i, j);
            if (unit == nullptr) {
                std::cout << "- ";
            }
            else {
                if (unit->getTeamName() == '1') {
                    std::cout << "\033[31m" << unit->getSymbol() << " \033[0m";
                }
                else {
                    std::cout << "\033[32m" << unit->getSymbol() << " \033[0m";
                }
            }
        }
        cout << " " << i << "     ";
        if (i == 0) {
            cout << "Team 1 Stats :";
        }
        else if (i >= 1 && i <= 3 && team1.size() >= i) {
            cout << "\033[31m" << (*unit1)->getSymbol() << " \033[0m" << " : " << (*unit1)->getHealth() << "hp";
            std::advance(unit1, 1);
        }
        else if (i == 4) {
            cout << "Team 2 Stats :";
        }
        else if (i >= 5 && i <= 7 && team2.size() >= (i - 4)) {
            cout << "\033[32m" << (*unit2)->getSymbol() << " \033[0m" << " : " << (*unit2)->getHealth() << "hp";
            std::advance(unit2, 1);
        }
        std::cout << endl;
    }
    std::cout << endl;
}

void Arena::clearConsoleOutput()
{
    // Séquence d'échappement ANSI pour déplacer le curseur en haut à gauche de l'écran
    std::cout << "\033[2J";

    // Séquence d'échappement ANSI pour positionner le curseur à la première colonne de la première ligne
    std::cout << "\033[1;1H";
}

ostream& operator<<(ostream& out, Arena& arena) {
    list<Units*>::iterator unit1 = arena.team1.begin();
    list<Units*>::iterator unit2 = arena.team2.begin();

    out << "0 1 2 3 4" << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 5; j++) {
            Units* unit = arena.getCell(i, j);
            if (unit == nullptr) {
                out << "- ";
            }
            else {
                if (unit->getTeamName() == '1') {
                    out << "\033[31m" << unit->getSymbol() << " \033[0m";
                }
                else {
                    out << "\033[32m" << unit->getSymbol() << " \033[0m";
                }
            }
        }
        out << " " << i << "     ";
        if (i == 0) {
            out << "Team 1 Stats :";
        }
        else if (i >= 1 && i <= 3 && arena.team1.size() >= i) {
            out << "\033[31m" << (*unit1)->getSymbol() << " \033[0m" << " : " << (*unit1)->getHealth() << "hp";
            std::advance(unit1, 1);
        }
        else if (i == 4) {
            out << "Team 2 Stats :";
        }
        else if (i >= 5 && i <= 7 && arena.team2.size() >= (i - 4)) {
            out << "\033[32m" << (*unit2)->getSymbol() << " \033[0m" << " : " << (*unit2)->getHealth() << "hp";
            std::advance(unit2, 1);
        }
        out << endl;
    }
    out << endl;
    return out;
}