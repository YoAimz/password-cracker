//password cracker that cracks hashes md5 or sha256
//another ULTIMATE option where the format of file like [username:hashpasswrd]
//you can add your own files in this program
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <iomanip>
#include <vector>
#include <chrono>
#include <stdbool.h>
#include <ctime>
#include <conio.h>

using namespace std;

//please add your hashDump file with usernames and password hashes name down here
//and make sure the file is in the same path
const string loginFile = "usrLogins.txt";
//your password dic
const string commonPasswordFile = "10millionMostCommonPasswords.txt";

//temporary file that opens when you have cracked the passwords from dumpfile
const string temporaryFile = "CRACKEDPASSWORDS.txt";

//md5 hashing function
string generateMD5Hash(const string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    stringstream stream;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        stream <<hex <<setw(2) <<setfill('0') <<static_cast<int>(digest[i]);
    }

    return stream.str();
}

//sha256 hashing function
string generateSHA256Hash(const string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), digest);

    stringstream stream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        stream <<hex <<setw(2) <<setfill('0') <<static_cast<int>(digest[i]);
    }

    return stream.str();
}

bool searchPasswordInfile(const string& filename, const string & userEnterdPassword){
    ifstream passwordFile(filename);
    //add error handling to improve code and avoid program crash
    if(!passwordFile.is_open()){
        //error 1
        cerr <<"error file opening, error 1 " <<filename <<"\n";
        return false;
    }

    string password;
    while (getline(passwordFile, password)){
        if (password == userEnterdPassword){
            cout <<"Password found in file: " <<filename <<"\n";
            passwordFile.close();
            return true;
            }
    }
    passwordFile.close();
    return false;       
}

//checks wheter the inputed is md5 or sha256
bool searchPasswordByHash(const string& hash, const vector<string>& passwords, string& foundPassword, bool isMD5) {
    for (const auto& password : passwords) {
        string hashedPassword = isMD5 ? generateMD5Hash(password) : generateSHA256Hash(password);

        if (hashedPassword == hash) {
            foundPassword = password;
            return true;
        }
    }

    return false;
}

//reads the contents of my files currently 2 files mostCommonPasswords and mostCommonPasswordsSWE
vector<string> readPasswords(const vector<string>& filenames) {
    vector<string> passwords;

    for (const auto& filename : filenames) {
        ifstream file(filename);

        if (!file.is_open()) {
            //error 2
            cerr <<"Error opening file: " <<filename <<"error 2\n";
            continue;  //countniue moves to the next file if cant be oppened
        }

        string password;
        while (getline(file, password)) {
            passwords.push_back(password);
        }

        file.close();
    }

    return passwords;
}

//function for time
string formatElapsedTime(const chrono::high_resolution_clock::time_point& startTime, const chrono::high_resolution_clock::time_point& endTime) {
    auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;

    stringstream formattedTime;
    formattedTime <<setfill('0') <<setw(2) <<hours <<"h " <<setw(2) <<minutes <<"m " <<setw(2) <<seconds <<"s";
    
    return formattedTime.str();
}

