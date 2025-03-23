#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <chrono>
#include <thread>

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

string repeat_processing(string str) {
    /*
    Функция, вставляющая 'X' между повторяющимися буквами (если они находятся в одной биграмме)
    */
    char last_let = '#';
    string new_message = "";
    for (int i = 0; i < str.size(); ++i) {
        if (last_let == str[i] and new_message.size() % 2 != 0) {
            new_message.push_back('X');
        }
        if (isalpha(str[i])) {
            last_let = str[i];
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
    int ind_1 = key_word.find(let_1);
    if (key_word.find(let_1) == std::string::npos) {
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
        ind_1 = left_ind;
    }

    ind_row_1 = ind_1 / 5;
    ind_col_1 = ind_1 % 5;

    pair<int, int> let;
    let.first = ind_row_1;
    let.second = ind_col_1;

    return let;
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

    int ind_row_2, ind_col_2;
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

        pair<int, int> let_1_inds = find_in_square(let_1, key_word);

        ind_col_2 = (let_2 - 'A' - (let_2 >= 'J')) % 5;

        char new_let_2 = let_2 + let_1_inds.second - ind_col_2;

        encrypt_message += punct_marks_1;
        encrypt_message.push_back(square[let_1_inds.first][ind_col_2]);

        encrypt_message += punct_marks_2;
        encrypt_message.push_back((new_let_2 == 'J' ? 'I' : new_let_2));
    }
    encrypt_message += message.substr(i, message.size() - i);
    return encrypt_message;
}

string Playfair_cipher(string message, string key_word) {
    /*
    Функция, шифрующая сообщение шифром Плейфера
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    message = replace_j(message);
    message = repeat_processing(message);
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

        pair <int, int> let_1_inds = find_in_square(let_1, key_word);
        pair <int, int> let_2_inds = find_in_square(let_2, key_word);

        if (let_1_inds.first == let_2_inds.first) {
            let_1_inds.second = (let_1_inds.second + 1) % 5;
            let_2_inds.second = (let_2_inds.second + 1) % 5;
        } else if (let_1_inds.second == let_2_inds.second) {
            let_1_inds.first = (let_1_inds.first + 1) % 5;
            let_2_inds.first = (let_2_inds.first + 1) % 5;
        } else {
            int temp = let_1_inds.second;
            let_1_inds.second = let_2_inds.second;
            let_2_inds.second = temp;
        }

        encrypt_message += punct_marks_1;
        encrypt_message.push_back(square[let_1_inds.first][let_1_inds.second]);

        encrypt_message += punct_marks_2;
        encrypt_message.push_back(square[let_2_inds.first][let_2_inds.second]);
    }
    return encrypt_message;
}

string Cardan_grille(string message) {
    /*
    Функция, шифрующая сообщение при помощи решётки Кардано
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";

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
    for (int q = 0; q < 4; ++q) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                while (!isalpha(message[ind]) && ind < message.size()) {
                    ++ind;
                }
                if (grille[i][j]) {
                    encrypt_mes_table[i][j] = message[ind];
                    ++ind;
                }
                grille_rotate[j][3 - i] = grille[i][j];
            }
        }
        grille = grille_rotate;
    }
    int count_punct = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            ind = i * 4 + j + count_punct;
            while (!isalpha(message[ind])) {
                encrypt_message.push_back(message[ind]);
                ++ind;
                ++count_punct;
            }
            encrypt_message.push_back(encrypt_mes_table[i][j]);
        }
    }
    ind = 16 + count_punct;
    while (ind < message.size()) {
        encrypt_message.push_back(message[ind]);
        ++ind;
    }
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

string pig_latin_word(string message) {
    /*
    Функция, шифрующая слово поросячьей латынью
    */
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";
    vector<int> notLettersIndexes;
    for (int i = message.size() - 1; i >= 0; i--) {
        if (!isalpha(message[i])) {
            notLettersIndexes.push_back(i);
        } else {
            break;
        }
    }
    int firstNotLetterChar = -1;
    if (!notLettersIndexes.empty()) {
        firstNotLetterChar = notLettersIndexes[notLettersIndexes.size() - 1];
    } else {
        firstNotLetterChar = message.size();
    }
    if (!letter_is_consonant(message[0])) {
        encrypt_message = message;
        encrypt_message += "AY";
    } else {
        int last_consonant_letter_index = message.size();
        for (int i = 0; i < message.size(); i++) {
            if (not letter_is_consonant(message[i])) {
                last_consonant_letter_index = i;
                break;
            }
        }
        for (int i = last_consonant_letter_index; i < firstNotLetterChar; i++) {
            encrypt_message.push_back(message[i]);
        }
        for (int i = 0; i <= last_consonant_letter_index - 1; i++) {
            encrypt_message.push_back(message[i]);
        }
        encrypt_message += "AY";
    }
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
        if (!encrypt_message.empty()) {
            encrypt_message += " ";
        }
        encrypt_message += pig_latin_word(word);
    }
    return encrypt_message;
}

