// PasswordStrengthChecker.cpp
// Author:  Kody Horstman
// Class:   CS3320

/* Password Requirements
 * 1. Must be 16 characters or more in length.
   2. Must contain at least 3 uppercase, 3 lowercase, and 2 digits.
   3. Must not use common names or character sequences including keyboard sequences.
   4. Must not be or contain commonly known or breached passwords.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

// Prototypes
void loadContents(const std::string&, std::vector<std::string>&);

int main() {
    // Constant filenames
    const std::string PWD_FILE = "password.txt";
    const std::string NAM_FILE = ""; // Common names
    const std::string SUB_FILE = "keyboard-combinations.txt"; // Common substrings
    const std::string BPW_FILE = "10000-common-passwords.txt"; // Previously breached passwords

    // Variable containers
    std::vector<std::string> passwords;
    std::vector<std::string> commonNames;
    std::vector<std::string> commonSubstrings;
    std::vector<std::string> breachedPasswords;

    // Read passwords from password.txt
    loadContents(PWD_FILE, passwords);
    loadContents(SUB_FILE, commonSubstrings);
    loadContents(BPW_FILE, breachedPasswords);

    std::cout << "Stored passwords:" << std::endl;
    for (auto pwd : passwords) {
        std::cout << pwd << std::endl;
    }
    
    std::cout << std::endl << "Stored keyboard combinations (first 10):" << std::endl;
    for (int i = 0; i <= 10; i++) {
        std::cout << commonSubstrings.at(i) << std::endl;
    }
    
    std::cout << std::endl << "Stored common passwords (first 10):" << std::endl;
    for (int i = 0; i <= 10; i++) {
        std::cout << breachedPasswords.at(i) << std::endl;
    }

    // for each password
        
        // check length

        // check character requirements

        // check substrings for common names

        // check substrings for common sequences

        // check substrings for previously breached passwords

        // output classification

}

// loadContents
// Description: Reads files line-by-line and inserts lines into a string vector passed into the function.
//  @param FILENAME
//  @param vect
void loadContents(const std::string& FILENAME, std::vector<std::string>& vect) {
    std::string line;
    // Create infile stream
    std::ifstream infile(FILENAME);

    // Check if file is open
    if (infile.is_open()) {
        // Read and store all non-empty lines
        while (!infile.eof()) {
            std::getline(infile, line);

            if (line != "") vect.push_back(line);
        }
        infile.close();
    }
}