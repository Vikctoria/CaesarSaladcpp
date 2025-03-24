#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <chrono>
#include <thread>
#include <windows.h>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::pair;
using std::istringstream;


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
    vector <vector <char>> square(5, vector <char>(5));   // матрица с квадратом
    int row = 0, column = 0;
    int let_ind;

    // начинаем заполнять нашу матрицу
    // кладём туда сначала символы ключевого слова
    for (char letter : key_word) {
        square[row][column] = letter;
        row = row + (column + 1) / 5;
        column = (column + 1) % 5;
    }
    // потом заполняем square оставшимися символами алфавита
    char letter = 'A';
    while (row < 5) {
        while (column < 5) {
            while (key_word.find(letter) != std::string::npos) {
                ++letter;   // если символ в ключевом слове, то пропускаем его
            }
            square[row][column] = letter;
            ++letter;
            letter += (letter == 'J');   // пропускаем 'J'
            ++column;
        }
        ++row;
        column = 0;
    }
    return square;   // возвращаем квадрат Полибия
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

string repeat_processing(string str) {
    /*
    Функция, вставляющая 'X' между повторяющимися буквами (если они находятся в одной биграмме)
    */
    char last_let = '#';
    string new_message = "";
    for (int i = 0; i < str.size(); ++i) {
        // вставляем 'X', если последняя и нынешняя буква совпадают и длина сообщения нечётна
        if (last_let == str[i] and new_message.size() % 2 != 0) {
            new_message.push_back('X');
        }
        if (isalpha(str[i])) {
            last_let = str[i];   // запоминаем последнюю букву
        }
        new_message.push_back(str[i]);
    }
    return new_message;
}

pair<int, int> find_in_square(char let_1, string key_word) {
    /*
    Функция, которая ищет букву let_1 бинпоиском или функцией find в квадрате Полибия с ключом
    */
    vector <vector <char>> square = Polybius_square(key_word);
    int ind_row_1, ind_col_1;
    int ind_1 = key_word.find(let_1);   // если буква в key_word, то индекс берём оттуда, индекс (если вытянуть квадрат в строку)
    if (key_word.find(let_1) == std::string::npos) {   // если нет, то ищем её по квадрату
        // бинпоиск
        int left_ind = key_word.size(), right_ind = 24, mid_ind;
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
        ind_1 = left_ind;   // ответ в левом индексе
    }

    ind_row_1 = ind_1 / 5;   // индекс строки в матрице
    ind_col_1 = ind_1 % 5;   // индекс столбца в матрице

    // удобный возрат функции, индексы в pair <строка, столбец>
    pair<int, int> let;
    let.first = ind_row_1;
    let.second = ind_col_1;

    return let;   // возвращаем индексы
}

