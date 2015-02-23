#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <list>
#include <typeinfo>

using namespace std;

class ThreeStack
{
    int s[4];
    int A[100];
    
    public:
    ThreeStack() 
    {
        s[0] = 0;
        s[1] = 0;
        s[2] = 0;
        s[3] = 0;
    };
    void enqueue(int q, int data)
    {
        if (q < 0 || q > 2 || s[3] == 100) {
            return;
        }
        for (int i = s[3] - 1; i >= s[q]; --i) {
            A[i+1] = A[i];
        }
        A[s[q]] = data;
        for (int i = 3; i > q; --i) {
            s[i]++;
        }
        return;
    }
    int dequeue(int q)
    {
        if (q < 0 || q > 2 || s[q+1] - s[q] == 0) {
            return 0;
        }
        int data = A[s[q]];
        for (int i = s[q]; i < s[3]; ++i) {
            A[i] = A[i+1];
        }
        for (int i = q + 1; i <= 3; ++i) {
            s[i]--;
        }
        return data;
    }
};


class MinStack
{
    MinStack *next;
    int data;
    int min;
    int len;

    public:
    
    MinStack(int data): data(data), next(NULL), min(data), len(1) 
    {
    }   
    void push(int data)
    {   
        if (len > 0) {
            MinStack *second = new MinStack(this->data);
            second->next = this->next;
            second->min = this->min;
            this->next = second;
            this->min = std::min(data, second->min);
        } else {
            this->next = NULL;
            this->min = data;
        }
        this->data = data;
        (this->len)++;
        return;
    }
    int pop()
    {
        try 
        {
            if (this->len > 0) {
                int return_data = this->data;
                (this->len)--;
                if (this->next != NULL) {
                    this->data = this->next->data;
                    this->min = this->next->min;
                    MinStack *third = this->next->next;
                    delete this->next;
                    this->next = third;
                } 
                return return_data;
            } 
            throw 0;    
        }
        catch (...) 
        {
            cerr << "Cannot pop from an empty stack." << '\n';
        }
    }
    int min_in_stack()
    {
        try
        {
            if (len > 0) {
                return this->min;
            } 
            throw 0;    
        }
        catch (...) 
        {
            cerr << "Cannot take minimal value of an empty stack." << '\n';
        }
    }
};

class StackSet
{
    stack < stack<int> > stacks;
    int n;
    int idx_s;
    public:
    StackSet(int n): n(n)
    {
        stack<int> s;
        stacks.push(s);
        idx_s=0;
    }
    int push(int data)
    {
        if (stacks.top().size() == n) {
            stack<int> s;
            s.push(data);
            stacks.push(s);
            idx_s++;
        } else {
            stacks.top().push(data);
        }
        return idx_s;
    }
    int pop()
    {
        int rdata = stacks.top().top();
        stacks.top().pop();
        if (stacks.top().empty()) {
            stacks.pop();
            idx_s--;
        }
        return rdata;
    }
};

class Hanoi_tower
{
    vector< stack<int> > hanoi;
    
    public:
    Hanoi_tower()
    {
        stack<int> s;
        for (int i = 9; i > 0; i -= 2) {
            s.push(i);
        }
        hanoi.push_back(s);
        stack<int> se;
        hanoi.push_back(se);
        hanoi.push_back(se);
    }
    void move_to_last()
    {   
        display_pyramid();
        move_pyramid(0, 2, hanoi[0].size());
    }

    private:
    void move_pyramid(int s, int e, int height)
    {
        if (height > 1) {
            move_pyramid(s, 3 - (s + e), height - 1);
        }
        hanoi[e].push(hanoi[s].top());
        hanoi[s].pop();
        display_pyramid();
        if (height > 1) {
            move_pyramid(3 - (s + e), e, height - 1);
        }
    }
    void display_pyramid()
    {
        for (int i = 0; i < 3; ++i) {
            stack<int> tmp;
            while (!hanoi[i].empty()) {
                tmp.push(hanoi[i].top());
                hanoi[i].pop();
            }
            while (!tmp.empty()) {
                cout << tmp.top() << "";
                hanoi[i].push(tmp.top());
                tmp.pop();
            }
            cout << '\n';
        }
        cout <<"---------------------- \n";
    }
};

