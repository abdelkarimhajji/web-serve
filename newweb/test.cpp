#include <string>
#include <iostream>

int main() {
    std::string str = "Hello,\n world!";
    size_t found = str.find("word");
    if (found < str.length()) {
        std::cout << "'world' found at: " << found << '\n';
        std::cout << "'world " << str.length() << '\n';
    } else {
        std::cout << "'world' not found\n";
        std::cout << "'world' found at: " << found << '\n';
        std::cout << "'world " << str.length() << '\n';
    }
    return 0;
}