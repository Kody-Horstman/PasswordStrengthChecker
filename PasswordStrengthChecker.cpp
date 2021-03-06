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

#include <iostream> // cin
#include <iterator> // distance(), sregex_iterator()
#include <fstream>  // ifstream, is_open(), eof(), getline(), close()
#include <vector>   // vector<>, empty(), push_back()
#include <string>   // string, size(), find(), npos
#include <regex>    // regex

#define TEST    0      // To run the test list I made, Make this 1
#define DEBUG   0     // To output debug information, Make this 1

#define MIN_PWD_LEN     16      // Minimum password length
#define MIN_UPPER       3       // Minimum number of uppercase characters
#define MIN_LOWER       3       // Minimum number of lowercase characters
#define MIN_DIGIT       2       // Minimum number of digit characters
#define MIN_LINE_LEN    3       // Minimum length of lines being loaded from a file
#define UC_PAT          "[A-Z]"  // Pattern for uppercase characters
#define LC_PAT          "[a-z]"  // Pattern for lowercase characters
#define DC_PAT          "[0-9]"  // Pattern for digit characters

// Types
enum class Strength { Weak, Strong };

// Prototypes
void loadContentsInto(const std::string&, std::vector<std::string>&);
bool meetsLengthRequirements(const std::string&);
bool meetsCharacterRequirements(const std::string&);
bool isFreeOf(const std::string&, const std::vector<std::string>&);
void classify(const std::vector<std::string>&, const std::vector<std::string>&);

int main() {
    // Constants
#if TEST
    const std::string TST_FILE = "test-pwds.txt";  // Passwords to test checker actually works as designed
#endif
    const std::string PWD_FILE = "password.txt";
    const std::string NAM_FILE = "10000-common-names.txt"; // Common names
    const std::string KEY_FILE = "keyboard-combinations.txt"; // Common substrings
    const std::string BPW_FILE = "10000-common-passwords.txt"; // Previously breached passwords

    // Variable containers
    std::vector<std::string> passwords;
    std::vector<std::string> commonPatterns;
#if TEST
    std::vector<std::string> testPwds;

    // Read passwords contents from files
    loadContentsInto(TST_FILE, testPwds);
#endif
    loadContentsInto(PWD_FILE, passwords);
    loadContentsInto(NAM_FILE, commonPatterns);
    loadContentsInto(KEY_FILE, commonPatterns);
    loadContentsInto(BPW_FILE, commonPatterns);

    // Classify
#if TEST && DEBUG
    std::cout << "Test passwords:" << '\n';
#endif
#if TEST
    classify(testPwds, commonPatterns);
#endif
#if DEBUG
    std::cout << "Passwords:" << '\n';
#endif
    classify(passwords, commonPatterns);
}

// loadContentsInto
// Description: Reads files line-by-line and inserts lines into a string vector passed into the function.
//  @param FILENAME
//  @param vect
void loadContentsInto(const std::string& FILENAME, std::vector<std::string>& vect) {
    std::string line;

#if DEBUG
    std::cout << "Loading " << FILENAME << "... ";
#endif

    // Create infile stream
    std::ifstream infile(FILENAME);

    // Check if file is open
    if (infile.is_open()) {
        // Read and store all lines longer than some amount
        while (!infile.eof()) {
            std::getline(infile, line);

            if (line.size() >= MIN_LINE_LEN) vect.push_back(line);
        }
        infile.close();
    }

#if DEBUG
    std::cout << "Success" << '\n' << '\n';
#endif
}

// meetsLengthRequirements
// Returns true if pwd parameter is greater than or equal to minimum characters in length.
//  @param pwd
bool meetsLengthRequirements(const std::string& pwd) {
    if (pwd.size() >= MIN_PWD_LEN) return true;

#if DEBUG
    std::cout << "\tDoes not meet length requirement." << '\n';
#endif

    return false;
}

// meetsCharacterRequirements
// Returns true if the pwd parameter meets all character related requirements.
//  @param pwd
bool meetsCharacterRequirements(const std::string& pwd) {
    // I struggled with this part, this solution was adapted from the example at https://en.cppreference.com/w/cpp/regex.

    const std::regex upperReg(UC_PAT), lowerReg(LC_PAT), digitsReg(DC_PAT);
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

#if DEBUG
    if (upperCount < MIN_UPPER) std::cout << "\tNot enough uppercase letters." << '\n';
    if (lowerCount < MIN_LOWER) std::cout << "\tNot enough lowercase letters." << '\n';
    if (digitsCount < MIN_DIGIT) std::cout << "\tNot enough digits." << '\n';
#endif

    if (upperCount >= MIN_UPPER
        && lowerCount >= MIN_LOWER
        && digitsCount >= MIN_DIGIT
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
#if DEBUG
                std::cout << "\tContains common pattern: " << pat << '\n';
#endif
                return false;
            }
        }
    }

    return true;
}

// classify
// Takes a list of passwords and a list of patterns and outputs Strong or Weak depending on whether they satisfy the requirements stated at the beginning of this file.
//  @param vectPwds
//  @param vectPatterns
void classify(const std::vector<std::string> &vectPwds, const std::vector<std::string> &vectPatterns) {
    static Strength strengthClass;

    // for each test password
    for (const auto& pwd : vectPwds) {
#if DEBUG
        std::cout << pwd << '\n';
#endif
        // check requirements
        if (meetsLengthRequirements(pwd)        // Length
            && meetsCharacterRequirements(pwd)  // Character
            && isFreeOf(pwd, vectPatterns)      // Common Patterns
           )
            strengthClass = Strength::Strong;
        else strengthClass = Strength::Weak;

        // output classification
        std::cout << (strengthClass == Strength::Strong ? "Strong" : "Weak") << '\n';
#if DEBUG
        std::cout << '\n';
#endif
    }

    std::cout << '\n';
}