#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

struct GameResult {
    string name;
    int attempts;
    string level;
};

class ScoreArchive {
private:
    vector<GameResult> list;
    const string filename = "scores.txt";

public:
    ScoreArchive() {
        load();
    }

    void load() {
        list.clear();
        ifstream file(filename);
        if (!file.is_open()) return;

        GameResult result;
        while (file >> result.name >> result.attempts >> result.level) {
            list.push_back(result);
        }

        file.close();
    }

    void save(const GameResult& newResult) {
        list.push_back(newResult);

        sort(list.begin(), list.end(), [](const GameResult& a, const GameResult& b) {
            return a.attempts < b.attempts;
        });

        if (list.size() > 5)
            list.resize(5);

        ofstream file(filename);
        for (const auto& r : list) {
            file << r.name << " "
                 << r.attempts << " "
                 << r.level << endl;
        }

        file.close();
    }

    bool hasResults() const {
        return !list.empty();
    }

    void showTop5() const {
        cout << "\n===== TOP 5 =====\n";
        for (size_t i = 0; i < list.size(); i++) {
            cout << i + 1 << ". "
                 << list[i].name << " | "
                 << list[i].attempts << " prob | "
                 << list[i].level << endl;
        }
        cout << "=================\n";
    }
};

class GuessingSession {
private:
    int minRange;
    int maxRange;
    int chosenNumber;
    string levelName;

public:
    GuessingSession(int minR, int maxR, string level)
        : minRange(minR), maxRange(maxR), levelName(level) {
        chosenNumber = rand() % (maxRange - minRange + 1) + minRange;
    }

    GameResult start() {
        int number;
        int tries = 0;

        cout << "\nZgadnij liczbe z zakresu "
             << minRange << " - " << maxRange << endl;

        while (true) {
            cout << "\nProba nr: " << tries + 1 << endl;
            cout << "Podaj liczbe: ";

            cin >> number;

            if (cin.fail()) {
                cout << "Blad! To nie jest liczba.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            tries++;

            if (number < chosenNumber) {
                cout << "Twoja liczba jest za mala.\n";
            }
            else if (number > chosenNumber) {
                cout << "Twoja liczba jest za duza.\n";
            }
            else {
                cout << "\nTrafiles w " << tries << " probach!\n";

                string name;
                cout << "Podaj swoje imie: ";
                cin >> name;

                return { name, tries, levelName };
            }
        }
    }
};

int safeReadInt() {
    int x;
    cin >> x;

    while (cin.fail()) {
        cout << "Niepoprawne dane. Sprobuj ponownie: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> x;
    }

    return x;
}

void showMenu(bool showTop) {
    cout << "\n===== ZGADNIJ LICZBE =====\n";
    cout << "1. Nowa gra\n";
    if (showTop)
        cout << "2. TOP5\n";
    cout << "0. Wyjscie\n";
    cout << "==========================\n";
    cout << "Wybor: ";
}

int main() {
    srand(time(nullptr));
    ScoreArchive archive;

    int choice;

    do {
        showMenu(archive.hasResults());
        choice = safeReadInt();

        if (choice == 1) {
            cout << "\nWybierz poziom trudnosci:\n";
            cout << "1. Latwy (1-50)\n";
            cout << "2. Sredni (1-100)\n";
            cout << "3. Trudny (1-250)\n";
            cout << "Wybor: ";

            int level = safeReadInt();

            GuessingSession game(1,50,"Latwy");

            if (level == 1)
                game = GuessingSession(1,50,"Latwy");
            else if (level == 2)
                game = GuessingSession(1,100,"Sredni");
            else if (level == 3)
                game = GuessingSession(1,250,"Trudny");
            else {
                cout << "Niepoprawny wybor poziomu.\n";
                continue;
            }

            GameResult result = game.start();
            archive.save(result);
        }
        else if (choice == 2 && archive.hasResults()) {
            archive.showTop5();
            cout << "\nWpisz dowolna liczbe aby wrocic: ";
            safeReadInt();
        }
        else if (choice != 0) {
            cout << "Niepoprawna opcja.\n";
        }

    } while (choice != 0);

    cout << "Koniec programu.\n";
    return 0;
}
