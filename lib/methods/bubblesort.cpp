#include "bubblesort.hpp"

namespace methods {
namespace bubblesort {

    unsigned int sort(Permutation& vector, int index_0, int index) {
        unsigned int performed_bops = 0;
        bool swaped = true;

        /* for (size_t i = index; swaped && i >= index_0 + 1; i--) {
            swaped = false;
            for (size_t j = index_0; j <= i - 1; j++) {
                if (performed_bops++ && vector.at(j) > vector.at(j + 1)) {
                    std::swap(vector.at(j), vector.at(j + 1));
                    swaped = true;
                }
            }
        }

        return performed_bops;*/

        for (size_t i = 0; swaped && i < vector.size() - 1; i++) {
            swaped = false;
            for (size_t j = 0; j < vector.size() - i - 1; j++) {
                if (performed_bops++ && vector.at(j) > vector.at(j + 1)) {
                    std::swap(vector.at(j), vector.at(j + 1));
                    swaped = true;
                }
            }
        }
    
        return performed_bops;
    }

} // namespace bubblesort
} // namespace methods