string Pol_square_method_1(string message) {
    /*
    Функция, шифрующая сообщение по одному из методов шифровки с помощью стандартного квадрата Полибия (без ключа)
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);   // message в верхний регистр
    message = replace_j(message);   // удаляем 'J'
    string encrypt_message = "";
    char new_let;
    for (char let_to_replace : message) {
        if (isalpha(let_to_replace)) {
            // к букве добавляем сдвиг 5 (тот же столбец но строка + 1) и вычитаем 1, если буква при шифровке "перескочит" или остановится на 'J'
            new_let = (let_to_replace + 5 + ('J' - 5 <= let_to_replace && let_to_replace <= 'J'));
            new_let = (new_let > 'Z' ? new_let - 26 : new_let);   // новая буква из квадрата
        } else {
            new_let = let_to_replace;   // не буква
        }
        encrypt_message.push_back(new_let);
    }
    return encrypt_message;   // возвращаем зашифрованное сообщение
}

string Wheatstone_cipher(string message, string key_word) {
    /*
    Функция, шифрующая сообщение шифром Уитстона
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);   // message в верхний регистр
    message = replace_j(message);   // удаляем 'J'

    // если нечётное кол-во буквенных символов, то добавляем в конец 'X'
    if (size_alph(message) % 2 != 0) {
        message.push_back('X');
    }

    string encrypt_message = "";
    vector <vector <char>> square = Polybius_square(key_word);   // квадрат Полибия с данным ключом

    int ind_row_2, ind_col_2;
    int i;
    for (i = 0; i < message.size() && size_alph(message.substr(i, message.size() - i)); i += 2) {
        string punct_marks_1 = "", punct_marks_2 = "";
        while (!isalpha(message[i])) {
            punct_marks_1.push_back(message[i]);   // собираем всю пунктуацию до следующей буквы
            ++i;
        }
        char let_1 = message[i];   // буква 1
        while (!isalpha(message[i + 1])) {
            punct_marks_2.push_back(message[i + 1]);   // собираем всю пунктуацию до следующей буквы
            ++i;
        }
        char let_2 = message[i + 1];   // буква 2

        pair<int, int> let_1_inds = find_in_square(let_1, key_word);   // находим индексы let_1 в квадрате

        ind_col_2 = (let_2 - 'A' - (let_2 >= 'J')) % 5;   // индекс let_2 в стандартном квадрате Полибия без ключа

        char new_let_2 = let_2 + let_1_inds.second - ind_col_2;   // новая вторая буква (сдвигаем столбец старой второй буквы)

        encrypt_message += punct_marks_1;                                 // добавление пунктуации до первой буквы
        encrypt_message.push_back(square[let_1_inds.first][ind_col_2]);   // добавление новой первой буквы

        encrypt_message += punct_marks_2;                                  // добавление пунктуации до второй буквы
        encrypt_message.push_back((new_let_2 == 'J' ? 'I' : new_let_2));   // добавление новой второй буквы
    }
    encrypt_message += message.substr(i, message.size() - i);
    return encrypt_message;   // возвращаем зашифрованное сообщение
}

string Playfair_cipher(string message, string key_word) {
    /*
    Функция, шифрующая сообщение шифром Плейфера
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);   // message в верхний регистр
    message = replace_j(message);   // удаляем 'J'
    message = repeat_processing(message);   // исключаем повторение двух символов в биграмме
    // если нечётное кол-во буквенных символов, то добавляем в конец 'X'
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
            punct_marks_1.push_back(message[i]);   // собираем всю пунктуацию до следующей буквы
            ++i;
        }
        char let_1 = message[i];   // буква 1
        while (!isalpha(message[i + 1])) {
            punct_marks_2.push_back(message[i + 1]);   // собираем всю пунктуацию до следующей буквы
            ++i;
        }
        char let_2 = message[i + 1];   // буква 2

        pair <int, int> let_1_inds = find_in_square(let_1, key_word);   // находим индексы let_1 в квадрате
        pair <int, int> let_2_inds = find_in_square(let_2, key_word);   // находим индексы let_2 в квадрате

        if (let_1_inds.first == let_2_inds.first) {            // если буквы в одной строке, то новые буквы стоят правее старых
            let_1_inds.second = (let_1_inds.second + 1) % 5;   // новая первая буква
            let_2_inds.second = (let_2_inds.second + 1) % 5;   // новая вторая буква
        } else if (let_1_inds.second == let_2_inds.second) {   // если буквы в одном столбце, то новые буквы стоят под старыми буквами
            let_1_inds.first = (let_1_inds.first + 1) % 5;     // новая первая буква
            let_2_inds.first = (let_2_inds.first + 1) % 5;     // новая вторая буква
        } else {       // иначе новые буквы стоят в других углах прямоугольника (но в тех же строках), который образован старыми буквами
            int temp = let_1_inds.second;           // меняем столбцы местами
            let_1_inds.second = let_2_inds.second;
            let_2_inds.second = temp;
        }

        encrypt_message += punct_marks_1;                                         // добавление пунктуации до первой буквы
        encrypt_message.push_back(square[let_1_inds.first][let_1_inds.second]);   // добавление новой первой буквы

        encrypt_message += punct_marks_2;                                         // добавление пунктуации до второй буквы
        encrypt_message.push_back(square[let_2_inds.first][let_2_inds.second]);   // добавление новой второй буквы
    }
    return encrypt_message;   // возвращаем зашифрованное сообщение
}

string Cardan_grille(string message) {
    /*
    Функция, шифрующая сообщение при помощи решётки Кардано
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);   // message в верхний регистр
    string encrypt_message = "";

    // решётка, при помощи которой будем шифровать
    vector <vector <int>> grille = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0}
    };
    vector <vector <int>> grille_rotate(4, vector <int>(4));
    vector <vector <char>> encrypt_mes_table(4, vector <char>(4));

    string no_alph = "";
    int ind = 0;
    // крутим решётку 4 раза и получаем зашифрованное сообщение в матрице encrypt_mes_table
    for (int q = 0; q < 4; ++q) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                while (!isalpha(message[ind]) && ind < message.size()) {
                    ++ind;   // пропускаем не буквенные символы
                }
                if (grille[i][j]) {
                    encrypt_mes_table[i][j] = message[ind];   // если в решётке 1, то берём букву
                    ++ind;
                }
                grille_rotate[j][3 - i] = grille[i][j];   // новая повёрнутая решётка
            }
        }
        grille = grille_rotate;   // разворот решётки
    }
    int count_punct = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ind = i * 4 + j + count_punct;   // индекс символа в message
            while (!isalpha(message[ind])) {
                // добавление пунктуации
                encrypt_message.push_back(message[ind]);
                ++ind;
                ++count_punct;
            }
            // добавление новой буквы
            encrypt_message.push_back(encrypt_mes_table[i][j]);
        }
    }
    // добавление "хвоста" пунктуации
    ind = 16 + count_punct;
    while (ind < message.size()) {
        encrypt_message.push_back(message[ind]);
        ++ind;
    }
    return encrypt_message;   // возвращаем зашифрованное сообщение
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

string pig_latin_word(string message) {
    /*
    Функция, шифрующая слово поросячьей латынью
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";
    vector<int> notLettersIndexes;
    // Ищем индексы, на которых стоят не буквы
    for (int i = message.size() - 1; i >= 0; i--) {
        if (!isalpha(message[i])) {
            notLettersIndexes.push_back(i);
        } else {
            break;
        }
    }
    int firstNotLetterChar = -1; // Индекс первого небуквенного элемента с конца
    if (!notLettersIndexes.empty()) {
        firstNotLetterChar = notLettersIndexes[notLettersIndexes.size() - 1];
    } else {
        firstNotLetterChar = message.size();
    }
    if (!letter_is_consonant(message[0])) { // Если первый символ - гласная, то просто добавляем "AY" в конец исходного слова
        encrypt_message = message;
        encrypt_message += "AY";
    } else {
        int last_consonant_letter_index = message.size(); // Индекс последней согласной буквы
        for (int i = 0; i < message.size(); i++) {
            if (not letter_is_consonant(message[i])) {
                last_consonant_letter_index = i;
                break;
            }
        }
        // Добавляем символы, начиная с первой гласной
        for (int i = last_consonant_letter_index; i < firstNotLetterChar; i++) {
            encrypt_message.push_back(message[i]);
        }
        // Добавляем все согласные до первой гласной
        for (int i = 0; i <= last_consonant_letter_index - 1; i++) {
            encrypt_message.push_back(message[i]);
        }
        encrypt_message += "AY";
    }
    // В конец добавляем небуквенные символы
    for (int i = notLettersIndexes.size() - 1; i >= 0; i--) {
        encrypt_message.push_back(message[notLettersIndexes[i]]);
    }
    return encrypt_message;
}

string pig_latin(string message) {
    /*
    Функция, шифрующая сообщение поросячьей латынью
    */
    istringstream fin(message);
    string word;
    string encrypt_message = "";
    while (fin >> word) {
        if (!encrypt_message.empty()) { // Если слово не пустое, добавляем пробел
            encrypt_message += " ";
        }
        // Шифруем текущее слово и добавляем его
        encrypt_message += pig_latin_word(word);
    }
    return encrypt_message;
}

