#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
using namespace std;

class Matrix{
private:
    unsigned int size;
    unsigned int savecount;

    vector<vector<unsigned> > m;
    vector<unsigned> printvec;
    vector<unsigned> printvecsave;

public:   
    Matrix(unsigned int N) : size(N){
        for(unsigned int i = 0; i < size; i++){
            savecount = size*size;
            vector<unsigned> temp;
            printvec.reserve(3*size*size);
            printvecsave.reserve(3*size*size);

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
		printvec.clear();
		printvecsave.clear();
        size = 0;
		savecount = 0;
    }

    void push_3_squares();
    void push_square(unsigned int x, unsigned int y, unsigned int size, unsigned int num);
    bool findzero(unsigned int & x, unsigned int & y);
    void search(unsigned int x, unsigned int y, unsigned int num);
    void border_check(unsigned int x, unsigned int y, unsigned int & size);
    void remove_square(unsigned int x, unsigned int y, unsigned int size);
    void print();
};

#endif