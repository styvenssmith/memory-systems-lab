#include <iostream>
#include <cstddef>


struct BadOrder{
    char a;
    double b;
    int c;
};

struct BetterOrder{
    double b;
    int c;
    char a;
};

int main(){

    std::cout<<"BadOrder\n";
    std::cout<<"sizeof: "<<sizeof(BadOrder)<<"\n";
    std::cout<<"alignof: "<<alignof(BadOrder)<<"\n";
    
    std::cout<<"BetterOrder\n";

    std::cout<<"sizeof: "<<sizeof(BetterOrder)<<"\n";
    std::cout<<"alignof: "<<alignof(BetterOrder)<<"\n";







    return 0;
}
