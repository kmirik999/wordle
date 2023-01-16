#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;


int WordLength = 5;
string filePath;
const int NotMatched= 0;
const int PartialMatch = 1;
const int Match = 2;
const int NumberOfGuesses = 6;

void toUpperCase(string &input){
    transform(input.begin(), input.end(), input.begin(), [](unsigned char c){return toupper(c);});
}

string getRandomWord()
{/*
    std::string word;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, alphabet.length() - 1);
    for(int i = 0; i < WordLength; i++)
    {
        word += alphabet[distribution(generator)];
    }
    return word;
    */
    int i = 0;
    vector<string> GenerateRandomWord ;
    string word1;

    random_device rd;
    mt19937 gen(rd());


    cout << filePath<< endl;
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
    uniform_int_distribution<> disRandomProduct(0, i-1);
    int id;
    do{
        id = disRandomProduct(gen);
    } while (GenerateRandomWord.at(id).length() != WordLength);
        return GenerateRandomWord.at(id);
}


bool isValidWord(const string& word){
    return word.length() == WordLength && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos;
}


void markMatches(vector<vector<int>> &matches, int currentGuessIndex, string targetWord, string guessWord)
{
    cout<< targetWord << endl;

    for (int i = 0; i < guessWord.length(); i++)
    {
        for(int j = 0; j < targetWord.length(); j++)
        {
            if (matches[currentGuessIndex][i] == NotMatched && guessWord[i] == targetWord[j])
            {
                if(i == j)
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
bool isAllMatched(string targetWord, string guessWord)
{
    for (int i = 0; i <targetWord.length();i++)
    {
        if(targetWord[i] != guessWord[i])
            return false;
    }
    return true;
}
void printMatchedWord(vector<vector<int>> &matches, string targetWord, int currentGuessIndex)
{
    for(int i = 0; i < targetWord.length(); i++)
    {
        if(matches[currentGuessIndex][i] == Match)
        {
            cout << targetWord[i];
        }
    }
    cout << endl;
}

void printWordle(vector<vector<int>> matches, vector<string> guesses, int currentGuessIndex)
{
    for (int i = 0; i <= currentGuessIndex && guesses.size(); i++) {
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
            if (i == 0) {
                std::cout << separator << std::endl;
            }
            std::cout << padding << std::endl;
            std::cout << text << std::endl;
            std::cout << padding << std::endl;
            std::cout << separator << std::endl;
        }
    }
}


int main() {
    int selection;
    cout <<  "Enter 1 --- Easy  --- 5 letters word"<< endl;
    cout <<  "Enter 2 --- Medium --- 6 letters word"<< endl;
    cout <<  "Enter 3 --- Hard --- 9 letters word"<< endl;
    cout << "Enter the level(1, 2, 3):"<< endl;
    cin >> selection;
    switch (selection) {
        case 1:
            WordLength = 5;
            filePath = "data.txt";
            break;
        case 2:
            WordLength = 6;
            filePath = "data1.txt";
            break;
        case 3:
            WordLength = 9;
            filePath = "data2.txt";
            break;
    }
    string targetWord = getRandomWord();
    vector<string> guesses;
    vector<vector<int>> matches(NumberOfGuesses, vector<int>(WordLength));
    int currentGuessIndex = -1;
    while (true) {
        string guess;
        cout << "Enter your guess (word length must be " << WordLength << ") or type Q to quit the game: ";
        cin >> guess;
        if (guess == "Q" || guess == "q") {
            break;
        }
        toUpperCase(guess);
        if (!isValidWord(guess)) {
            cout << "Invalid word. Please enter a valid word of length " << WordLength << endl;
            continue;
        }
        currentGuessIndex++;
        guesses.push_back(guess);
        markMatches(matches, currentGuessIndex, targetWord, guess);
        printWordle(matches, guesses, currentGuessIndex);
        if (isAllMatched(targetWord,guess)) {
            cout << "Congratulations! You won the game!" << endl;
            break;
        }
        if(currentGuessIndex == NumberOfGuesses - 1){
            cout << "Sorry, you lose the game! The word was: "<< targetWord << endl;
            break;
        }
    }
    return 0;
}