string caesar_cipher(string message) {
    /*
    Функция, шифрующая сообщение шифром цезаря с рандомным сдвигом
    */
    std::srand(std::time(0));
    int shift = std::rand() % 25 + 1; // Генерируем случайное число от 1 до 25
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";
    for (int i = 0; i < message.size(); i++) {
        if (!isalpha(message[i])) { // Если символ - не буква, то просто добавляем его
            encrypt_message += message[i];
        } else {
            encrypt_message.push_back('A' + (message[i] - 'A' + shift) % 26); // Если символ - буква, то шифруем его сдвигом
        }
    }
    return encrypt_message;
}

string vigener_cipher(string message, string key_word) {
    /*
    Функция, шифрующая сообщение шифром Вижинера
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";
    string repeated_key_word = "";
    int i = 0;
    // Создаем повторяющееся ключевое слово, дублируя ключевое слово так, чтобы длина совпадала с длиной исходного слова
    while (repeated_key_word.size() != message.size()) {
        repeated_key_word.push_back(key_word[i % key_word.size()]);
        i++;
    }
    for (int i = 0; i < repeated_key_word.size(); i++) {
        while (!isalpha(message[i]) && i < message.size()) { // Если символ - не буква, то просто добавляем его
            encrypt_message.push_back(message[i]);
            i++;
        }
        if (isalpha(message[i])) { // Шифруем букву
            char let = (repeated_key_word[i] - 'A' + message[i]);
            encrypt_message.push_back((let > 'Z' ? let - 'Z' + 'A' - 1 : let)); // Не даем букве выйти за границы алфавита
        }
    }
    return encrypt_message;
}

// глобальные переменные для двух потоков и не только
string user_response;   // ввод пользователя
bool play;              // ждём ли мы ещё ответа пользователя
string reason;          // почему закончили ждать

void input() {
    /*
    Функция для ввода с клавиатуры ответа
    */

    // ждём пока пользователь не введёт что-нубудь или не закончится время
    while (play && user_response == "") {
        cout << "input:\n";
        cin >> user_response;
        cout << "Your choice: " << user_response << "\n";
    }
    play = false;         // раунд окончен
    if (reason == "") {   // если причины окончания ещё нет
        reason = "in";    // то причина - ввод пользователя
    }
}

