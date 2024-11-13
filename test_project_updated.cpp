#include <iostream>
#include <vector>
#include <string>

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
            if (ch < 'A' || ch > 'Z') {
                cout << "Invalid character in word: " << word << endl;
                return; // Ignore words with invalid characters
            }
            int index = ch - 'A';
            if (!node->children[index])
                node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    bool search(const string &word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (ch < 'A' || ch > 'Z') return false; // Reject words with invalid characters
            int index = ch - 'A';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return node->isEndOfWord;
    }
};

class CrosswordGrid {
private:
    vector<vector<char>> grid;
    Trie validWords;

public:
    CrosswordGrid() {
        grid.resize(GRID_SIZE, vector<char>(GRID_SIZE, '-'));
    }

    void addWord(const string &word) {
        validWords.insert(word);
        if (!solvePuzzle(word)) {
            cout << "Failed to place the word \"" << word << "\" on the grid." << endl;
        } else {
            cout << "Successfully placed \"" << word << "\" on the grid:" << endl;
            displayGrid();
        }
    }

    void displayGrid() {
        for (const auto &row : grid) {
            for (char cell : row) {
                cout << cell << ' ';
            }
            cout << endl;
        }
    }

private:
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
    }

    void removeWord(int row, int col, const string &word, bool horizontal) {
        for (int i = 0; i < word.length(); i++) {
            if (horizontal) grid[row][col + i] = '-';
            else grid[row + i][col] = '-';
        }
    }

    bool solvePuzzle(const string &word) {
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
};

int main() {
    CrosswordGrid crossword;
    string word, clue;
    char choice;
    vector<pair<string, string>> cluesAndWords; // Vector of pairs to store clues and words

    cout << "Enter sentences for the crossword puzzle (type 'n' when prompted to stop):\n";

    while (true) {
        cout << "Enter a sentence: ";
        getline(cin, clue);

        // Ask for the corresponding word
        cout << "Enter a word for this sentence: ";
        cin >> word;
        cin.ignore(); // Ignore the newline character left in the input buffer

        // Convert the word to uppercase
        for (char &c : word) c = toupper(c);

        // Add the clue and word as a pair to the vector
        cluesAndWords.push_back(make_pair(clue, word));

        cout << "Do you want to add another sentence? (y/n): ";
        cin >> choice;
        cin.ignore(); 

        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    cout << "\nAttempting to place words into the crossword grid:\n";
    for (const auto &pair : cluesAndWords) {
        cout << "Clue: " << pair.first << endl; 
        
        // Keep asking until the correct word is entered
        while (true) {
            cout << "Enter the word that matches this clue: ";
            cin >> word;
            for (char &c : word) c = toupper(c);

            if (word == pair.second) {
                crossword.addWord(word); 
                break; // Exit the loop if the word matches
            } else {
                cout << "The entered word does not match the original. Please try again." << endl;
            }
        }
    }

    cout << "Final crossword grid:" << endl;
    crossword.displayGrid();

    return 0;
}
