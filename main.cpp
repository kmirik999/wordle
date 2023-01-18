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

class Wordle {
private:
    int WordLength;
    string filePath;
    const int NotMatched = 0;
    const int PartialMatch = 1;
    const int Match = 2;
    const int NumberOfGuesses = 6;
    vector<vector<int>> matches;
    vector<string> guesses;

public:
    int getNumberOfGuesses() {
        return NumberOfGuesses;
    }

    int getWordLength() {
        return WordLength;
    }
    void setWordLength(int newWordLength) {
        WordLength = newWordLength;
    }
    string getFilePath() {
        return filePath;
    }
    void setFilePath(string newFilePath) {
        filePath = std::move(newFilePath);
    }
    Wordle() {}
    Wordle(int wordLength, string filePath) : WordLength(wordLength), filePath(move(filePath)) {}

     void toUpperCase(string& input) {
        transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return toupper(c); });
     }

    string getRandomWord()
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

    bool isValidWord(const string& word) const {
        return word.length() == WordLength && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos;
    }

    void markMatches(vector<vector<int>>& matches, int currentGuessIndex, string targetWord, string guessWord) const
    {
        cout << targetWord << endl;
        for (int i = 0; i < guessWord.length(); i++)
        {
            for (int j = 0; j < targetWord.length(); j++)
            {
                if (/*matches[currentGuessIndex][i] == NotMatched &&*/ guessWord[i] == targetWord[j])
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


    void printWordle(vector<vector<int>> matches, vector<string> guesses, int currentGuessIndex)
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
                } else if (matches[i][j] == Match) {
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


int main() {
    int selection;
    Wordle w = Wordle();
    cout << "Enter 1 --- Easy  --- 5 letters word" << endl;
    cout << "Enter 2 --- Medium --- 7 letters word" << endl;
    cout << "Enter 3 --- Hard --- 9 letters word" << endl;
    cout << "Enter the level(1, 2, 3):" << endl;
    cin >> selection;
    switch (selection) {
        case 1:

            w.setWordLength(5);
            w.setFilePath("data.txt");
            break;
        case 2:
            w.setWordLength(7);
            w.setFilePath("data1.txt");
            break;
        case 3:
            w.setWordLength(9);
            w.setFilePath("data2.txt");
            break;
    }
    string targetWord = w.getRandomWord();
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
        w.toUpperCase(guess);
        if (!w.isValidWord(guess)) {
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