string caesar_cipher(string message) {
    /*
    Функция, шифрующая сообщение шифром цезаря с рандомным сдвигом
    */
    std::srand(std::time(0));
    int shift = std::rand() % 25 + 1;
    std::transform(message.begin(), message.end(), message.begin(), toupper);
    string encrypt_message = "";
    for (int i = 0; i < message.size(); i++) {
        if (!isalpha(message[i])) {
            encrypt_message += message[i];
        } else {
            encrypt_message.push_back('A' + (message[i] - 'A' + shift) % 26);
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
    while (repeated_key_word.size() != message.size()) {
        repeated_key_word.push_back(key_word[i % key_word.size()]);
        i++;
    }
    for (int i = 0; i < repeated_key_word.size(); i++) {
        while (!isalpha(message[i]) && i < message.size()) {
            encrypt_message.push_back(message[i]);
            i++;
        }
        if (isalpha(message[i])) {
            char let = (repeated_key_word[i] - 'A' + message[i]);
            encrypt_message.push_back((let > 'Z' ? let - 'Z' + 'A' - 1 : let));
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
        cout << name << "\n";
    }

    void rounds() {
        rounds_count = 1;
        while (lives > 0) {

            // настройки для корректного ввода
            play = true;
            user_response = "";
            reason = "";

            cout << "Round " << rounds_count << "\n";

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
                word = words[ind_word % words_for_grille.size()];
                ans = Cardan_grille(word);
            }


            // вывод доп информации (алфавит, квадрат Полибия, ключевое слово)
            
            // здесь красивый вывод word -> ans
            cout << word << "\n" << ans << "\n";
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
                break;
            }

            // если ответ неправильный
            if (cipher_to_use != ciphers[user_response_ind]) {
                --lives;
                cout << "Wrong Answer\n";
                cout << "correct answer is " << cipher_to_use << "\n";
                cout << "minus live)\n";
            } else {                                     // иначе ответ правильный
                ++score;
                cout << "This is the correct answer\n";
            }

            // раунд окончен даём отдохнуть 5 секунд
            ++rounds_count;
            cout << "Relax 5 seconds\n";
            std::chrono::seconds dura(5);
            std::this_thread::sleep_for(dura);
        }
        // обновляем лучший скор
        if (score > best_score) {
            best_score = score;
        }

        // вывод информации в конце серии ранудов
        cout << "End of the game" << "\n";
        cout << "Rounds played: " << rounds_count << "\n";
        cout << "Your score: " << score << "\n";
        sum_rounds_count += rounds_count;
        info(); // окончание игры вывод какой-то общей инфы
    }

    void info() {
        // вывод какой-то общей инфы
        cout << "Best score: " << best_score << "\n";
        cout << "Rounds played for all the games: " << sum_rounds_count << "\n";

        // здесь откатываем до заводских настроек свойства для следующей игры
        score = 0;
        lives = 3;
    }
};


int main() {

    string user_cin;   // ответы пользователя
    Player player;     // создаём игрока

    player.hello_world();   // здороваемся с ним

    // хочет ли он вообще играть?
    cout << "Start now? (if yes, enter \"yes\" without quotes)\n";
    cin >> user_cin;
    std::transform(user_cin.begin(), user_cin.end(), user_cin.begin(), toupper);

    // цикл игр
    while (user_cin == "YES") {
        player.rounds();

        // хочет ли он ещё?
        cout << "Play again? (if yes, enter \"yes\" without quotes)\n";
        cin >> user_cin;
        std::transform(user_cin.begin(), user_cin.end(), user_cin.begin(), toupper);
    }

    // вывод про конец всех игр и пожелание возвращаться в игру
    cout << "Bye bye\n";

    player.info();   // вывод информации по всем играм

    // ждём 5 секунд, пусть любуется концом
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);
    return 0;
}