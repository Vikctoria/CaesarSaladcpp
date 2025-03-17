#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::string;

vector <vector <char>> Polybius_square() {
    /*
    Функция, возвращающая квадрат Полибия:
    A B C D E
    F G H I K
    L M N O P
    Q R S T U
    V W X Y Z
    */
    vector <vector <char>> square(5, vector <char>(5));
    char letter;
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++column) {
            letter = 'A' + row * 5 + column;
            square[row][column] = letter + (letter >= 'J');
        }
    }
    return square;
}

string replace_j(string message) {
    /*
    Функция меняющая в сообщении все "J" на "I"
    */
    while (message.find('J') != std::string::npos) {
        message.replace(message.find('J'), 1, 1, 'I');
    }
    return message;
}

string Pol_square_method_1(string message) {
    /*
    Функция шифрующая сообщение по одному из методов шифровки с помощью квадрата Полибия
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    message = replace_j(message);
    vector <vector <char>> polyb_square = Polybius_square();
    string encrypt_message = "";
    char new_let;
    for (char let_to_replace : message) {
        if (isalpha(let_to_replace)) {
            int row = ((let_to_replace - 'A' - (let_to_replace >= 'J')) / 5 + 1) % 5;
            int col = (let_to_replace - 'A' - (let_to_replace >= 'J')) % 5;
            new_let = polyb_square[row][col];
        } else {
            new_let = let_to_replace;
        }
        encrypt_message.push_back(new_let);
    }
    return encrypt_message;
}

int main() {
    
    return 0;
}