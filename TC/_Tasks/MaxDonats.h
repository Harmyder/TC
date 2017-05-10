#pragma once

namespace tasks
{
    class MaxDonats
    {
    public:
        // Constraints
        //     - donations contains between 2 and 40 elements, inclusive.
        //     - Each element in donations is between 1 and 1000, inclusive.
        static int Run(std::vector<int> donations);
    };
}
