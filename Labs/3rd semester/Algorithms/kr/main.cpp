#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <fstream>

using namespace std;

struct node // структура для представления узлов дерева
{
    int key;
    int size;
    node* left;
    node* right;
    node(int k) { key = k; left = right = 0; size = 1; }
};

int getsize(node* T) // обертка для поля size, работает с пустыми деревьями (t=NULL)
{
    if( !T ) return 0;
    return T->size;
}

void fixsize(node* T) // установление корректного размера дерева
{
    T->size = getsize(T->left)+getsize(T->right)+1;
}

node* join(node* T, node* q) // объединение двух деревьев
{
    if( !T ) return q;
    if( !q ) return T;
    if( rand()%(T->size+q->size) < T->size )
    {
        T->right = join(T->right,q);
        fixsize(T);
        return T;
    }
    else
    {
        q->left = join(T,q->left);
        fixsize(q);
        return q;
    }
}

node* insert(node* T, int k, unsigned int* count) // вставка нового узла с ключом k в дерево p
{
    if( !T ) return new node(k);
    if( T->key>k ){
        (*count)++;
        T->left = insert(T->left,k, count);
    }
    else{
        (*count)++;
        T->right = insert(T->right,k, count);
    }
    fixsize(T);
    return T;
}

node* remove(node* T, int k, unsigned int* comparison) // удаление из дерева p первого найденного узла с ключом k
{
    if( !T ) return T;

    if( T->key==k )
    {
        (*comparison)++;
        node* q = join(T->left,T->right);
        delete T;
        return q;
    }
    else if( k<T->key ){
        (*comparison)++;
        T->left = remove(T->left,k, comparison);
    }
    else{
        (*comparison)++;
        T->right = remove(T->right,k, comparison);
    }
    return T;
}

node* destroy(node* T){
        if (T->left)
             delete T->left;
        if (T->right)
                delete T->right;
        delete T;
        return T = NULL;
}

int main()
{
    srand(unsigned(time(0)));
    node *T = NULL;
    unsigned int count, ins, del, step, comparison = 0;
    ofstream file;
    file.open("output.txt");

    cout << "Enter the number of nodes:" << endl;
    cin >> count;
    cout << "Enter the number of elements to be inserted: " << endl;
    cin >> ins;
    cout << "Enter the number of elements to be deleted: " << endl;
    cin >> del;
    cout << "Enter the segment step:" << endl;
    cin >> step;

    if(count > 50000) count = 50000;
    if(ins > 50000) ins = 50000;
    vector<unsigned int> x(count);
    for(unsigned int i = 0; i < count; i++){
        x[i] = i;
    } 
    random_shuffle(x.begin(), x.end());

    for(unsigned int i = 0; i < count; i++){
        T = insert(T, x[i], &comparison);
    }

    file << "For insertion:" << endl;
    file << "Number of elements\tNumber of comparisons" << endl;
    comparison = 0;
    for(unsigned int i = step; i <= ins; i+=step){
        for (int k = i; k < i+step; k++) {
            T = insert(T, x[k], &comparison);
        }
        file.width(18);
        file << i;
        file.width(23);
        file << comparison;
        file << endl;
    }

    file << endl << "For delete:" << endl;
    file << "Number of elements\tNumber of comparisons" << endl;
    comparison = 0;
    if(del > count+ins) del = count + ins;
    for(int i = del; i >= 0; i-=step){
        for (int k = i+step; k > i; k--) {
            T = remove(T, x[k], &comparison);
        }
        file.width(18);

        file << i;
        file.width(23);
        file << comparison;
        file << endl;
    }

    destroy(T);
    cout << "The program finished correctly";
    return 0;
}
