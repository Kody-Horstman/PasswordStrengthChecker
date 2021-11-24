// PasswordStrengthChecker.cpp
// Author:  Kody Horstman
// Class:   CS3320

/* Password Requirements
 * 1. Must be 16 characters or more in length.
   2. Must contain at least 3 uppercase, 3 lowercase, and 2 digits.
   3. Must not use common names or character sequences including keyboard sequences.
   4. Must not be or contain commonly known or breached passwords.

   I hadn't checked the passwords before designing my requirements and may have taken it a little too far.

   Source for common names, keyboard combinations, common passwords is https://github.com/danielmiessler/SecLists (branch: master).
 */

#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

#define MIN_LEN 16 // Minimum password length
#define UC_PAT "[A-Z]"  // Pattern for uppercase characters
#define LC_PAT "[a-z]"  // Pattern for lowercase characters
#define DC_PAT "[0-9]"  // Pattern for digit characters

 // Types
enum class Strength { Weak, Strong };

// Prototypes
void loadContentsInto(const std::string&, std::vector<std::string>&);
bool meetsLengthRequirements(const std::string&);
bool meetsCharacterRequirements(const std::string&);
bool isFreeOf(const std::string&, const std::vector<std::string>&);
//void classify(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&);
void classify(const std::vector<std::string>&, const std::vector<std::string>&);

int main() {
    // Constants
    const std::string TST_FILE = "test-pwds.txt";  // Passwords to test checker actually works as designed
    const std::string PWD_FILE = "password.txt";
    const std::string NAM_FILE = "10000-common-names.txt"; // Common names
    const std::string KEY_FILE = "keyboard-combinations.txt"; // Common substrings
    const std::string BPW_FILE = "10000-common-passwords.txt"; // Previously breached passwords

    // Variable containers
    std::vector<std::string> testPwds;
    std::vector<std::string> passwords;
    std::vector<std::string> commonNames;
    std::vector<std::string> commonCombinations;
    std::vector<std::string> breachedPasswords;
    std::vector<std::string> commonPatterns;
    
    // Read passwords from password.txt
    loadContentsInto(TST_FILE, testPwds);
    loadContentsInto(PWD_FILE, passwords);
    /*loadContentsInto(NAM_FILE, commonNames);
    loadContentsInto(KEY_FILE, commonCombinations);
    loadContentsInto(BPW_FILE, breachedPasswords);*/
    loadContentsInto(NAM_FILE, commonPatterns);
    loadContentsInto(KEY_FILE, commonPatterns);
    loadContentsInto(BPW_FILE, commonPatterns);

    // Classify
    //classify(testPwds, commonNames, commonCombinations, breachedPasswords);
    //classify(passwords, commonNames, commonCombinations, breachedPasswords);
    classify(testPwds, commonPatterns);
    classify(passwords, commonPatterns);
}

// loadContentsInto
// Description: Reads files line-by-line and inserts lines into a string vector passed into the function.
//  @param FILENAME
//  @param vect
void loadContentsInto(const std::string& FILENAME, std::vector<std::string>& vect) {
    std::string line;
    // Create infile stream
    std::ifstream infile(FILENAME);

    // Check if file is open
    if (infile.is_open()) {
        // Read and store all lines with more than 4 characters
        while (!infile.eof()) {
            std::getline(infile, line);

            if (line.size() >= 3) vect.push_back(line);
        }
        infile.close();
    }
}

// meetsLengthRequirements
// Returns true if pwd parameter is greater than or equal to MIN_LEN characters in length.
//  @param pwd
bool meetsLengthRequirements(const std::string& pwd) {
    if (pwd.size() >= MIN_LEN) return true;
    return false;
}

// meetsCharacterRequirements
// Returns true if the pwd parameter meets all character related requirements.
//  @param pwd
bool meetsCharacterRequirements(const std::string& pwd) {
    // I struggled with this part, this solution was adapted from the example at https://en.cppreference.com/w/cpp/regex.

    static const std::regex upperReg(UC_PAT), lowerReg(LC_PAT), digitsReg(DC_PAT);
    int upperCount = 0, lowerCount = 0, digitsCount = 0;

    // Uppercase characters
    auto upperBegin = std::sregex_iterator(pwd.begin(), pwd.end(), upperReg);
    auto upperEnd = std::sregex_iterator();
    upperCount = std::distance(upperBegin, upperEnd);

    // Lowercase characters
    auto lowerBegin = std::sregex_iterator(pwd.begin(), pwd.end(), lowerReg);
    auto lowerEnd = std::sregex_iterator();
    lowerCount = std::distance(lowerBegin, lowerEnd);

    // Digit characters
    auto digitsBegin = std::sregex_iterator(pwd.begin(), pwd.end(), digitsReg);
    auto digitsEnd = std::sregex_iterator();
    digitsCount = std::distance(digitsBegin, digitsEnd);

    if (upperCount >= 3
        && lowerCount >= 3
        && digitsCount >= 2
        )
        return true;
    return false;
}

// isFreeOf
// Returns true if the first parameter contains none of the elements in the second parameter.
//  @param pwd
//  @param patterns
bool isFreeOf(const std::string& pwd, const std::vector<std::string>& patterns) {
    // Assure patterns is non-empty
    if (!patterns.empty()) {
        // Loop through patterns
        for (const auto& pat : patterns) {
            // Return false if pat appears in pwd
            if (pwd.find(pat) != std::string::npos) {
                return false;
            }
        }
    }

    return true;
}

// classify
// Takes a list of passwords and 3 lists of patterns and outputs Strong or Weak depending on whether they satisfy the requirements stated at the beginning of this file.
//  @param vectPwds
//  @param vectNames
//  @param vectCombos
//  @param vectCommonPwds
//void classify(const std::vector<std::string> &vectPwds, const std::vector<std::string> &vectNames, const std::vector<std::string> &vectCombos, const std::vector<std::string> &vectCommonPwds) {
void classify(const std::vector<std::string> &vectPwds, const std::vector<std::string> &vectPatterns) {
    static Strength strengthClass;

    // for each test password
    for (const auto& pwd : vectPwds) {
        std::cout << pwd << std::endl;
        // check requirements
        if (meetsLengthRequirements(pwd)        // Length
            && meetsCharacterRequirements(pwd)  // Character
            //&& isFreeOf(pwd, vectNames)         // Common names
            //&& isFreeOf(pwd, vectCombos)        // Common keyboard combinations
            //&& isFreeOf(pwd, vectCommonPwds)    // Common passwords
            && isFreeOf(pwd, vectPatterns)
           )
            strengthClass = Strength::Strong;
        else strengthClass = Strength::Weak;

        // output classification
        std::cout << (strengthClass == Strength::Strong ? "Strong" : "Weak") << std::endl;
    }

    std::cout << std::endl;
}