// PasswordStrengthChecker.cpp
// Author:  Kody Horstman
// Class:   CS3320

/* Password Requirements
 * 1. Must be 16 characters or more in length.
   2. Must contain at least 3 uppercase, 3 lowercase, and 2 digits.
   3. Must not use common names or character sequences including keyboard sequences.
   4. Must not be or contain commonly known or breached passwords.

   Source for common names, keyboard combinations, common passwords is https://github.com/danielmiessler/SecLists (branch: master).
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

// Prototypes
void loadContentsInto(const std::string&, std::vector<std::string>&);
bool isLongEnough(const std::string&);
bool meetsCharacterRequirements(const std::string&, const std::string&);
bool isFreeOf(const std::string&, std::vector<std::string>&);

int main() {
    // Types
    enum class Strength { Weak, Strong };

    // Constants
    const std::string PWD_FILE = "password.txt";
    const std::string NAM_FILE = "10000-common-names.txt"; // Common names
    const std::string SUB_FILE = "keyboard-combinations.txt"; // Common substrings
    const std::string BPW_FILE = "10000-common-passwords.txt"; // Previously breached passwords
    
// TODO: Check and revise pattern
    std::string pattern = "[a-z]{3,}[A-Z]{3,}[0-9]{2,}";
    //std::regex(, std::regex_constants::ECMAScript | std::regex_constants::icase);

    // Variable containers
    std::vector<std::string> passwords;
    std::vector<std::string> commonNames;
    std::vector<std::string> commonSubstrings;
    std::vector<std::string> breachedPasswords;
    
    // Variables
    Strength strengthClass = Strength::Strong;
    
    // Read passwords from password.txt
    loadContentsInto(PWD_FILE, passwords);
    loadContentsInto(NAM_FILE, commonNames);
    loadContentsInto(SUB_FILE, commonSubstrings);
    loadContentsInto(BPW_FILE, breachedPasswords);

    std::cout << "Stored passwords:" << std::endl;
    for (auto pwd : passwords) {
        std::cout << pwd << std::endl;
    }
    std::cout << "Stored names (first 10):" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << commonNames.at(i) << std::endl;
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
    for (const auto& pwd : passwords) {
        // check length
        if (isLongEnough(pwd) 
            && meetsCharacterRequirements(pwd, pattern)
            && isFreeOf(pwd, commonSubstrings)
            && isFreeOf(pwd, breachedPasswords)
// TODO: Add common names
           )
            strengthClass = Strength::Weak;
        // check character requirements
        
        
        // check substrings for common names

        // check substrings for common sequences

        // check substrings for previously breached passwords

        // output classification

    }


}

// loadContents
// Description: Reads files line-by-line and inserts lines into a string vector passed into the function.
//  @param FILENAME
//  @param vect
void loadContentsInto(const std::string& FILENAME, std::vector<std::string>& vect) {
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

// isLongEnough
// Returns true if parameter is greater than or equal to 16 characters in length
//  @param pwd
bool isLongEnough(const std::string& pwd) {
    if (pwd.size() >= 16) return true;
    else return false;
}

// meetsCharacterRequirements
// Returns true if there is at least 1 match of the regular expression in the given string.
//  @param pwd
//  @param pattern
bool meetsCharacterRequirements(const std::string& pwd, const std::regex& pattern) {
    if (std::regex_match(pwd, std::regex(pattern))) return true;
    return false;
}

// isFreeOf
// Returns true if the first parameter contains none of the elements in the second parameter.
//  @param pwd
//  @param substrings
bool isFreeOf(const std::string& pwd, const std::vector<std::string>& substrings) {
    
// TODO: add loop through all elements

    if () return true;
    else return false;
}