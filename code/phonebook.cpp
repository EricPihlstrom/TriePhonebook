#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

// TrieNode class representing a node in the Trie data structure
class TrieNode {
public:
    unordered_map<char, shared_ptr<TrieNode> > children; // Map of characters to child nodes
    bool isEndOfWord; // Flag indicating end of a word

    TrieNode() {
        isEndOfWord = false; //Constructor for TrieNode that sets isEndOfWord to false
    }
};

// Phonebook class implementing a phonebook using Trie data structure
class Phonebook {
private:
    shared_ptr<TrieNode> root; // Root node of the Trie
    unordered_map<string, string> phoneNumbers; // Store phone numbers associated with names

public:
    Phonebook() {
        root = shared_ptr<TrieNode>(new TrieNode); // Constructor that initializes the Trie with an empty root node
    }

    // Insert a new entry (name and phone number) into the phonebook Trie
    void insert(const string& name, const string& number) {
        shared_ptr<TrieNode> current = root; // Start at the root of the Trie
        for (char c : name) {
            if (!current->children[c]) { // If character doesn't exist as a child
                current->children[c] = shared_ptr<TrieNode>(new TrieNode); // Create a new node for the character
            }
            current = current->children[c]; // Move to the next node
        }
        current->isEndOfWord = true; // Mark the last node as the end of a word
        phoneNumbers[name] = number; // Store the phone number associated with the name
    }

    // Search for a name in the phonebook Trie
    bool search(const string& name) {
        shared_ptr<TrieNode> current = root; // Start at the root of the Trie
        for (char c : name) {
            if (!current->children[c]) { // If character doesn't exist as a child
                return false; // Name is not found
            }
            current = current->children[c]; // Move to the next node
        }
        if (current->isEndOfWord) {
            // Print the name and associated phone number
            cout << "Name: " << name << " - Phone: " << phoneNumbers[name] << "\n";
            return true; // Entry found
        }
        return false; // Entry not found
    }

    // Delete an entry (name and phone number) from the phonebook Trie
    bool deleteEntry(const string& name) {
        if (!search(name)) {
            return false; // Name not found, deletion fails
        }

        shared_ptr<TrieNode> current = root; // Start at the root of the Trie
        for (char c : name) {
            current = current->children[c]; // Move to the node representing the character
        }
        current->isEndOfWord = false; // Unmark the last node as the end of a word
        phoneNumbers.erase(name); // Remove associated phone number
        return true; // Deletion successful
    }

    // Provide autocomplete suggestions based on a prefix
    void autocomplete(const string& prefix) {
        shared_ptr<TrieNode> current = root; // Start at the root of the Trie
        for (char c : prefix) {
            if (!current->children[c]) {
                return; // Prefix not found, no autocomplete suggestions
            }
            current = current->children[c]; // Move to the next node
        }

        vector<string> results; // Store autocomplete suggestions
        autocompleteHelper(current, prefix, results); // Call the helper function to collect suggestions

        if (results.empty()) {
            cout << "No autocomplete suggestions for \"" << prefix << "\".\n";
        } else {
            cout << "Autocomplete suggestions for \"" << prefix << "\":\n";
            for (const string& suggestion : results) {
                cout << suggestion << "\n"; // Print each suggestion
            }
        }
    }

    // Print all entries (names and phone numbers) in the phonebook Trie
    void printAllEntries() {
        string currentEntry; // Current name being constructed during traversal
        printAllEntriesHelper(root, currentEntry); // Call the helper function to print entries
    }

private:
    // Helper function for autocomplete, recursively collects suggestions
    void autocompleteHelper(shared_ptr<TrieNode> node, const string& prefix, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(prefix); // Add the completed name to the suggestions list
        }
        for (const auto& childPair : node->children) {
            autocompleteHelper(childPair.second, prefix + childPair.first, results); // This line recursively calls the autocompleteHelper function for each child node, appending the character of the child node to the prefix.
        }
    }

    // Helper function for printing all entries, including phone numbers
    void printAllEntriesHelper(shared_ptr<TrieNode> node, string& currentEntry) {
        if (node->isEndOfWord) {
            cout << "Name: " << currentEntry;
            if (phoneNumbers.find(currentEntry) != phoneNumbers.end()) {
                cout << ", Phone Number: " << phoneNumbers[currentEntry];
            }
            cout << endl;
        }
        
        for (const auto& childPair : node->children) {
            currentEntry.push_back(childPair.first);
            printAllEntriesHelper(childPair.second, currentEntry);
            currentEntry.pop_back();
        }
    }
};

int main() {
    Phonebook phonebook;

    while (true) {
        cout << "\nPhonebook Menu:\n";
        cout << "1. Insert new entry\n";
        cout << "2. Search for entry\n";
        cout << "3. Delete entry\n";
        cout << "4. Autocomplete suggestions\n";
        cout << "5. Print all entries\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, number;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter phone number: ";
                getline(cin, number);
                phonebook.insert(name, number);
                cout << "Entry added.\n";
                break;
            }
            case 2: {
                string name;
                cout << "Enter name to search: ";
                cin.ignore();
                getline(cin, name);
                if (!phonebook.search(name)) {
                    cout << "Entry not found.\n";
                }
                break;
            }
            case 3: {
                string name;
                cout << "Enter name to delete: ";
                cin.ignore();
                getline(cin, name);
                if (phonebook.deleteEntry(name)) {
                    cout << "Entry deleted.\n";
                } else {
                    cout << "Entry not found.\n";
                }
                break;
            }
            case 4: {
                string prefix;
                cout << "Enter prefix for autocomplete: ";
                cin.ignore();
                getline(cin, prefix);
                phonebook.autocomplete(prefix);
                break;
            }
            case 5: {
                cout << "All entries in the phonebook:\n";
                phonebook.printAllEntries();
                break;
            }
            case 6: {
                cout << "Exiting the program.\n";
                return 0;
            }
            default: {
                cout << "Invalid choice. Please choose a valid option.\n";
                break;
            }
        }
    }

    return 0;
}
