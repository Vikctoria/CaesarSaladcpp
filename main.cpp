#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::string;

vector <vector <char>> Polybius_square(string key_word) {
    /*
    Функция, возвращающая квадрат Полибия с ключевым словом.
    Пример:
    Ключ: "KEYWORD"

    K E Y W O
    R D A B C
    F G H I L
    M N P Q S
    T U V X Z
    */
    vector <vector <char>> square(5, vector <char>(5));
    int row = 0, column = 0;
    int let_ind;
    for (char letter : key_word) {
        square[row][column] = letter;
        row = row + (column + 1) / 5;
        column = (column + 1) % 5;
    }
    char letter = 'A';
    while (row < 5) {
        while (column < 5) {
            while (key_word.find(letter) != std::string::npos) {
                ++letter;
            }
            square[row][column] = letter;
            ++letter;
            letter += (letter == 'J');
            ++column;
        }
        ++row;
        column = 0;
    }
    return square;
}

int size_alph(string str) {
    /*
    Функция, возвращающая количество буквенных символов в строке
    */
    int length = 0;
    for (char let : str) {
        length += (isalpha(let));
    }
    return length;
}

string replace_j(string message) {
    /*
    Функция, меняющая в сообщении все "J" на "I"
    */
    while (message.find('J') != std::string::npos) {
        message.replace(message.find('J'), 1, 1, 'I');
    }
    return message;
}

string Pol_square_method_1(string message) {
    /*
    Функция, шифрующая сообщение по одному из методов шифровки с помощью квадрата Полибия
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    message = replace_j(message);
    string encrypt_message = "";
    char new_let;
    for (char let_to_replace : message) {
        if (isalpha(let_to_replace)) {
            new_let = (let_to_replace + 5 + ('J' - 5 <= let_to_replace && let_to_replace <= 'J'));
            new_let = (new_let > 'Z' ? new_let - 26 : new_let);
        } else {
            new_let = let_to_replace;
        }
        encrypt_message.push_back(new_let);
    }
    return encrypt_message;
}

string Wheatstone_cipher(string message, string key_word) {
    /*
    Функция, шифрующая сообщение шифром Уитстона
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    message = replace_j(message);
    if (size_alph(message) % 2 != 0) {
        message.push_back('X');
    }
    string encrypt_message = "";

    vector <vector <char>> square = Polybius_square(key_word);
    int ind_row_1, ind_col_1, ind_row_2, ind_col_2;
    int i;
    for (i = 0; i < message.size() && size_alph(message.substr(i, message.size() - i)); i += 2) {
        string punct_marks_1 = "", punct_marks_2 = "";
        while (!isalpha(message[i])) {
            punct_marks_1.push_back(message[i]);
            ++i;
        }
        char let_1 = message[i];
        while (!isalpha(message[i + 1])) {
            punct_marks_2.push_back(message[i + 1]);
            ++i;
        }
        char let_2 = message[i + 1];
        int ind_1 = key_word.find(let_1);
        if (key_word.find(let_1) == std::string::npos) {
            int left_ind = key_word.size(), right_ind = 25, mid_ind;
            char is_it_my_let;
            while (left_ind < right_ind) {
                mid_ind = (left_ind + right_ind + 1) / 2;
                is_it_my_let = square[mid_ind / 5][mid_ind % 5];
                if (is_it_my_let > let_1) {
                    right_ind = mid_ind - 1;
                } else {
                    left_ind = mid_ind;
                }
            }
            ind_1 = left_ind;
        }
        ind_row_1 = ind_1 / 5;
        ind_col_1 = ind_1 % 5;

        ind_row_2 = (let_2 - 'A' - (let_2 >= 'J')) / 5;
        ind_col_2 = (let_2 - 'A' - (let_2 >= 'J')) % 5;

        encrypt_message += punct_marks_1;
        encrypt_message.push_back(square[ind_row_1][ind_col_2]);

        encrypt_message += punct_marks_2;
        let_2 = let_2 + ind_col_1 - ind_col_2;
        encrypt_message.push_back((let_2 == 'J' ? 'I' : let_2));
    }
    encrypt_message += message.substr(i, message.size() - i);
    return encrypt_message;
}

bool letter_is_consonant(char letter) {
    /*
    Функция, проверяющая, является ли буква согласной
    */
    vector<char> consonants = {
        'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M',
        'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X', 'Z'
    };
    letter = toupper(letter);
    for (int i = 0; i < consonants.size(); i++) {
        if (letter == consonants[i]) {
            return 1;
        }
    }
    return 0;
}

string pig_latin(string message) {
    /*
    Функция, шифрующая сообщение поросячьей латынью
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    message = replace_j(message);
    string encrypt_message = "";
    if (!letter_is_consonant(message[0])) {
        encrypt_message = message;
        encrypt_message += "AY";
        return encrypt_message;
    }
    int last_consonant_letter_index = -1;
    for (int i = 0; i < message.size(); i++) {
        if (not letter_is_consonant(message[i])) {
            last_consonant_letter_index = i;
            break;
        }
    }
    for (int i = last_consonant_letter_index + 1; i < message.size(); i++) {
        encrypt_message.push_back(message[i]);
    }
    for (int i = 0; i <= last_consonant_letter_index; i++) {
        encrypt_message.push_back(message[i]);
    }
    encrypt_message += "AY";
    return encrypt_message;
}

int main() {
    
    return 0;
}