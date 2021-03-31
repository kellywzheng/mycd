#include <iostream>
#include <string>
#include <regex>

using namespace std;

void simplify(string &directory)
{
    while (true) {
        string old_dir = directory;
        regex slashes("(//+)");
        regex dots("(/../)");
        smatch matches;
        
        directory = regex_replace(directory, slashes, "/");                  // replace multiple "/" to a single slash
        
        regex_search(directory, matches, dots);
        int idx = matches.position(0);
        //cout << matches[0] << " " << "idx " << idx << endl;
        if (idx > 0 and matches[0] == "/../") {
            int prev_idx = directory.rfind('/', idx - 1);
            //cout << "prev_idx " << prev_idx << endl;
            idx += 4;
            //cout << "idx inside " << idx << endl;
            directory = directory.substr(0, prev_idx + 1) + directory.substr(idx);
            //cout << "directory " << directory << endl;
        }
        
        if (directory == old_dir)
            break;
    }
}

int main(int argc, char** argv)
{
    string curr_directory = argv[1];
    string new_directory = argv[2];

    simplify(new_directory);
    cout << new_directory << endl;
    return 0;
}