#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <unordered_map>
using namespace std;
enum sus {c1 = 0, c2 = 2, c3 = 5};
class obj{
protected:
    int x, y = 2;
public:
    obj(int x) : x(x){
        y = 6;
    };
    obj(const obj& other){
        *this = other;
    }
    void setY(int y){
        this->y = y;
    }
    void caseSus(sus op){
        cout << op << endl;
    }
    void operator =(const obj& other){this->x = other.x, this->y = other.y; cout << "Copied" << endl;}
    friend bool Comp(const obj& o1, const obj& o2);
    friend istream& operator >>(istream& op, obj& O);
};

istream& operator >>(istream& op, obj& O){
    op >> O.x >> O.y;
    return op;
}
class inherit : public obj{
private:
    int z;
public:
    inherit(int x, int z) : obj(x), z(z){}
    friend istream& operator >>(istream& op, inherit& I);
};
istream& operator >>(istream& op, inherit& I){
    op >> (obj&)I >>  I.z;
    return op;
}
bool Comp(const obj& o1, const obj& o2){
    return o1.x < o2.x;
}
class obj_vec{
private:
    vector<obj> v_obj;
public:
    obj_vec(vector<obj>& v) : v_obj(v){}
    const vector<obj>& getVector() const{return v_obj;}
    void setVector(const vector<obj>& v){v_obj = v;}
};

const char NR_OUTFIELD_STATS = 8;
const char OUTFIELD_STATS[NR_OUTFIELD_STATS][4] = {"PAC", "SHO", "DEF", "PHY", "STA", "DRI", "PAS", "AGG"};

istream& operator >>(istream& op, unordered_map<string, double>& stats){
    for (auto& s : OUTFIELD_STATS){
        cout << s << ": "; op >> stats[s];
    }
    return op;
}
istream& operator >>(istream& op, vector<double>& stats){
    for (int i = 0; i < 2; i++)
        op >> stats[i];
    return op;
}
int main(){
    inherit x(1, 2);
    cin >> x;

    

    /*
    long int v_size = 100000;
    vector<obj*> v;
    v.reserve(v_size);
    auto start = chrono::system_clock::now();

    for (int i = 0; i < v_size; i++)
        v.push_back(new obj(0));

    for (int i = v_size - 1; i >= 0; i--) {
        delete v[i];
        v.pop_back();
        v.shrink_to_fit();
    }
    
    auto end = chrono::system_clock::now();
    
    chrono::duration<double> elapsed_seconds = end - start;
    
    cout << "pop time: " << elapsed_seconds.count() << endl;

    for (int i = 0; i < v_size; i++)
        v.push_back(new obj(0));
    
    for (int i = v_size - 1; i >= 0; i--)
        delete v[i];

    v.clear();
    v.shrink_to_fit();
    

    elapsed_seconds = chrono::system_clock::now() - end;

    cout << "clear time: "<< elapsed_seconds.count() << endl;
    */
    
}