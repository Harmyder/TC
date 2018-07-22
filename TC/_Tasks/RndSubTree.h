#pragma once

namespace tasks
{
    class RndSubTree final
    {
    public:
        int count(int k) { return Run(k); }
        int Run(int k);
    };
}