void sleep() {
    /*
    Функция, которая следит за временем
    */

    time_t start, end;   // время начала и конца замеров
    time(&start);
    time(&end);

    // ждём пока пользователь не введёт что-нубудь или не закончится время
    while (double(end - start) < 30 && play) {
        time(&end);
    }
    double time_taken = double(end - start);   // время, затраченное на ответ
    cout << "It took " << time_taken << " seconds to respond\n";
    play = false;         // раунд окончен
    if (reason == "") {   //если причины окончания ещё нет, то причина - истёкшее время
        // сообщаем об этом пользователю
        cout << "You've overstayed your time. Get out? (if yes, enter \"yes\" without quotes)\n";
        reason = "time";
    }
}



string CoutCentered(string text) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    PCONSOLE_SCREEN_BUFFER_INFO lpScreenInfo = new CONSOLE_SCREEN_BUFFER_INFO();
    GetConsoleScreenBufferInfo(hConsole, lpScreenInfo);
    COORD NewSBSize = lpScreenInfo->dwSize;
    if (NewSBSize.X > text.size()) {
        int newpos = ((NewSBSize.X - text.size()) / 2);
        for (int i = 0; i < newpos; i++) cout << " ";
    }
    return text;
}

void Boldness(string text){
    char esc_char = 27;
    cout << esc_char << "[1m" << text << esc_char << "[0m" << std::endl; // вывод жирным
}

void Heart(int count) {
    system("cls");
    string top = "/\\/\\";
    string mid = "\\  /";
    string bot = " \\/ ";
    if (count == 2) {
        cout << top << " " << top << " " << "\033[5m" << top << "\033[0m" << "\n"
            << mid << " " << mid << " " << "\033[5m" << mid << "\033[0m" << "\n"
            << bot << " " << bot << " " << "\033[5m" << bot << "\033[0m" << "\n"; //Blinking text
    } else if (count == 1) {
        cout << top << " " << "\033[5m" << top << "\033[0m" << "\n"
            << mid << " " << "\033[5m" << mid << "\033[0m" << "\n"
            << bot << " " << "\033[5m" << bot << "\033[0m" << "\n"; //Blinking text
    } else {
        cout << "\033[5m" << top << "\033[0m" << "\n"
            << "\033[5m" << mid << "\033[0m" << "\n"
            << "\033[5m" << bot << "\033[0m" << "\n"; //Blinking text
    }
    std::chrono::seconds duraw(2);
    std::this_thread::sleep_for(duraw);
    system("cls");
    if (count == 2) {
        cout << top << " " << top << "\n"
            << mid << " " << mid << "\n"
            << bot << " " << bot << "\n";
    } else if (count == 1) {
        cout << top << "\n"
            << mid << "\n"
            << " \\/ " << "\n";
    }
    std::chrono::seconds duraww(1);
    std::this_thread::sleep_for(duraww);
    system("cls");
}