int main(){
    vector<string> filenames = {"mostCommonPass.txt", "mostCommonPassSWE.txt"};
    
    const vector<string> allPasswords = readPasswords(filenames);

    while (1){
        string plainText;
        string inputedHash;
        cout <<"\n---------------passwordcracker v2-----------------\n";
        cout <<"Choose option\n";
        cout <<"1. Crack A Hash (MD5/SHA256)\n";
        cout <<"2.'ULTIMATE' CRACK FROM FILE DIRECTLY IF YOU HAVE A File [username:pass]\n(MD5/SHA256)\n";
        cout <<"3. EXIT\n";
        cout <<"-----------------------------------------------------\n";

        int choice;
        cin>> choice;
        
        switch (choice){
        case 1: {
            //Case 1 will take an already hashed password, either MD5 or SHA256
            string enteredHashedPassword;

            cout <<"Enter hashed password: ";
            cin>> enteredHashedPassword;

            if (enteredHashedPassword.length() != 32 && enteredHashedPassword.length() != 64) {
                cout <<"Invalid. Please enter a valid MD5 (32 characters) or SHA256 (64 characters) hash\n";
                break;
            }

            ifstream passwordsFile(commonPasswordFile);
            if (!passwordsFile.is_open()) {
                cerr << "Error opening file error 4\n";
                break;
            }

            string line;
            bool found = false;
            bool isMD5 = enteredHashedPassword.length() == 32;

            //Start the timer
            auto startTime = chrono::high_resolution_clock::now();

            while (getline(passwordsFile, line)) {
                string originalPassword = line;

                //Hash the password using MD5 if the entered hash is MD5
                if (isMD5) {
                    string md5HashedPassword = generateMD5Hash(originalPassword);

                    // Check if the entered hash matches the MD5 hash of the password
                    if (md5HashedPassword == enteredHashedPassword) {
                        cout <<"Password found: " <<originalPassword << endl;
                        found = true;
                        break;  //Display the first matching password
                    }
                } 
                else { //Hash the password using SHA256 if the entered hash is SHA256
                    string sha256HashedPassword = generateSHA256Hash(originalPassword);

                    //Check if the entered hash matches the SHA256 hash of the password
                    if (sha256HashedPassword == enteredHashedPassword) {
                        cout << "Password found: " << originalPassword << endl;
                        found = true;
                        break;  // Display the first matching password
                    }
                }
            }

            //Stop the timer
            auto endTime = chrono::high_resolution_clock::now();

            if (!found) {
                cout <<"No password found for the provided hash\n";
            }

            //Call the formatElapsedTime function to get the formatted elapsed time
            cout <<"Time taken: " <<formatElapsedTime(startTime, endTime) <<endl;

            passwordsFile.close();

            cout <<"\n1. Main Menu\n";
            cout <<"2. Exit\n";
            int menuChoice;
            cin>> menuChoice;

            if (menuChoice == 1) {
                break;
            } 
            else if (menuChoice == 2) {
                return 0;
            } 
            else {
                cout <<"Invalid choice. Exiting the program.\n";
                return 0;
            }
        }

        case 2: {
            cout <<"Please wait cracking passwords... Press 's' to stop.\n";
            ifstream loginFileStream(loginFile);
            if (!loginFileStream.is_open()) {
                cerr <<"Error opening login file\n";
                break;
            }

            ofstream tempFile(temporaryFile, ios::trunc);
            if (!tempFile.is_open()) {
                cerr <<"Error creating temporary file\n";
                break;
            }

            string line;
            bool found = false;

            //Start timer
            auto startTime = chrono::high_resolution_clock::now();

            while (getline(loginFileStream, line)) {
                //Check if the user pressed 's' to stop
                if (_kbhit()) {
                    char key = _getch();
                    if (key == 's' || key == 'S') {
                        cout << "Stopping the cracking process...\n";
                        break;
                    }
                }

                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string enteredHashedPassword = line.substr(pos + 1);

                    string foundPassword;
                    bool passwordFound = false;
                    if (enteredHashedPassword.length() == 32) {
                        passwordFound = searchPasswordByHash(enteredHashedPassword, allPasswords, foundPassword, true); // md5
                    } 
                    else if (enteredHashedPassword.length() == 64) {
                        passwordFound = searchPasswordByHash(enteredHashedPassword, allPasswords, foundPassword, false); // sha256
                    }
                    if (passwordFound) {
                        string username = line.substr(0, pos);

                        tempFile << username << ":" << foundPassword << endl;
                        found = true;
                    }
                }
            }

            //Stop the timer
            auto endTime = chrono::high_resolution_clock::now();

            loginFileStream.close();
            tempFile.close();

            if (found) {
                //calculate and print elapsed time in HH:MM:SS format
                cout << "Cracked all passwords and added them to a '" <<temporaryFile <<"' file in the same folder as the program" <<endl;
                cout <<"note the file will be deleted after closing the program" <<endl;
                cout << "Time taken: " <<formatElapsedTime(startTime, endTime) <<endl;
            } else {
                cout <<"No passwords could be cracked from the provided file\n";
            }

            cout << "\n1. Main Menu\n";
            cout << "2. Exit\n";
            int menuChoice;
            cin >> menuChoice;

            if (menuChoice == 1) {
                break;
            } 
            else if (menuChoice == 2) {
                remove(temporaryFile.c_str());
                return 0;
            } 
            else {
                cout <<"Invalid choice. Exiting the program.\n";
                remove(temporaryFile.c_str());
                return 0;
            }
        }
        
        case 3:
            return 0;

        default:
            break;
        }
    }
    
    return 0;
}