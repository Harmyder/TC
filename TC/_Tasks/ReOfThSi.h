#pragma once

namespace tasks
{
    class ReOfThSi final
    {
    public:
        // steps - differences between plates
        // T - number of plates we allowed to move
        // D - safe height to jump
        int Run(const std::vector<int>& steps, int T, int D);

    private:
        int& Subtask(size_t bottom, size_t top, size_t moveable);
        
    private:
        std::vector<std::vector<std::vector<int>>> dp_;
    };
}
