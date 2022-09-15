#include "common.h"
#include "test.cpp"

int main(int argc, char *argv[])
{
    std::vector< std::pair<uint64_t, typename std::unordered_map<uint64_t,uint64_t>::iterator > >  A;
    std::unordered_map<uint64_t, uint64_t> M;


    ///////// check the working set ///////
    for (int i = 1; i < 100; i ++ )
    {
        auto it = M.emplace_hint( 
            M.end() , std::piecewise_construct, std::forward_as_tuple(i),
        std::forward_as_tuple(i)
        );
        A.emplace_back(i, it);
    }


    
    //////////// check the vector of ptr ( as iterator ) ////////
    //   for (auto& e: M)
    //   {
    //       std::cout << e.first << " ";
    //   }
    //   std::cout << " === \n";
    //   for (auto& e: A){
    //       std::cout << e.second->second << " ";
    //   }



    ///////   check the find version  ////////
    /*
    Value<Version> value;
    for (int i = 1; i < 30; i += 4)
    {
        Version *tmp = new Version();
        memcpy(tmp, value.version, sizeof(Version));

        value.version = new Version();
        value.version->prev = tmp;
        value.version->wtts = i; // immutable
    }

    get_correct_version(&value);
    */

    ///// using the first version ////////
    /*
    Value<Version> value;
    for (int i = 1; i < 30; i += 4)
    {
        Version *tmp = new Version();
        memcpy(tmp, value.wt_rec, sizeof(Version));

        value.version = new Version();
        value.version->prev = tmp;
        value.version->wtts = i; // immutable
        
    }

    get_correct_version(&value);
    */

    // test1();
    uint64_t num_thread = atoi(argv[1]);
    test2(num_thread);

}