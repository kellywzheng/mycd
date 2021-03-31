#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <cstring>
#include <locale> 

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
        regex dots("(/../)");
        smatch matches;
        
        directory = regex_replace(directory, slashes, "/");                             // replace multiple "/" to a single slash
        
        regex_search(directory, matches, dots);                                         // find the first occurence of "/../"
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

*/
bool isAlnumStr(string str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i]))
            return false;
    }
    return true;
}

stack<string> processStack(stack<string> stack, vector<string> splitList) {
    for (int i = 0; i < splitList.size(); i++) {
        if (splitList[i] == "..") {
            if (!stack.empty())
                stack.pop();
        }
        else if (splitList[i] == "." || splitList[i] == "/")
            continue;
        else
            stack.push(splitList[i]);
    }
    
    return stack;
}

string getNewPath(string curr_dir, string new_dir) {     
    stack<string> curr_dir_stack;                                       // Split the current directory by "/" and add to stack
    char *curr_dir_arr = &curr_dir[0];
    char *token = strtok(curr_dir_arr, "/");                            
    while (token) {
        if (token)
            curr_dir_stack.push(token);
        token = strtok(NULL, "/");
    }
    
    vector<string> new_dir_split;
    char *new_dir_arr = &new_dir[0];                                    // Split the new directory by "/" and keep "/"
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
        
    if (!isAlnumStr(new_dir_split[0])                                   // Check if the new directory path is valid
        && (new_dir_split[0] != "/") 
        && (new_dir_split[0] != ".")
        && (new_dir_split[0] != ".."))
        return (new_dir + ":" + " No such file or directory");
    
    stack<string> s;
    if (new_dir_split[0] == "/") {                                      // If the new directory path contains a leading "/"
        stack<string> new_dir_stack;
        for (int i = 0; i < new_dir_split.size(); i++) {
            if (new_dir_split[i] != "" && isAlnumStr(new_dir_split[i])) {    // Fill stack according to the path in new_dir
                new_dir_stack.push(new_dir_split[i]);
                new_dir_split.erase(remove(new_dir_split.begin(), new_dir_split.end(), new_dir_split[i]), new_dir_split.end());
            }
        }
        s = processStack(new_dir_stack, new_dir_split);
    }
    else
        s = processStack(curr_dir_stack, new_dir_split);

    string result = "";                                                         // Create a path based on the stack
    if (s.empty())
        result = "/";
    else {
        while (!s.empty()) {
            string section = s.top();
            s.pop();
            if (section != "") {
                result += "/";
                result += section;
            }
        }
    }
    
    return result;
}

int main(int argc, char** argv)
{
    string curr_directory = argv[1];
    string new_directory = argv[2];

    simplify(new_directory);
    cout << "new_directory: " << new_directory << endl;
    
    cout << getNewPath(curr_directory, new_directory) << endl;
    return 0;
}