#include "st_interf1.h"
#include "error.h"

#define N 1000
using namespace std;
using namespace st_modul1;
int main()
{
    string str;
    int k = 0;
    bool b;

    while(k != 3){
        strstream x;
        Stack s;
        char str0[N] = "\0";
        cout << endl << "1 - Reading from file, 2 - Keyboard input, 3 - Exit from the program." << endl;
        cin >> k;
        switch (k) {
            case 1:{
                cout << "Enter file name:" << endl;
                cin >> str;
                ifstream outfile(str);
                try {
                    if(!outfile) throw ExpException("Input file not open.\n");
                } catch (ExpException &e) {
                    cout << e.what();
                    continue;
                }
                outfile.read(str0, N);
                outfile.close();
                str = Space(str0);
                x << str;
                break;
            }
            case 2:{
                cout << "Enter expression: " << endl;
                cin.get();
                cin.getline(str0, N);
                str = Space(str0);
                x << str;
                break;
            }
            case 3:{ cout << "Press Enter\n"; return 0; }
        }
        try {
            b = expression(s, x, str);
        }
        catch(bool a){
            b = a;
        }
        catch (ExpException &e){
            cout << endl;
            cout << e.what();
            continue;
        }
        cout << endl << "Answer: " << b << endl;
    }
    return 0;
}
