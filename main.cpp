#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

vector <vector <char>> Polybius_square() {
    /*
    Функция, возвращающая квадрат Полибия:
    A B C D E
    F G H I K
    L M N O P
    Q R S T U
    V W X Y Z
    */
    vector <vector <char>> square (5, vector <char> (5));
    char letter;
    for (int row = 0; row < 5; ++row) {
        for (int column = 0; column < 5; ++column) {
            letter = 'A' + row * 5 + column;
            if (letter >= 'J') {
                ++letter;
            }
            square[row][column] = letter;
        }
    }
    return square;
}

int main() {
    return 0;
}