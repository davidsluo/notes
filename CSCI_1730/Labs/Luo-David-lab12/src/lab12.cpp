#include <string>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    string line;
    while (true) {
        printf("repl$ ");
        getline(cin, line);
        
        vector<string> argv = {""};
        int word_index = 0;
        char prev = '\0';
        bool quote_flag = false;
        bool escape_flag = false;
        for (char c : line) {
            switch (c) {
                case ' ':
                    if (quote_flag) {
                        argv[word_index].push_back(c);
                    } else if (prev != ' ') {
                        argv.push_back("");
                        word_index++;
                    }
                    break;
                case '|':
                    break;
                case '>':
                    break;
                case '\\':
                    escape_flag = true;
                    break;
                case '"':
                    quote_flag = !quote_flag;
                    break;
                default:
                    argv[word_index].push_back(c);
                    break;
            }
            prev = c;
        }
    }
}
