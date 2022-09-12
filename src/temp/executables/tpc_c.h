
#include <unordered_map>


namespace MVTO {

struct Warehouse
{
    uint16_t w_id;
    double w_tax;
    double w_ytd;
};

struct Stock
{
    uint16_t s_w_id;
    uint32_t s_i_id;
    int16_t s_quantity;
    uint32_t ytd;
};

struct District
{
    uint8_t d_id; // 1
    uint32_t next_o_id; // 4
    double d_tax; //sizeof(double)
    double d_ytd;
};

struct NewOrder
{
    uint16_t no_w_id; //2
    uint8_t no_d_id; //1
    uint32_t no_o_id; // 4
};

struct OrderLine
{
    uint16_t ol_w_id;
    uint8_t ol_d_id;
    uint32_t ol_o_id;
    uint8_t ol_number;
    uint32_t ol_i_id;
    uint16_t ol_supply_w_id;
    uint8_t ol_quantity;
    double ol_amount;
};


class DataBase
{
  public:

    void CreateTable()
    {
        DataTable *table = new DataTable("District");
        tables_["District"] = table;
    }
    void GenerateInitData(int NUM)
    {
        size_t double_sz = sizeof(double);
        size_t value_sz = 1+4+double_sz + double_sz;
        for (int i = 0; i < NUM; i ++ )
        {
            // District *d = new District();
            if (i % 100000 == 0 )
                std::cout << i << "\n";
            
            Tuple *tuple = new Tuple(i, value_sz);
            District *d = reinterpret_cast<District *>( tuple->value_ );
            d->d_id = i;
            d->next_o_id += 1;
            d->d_tax = 0.1;
            d->d_ytd = 0.2;
            tables_["District"]->Insert(nullptr, i, *tuple);
        }

        std::vector<Tuple> out;
        std::cout << tables_["District"]->Select(nullptr, 86, out) << std::endl;

        std::cout << reinterpret_cast<District *>( out[0].value_)->d_tax << std::endl;


    }

    void RunNewOrder()
    {
        // update district next_oid
        // tables_["District"].update()

        //oid = next_o_id
        // insert into Orders

        // insert into NewOrder

        //  


        // update Stock s_quantity

        // ol_amount, total

        // insert into orderlines

    }

    std::unordered_map<std::string, DataTable *> tables_;
};


} // namepsace MVTO