namespace std
{
    template<> struct hash<pair<int, int>>
    {
        typedef pair<int, int> argument_type;
        typedef size_t result_type;
        result_type operator()(argument_type const& s) const
        {
            return s.first ^ (s.second * 31);
        }
    };
}