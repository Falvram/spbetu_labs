//Вариант 8
// Program SyntaxAnalysisOfBracket;
// вариант с синхронным выводом входной строки (до места ошибки включительно)
/* Определения (синтаксис)
простое_логическое::= TRUE | FALSE | простой_идентификатор |
NOT простое_логическое
(простое_логическое знак_операции простое_логическое)
простой-идентификатор::=буква
знак-операции::= AND | OR
*/
#include <iostream>
#include <strstream>
#include <fstream>
using namespace std ;

bool Logical (strstream &x, string word);
bool Mark (string word );
string Not(strstream &x, string word);
string Space(char str0[]);
void Error (short k);

int main ()
{
    string word;
    string str;
    char str0[1000];
    short k;
    bool b;
    char c = '\0';
    setlocale (0,"Rus");

    while(k != 3){
        b = false;
        cout << endl <<"Анализатор выражения:" << endl << "1 - чтение из файла, 2 - ввод с клавиатуры, 3 - выход из программы." << endl;
        cin >> k;

        switch (k) {
        case 1:{
            strstream x;
            cout << "Введите имя файла" << endl;
            cin >> str;
            ifstream outfile(str);
            if (!outfile) { cout << "Входной файл не открыт!" << endl; break; }
            outfile.read(str0, 1000);
            outfile.close();
            str = Space(str0);
            x << str;
            if(x >> word){
                cout << word << " ";
                word = Not(x, word);
                if(word[0] && word[0] != '('){ Error(2); return 0; }
                b = Logical(x, word);
            }
            else { Error(0); b = false; }
            x >> c;
            if(c != '\0' && b){ Error(1); b = false; }
            break;
        }
        case 2:{
            cout << "Введите строку" ;
            cin.get();
            cin.getline(str0, 1000);
            strstream x;
            str = Space(str0);

            x << str;
            if(x >> word)
            {
                cout << word << " ";
                word = Not(x, word);
                if(word[0] != '\0' && word[0] != '('){ Error(2); break; }
                if(word[0] == '\0'){ Error(0); break; }
                b = Logical(x, word);
            }
            else { Error(0); b = false; }
            x >> c;
            if(c != '\0' && b){ Error(1); b = false; }

            break;
        }
        case 3: break;
        default:{ Error(6); break; }
        }
        cout << endl;
        if(k == 1 || k == 2){
            if (b) cout << "ЭТО ВЫРАЖЕНИЕ!" << endl;
            else cout <<"НЕТ, ЭТО НЕ ВЫРАЖЕНИЕ!" << endl;
        }
    }
    return 0;
}


bool Logical (strstream &x, string word){
    if(word[0] == '(')
    {
        if(x >> word){
            cout << word << " ";
            word = Not(x, word);
            if(word[0] == '('){
                if(!Logical(x, word)) return false;
            }
        }
        else{ Error(7); return false; }

        if(x >> word){
            cout << word << " ";
            word = Not(x, word);
            if(!Mark(word)) return false;
        }
        else{ Error(5); return false; }

        if(x >> word){
            cout << word << " ";
            word = Not(x, word);
            if(!Logical(x, word)) return false;
        }
        else{ Error(7); return false; }

        if(x >> word){
            cout << word << " ";
            if(word[0] == ')') return true;
        }
        else{ Error(4); return false; }

        }
    if((isalpha(word[0]) && word.length() == 1)) return true;
    if((word == "FALSE") || (word == "TRUE")) return true;
    else{ Error(7); return false; }
}


bool Mark (string word){
    if(word[0] == '\0') return false;
    if((word == "AND") || (word == "OR")) return true;
    else{ Error(5); return false; }
}

string Not(strstream &x, string word){
    if( word == "NOT" ){
        if(x >> word){
            if((word == "AND") || (word == "OR")){ Error(8); word[0] = '\0'; return word; }
            cout << word << " "; return word;

        }
        else{  Error(3); word[0] = '\0'; return word; }
    }
    return word;
}

string Space(char str0[]){
    int i = 0, k = 0;
    string str1;
    char str[1000];
    while(str0[i] != '\0'){
        if(str0[i] == '(' || str0[i] == ')'){
            str[k] = ' ';
            str[k+1] = str0[i];
            str[k+2] = ' ';
            k += 3;
            i++;
        }
        else{
            if(str0[i] == '\n') break;
            str[k] = str0[i];
            i++;
            k++;
        }
    }
    str[k] = '\0';
    str1 = str;
    return str1;
}


void Error(short k){
    cout << endl << "err#" << k << endl;
    switch (k) {
    case 0: cout << "! - Пустая входная строка" << endl; break;
    case 1: cout << "! - Лишние символы во входной строке" << endl; break;
    case 2: cout << "! - Недопустимый начальный символ" << endl; break;
    case 3: cout << "! - Отсутствует выражение после NOT." << endl; break;
    case 4: cout << "! - Отсутствует ')'" << endl; break;
    case 5: cout << "! - Отсутствует знак операции" << endl; break;
    case 6: cout << "! - Неверный выбор способа ввода." << endl; break;
    case 7: cout << "! - Отсутствует простое логическое" << endl; break;
    case 8: cout << "! - Недопустимо отрицание операции" << endl; break;
    default : cout << "! - ...";break;
    };
}
