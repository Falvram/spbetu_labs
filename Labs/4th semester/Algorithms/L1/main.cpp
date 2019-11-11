#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class Matrix{
private:
    unsigned int size;
    unsigned int savecount;
    unsigned int operations;

    vector<vector<unsigned> > m;
    vector<vector<unsigned> > savematrix;
    vector<unsigned> printvec;
    vector<unsigned> printvecsave;

public:   
    Matrix(unsigned int N) : size(N){
        for(unsigned int i = 0; i < size; i++){
            savecount = size*size;
            vector<unsigned> temp;
            printvec.reserve(3*size*size);
            printvecsave.reserve(3*size*size);
            operations = 0;

            for(unsigned int j = 0; j < size; j++)
                temp.push_back(0);
            m.push_back(temp);
        }
    }
    ~Matrix(){
        for(unsigned int i = 0; i < size; i++){
            m[i].clear();
        }
        m.clear();
        savematrix.clear();
        printvec.clear();
        printvecsave.clear();
        size = 0;
    }

    void push_3_squares();
    void push_square(unsigned int x, unsigned int y, unsigned int size, unsigned int num);
    bool findzero(unsigned int & x, unsigned int & y);
    void search(unsigned int x, unsigned int y, unsigned int num);
    void border_check(unsigned int x, unsigned int y, unsigned int & size);
    void remove_square(unsigned int x, unsigned int y, unsigned int size);
    void printcheck();
    void print();
};

void Matrix :: push_3_squares(){
    operations += 3;
    if(size % 2 == 0){
        push_square(0, 0, size/2, 1);
        push_square(0, size/2, size/2, 2);
        push_square(size/2, 0, size/2, 3);
        search(size/2, size/2, 3);
    }
    else if(size % 3 == 0){
        push_square(0, 0, 2*size/3, 1);
        push_square(0, 2*size/3, size/3, 2);
        push_square(2*size/3, 0, size/3, 3);
        search(2*size/3, 2*size/3, 3);
    }
    else if(size % 5 == 0){
        push_square(0, 0, 3*size/5, 1);
        push_square(0, 3*size/5, 2*size/5, 2);
        push_square(3*size/5, 0, 2*size/5, 3);
        search(3*size/5, 2*size/5, 3);
    }
    else{
        push_square(0, 0, size/2 + 1, 1);
        push_square(size/2 + 1, 0, size/2, 2);
        push_square(0, size/2 + 1, size/2, 3);
        search(size/2 + 1, size/2, 3);
    }
}

bool Matrix :: findzero(unsigned int & x, unsigned int & y){
    for(unsigned int i = 0; i < size; i++)
        for (unsigned int j = 0; j < size; j++)
            if(m[i][j] == 0){
                x = i;
                y = j;
                return true;
            }
    return false;
}

void Matrix :: remove_square(unsigned int x, unsigned int y, unsigned int size){
    printvec.pop_back();
    printvec.pop_back();
    printvec.pop_back();
    for (unsigned int i = x; i < x + size; i++) {
        for (unsigned int j = y; j < y + size; j++){
                m[i][j] = 0;
        }

    }
}

void Matrix :: push_square( unsigned int x, unsigned int y, unsigned int size, unsigned int num){
    printvec.push_back(x);
    printvec.push_back(y);
    printvec.push_back(size);
    for(unsigned int i = x; i < x + size; i++){
        for (unsigned int j = y; j < y + size; j++){
                m[i][j] = num;
        }
    }
}

void Matrix :: search(unsigned int x, unsigned int y, unsigned int num){
        operations++;
        if(findzero(x, y)){
            if(num == savecount){
                return;
            }
            for (unsigned int tempsize = size - 1; tempsize > 0 ; tempsize--) {
                border_check(x, y, tempsize);
                push_square(x, y, tempsize, num + 1);
                search(x, y, num + 1);
                remove_square(x, y, tempsize);
            }
        }
        else {
            if(savecount > num){
                savecount = num;
                savematrix = m;
                printvecsave = printvec;
            }
            return;
        }

}

void Matrix :: border_check(unsigned int x, unsigned int y, unsigned int & size){
    unsigned int i = x, j = y;
    if(x + size > this->size){
         size = this->size - x;
     }

     if(y + size > this->size){
         size = this->size - y;
     }

    for(; i < x + size; i++){
        if(m[i][y] != 0){
            break;
        }
    }
    for (; j < y + size; j++) {
        if(m[x][j] != 0){
            break;
        }
    }
    if(i - x < j - y)
        size = i - x;
    else if(j - y < i - x){
        size = j - y;
    }
    return;
}

void Matrix :: printcheck(){
    cout << endl;
    cout << "operations: " << operations << endl;
    for(unsigned int i = 0; i < this->size; i++){
        for (unsigned int j = 0; j < this->size; j++){
            cout << setw(2) <<  savematrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Matrix :: print(){
    cout << savecount << endl;
    for (unsigned i = 0; i < printvecsave.size(); i += 3) {
        cout << printvecsave[i] << ' ' << printvecsave[i + 1] << ' ' << printvecsave[i + 2] << endl;
    }
}

int main()
{
    unsigned int N;
    cin >> N;
    Matrix table(N);
    table.push_3_squares();
    table.print();
    table.printcheck();
    return 0;
}
