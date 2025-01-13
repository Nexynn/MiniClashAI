#include "Knight.h"
#include "Archer.h"
#include "Barbarian.h"
#include <thread>
#include <chrono>
#include <fstream>
#include <ctime>
#include <cstdlib>  // for system() function




int initialisationPredifined(Arena* arena) {
	Knight* knight1 = new Knight(3, 1, '1');
	Archer* archer1 = new Archer(0, 0, '1');
	Barbarian* barbarian1 = new Barbarian(3, 0, '1');

	Knight* knight2 = new Knight(4, 4, '2');
	Archer* archer2 = new Archer(5, 1, '2');
	Barbarian* barbarian2 = new Barbarian(4, 3, '2');

	arena->setUnits(knight1);
	arena->setUnits(archer1);
	arena->setUnits(barbarian1);

	arena->setUnits(knight2);
	arena->setUnits(archer2);
	arena->setUnits(barbarian2);
	return 1;
}



int play() {
	Arena* arena = new Arena;
	//initialisationPredifined(arena);
	Arena::initialisationEquipe();
	Arena::randomPlacement('1');
	Arena::randomPlacement('2');
	cout << *arena << endl;
	Arena::playTeam('1');
	Arena::playTeam('2');
	this_thread::sleep_for(chrono::seconds(1));

	while (1) {
		Arena::clearConsoleOutput();
		//system("cls");
		// Do something here while the user presses Enter
		list<Units*> team1 = Arena::getTeam1();
		list<Units*> team2 = Arena::getTeam2();
		arena->updateSide(team1);
		arena->updateSide(team2);
		team1 = Arena::getTeam1();
		team2 = Arena::getTeam2();
		cout << *arena;
		cout << endl << endl << endl;

		if (Arena::getTeam1().size() == 0) {
			cout << "\033[32mVictory team 2!\033[0m" << endl;
			return 1;
		}
		else if (Arena::getTeam2().size() == 0) {
			cout << "\033[31mVictory team 1!\033[0m" << endl;
			return 1;
		}
		this_thread::sleep_for(chrono::seconds(1));
	}
	delete arena;

	return 1;
}


int playAuto(Arena* arena) {
	while (1) {
		// Do something here while the user presses Enter
		list<Units*> team1 = Arena::getTeam1();
		list<Units*> team2 = Arena::getTeam2();
		arena->updateSide(team1);
		arena->updateSide(team2);
		team1 = Arena::getTeam1();
		team2 = Arena::getTeam2();

		if (Arena::getTeam1().size() == 0) {
			//cout << "\033[32mVictory team 2!\033[0m" << endl;
			return 2;
		}
		else if (Arena::getTeam2().size() == 0) {
			//cout << "\033[31mVictory team 1!\033[0m" << endl;
			return 1;
		}
	}

	return 0;
}


void playAndWrite(int repetition) {
	ofstream fichier("resultats.txt");
	if (!fichier)
	{
		// Gestion de l'erreur si le fichier ne peut pas être ouvert
		std::cerr << "Impossible d'ouvrir le fichier." << std::endl;
		return;
	}

	int statEquipe1 = 0;
	int statEquipe2 = 0;

	for (int i = 0; i < repetition; i++) {
		Arena* arena = new Arena;
		Arena::initialisationEquipe();
		Arena::randomPlacement('1');
		Arena::randomPlacement('2');

		for (int row = 0; row < 8; row++) {
			for (int col = 0; col < 5; col++) {
				if (arena->getCell(row, col) != nullptr) {
					char unit = arena->getCell(row, col)->getSymbol();
					fichier << unit;
				}
				else {
					fichier << '-';
				}
			}
			fichier << endl;
		}
		int resultat = playAuto(arena);
		if (resultat == 1) {
			statEquipe1++;
			fichier << "un" << endl;
		}
		else {
			statEquipe2++;
			fichier << "deux" << endl;
		}

		delete arena;
	}
	cout << "Equipe 1 : " << statEquipe1 << endl;
	cout << "Equipe 2 : " << statEquipe2 << endl;
}


bool fichierExiste(const std::string& nomFichier) {
	std::ifstream fichier(nomFichier.c_str());
	return fichier.good();
}


int main() {
	char userInput;
	cout << "Press A to automaticaly create a text of the numbers of games you want with their result." << endl;
	cout << "Press S to start a new model of Neural Network." << endl;
	cout << "Press T to train the model with the text file." << endl;
	cout << "Press E to exercise model from result text file." << endl;
	cout << "Press P to play." << endl;
	cout << "Press X to quit." << endl;
	cout << "Enter a character: ";
	cin >> userInput;

	while (userInput != 'X') {
		if (userInput == 'A') {
			// Utiliser la fonction system() avec la commande "cls" pour effacer la console
			system("cls");
			
			int userNumber;
			cout << "Enter number of games you want to generate: ";
			cin >> userNumber;
			cout << endl;
			while (cin.fail()) {
				cout << "Invalid input. Please enter a valid number: ";

				// Clear the error flag and discard any invalid input
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				// Try reading the user input again
				cin >> userNumber;
			}

			// Enregistrer le temps de départ
			std::clock_t start = std::clock();
			playAndWrite(userNumber);

			// Enregistrer le temps d'arrêt
			std::clock_t end = std::clock();
			// Calculer la durée en secondes
			double duration = (end - start) / (double)CLOCKS_PER_SEC;
			std::cout << "The program take " << duration << " seconds to proceed." << std::endl;
		}
		else if (userInput == 'S') {
			system("cls");
			// Run the Python script using the Python interpreter
			int returnValue = system("python NN.py S");
		}
		else if (userInput == 'T') {
			system("cls");
			// Run the Python script using the Python interpreter
			int returnValue = system("python NN.py T");
		}
		else if (userInput == 'E') {
			system("cls");
			// Run the Python script using the Python interpreter
			int returnValue = system("python NN.py E");
		}
		else if (userInput == 'P') {
			if (!fichierExiste("fichier_de_poids.h5")) {
				std::cout << "The file 'fichier_de_poids.h5' does not exist." << std::endl;
			}
			else {
				system("cls");
				play();
			}
		}
		
		cout << "Press X to quit." << endl;
		cin >> userInput;
		while (userInput != 'X') {
			cin >> userInput;
			cout << "Press X to quit." << endl;
		}

		system("cls");
		cout << "Press A to automaticaly create a text of the numbers of games you want with their result." << endl;
		cout << "Press S to start a new model of Neural Network." << endl;
		cout << "Press T to train the model with the text file." << endl;
		cout << "Press E to exercise model from result text file." << endl;
		cout << "Press P to play." << endl;
		cout << "Press X to quit." << endl;
		cout << "Enter a character: ";
		cin >> userInput;
	}

	return 1;
}