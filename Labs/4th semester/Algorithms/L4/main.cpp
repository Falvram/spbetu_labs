#include <iostream>
#include <vector>
#include <string>

#define K 4

using namespace std;

class Kmp{
public:
    Kmp() = default;
    ~Kmp() = default;

    void init(size_t ch){
        cout << "Pattern, target, number of parts" << endl;
        size_t k, partleng;
        cin >> pattern >> target;
        prefix.resize(pattern.length());
        prefixfunction();
        k = K;
        switch (ch) {
            case 1:{
                partleng = target.length()/k;
                for (size_t i = 0; i < k - 1; i++) {
                    kmpfunction(i * partleng, (i + 1) * partleng + pattern.length() - 2);
                }
                kmpfunction((k - 1)*partleng, target.length());
                if(result.empty()) cout << -1;
                else{
                    auto it = result.begin();
                    for (; it < result.end() - 1; it++) {
                        cout << *it << ",";
                    }
                    cout << *it;
                }
                break;
            }
            case 2:{
                if(target.length() != pattern.length()){
                    cout << -1;
                    break;
                }
                string tmp = target;
                target = pattern;
                pattern = tmp;
                target += target;
                partleng = target.length()/k;
                for (size_t i = 0; i < k - 1; i++) {
                    kmpfunction(i * partleng, (i + 1) * partleng + pattern.length() - 2);
                }
                kmpfunction((k - 1)*partleng, target.length());
                if(!result.empty()) cout << result[0];
                else cout << -1;
                break;
            }
            default:{}
        }
    }

private:
    string target;
    string pattern;
    vector<size_t> prefix;
    vector<size_t> result;

    void kmpfunction(size_t from, size_t to)
    {
        for(size_t j = 0, i = from; i < to; ++i)
        {
            while ((j > 0) && (pattern[j] != target[i]))
                j = prefix[j - 1];

            if (pattern[j] == target[i])
                j++;

            if (j == pattern.length())
                result.push_back(i - pattern.length() + 1);
        }
    }

    void prefixfunction()
    {
        prefix[0] = 0;
        for (size_t j = 0, i = 1; i < pattern.length(); ++i)
        {
            while ((j > 0) && (pattern[i] != pattern[j]))
                j = prefix[j - 1];

            if (pattern[i] == pattern[j])
                j++;

            prefix[i] = j;
        }
    }
};

int main()
{
    size_t ch;
    cout << "Task" << endl;
    cin >> ch;
    Kmp k;
    k.init(ch);
    return 0;
}
