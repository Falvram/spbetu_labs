#include <iostream>
#include <strstream>
#include <cstdlib>
#include <fstream>

namespace st_modul1{

    typedef bool base;
	
    class Stack{
	private:
		struct node; 
	node *topOfStack;

	public:
        Stack ()
            { topOfStack = nullptr; }
        base pop (void);
		void push (const base &x);
		bool isNull(void);
		void destroy (void);
	};
} 

using namespace st_modul1;
using namespace std;

bool expression(Stack s, strstream &x, string str);
bool Bracket(Stack s, strstream &x, string str);
bool readOne(Stack s, strstream &x, string str);
bool Mark(Stack s, strstream &x, string str);
bool oneMark(Stack s, strstream &x, string str);
bool Not(Stack s, strstream &x, string str);
string Space(char str0[]);
Stack read(Stack s, strstream &x, string str);
string readStr(Stack s, strstream &x, string str);
