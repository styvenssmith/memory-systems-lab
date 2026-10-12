#include <iostream>
#include <memory>



template<typename T>

class Arr{
private:
T* arr_{};
std::size_t size{};
std::size_t capacity{};

public:
//default constructor
Arr(){
}

//allocate more size if the current container is full
void extend(){

capacity = (capacity==0)?1:capacity*2;

T* new_arr = reinterpret_cast<T*>(::operator new(capacity*sizeof(T)));

for(std::size_t i = 0;i<size;i++){
std::construct_at(new_arr+i, std::move(arr_[i])); 
}

for(std::size_t i = 0;i<size;i++){
std::destroy_at(arr_+i);
}

::operator delete(arr_);

arr_ = new_arr;

}

//add new elements
void push_back(const T& val){
if(size==capacity) extend();
std::construct_at(arr_+size, val);
size++;
}

T& operator[](std::size_t idx){
return arr_[idx];
}
const T&operator[](std::size_t idx)const{
return arr_[idx];
}

~Arr(){
for(std::size_t i = 0;i<size;i++){
std::destroy_at(arr_+i);
}
::operator delete(arr_);
}

};


struct Test {
    int x;

    Test(int x) : x(x) {
        std::cout << "constructed\n";
    }

    ~Test() {
        std::cout << "destroyed\n";
    }
};





int main(){

Arr<Test>v;
v.push_back(Test(5));
return 0;


}
