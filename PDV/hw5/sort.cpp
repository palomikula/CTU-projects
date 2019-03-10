#include "sort.h"
#include <omp.h>

void rec(std::vector<std::string *> &vector_to_sort, const MappingFunction &mapping_function,
         unsigned long alphabet_size, unsigned long string_lengths, const unsigned int index) {
    if (index == string_lengths)
        return;

    std::vector<std::string *> buckets[alphabet_size];
    for (auto str_ptr : vector_to_sort)
        buckets[mapping_function((*str_ptr)[index])].push_back(str_ptr);

    #pragma omp parallel for
        for (auto i = 0; i < alphabet_size; i++)
            rec(buckets[i], mapping_function, alphabet_size, string_lengths, index + 1);

    auto offset = 0;
    for (auto i = 0; i < alphabet_size; i++) {
        std::copy(buckets[i].begin(), buckets[i].end(), vector_to_sort.begin()+offset);
        offset += buckets[i].size();
    }
}

void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mapping_function,
               unsigned long alphabet_size, unsigned long string_lengths) {
    rec(vector_to_sort, mapping_function, alphabet_size, string_lengths, 0);
}