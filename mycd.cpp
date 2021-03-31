#include <iostream>
#include <string>
#include <regex>
#include <deque>
#include <cstring>
#include <locale> 
#include <utility>

using namespace std;

/*
Reduce multiple consecutive forward-slashes ("/") to one forward-slash
Remove any sequence that consists of a file name component (other than "." or ".."), 
followed by "/", followed by the file name component ".."
*/
void simplify(string &directory)
{
    while (true) {
        string old_dir = directory;
        regex slashes("(//+)");
        regex dots("([A-Za-z0-9]+/../)");
        smatch matches;
        
        directory = regex_replace(directory, slashes, "/");                             // replace multiple "/" to a single slash
        
        regex_search(directory, matches, dots);                                         // find the first occurence of "filename/../"
        int idx = matches.position(0);
        if (idx > 0 and matches[0] == "/../") {
            int prev_idx = directory.rfind('/', idx - 1);                               // find the last occurence of "/" in the range 0 to idx - 1
            idx += 4;                                                                   // increment idx to now be at the start of the next file name component
            directory = directory.substr(0, prev_idx + 1) + directory.substr(idx);      // remove the "/../" sequence
        }
        
        if (directory == old_dir)                                                       // break out of loop when the above conditions no longer apply
            break;
    }
}

/*
Check if a given string is alphanumeric
*/
bool isAlnumStr(string str) {
    for (int i = 0; str[i] != '\0'; i++) {                          // For every character in the string, check if it's alphanumeric
        if (!isalnum(str[i]))
            return false;
    }
    return true;
}

/*
Given a double-ended queue and a vector of a split directory path, 
update the deque based on the commands seen in the list
*/
deque<string> processQueue(deque<string> deque, vector<string> splitList) {
    for (int i = 0; i < splitList.size(); i++) {
        if (splitList[i] == "..") {
            if (!deque.empty())
                deque.pop_back();
        }
        else if (splitList[i] == "." || splitList[i] == "/")
            continue;
        else
            deque.push_back(splitList[i]);
    }
    
    return deque;
}

/*
Given the current directory path and a new directory path, 
return either a new path or an error message
*/
string getNewPath(string curr_dir, string new_dir) {     
    deque<string> curr_dir_deque;                                           // Split the current directory by "/" and add to deque
    char *curr_dir_arr = &curr_dir[0];
    char *token = strtok(curr_dir_arr, "/");                            
    while (token) {
        if (token)
            curr_dir_deque.push_back(token);
        token = strtok(NULL, "/");
    }
    
    vector<string> new_dir_split;
    char *new_dir_arr = &new_dir[0];                                        // Split the new directory by "/" and keep "/"
    if (strncmp(new_dir_arr, "/", 1) == 0)
        new_dir_split.push_back("/");
    token = strtok(new_dir_arr, "/");                            
    while (token) {
        if (token) {
            new_dir_split.push_back(token);
            new_dir_split.push_back("/");
        }
        token = strtok(NULL, "/");
    }
        
    if (!isAlnumStr(new_dir_split[0])                                       // Check if the new directory path is valid
        && (new_dir_split[0] != "/") 
        && (new_dir_split[0] != ".")
        && (new_dir_split[0] != ".."))
        return (new_dir + ":" + " No such file or directory");
    
    deque<string> dq;
    if (new_dir_split[0] == "/") {                                          // If the new directory path contains a leading "/"
        deque<string> new_dir_deque;
        for (int i = 0; i < new_dir_split.size(); i++) {
            if (new_dir_split[i] != "" && isAlnumStr(new_dir_split[i])) {    // Fill deque according to the path in new_dir
                new_dir_deque.push_back(new_dir_split[i]);
                new_dir_split.erase(remove(new_dir_split.begin(), new_dir_split.end(), new_dir_split[i]), new_dir_split.end());
            }
        }
        dq = processQueue(new_dir_deque, new_dir_split);
    }
    else
        dq = processQueue(curr_dir_deque, new_dir_split);

    string result = "";                                                      // Create a path based on the deque
    if (dq.empty())
        result = "/";
    else {
        while (!dq.empty()) {
            string section = dq.front();
            dq.pop_front();
            if (section != "") {
                result += "/";
                result += section;
            }
        }
    }
    
    return result;
}

void runTests() {
    vector<pair<string, string> > testPairs{ 
            make_pair("/", "abc"), 
            make_pair("/abc/def", "ghi"),
            make_pair("/abc/def", ".."),
            make_pair("/abc/def", "/abc"),
            make_pair("/abc/def", "/abc/klm"),
            make_pair("/abc/def", "../.."),
            make_pair("/abc/def", "../../.."),
            make_pair("/abc/def", "."),
            make_pair("/abc/def", "..klm"),
            make_pair("/abc/def", "//////"),
            make_pair("/abc/def", "......"),
            make_pair("/abc/def", "../gh///../klm/.")
        };
    
    for (int i = 0; i < testPairs.size(); i++) {
        simplify(testPairs[i].second);
        cout << getNewPath(testPairs[i].first, testPairs[i].second) << endl;
    }
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        runTests();
        return 0;
    }
    else if (argc == 3) {
        string curr_directory = argv[1];
        string new_directory = argv[2];
    
        simplify(new_directory);  
        cout << getNewPath(curr_directory, new_directory) << endl;
        return 0;
    }
    else {
        cout << "Invalid number of inputs" << endl;
        return 0;
    }
}