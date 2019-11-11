#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#define N 3000

using namespace std;

struct node{
    map<char, node*> go;
    map<char, node*> son;
    node* suffLink = nullptr;
    node* parent = nullptr;
    node* up = nullptr;
    char charToParent;
    bool isEnd;
    size_t endNumber;
    size_t patternLeng;
};

class Aho{
public:

    Aho(const string & t, const string & str, char j){
		root = new node;
        root->isEnd = false;
        root->suffLink = root;
        joker = j;
        makeTree(str, 0);
        processText(t);
    }


    Aho(const string & t, const string str[], size_t n){
        root = new node;
		root->isEnd = false;
        root->suffLink = root;
        joker = 0;
        for (size_t i = 0; i < n; i++) {
            makeTree(str[i], i + 1);
        }
        processText(t);
    }

    ~Aho(){
        delete root;
        for(auto it : vertexes)
            delete it;
    }

    void print(){
        for (auto it : result) {
            for (auto tr : it.second) {
                if(tr.first == 0)
                    cout << it.first << endl;
                else
                    cout << it.first << ' ' << tr.first << endl;
            }
        }
    }

private:
    void makeTree(const string & s, size_t patternNumber){
        node* cur = root;
        size_t i;
        for (i = 0; i < s.length(); i++) {
            char c = s[i];
            if(cur->son[c] == nullptr){
                cur->son[c] = new node;
                vertexes.push_back(cur->son[c]);
                cur->son[c]->parent = cur;
                cur->son[c]->charToParent = c;
                cur->son[c]->isEnd = false;
                cur->son[c]->go[c] = nullptr;
                if(c == joker){
                    for (auto it : alph) {
                        cur->go[it] = cur->son[c];
                    }
                }
            }
            cur = cur->son[c];
        }
        cur->isEnd = true;
        cur->patternLeng = i;
        cur->endNumber = patternNumber;
    }

    node* getSuffLink(node* v){
        if (v->suffLink == nullptr){
            if(v == root || v->parent == root)
                v->suffLink = root;
            else {
                v->suffLink = getLink(getSuffLink(v->parent), v->charToParent);
            }
        }
        return v->suffLink;
    }

    node* getLink(node* v, char c){
        if(v->go[c] == nullptr){
            if(v->son[c])
                v->go[c] = v->son[c];
            else if(v == root)
                v->go[c] = root;
            else
                v->go[c] = getLink(getSuffLink(v),c);
        }
        return v->go[c];
    }

    node* getUp(node* v){
        if(v->up == nullptr){
            if(getSuffLink(v)->isEnd)
                v->up = getSuffLink(v);
            else if(getSuffLink(v) == root)
                v->up = root;
            else
                v->up = getUp(getSuffLink(v));
        }
        return v->up;
    }

    void processText(const string & t){
        node* cur = root;
        for(size_t i = 0; i < t.length(); i++){
            cur  = getLink(cur, t[i]);
            if(cur->isEnd == true){
                result[i - cur->patternLeng + 2].insert(pair<size_t, bool>(cur->endNumber, 1));
            }
            node* tmpcur = cur;
            while(getUp(tmpcur) != root){
                tmpcur = getUp(tmpcur);
                if(tmpcur->isEnd == true)
                    result[i - tmpcur->patternLeng + 2].insert(pair<size_t, bool>(tmpcur->endNumber, 1));
            }
        }
    }

    node* root;
    char joker;
    vector<char> alph = {'A', 'C', 'G', 'T', 'N'};
    map<size_t, map<size_t, bool>> result;
    vector<node*> vertexes;
};

int main()
{
    string text, pattern[N];
    char joker;
    size_t n;
    cin >> text >> pattern[0];
    n = static_cast<size_t>(atoi(pattern[0].c_str()));
    if(n){
        for (size_t i = 0; i < n; i++) {
            cin >> pattern[i];
        }
        Aho aho(text, pattern, n);
        aho.print();
    }
    else{
        cin >> joker;
        Aho aho(text, pattern[0], joker);
        aho.print();
    }
    return 0;
}