void Good(int count) {
    system("cls");
    string top = "/\\/\\";
    string mid = "\\  /";
    string bot = " \\/ ";
    if (count == 2) {
        cout << top << " " << top << "\n"
            << mid << " " << mid << "\n"
            << bot << " " << bot << "\n";
    } else if (count == 1) {
        cout << top << "\n"
            << mid << "\n"
            << " \\/ " << "\n";
    } else {
        cout << top << " " << top << " " << top << "\n"
            << mid << " " << mid << " " << mid << "\n"
            << bot << " " << bot << " " << bot << "\n";
    }
    std::chrono::seconds duraww(2);
    std::this_thread::sleep_for(duraww);
    system("cls");
}

int Instr() {
    system("cls");
    string instruction = "Welcome to the CaesarSalad game!\n\
    Here you'll have to be smart and even lucky!\n\
    You have to guess which cipher a message is encrypted with.\n\
    There are 7 ciphers in the game:\n\
    Pol_square_method_1, Wheatstone_cipher, Playfair_cipher,\n\
    Cardan_grille, pig_latin, caesar_cipher, vigener_cipher.\n\n\
    The rules of the game are simple: you will be given the original message and the same message, but encrypted,\n\
    you will need to choose one of the four suggested answers and enter the answer number(1, 2, 3 or 4).\n\
    Each round lasts 30 seconds, you need to be able to enter the answer in the given time!!\n\
    You will initially have 3 lives and the game will last until they are gone.\n\
    Lives are removed for\n\
    1) Incorrect answers\n\
    2) Incorrect input\n\
    3) Expired time.\n\
    Good luck! ^_^\n\
    Prove that you really know the ciphers!\n";
    Boldness(CoutCentered(instruction));
    system("Color 0B");
    cout << "Enter anything to continue: ";
    string pl_an;
    cin >> pl_an;
    return 0;
}

class Player {
private:
    string name = "Young cryptographer";   // как мы его будем называть
    int score = 0;                         // его набранные очки
    int best_score = 0;                    // его набранные очки за лучшую игру
    int lives = 3;                         // количество жизней
    int rounds_count = 1;                  // номер раунда now
    int sum_rounds_count = 0;              // количество сыгранных раундов за все игры

    // названия шифров
    vector <string> cipher_names = {
        "Pol_square_method_1", "Wheatstone_cipher", "Playfair_cipher",
        "Cardan_grille", "pig_latin", "caesar_cipher", "vigener_cipher"
    };

    // фразы для шифрования
    vector <string> words = {
        "Never gonna give you up", "Never gonna let you down", "I feel so sigma",
        "Road work ahead? Yeah, I sure hope it does", "I got diagnosed with ligma", "Deez nuts",
        "I'm out of ideas for now", "I'm in Spain without the s", "CaesarSalad on top",
        "Hello, cats and dogs!", "Hello, World!", "I'm a crepe I'm a weird dough",
        "Opa Gangamstyle", "What does the fox say?", "Your mom", "Death is inherited", "Compilation error",
        "String is not allowed"
    };

    // фразы для шифрования шифром Cardan_grille (для него нужна длина буквенных символов 16)
    vector <string> words_for_grille = {
        "Hello, cats and dogs!", "CaesarSalad on top", "Death is inherited", "Compilation error"
    };

    // ключевые слова
    vector <string> keys = {
        "KEYWORD", "WHEATSON", "WORK"
    };

public:
    void hello_world() {
        // привет пользователю
        string start = "Hello, Young cryptographer!";
        string wel = "Welcome to Caesar Salad Cifer Game!";
        Boldness(CoutCentered(start));
        system("Color 0A");
        Boldness(CoutCentered(wel));
    }

    void RoundCount(int count) {
        system("cls");
        cout << "\x1B[33mRound " << count << "\033[0m\t\t" << "\n";
    }

