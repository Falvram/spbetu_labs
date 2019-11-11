#include "matrix.h"

void Matrix :: push_3_squares(){
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

void Matrix :: print(){
    cout << savecount << endl;
    for (unsigned i = 0; i < printvecsave.size(); i += 3) {
        cout << printvecsave[i] << ' ' << printvecsave[i + 1] << ' ' << printvecsave[i + 2] << endl;
    }
}