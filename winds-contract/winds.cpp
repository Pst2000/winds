/**
 * https://github.com/wolven-storm-shtu/winds
 * Hackers are those who change the way how old stuff
 * works and voila... The world becomes a better place.
 * Whoever said that.
 * 
 * push(): hello.cpp
 * subscribes: table
 */

#include <graphenelib/contract.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/print.hpp>
#include <graphenelib/types.h>

using namespace graphene;

class winds : public contract
{
    struct subscribes; // id** usr* ven* act Btime (index*)

  public:
    winds(uint64_t id)
        : contract(id)
        , offers(_self, _self)
    {
    }

    // @abi action
    void push(std::string x) // usr point of interest and content refresh request
    {
        print(x);
    }
    
    //@abi action
    void additem(uint64_t venderID, uint64_t subORunsub)
    {
        uint64_t pk = offers.available_primary_key();
        print("pk=", pk);
        offers.emplace(0, [&](auto &o) {
            o.id = pk;
            o.usr = get_trx_sender();
            o.ven = venderID;
            o.act = subORunsub; // subsctribe: 1, unsubscribe: 0
            o.Btime = get_head_block_time();
        });
    }

    //@abi action
    void getbypk(uint64_t key)
    {
        auto it = offers.find(key);
        if (it != offers.end()) {
            dump_item(*it);
        }
    }

    //@abi action
    void getbyusr(uint64_t key)
    {
        auto idx = offers.template get_index<N(usr)>();
        auto matched_offer_itr = idx.lower_bound(key);
        if (matched_offer_itr != idx.end()) {
            dump_item(*matched_offer_itr);
        }
    }

    //@abi action
    void getbyven(uint64_t key)
    {
        auto idx = offers.template get_index<N(ven)>();
        auto matched_offer_itr = idx.lower_bound(key);
        if (matched_offer_itr != idx.end()) {
            dump_item(*matched_offer_itr);
        }
    }

  private:
    void dump_item(const subscribes &o)
    {
        print("subscribes.id:", o.id, "\n");
        print("subscribes.usr:", o.usr, "\n");
        print("subscribes.ven:", o.ven, "\n");
        print("subscribes.act:", o.act, "\n");
        print("subscribes.Btime:", o.Btime, "\n");
    }

  private:
    //@abi table subscribes i64
    struct subscribes {
        uint64_t id;
        uint64_t usr;
        uint64_t ven;
        uint64_t act;
        uint64_t Btime;

        uint64_t primary_key() const { return id; }

        uint64_t by_index1() const { return usr; }

        uint64_t by_index2() const { return ven; }

        GRAPHENE_SERIALIZE(subscribes, (id)(usr)(ven)(act)(Btime))
    };

    typedef multi_index<N(subscribes), subscribes,
                        indexed_by<N(usr), const_mem_fun<subscribes, uint64_t, &subscribes::by_index1>>,
                        indexed_by<N(ven), const_mem_fun<subscribes, uint64_t, &subscribes::by_index2>>>
        subscribes_index;

    subscribes_index offers;
};

GRAPHENE_ABI(winds, (push)(additem)(getbypk)(getbyusr)(getbyven))