    void rounds() {
        while (lives > 0) {

            // настройки для корректного ввода
            play = true;
            user_response = "";
            reason = "";

            RoundCount(rounds_count);

            // выбираем 4 случайных шифра
            vector <string> for_selection = cipher_names;
            vector <string> ciphers = {};
            for (int i = 0; i < 4; ++i) {
                std::srand(std::time(0));
                int ind = std::rand() % for_selection.size();
                ciphers.push_back(for_selection[ind]);
                for_selection.erase(for_selection.begin() + ind);
            }

            // случайный выбор одного шифра из четырёх
            std::srand(std::time(0));
            int ind_cipher = std::rand() % 4;

            // случайный выбор слова для шифровки
            std::srand(std::time(0));
            int ind_word = std::rand();
            string word = words[ind_word % words.size()];   // слово для шифровки

            // случайный выбор ключа для шифровки (даже если он не нужен)
            std::srand(std::time(0));
            string key = keys[std::rand() % keys.size()];

            string cipher_to_use = ciphers[ind_cipher];   // используемый шифр
            string ans;   // результат шифрования
            
            // применение выпавшей шифровки
            if (cipher_to_use == "Pol_square_method_1") { ans = Pol_square_method_1(word); }
            else if (cipher_to_use == "Wheatstone_cipher") { ans = Wheatstone_cipher(word, key); }
            else if (cipher_to_use == "Playfair_cipher") { ans = Playfair_cipher(word, key); }
            else if (cipher_to_use == "pig_latin") { ans = pig_latin(word); }
            else if (cipher_to_use == "caesar_cipher") { ans = caesar_cipher(word); }
            else if (cipher_to_use == "vigener_cipher") { ans = vigener_cipher(word, key); }
            else if (cipher_to_use == "Cardan_grille") {
                word = words_for_grille[ind_word % words_for_grille.size()];
                ans = Cardan_grille(word);
            }


            // вывод доп информации (алфавит, квадрат Полибия, ключевое слово)

            cout << "Alphabet:" << "\n" << "\x1B[96mA B C D E F G H I J K L M\033[0m\t\t" << "\n";
            cout << "\x1B[96mN O P Q R S T U V W X Y Z\033[0m\t\t" << "\n";
            
            cout << "Polybius square:" << "\n";
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    cout << "\x1B[96m" << Polybius_square(key)[i][j] << "\033[0m\t\t" << " ";
                }
                cout << "\n";
            }

            // здесь красивый вывод word -> ans
            cout << "\x1B[95m" << word << "\n" << " || || ||" << "\n" << " \\/ \\/ \\/" << "\n" << ans << "\033[0m\t\t" << "\n";
            // здесь красивый вывод поочереди вариантов ответа (ciphers)
            for (int i = 1; i < 5; ++i) {
                cout << i << ". " << ciphers[i-1] << "\n";
            }

            // запуск ввода
            std::thread in(input);
            std::thread wait(sleep);

            in.join();
            wait.join();

            // если ввод окончился из-за TL
            if (reason == "time") {
                // может он хочет сыграть этот раунд заново?
                if (user_response == "yes") {
                    break;
                } else {
                    --lives;
                    ++rounds_count;
                    Heart(lives);
                    continue;
                }
            }

            // обработка ответа пользователя
            int user_response_ind = 0;
            if (user_response == "1") { user_response_ind = 0; }        // выбрал вариант ответа 1
            else if (user_response == "2") { user_response_ind = 1; }   // выбрал вариант ответа 2
            else if (user_response == "3") { user_response_ind = 2; }   // выбрал вариант ответа 3
            else if (user_response == "4") { user_response_ind = 3; }   // выбрал вариант ответа 4
            else {                                                      // ввёл что-то не то, за такое наказываем)
                cout << "incorrect input\n";
                cout << "minus live)\n";
                --lives;
                ++rounds_count;
                std::chrono::seconds dap(2);
                std::this_thread::sleep_for(dap);
                Heart(lives);
                continue;
            }

            // если ответ неправильный
            string anss;
            if (cipher_to_use != ciphers[user_response_ind]) {
                --lives;
                anss = "false";
                Boldness("\x1B[31mWrong Answer\033[0m\t\t");
                cout << "\n" << "correct answer is " << cipher_to_use << "\n";
                cout << "minus live)\n";
            } else {                                     // иначе ответ правильный
                ++score;
                anss = "true";
                Boldness("\x1B[92mThis is the correct answer\033[0m\t\t");
                cout << "\n";
            }
            
            std::chrono::seconds durak(2);
            std::this_thread::sleep_for(durak);
            
            if (anss == "false") {
                Heart(lives);
            } else {
                Good(lives);
            }