class queue_with_stacks
{
    stack<int> s1;
    stack<int> s2;
    public:
    void push(int data)
    {
        while(!s2.empty()) {
            s1.push(s2.top());
            s2.pop();
        }
        s1.push(data);
    }
    int pop()
    {
        while(!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        int data = s2.top();
        s2.pop();
        return data;
    }
};

stack<int> sort_stack(stack<int> &s)
{
    stack<int> aux;
    while(!s.empty()) {
        int x = s.top();
        s.pop();
        while (!aux.empty() && x > aux.top()) {
            s.push(aux.top());
            aux.pop();
        }
        aux.push(x);    
    }
    while (!aux.empty()) {
        s.push(aux.top());
        aux.pop();
    }
    return s;
}
class animal
{
    public:
    animal() {cout << "An animal created!\n";}
    virtual void talk() = 0;
};

class cat: public animal
{
    string name;
    public:
    cat(string name): name(name) {cout << "I am a cat. My name is " << name << "!\n";}
    void talk()
    {
        cout << "Meow!\n";
    }
};

class dog: public animal
{
    string name;
    public:
    dog(string name): name(name) {cout << "I am a dog. My name is " << name << "!\n";}
    void talk()
    {
        cout << "Waff!\n";
    }
};

class animals_shelter
{
    list<animal*> a;
    public:
    void enqueue(animal *new_animal)
    {
        a.insert(a.begin(),new_animal);
    }
    dog* dequeueDog()
    {   
        try 
        {
            list<animal*>::iterator oldest_dog = a.end();   
            while (oldest_dog-- != a.begin()) {
                if (typeid(**oldest_dog) == typeid(dog)) {
                    dog *returning_dog = dynamic_cast<dog*> (*oldest_dog);
                    a.erase(oldest_dog);
                    return returning_dog;
                }
            }
            throw 1;
        }
        catch (int e) 
        {
            cerr << "No dogs in the shelter!" << endl;
        }

    }
    animal* dequeue()
    {
        try
        {
            if (a.size() > 0) {
                animal *animal_returning = a.back();
                a.pop_back();
                return animal_returning;
            }
            throw 1;
        }
        catch (int e)
        {
            cerr << "No animals in the shelter!\n";
        }
    }
};

int main()
{
    /*
    ThreeStack q3;
    q3.enqueue(0, 111);
    cout << q3.dequeue(0) << '\n';
    q3.enqueue(2, 344);
    cout << q3.dequeue(0) << '\n';
    q3.enqueue(0, 178);
    cout << q3.dequeue(2) << '\n';
    q3.enqueue(1, 290);
    cout << q3.dequeue(0) << '\n';
 
    MinStack ms(3);
    cout << ms.min_in_stack() << '\n';
    ms.push(1);
    cout << ms.min_in_stack() << '\n';
    ms.push(-6);
    cout << ms.min_in_stack() << '\n';
    ms.push(6);
    cout << ms.min_in_stack() << '\n';
    cout << ms.pop() << '\n';
    cout << ms.min_in_stack() << '\n';
    cout << ms.pop() << '\n';
    cout << ms.min_in_stack() << '\n';
    cout << ms.pop() << '\n';
    cout << ms.min_in_stack() << '\n';
    cout << ms.pop() << '\n';
    ms.push(6);
    cout << ms.min_in_stack() << '\n';

    StackSet ss(2);
    cout << ss.push(1) << '\n';;
    cout << ss.push(11) << '\n';;
    cout << ss.push(111) << '\n';;
    cout << ss.push(18) << '\n';;
    cout << ss.pop() << '\n';
    cout << ss.pop() << '\n';
    cout << ss.pop() << '\n';
    cout << ss.push(111) << '\n';;*/
    
    //Hanoi_tower t;
    //t.move_to_last();
    /*
    queue_with_stacks q;
    q.push(2);
    q.push(3);
    cout << q.pop() << " ";
    q.push(4);
    cout << q.pop() << " ";
    cout << q.pop() << " ";
    q.push(5);
    cout << q.pop() << "\n";*/

    /*
    stack<int> s;
    s.push(2); s.push(4); s.push(1); s.push(5);
    s = sort_stack(s);
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << '\n';*/

    cat c1("Katti");
    cat c2("Jessie");
    dog d1("Brie");
    cat c3("Mickey");
    dog d2("Jack");
    animals_shelter sh;
    sh.enqueue(&c1);
    sh.enqueue(&d1);
    sh.enqueue(&c2);
    sh.enqueue(&d2);
    animal *a1 = sh.dequeue();
    a1->talk();
    dog* dd1 = sh.dequeueDog();
    dd1->talk();
    dog* dd2 = sh.dequeueDog();
    dd2->talk();
    return 0;
}
