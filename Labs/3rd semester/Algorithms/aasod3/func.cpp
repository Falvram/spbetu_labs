#include "st_interf1.h"
#include "error.h"

const char* ExpException::what() const noexcept{
    return whatStr.c_str();
}

Stack read(Stack s, strstream &x, string str){
    if(str == "(" ) s.push(Bracket(s, x, str));
    else
        if(str == "true") s.push(1);
            else
            if(str == "false") s.push(0);
                else
                if(str == "{" || str == "[" || str == "}" || str == "]") throw ExpException("Wrong brackets.");
                    else throw ExpException("Wrong logical.");
    return s;
}

string readStr(Stack s, strstream &x, string str){
    if(!(x >> str)){
        if(!(s.isNull())){
            bool a = s.pop();
            s.destroy();
            throw a;
        }
        else throw ExpException("Stack is null.");
    }
    cout << str << " ";
    return str;
}

bool expression(Stack s, strstream &x, string str){
    str = readStr(s, x ,str);
    if(str == "!") throw ExpException("'!' must be in brackets.");
    s = read(s, x, str);
    return Mark(s, x, str);
}

bool readOne(Stack s, strstream &x, string str){
    str = readStr(s, x, str);
    if(str == "!") return !readOne(s, x, str);
    s = read(s, x, str);
    return s.pop();
}

bool Mark(Stack s, strstream &x, string str){
    if(!(x >> str)){
        if(!(s.isNull())){
            bool a = s.pop();
            s.destroy();
            return a;
        }
        else throw ExpException("Stack is null.");
    }
    cout << str << " ";
    if(str == "+") s.push(s.pop() + expression(s, x, str));
    else
        if(str == "*"){
            s.push(s.pop() * readOne(s, x, str));
            return oneMark(s, x, str);
        }
        else
            if(str == "{" || str == "[" || str == "}" || str == "]" ) throw ExpException("Wrong brackets.");
            else throw ExpException("Wrong mark.");
    return expression(s, x, str);
}

bool oneMark(Stack s, strstream &x, string str){
    if(!(x >> str)) return s.pop();
    cout << str << " ";
    if(str == "+") s.push(s.pop() + readOne(s, x, str));
    else
        if(str == "*") s.push(s.pop() * readOne(s, x, str));
        else
            if(str == "{" || str == "[" || str == "}" || str == "]") throw ExpException("Wrong brackets.");
            else throw ExpException("Wrong mark.");

    return readOne(s, x, str);
}

bool Bracket(Stack s, strstream &x, string str){
    bool b = readOne(s, x, str);
    if(!(x >> str)){
        throw ExpException("No closing brackets or empty mark.");
    }
    cout << str << " ";
    if(str == ")") return b;
    if(str == "+") s.push(b + readOne(s, x, str));
		else
		if(str == "*") s.push(b * readOne(s, x, str));
			else throw ExpException("Wrong mark.");
	if(!(x >> str)) throw ExpException("Missing brackets.");			
    cout << str << " ";
    if(str == ")") return s.pop();
    else throw ExpException("Wrong brackets.");
}

string Space(char str0[]){
    int i = 0, k = 0;
    string str1;
    char str[1000];
    while(str0[i] != '\0'){
        if(str0[i] == '(' || str0[i] == ')' || str0[i] == '+' || str0[i] == '*' || str0[i] == '!'|| str0[i] == '\0'|| str0[i] == ']'|| str0[i] == '['|| str0[i] == '{'|| str0[i] == '}'){
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