            // раунд окончен даём отдохнуть 6 секунд
            ++rounds_count;
            system("cls");
            if (lives > 0) {
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                    << "    z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds dura(1);
                std::this_thread::sleep_for(dura);
                system("cls");
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                    << "  z z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds da(1);
                std::this_thread::sleep_for(da);
                system("cls");
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                    << "Z z z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds pip(1);
                std::this_thread::sleep_for(pip);
                system("cls");
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                    << "    z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds aaa(1);
                std::this_thread::sleep_for(aaa);
                system("cls");
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                    << "  z z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds eee(1);
                std::this_thread::sleep_for(eee);
                system("cls");
                cout << "Relax 6 seconds\n";
                cout << "      /|      _,,,---,,_\n"
                 << "Z z z /,`.-'`'    -.  ;-;;,_\n"
                    << "     |,4-  ) )-,_. ,| (  `'-'\n"
                    << "    '---''(_/--'  `-'|_)  \n";
                std::chrono::seconds pipi(1);
                std::this_thread::sleep_for(pipi);
                system("cls");
            }
            
            
        }
        // обновляем лучший скор
        if (score > best_score) {
            best_score = score;
        }

        // вывод информации в конце серии ранудов
        system("cls");
        string b_ss = "Your score: " + std::to_string(score);
        string r_pp = "Rounds played: " + std::to_string(rounds_count);
        string en = "End of the game";
        string uno = "    /\\_____/\\    ";
        string uno2 = "   /  o   o  \\   ";
        string uno3 = "  ( ==  ^  == )  ";
        string uno4 = "   )         (   ";
        string uno5 = "  (           )  ";
        string uno6 = " ( (  )   (  ) ) ";
        string uno7 = "(__(__)___(__)__)";
        Boldness(CoutCentered(b_ss));
        Boldness(CoutCentered(r_pp));
        Boldness(CoutCentered(en));
        Boldness(CoutCentered(uno));
        Boldness(CoutCentered(uno2));
        Boldness(CoutCentered(uno3));
        Boldness(CoutCentered(uno4));
        Boldness(CoutCentered(uno5));
        Boldness(CoutCentered(uno6));
        Boldness(CoutCentered(uno7));
        system("Color 0D");
        sum_rounds_count += rounds_count - 1;
    }

    void reset() {
        // здесь откатываем до заводских настроек свойства для следующей игры
        rounds_count = 1;
        score = 0;
        lives = 3;
    }

    void info() {
        // вывод какой-то общей инфы
        system("cls");
        string b_s = "Best score: " + std::to_string(best_score);
        string r_p = "Rounds played for all the games: " + std::to_string(sum_rounds_count);
        string by = "Bye bye!";
        string uno = "                   _ |\\_  ";
        string uno2 = "                   \\` ..\\ ";
        string uno3 = "              __,.-\" =__Y=";
        string uno4 = "            .\"        )   ";
        string uno5 = "      _    /   ,    \\/\\_  ";
        string uno6 = "     ((____|    )_-\\ \\_-` ";
        string uno7 = "     `-----'`-----` `--`  ";
        Boldness(CoutCentered(b_s));
        Boldness(CoutCentered(r_p));
        Boldness(CoutCentered(by));
        Boldness(CoutCentered(uno));
        Boldness(CoutCentered(uno2));
        Boldness(CoutCentered(uno3));
        Boldness(CoutCentered(uno4));
        Boldness(CoutCentered(uno5));
        Boldness(CoutCentered(uno6));
        Boldness(CoutCentered(uno7));
        
        system("Color 0D");

        
    }
};


int main() {
    system("cls");

    string user_cin;   // ответы пользователя
    Player player;     // создаём игрока

    player.hello_world();   // здороваемся с ним

    // хочет ли он вообще играть?
    string first = "Start now? (if yes, enter \"yes\" without quotes)";
    Boldness(CoutCentered(first));
    cin >> user_cin;
    std::transform(user_cin.begin(), user_cin.end(), user_cin.begin(), toupper);
    system("cls");
    Instr();

    // цикл игр
    while (user_cin == "YES") {
        
        player.rounds();

        // хочет ли он ещё?
        cout << "\x1B[95m" << CoutCentered("Play again? (if yes, enter \"yes\" without quotes)") << "\033[0m\t\t" << "\n";
        cin >> user_cin;
        player.reset();
        std::transform(user_cin.begin(), user_cin.end(), user_cin.begin(), toupper);
    }

    // вывод про конец всех игр и пожелание возвращаться в игру


    player.info();   // вывод информации по всем играм

    // ждём 5 секунд, пусть любуется концом
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);
    return 0;
}