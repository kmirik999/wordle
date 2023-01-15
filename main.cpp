#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <random>


const int WordLength = 5;
const int NotMatched= 0;
const int PartialMatch = 1;
const int Match = 2;
const int NumberOfGuesses = 6;

void toUpperCase(std::string &input){
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c){return std::toupper(c);});
}



bool isValidWord(const std:: string& word){
    return word.length() == WordLength && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos;
}


void markMatches(std::vector<std::vector<int>> &matches, int currentGuessIndex, std::string targetWord, std::string guessWord)
{
    std::cout<< targetWord << std::endl;

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
bool isAllMatched(std::string targetWord, std::string guessWord)
{
    for (int i = 0; i <targetWord.length();i++)
    {
        if(targetWord[i] != guessWord[i])
            return false;
    }
    return true;
}
void printMatchedWord(std::vector<std::vector<int>> &matches, std::string targetWord, int currentGuessIndex)
{
    for(int i = 0; i < targetWord.length(); i++)
    {
        if(matches[currentGuessIndex][i] == Match)
        {
            std::cout << targetWord[i];
        }
    }
    std::cout << std::endl;
}

int main() {

    std::vector<std::string> guesses;
    std::vector<std::vector<int>> matches(NumberOfGuesses, std::vector<int>(WordLength));
    int currentGuessIndex = -1;
    while (true) {
        std::string guess;
        std::cout << "Enter your guess (word length must be " << WordLength << ") or type Q to quit the game: ";
        std::cin >> guess;
        if (guess == "Q" || guess == "q") {
            break;
        }
        toUpperCase(guess);
        if (!isValidWord(guess)) {
            std::cout << "Invalid word. Please enter a valid word of length " << WordLength << std::endl;
            continue;
        }
        currentGuessIndex++;
        guesses.push_back(guess);
        markMatches(matches, currentGuessIndex, targetWord, guess);

        if (isAllMatched(targetWord,guess)) {
            std::cout << "Congratulations! You won the game!" << std::endl;
            break;
        }
        if(currentGuessIndex == NumberOfGuesses - 1){
            std::cout << "Sorry, you lose the game! The word was: "<< targetWord << std::endl;
            break;
        }
    }
    return 0;
}