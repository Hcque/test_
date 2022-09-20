
#include <list>
#include <iostream>


int main()
{

    std::list<int> L ;
    L.push_back(1);
    L.push_back(2);
    L.push_back(3);

    for (auto it = L.begin(); it != L.end(); )
    {
        if (*it == 3){
            it = L.erase(it);
        }
        else it ++;
    }
    for (auto it = L.begin(); it != L.end(); ++it) std::cout << (*it) << std::endl;


}