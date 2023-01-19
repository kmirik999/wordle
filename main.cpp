#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;
class Game {
private:
    int WordLength{};
    int NumberOfGuesses = 6;

public:
    int getNumberOfGuesses() const  {
        return NumberOfGuesses;
    }

    int getWordLength() const {
        return WordLength;
    }
    void setWordLength(int newWordLength) {
        WordLength = newWordLength;
    }
};

class Wordle : public Game{
private:
    int WordLength{};
    const int PartialMatch = 1;
    const int Match = 2;
    const int NumberOfGuesses = 6;

public:
    Wordle() = default;

    void markMatches(vector<vector<int>>& matches, int currentGuessIndex, string targetWord, string guessWord) const
    {
        cout << targetWord << endl;
        for (int i = 0; i < guessWord.length(); i++)
        {
            for (int j = 0; j < targetWord.length(); j++)
            {
                if ( guessWord[i] == targetWord[j])
                {
                    if (i == j)
                    {
                        matches[currentGuessIndex][i] = Match;
                        break;
                    }
                    else
                    {
                        matches[currentGuessIndex][i] = PartialMatch;
                    }
                }
            }
        }
    }

    static bool isAllMatched(string targetWord, string guessWord) {
        for (int i = 0; i < targetWord.length(); i++) {
            if (tolower(targetWord[i]) != tolower(guessWord[i]))
                return false;
        }
        return true;
    }


    void printWordle(vector<vector<int>> matches, vector<string> guesses, int currentGuessIndex) const
    {
        for (int i = 0; i <= currentGuessIndex; i++) {
            std::string separator = "-";
            std::string padding = "|";
            std::string text = "|";

            for (int j = 0; j < guesses[i].length(); ++j) {
                separator += "------";
                padding += "     |";
                char value = toupper(guesses[i][j]);
                text += " ";
                if (matches[i][j] == PartialMatch) {
                    text += "\033[33m";
                }
                else if (matches[i][j] == Match) {
                    text += "\033[32m";
                }
                text += value;
                if (matches[i][j] == PartialMatch || matches[i][j] == Match) {
                    text += "\033[0m";
                }
                text += "   |";
            }
            if (i == 0) {
                std::cout << separator << std::endl;
            }
            std::cout << padding << std::endl;
            std::cout << text << std::endl;
            std::cout << padding << std::endl;
            std::cout << separator << std::endl;
        }
    }
};

class WordProvider : public Game{
private:
    string filePath;
public:
    void setFilePath(string newFilePath) {
        filePath = std::move(newFilePath);
    }
    WordProvider() = default;

    static void toUpperCase(string& input) {
        transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return toupper(c); });
    }

    string getRandomWord(int WordLength)
    {
        int i = 0;
        vector<string> GenerateRandomWord;
        string word1;

        random_device rd;
        mt19937 gen(rd());


        cout << filePath << endl;
        ifstream fileRandomNames(filePath);
        if (!fileRandomNames.is_open()) {
            cout << "Error: unable to open file" << endl;
            return "";
        }


        string line;
        while (getline(fileRandomNames, line))
        {
            istringstream iss(line);

            iss >> word1;

            GenerateRandomWord.push_back(word1);
            i++;
        }
        uniform_int_distribution<> disRandomProduct(0, i - 1);
        int id;
        do {
            id = disRandomProduct(gen);
        } while (GenerateRandomWord.at(id).length() != WordLength);
        string result = GenerateRandomWord.at(id);
        toUpperCase(result);
        return result;
    }

    static bool isValidWord(const string& word, int WordLength) {
        return word.length() == WordLength && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos;
    }
};


int main() {
    int selection;
    Wordle w = Wordle();
    WordProvider wp = WordProvider();
    cout << "Enter 1 --- Easy  --- 5 letters word" << endl;
    cout << "Enter 2 --- Medium --- 7 letters word" << endl;
    cout << "Enter 3 --- Hard --- 9 letters word" << endl;
    cout << "Enter the level(1, 2, 3):" << endl;
    cin >> selection;
    switch (selection) {
        case 1:
            w.setWordLength(5);
            wp.setFilePath("data.txt");
            break;
        case 2:
            w.setWordLength(7);
            wp.setFilePath("data1.txt");
            break;
        case 3:
            w.setWordLength(9);
            wp.setFilePath("data2.txt");
            break;
    }
    string targetWord = wp.getRandomWord(w.getWordLength());
    vector<string> guesses;
    vector<vector<int>> matches(w.getNumberOfGuesses(), vector<int>(w.getWordLength()));
    int currentGuessIndex = -1;
    while (true) {
        string guess;
        cout << "Enter your guess (word length must be " << w.getWordLength() << ") or type Q to quit the game: ";
        cin >> guess;
        if (guess == "Q" || guess == "q") {
            break;
        }
        WordProvider::toUpperCase(guess);
        if (!WordProvider::isValidWord(guess, w.getWordLength())) {
            cout << "Invalid word. Please enter a valid word of length " << w.getWordLength() << endl;
            continue;
        }
        currentGuessIndex++;
        guesses.push_back(guess);
        w.markMatches(matches, currentGuessIndex, targetWord, guess);
        w.printWordle(matches, guesses, currentGuessIndex);
        if (Wordle::isAllMatched(targetWord, guess)) {
            cout << "Congratulations! You won the game!" << endl;
            break;
        }
        if (currentGuessIndex == w.getNumberOfGuesses() - 1) {
            cout << "Sorry, you lose the game! The word was: " << targetWord << endl;
            break;
        }
    }
    return 0;
}