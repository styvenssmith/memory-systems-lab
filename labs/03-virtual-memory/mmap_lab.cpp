#include <iostream>
#include <memory>
#include <unistd.h>
#include <sys/mman.h>
#include <fstream>

struct Point{
    
    int x;
    int y;

    Point(int a, int b):x(a), y(b){
        std::cout<<"Point constructor\n";
    }
    ~Point(){
        std::cout<<"Point destructor\n";
    }

};

int main(){
    //getting the page size from the OS
    long page_size = sysconf(_SC_PAGESIZE);
    //printing the actual page size for my system
    std::cout<<"Page size: "<<page_size<<"\n";
    
    std::size_t length = page_size*4;

    void* memory = mmap(
            nullptr,
            length,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1,0
    );

    if(memory == MAP_FAILED){
        std::cout<<"mmap failed\n";
        return 1;
    }

    std::cout<<"Mapping address: "<<memory<<"\n";
    
    //convert the raw memory into a char* type
    //that can store char*
    char* data = static_cast<char*>(memory);
    
    //write information on each page
    for(std::size_t i = 0;i<length;i+=page_size){
        data[i] = 'A';
    }
    
    munmap(memory, length);

    //allocating memory for the Point object
    
    std::size_t size = sizeof(Point);

    void* point_memory = mmap(
            nullptr,
            size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,0
    );
 
    Point* point = static_cast<Point*>(point_memory);

    std::construct_at(point, 10, 20);

    std::cout<<"point x: "<<point->x<<"\n";
    std::destroy_at(point);
    munmap(point_memory, size);





    return 0;
}
