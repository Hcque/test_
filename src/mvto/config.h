


class Config
{

  public:
    uint32_t num_Threads, num_warehouse;

    static Config& get_mutable_config()
    {
        static Config& config;
        return config;
    }
    static const Config& get_config()
    {
        return get_mutable_config();
    }


};
