



#include <iostream>
#include <set>
#include <string>


int main()
{
    std::set<std::string> myset;

    std::set<std::string>::iterator iter = myset.emplace_hint(myset.begin(), "thhsp");
    std::cout << "myset" << myset.size() << std::endl;

    std::cout << *iter << std::endl;
}

