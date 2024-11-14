#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

const int GRID_SIZE = 10;

class TrieNode {
public:
    bool isEndOfWord;
    TrieNode* children[26];

    TrieNode() {
        isEndOfWord = false;
        fill(begin(children), end(children), nullptr);
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    void insert(const string &word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'A';
            if (!node->children[index]) node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    bool search(const string &word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'A';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return node->isEndOfWord;
    }
};

struct WordPosition {
    int row;
    int col;
    bool horizontal;
};

class CrosswordGrid {
private:
    vector<vector<char>> grid;
    Trie validWords;
    unordered_map<string, WordPosition> wordPositions;

public:
    CrosswordGrid() {
        grid.resize(GRID_SIZE, vector<char>(GRID_SIZE, '-'));
    }

    void insertWordIntoTrie(const string &word) {
        validWords.insert(word);
    }

    void displayGrid() {
        for (const auto &row : grid) {
            for (char cell : row) {
                cout << cell << ' ';
            }
            cout << endl;
        }
    }

    bool isValidPlacement(int row, int col, const string &word, bool horizontal) {
        if (horizontal) {
            if (col + word.length() > GRID_SIZE) return false;
            for (int i = 0; i < word.length(); i++) {
                if (grid[row][col + i] != '-' && grid[row][col + i] != word[i]) return false;
            }
        } else {
            if (row + word.length() > GRID_SIZE) return false;
            for (int i = 0; i < word.length(); i++) {
                if (grid[row + i][col] != '-' && grid[row + i][col] != word[i]) return false;
            }
        }
        return true;
    }

    void placeWord(int row, int col, const string &word, bool horizontal) {
        for (int i = 0; i < word.length(); i++) {
            if (horizontal) grid[row][col + i] = word[i];
            else grid[row + i][col] = word[i];
        }
        wordPositions[word] = {row, col, horizontal};
    }

    void removeWord(const string &word) {
        if (wordPositions.find(word) != wordPositions.end()) {
            WordPosition pos = wordPositions[word];
            for (int i = 0; i < word.length(); i++) {
                if (pos.horizontal) grid[pos.row][pos.col + i] = '-';
                else grid[pos.row + i][pos.col] = '-';
            }
            wordPositions.erase(word);
        }
    }

    bool tryPlaceWord(const string &word) {
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                if (isValidPlacement(row, col, word, true)) {
                    placeWord(row, col, word, true);
                    return true;
                }
                if (isValidPlacement(row, col, word, false)) {
                    placeWord(row, col, word, false);
                    return true;
                }
            }
        }
        return false;
    }

    void solveCrosswordWithUserInput(const unordered_map<string, string>& cluesAndWords) {
        for (const auto &pair : cluesAndWords) {
            string clue = pair.first;
            string correctWord = pair.second;
            string userWord;

            cout << "Clue: " << clue << endl;
            while (true) {
                cout << "Enter the word that matches this clue: ";
                cin >> userWord;

                for (char &c : userWord) c = toupper(c);

                if (userWord == correctWord) {
                    if (tryPlaceWord(correctWord)) {
                        cout << "Successfully placed \"" << correctWord << "\" on the grid." << endl;
                        displayGrid();
                    } else {
                        cout << "Failed to place the word \"" << correctWord << "\" on the grid." << endl;
                    }
                    break;
                } else {
                    cout << "Incorrect word. Please try again." << endl;
                }
            }
        }
    }
};

int main() {
    CrosswordGrid crossword;
    string word, clue;
    unordered_map<string, string> cluesAndWords;

    cout << "Enter sentences for the crossword puzzle (type 'n' when prompted to stop):\n";

    while (true) {
        cout << "Enter a sentence: ";
        getline(cin, clue);

        cout << "Enter a word for this sentence: ";
        cin >> word;
        cin.ignore();

        for (char &c : word) c = toupper(c);
        cluesAndWords[clue] = word;
        crossword.insertWordIntoTrie(word);

        cout << "Do you want to add another sentence? (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore();

        if (choice == 'n' || choice == 'N') break;
    }

    cout << "\nAttempting to solve the crossword puzzle with user input:\n";
    crossword.solveCrosswordWithUserInput(cluesAndWords);

    return 0;
}

