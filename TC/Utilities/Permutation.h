#pragma once

namespace utilities
{
    class Permutation final
    {
        enum { NO_INDEX = -1 };
    public:
        template <class ArrayRandomIterator, class ConstIndicesRandomIterator>
        static void FromIndices(
            ArrayRandomIterator array_begin, ArrayRandomIterator array_end, 
            ConstIndicesRandomIterator indices_begin);
    };

    template <class ArrayRandomIterator, class ConstIndicesRandomIterator>
    void Permutation::FromIndices(
        ArrayRandomIterator array_begin, ArrayRandomIterator array_end,
        ConstIndicesRandomIterator indices_begin)
    {
        const int elems_count = static_cast<int>(array_end - array_begin);

        std::set<int> not_revised;
        for (int i = 0; i < elems_count; ++i)
        {
            not_revised.insert(not_revised.end(), i);
        }

        while (!not_revised.empty())
        {
            ArrayRandomIterator::value_type saved_value;
            int saved_index = NO_INDEX;
            int index_next = *not_revised.begin();

            while (index_next != saved_index)
            {
                const int index_to = index_next;

                if (saved_index == NO_INDEX)
                {
                    saved_index = index_to;
                    saved_value = *(array_begin + index_to);
                }
                index_next = *(indices_begin + index_to);
                *(array_begin + index_to) = index_next == saved_index ? saved_value : *(array_begin + index_next);
                not_revised.erase(index_to);
            }
        }
    }
}
