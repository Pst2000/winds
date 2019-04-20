/**
 * https://github.com/wolven-storm-shtu/winds
 * Hackers are those who change the way how old stuff
 * works and voila... The world becomes a better place.
 * Whoever said that.
 * 
 * push(): hello.cpp
 */

#include <graphenelib/contract.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/print.hpp>
#include <graphenelib/types.h>

using namespace graphene;

class winds : public contract
{
  public:
    winds(uint64_t id)
        : contract(id)
    {
    }

    // @abi action
    void push(std::string x)
    {
        print(x);
    }
};

GRAPHENE_ABI(winds, (push))
