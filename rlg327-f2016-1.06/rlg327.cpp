//#include <stdio.h>
//#include <string.h>
//#include <sys/time.h>
//#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include "npc.h"

//#include "dungeon.h"
//#include "pc.h"
//#include "npc.h"
//#include "move.h"
//#include "io.h"
using namespace std;

//local functions
void printErr(string errMsg, int lineNum);

//int bool valid_header(string str);

int main(int argc, char *argv[]) {

    //Parsing sequence
//    string path = getenv("HOME");
//    path += "/.rlg327/monster_desc.txt";
//    cout << path << endl;


    fstream in(".rlg327/monster_desc.txt");

    //Check if file was found.
    if (in == NULL) {
        cout << "file open error PATH: .rlg327/monster_desc.txt" << endl;
    }


    string line;
    int lineCount = 0;

    string name;
    string sym;
    string color;
    string desc;
    string speed;
    string hp;
    string abil;

    while (getline(in, line) != NULL) {
        lineCount++;
        if (lineCount == 1 && line != "RLG327 MONSTER DESCRIPTION 1") {
            cout << "First line error." << endl;
            exit(-1);
        }

        if (line == "" || line == "\n") {
            continue;
        }

        //Variables needed for monster creation.
        vector<string> foundFields;
        vector<string> unParsedLine;
//        vector fieldData<void *>;

        //npc *monster;


        if (line == "BEGIN MONSTER") {
            cout << "\nBEGIN MONSTER" << endl;

            bool flagNAME = false;
            bool flagDESC = false;
            bool flagCOLOR = false;
            bool flagSYMB = false;
            bool flagSPEED = false;
            bool flagABIL = false;


            string monLine;
            getline(in, monLine);
            lineCount++;
            while (monLine != "END") {

                unParsedLine.clear();
                string token;

                //Push on line tokens.
                for (stringstream s(monLine); s >> token;) {
                    unParsedLine.push_back(token);
                }

                cout << "\nTOKENS:" << endl;
                for (int i = 0; (unsigned int) i < unParsedLine.size(); i++) {
                    cout << unParsedLine[i] << endl;
                }
                cout << "\nEND TOKENS:" << endl;


                npc *monster;

                string result = "";


                int i = 0;
                for (string::iterator itr = monLine.begin(); itr != monLine.end(); ++itr) {

                    if (i > 0) {
                        if (unParsedLine[i] == "NAME") {
                            foundFields.push_back("NAME");
                            result += *itr + " ";
                            cout << result << endl;
                        } else if (unParsedLine[i] == "DESC") {
                            foundFields.push_back("DESC");
                            string data;
                            getline(in, data);
                            lineCount++;
                            while (data != ".") {
                                result += data;
                            }
                            cout << result << endl;
                            break;
                        } else if (unParsedLine[i] == "SYMB") {
                            foundFields.push_back("SYMB");
                            result += *itr;
                            break;
                        } else if (unParsedLine[i] == "COLOR") {
                            foundFields.push_back("COLOR");
                            result += *itr;
                            cout << result << endl;
                            break;

                        } else if (unParsedLine[i] == "SPEED") {
                            foundFields.push_back("SPEED");
                            result += *itr;
                            cout << result << endl;
                            break;
                        } else if (unParsedLine[i] == "ABIL") {
                            foundFields.push_back("ABIL");
                            result += *itr;
                            continue;
                        } else if (unParsedLine[i] == "HP") {
                            foundFields.push_back("HP");
                            result += *itr;
                            cout << result << endl;
                            break;
                        } else if (unParsedLine[i] == "DAM") {
                            foundFields.push_back("DAM");
                            result += *itr;
                            cout << result << endl;
                            break;
                        } else {
                            printErr("invalid field", lineCount);
                            foundFields.clear();
                            break;
//                            fieldData.clear();
//                            free(fieldData);
                        }
                    } else {
                        if (unParsedLine[i] == "NAME") {
                            foundFields.push_back("NAME");
                            if (flagNAME) {
                                printErr("Duplicate field", lineCount);
                            }
                            bool flagNAME = true;
                        } else if (unParsedLine[i] == "DESC") {
                            foundFields.push_back("DESC");
                            bool flagDESC = true;
                        } else if (unParsedLine[i] == "SYMB") {
                            foundFields.push_back("SYMB");

                        } else if (unParsedLine[i] == "COLOR") {
                            foundFields.push_back("COLOR");
                            bool flagCOLOE = true;
                        } else if (unParsedLine[i] == "SPEED") {
                            foundFields.push_back("SPEED");
                        } else if (unParsedLine[i] == "ABIL") {
                            foundFields.push_back("ABIL");
                            bool flagABIL = true;
                        } else if (unParsedLine[i] == "HP") {
                            foundFields.push_back("HP");
                            bool flagHP = true;
                        } else if (unParsedLine[i] == "DAM") {
                            foundFields.push_back("DAM");
                            bool flagDAM = true;
                        } else {
                            printErr("invalid field", lineCount);
                            foundFields.clear();
//                            fieldData.clear();
//                            free(fieldData);
                        }
                    }
                    i++;
                }
                getline(in, monLine);
            }
            if (monLine == "END") {
                if (foundFields.size() != 6) {
                    printErr("Incorrect number of fields.\nSkipping...", lineCount);
                    continue;
                } else {
                    continue;
                }
            } else {
                cout << "\nError: no END token" << endl;
                continue;
            }
        }

        return 0;
    }
}

//int bool valid_header(string str) {
//    if (str == "NAME" || str == "DESC" || str == "COLOR" || str == "SPEED" || str == "ABIL" || str == "HP" ||
//        str == "DAM") {
//        return true;
//    }
//    return false;
//}

void printErr(string errMsg, int lineNum) {
    cout << endl << "Error Line " << lineNum << errMsg << endl;
}


/*
        if (parsedLine.size() == 1) {
            int i = 0;
            int fieldCount = 0;
            if (line == "BEGIN MONSTER") {
                if (parsedLine.size() >= 2) {
                    if (parsedLine[0] == "NAME") {
                        for(int i = 1; i < parsedLine.size(); i++){
                            //if(parsedLine[i] == )
                        }
                        fieldCount++;
                    } else if (parsedLine[0] == "DESC") {
                        fieldCount++;
                    } else if (parsedLine[0] == "COLOR") {
                        fieldCount++;
                    } else if (parsedLine[0] == "SPEED") {
                        fieldCount++;
                    } else if (parsedLine[0] == "ABIL") {
                        fieldCount++;
                    } else if (parsedLine[0] == "HP") {
                        fieldCount++;
                    } else if (parsedLine[0] == "DAM") {
                        fieldCount++;
                    } else {
                        cout << "\n Line : " << lineCount << "invalid field\nskipping...\n" << endl;
                        continue;
                    }
                }
            } else if (line == "END") {

            }
        } else {
            continue;
        }

//char string itr
//        for(string::iterator itr = line.begin(); itr != line.end(); ++itr){
//            cout << "ORG: " << *itr;
//        }
//        cout << endl;


        //end of while EOF
    }

    return 0;
}
*/
