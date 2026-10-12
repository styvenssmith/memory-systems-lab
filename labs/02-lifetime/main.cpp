#include <iostream>


class Tracker{
    public:
        Tracker(){
            std::cout<<"default constructor\n";
        }
        Tracker(int x){
            std::cout<<"value constructor\n";
        }

        Tracker(const Tracker& other){
            std::cout<<"copy constructor\n";
        }

        Tracker& operator=(const Tracker& other){
            std::cout<<"copy assignment\n";
            return *this;
        }

        Tracker(Tracker&& other)noexcept{
            std::cout<<"move constructor\n";
        }
        
        Tracker& operator=(Tracker&& other)noexcept{
            std::cout<<"move assignment\n";
            return *this;
        }

        ~Tracker(){
            std::cout<<"destructor\n";
        }
};


int main(){
    //create a object
    Tracker a;
    //construct b Tracker
    Tracker b(10);
    //use the copy constructor
    Tracker c = b;
    //use the move constructor
    Tracker d = std::move(b);
    //use the copy assignment
    c = a;
    //use the move assignment
    d = std::move(a);


    return 0;
}
