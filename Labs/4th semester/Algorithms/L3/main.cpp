#include <iostream>
#include <map>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

struct node{
    char value;
    map<node*, pair<int, int>> neighbours;

    node() = default;
    node(char value) : value(value){}
};

class list{
private:
    node* source;
    node* stock;
    map<node*, node*> way;
    map<char, node*> pointers;
    multimap<char, pair<node*, node*>> toprint;
    map<char, map<char, int>> sorted;
    map<char, bool> viewed;

public:
    list(){
        source = new node();
        stock = new node();
    }

    ~list(){
        for (auto it = pointers.begin(); it != pointers.end(); it++)
            delete it->second;
        way.clear();
        toprint.clear();
        pointers.clear();
    }

    void read(){
        int N, w;
        char vi, vj, v0, vn;
        cin >> N >> v0 >> vn;

        pointers[v0] = source;
        pointers[vn] = stock;
        source->value = v0;
        stock->value = vn;
        for (int k = 0; k < N; k++) {
            cin >> vi >> vj >> w;
            sorted[vi].insert(pair<char, int>(vj, w));
        }
        for (auto &it : sorted){
            if(!pointers[it.first]) pointers[it.first] = new node(it.first);
            for(auto &tr : it.second){
                if(!pointers[tr.first]) pointers[tr.first] = new node(tr.first);
                    pointers[it.first]->neighbours[pointers[tr.first]] = pair<int, int>(tr.second, tr.second);
                if(!pointers[tr.first]->neighbours[pointers[it.first]].first)
                    pointers[tr.first]->neighbours[pointers[it.first]] = pair<int, int>(0, tr.second);
                toprint.insert(pair<char, pair<node*, node*>>(pointers[it.first]->value, pair<node*, node*>(pointers[it.first], pointers[tr.first])));
            }
        }
    }

    node* ws(char from){
        if(pointers[from] == stock) return stock;
        viewed[from] = true;
        for (auto &it : sorted[from]) {
            if(pointers[from]->neighbours[pointers[it.first]].first > 0 && viewed[it.first] == false){
                node* to = ws(it.first);
                if(to != nullptr){
                    way[pointers[to->value]] = pointers[from];
                    return pointers[from];
                }
                else continue;
            }
        }
        return nullptr;
    }

    int fordFulkerson(){
        node* from;
        node* to;
        int flow = 0;
        while(ws(source->value) != nullptr){
            int pathflow = numeric_limits<int> :: max();
            for (to = stock; to != source; to = way[to]) {
                from = way[to];
                pathflow = min(pathflow, from->neighbours[to].first);
            }
            for (to = stock; to != source; to = way[to]) {
                from = way[to];
                if(from->neighbours[to].first - pathflow < 0){
                    from->neighbours[to].first = 0;
                }
                else from->neighbours[to].first -= pathflow;
                if(to->neighbours[from].first + pathflow > to->neighbours[from].second){
                    to->neighbours[from].first = to->neighbours[from].second;
                }
                else to->neighbours[from].first += pathflow;
            }
            flow += pathflow;
            viewed.clear();
        }
        return flow;
    }

    void print(){
        for (auto &it : toprint)
            cout << it.first << " " << it.second.second->value << " " << abs(it.second.first->neighbours[it.second.second].first - it.second.first->neighbours[it.second.second].second) << endl;
    }
};

int main()
{
    list lst;
    lst.read();
    cout << lst.fordFulkerson() << endl;
    lst.print();
    return 0;
}
