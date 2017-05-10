#pragma once

namespace tasks
{
    class MappinABC
    {
    public:
        // Constraints
        //    - number of elements is between 3 and 3000
        //    - each element in is between 1 and 3000
        static int Run(const std::vector<int>& t);
    };
}

