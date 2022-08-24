
// TPC-C table layout

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
    int16_t s_quantuty;
    uint32_t s_ytd;
    uint16_t s_order_count;
    uint16_t s_remote_count;
};

struct District
{
    uint16_t d_w_id;
    uint8_t d_id;
    uint32_t d_next_o_id;
    double d_tax;
    double d_ytd;
};


struct Customer
{
    uint16_t c_w_id;
    uint8_t c_d_id;
    uint32_t c_id;
    uint16_t c_payment_cnt;
    uint16_t c_delivery_cnt;
};

struct History
{

};

struct NewOrder
{
    uint16_t no_w_id;
    uint8_t no_d_id;
    uint32_t no_o_id;

};


struct OrderLine
{
    uint16_t ol_w_id;
    uint8_t ol_d_id;
    uint32_t ol_o_id;
    uint8_t ol_number;
    uint32_t ol_i_id;
    uint16_tol_supply_w_id;
    uint8_t ol_quantity;
    double ol_amount;
};


// new order txn
//
class NewOrderTransaction
{
  public:
    NewOrderTransaction()
    {
        generate();
    }
    void generate()
    {

    }

    void run()
    {
        // input is : wid, d_id, c_id, 

        // update district next_o_i + 1

        // insert into ORDERS : (o_id, d_id, w_id, c_id, datetime, o_ol_cnt, O_all_local

        // insert into NewOrder (O_id, d_id, w_id )
        // ol_supply_id,
        // ol_i_id,
        // ol_quantity

        // update Stock : s_quantity
        // ol_amount = ol_quantity * i_price * (1 + w_tax + d_tax ) * (1 - c_discount );

        // insert into OrderLine (ol_o_id, ol_d_id, ol_w_id,ol_number, 
                                //  ol_i_id, ol_supply_w_id, 
                                //  ol_quantity, ol_amount, ol_dist_info
                                //  );
        // commit




    }



};


// payment txn

////////






