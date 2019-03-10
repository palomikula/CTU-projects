#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda pro VSECHNY dilci dotazy (obsazene ve
    // vektoru 'predicates') existuje alespon jeden zaznam v tabulce (reprezentovane vektorem
    // 'data_table'), pro ktery je dany predikat splneny.

    // Pro inspiraci si prostudujte kod, kterym muzete zjistit, zda prvni dilci dotaz plati,
    // tj., zda existuje alespon jeden radek v tabulce, pro ktery predikat reprezentovany
    // funkci predicates[i] plati:

    bool ret = true;
    auto length = predicates.size();
    auto row_count = data_table.size();

#pragma omp parallel for shared(ret)
    for (int i = 0; i < length; i++) {
        if (!ret) continue;

        auto &predicate = predicates[i];
        auto satisfied = false;

        for (unsigned int j = 0; j < row_count; j++) {
            auto &row = data_table[j];                // j-ty radek tabulky ...

            if (predicate(row)) {
                satisfied = true;
                break;
            }
        }

        if (!satisfied) {
            ret = false;
        }

    }

    return ret;
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda je ALESPON JEDEN dilci dotaz pravdivy.
    // To znamena, ze mate zjistit, zda existuje alespon jeden predikat 'p' a jeden zaznam
    // v tabulce 'r' takovy, ze p(r) vraci true.

    // Zamyslete se nad tim, pro ktery druh dotazu je vhodny jaky druh paralelizace. Vas
    // kod optimalizujte na situaci, kdy si myslite, ze navratova hodnota funkce bude true.
    // Je pro Vas dulezitejsi rychle najit splnujici radek pro jeden vybrany predikat, nebo
    // je dulezitejsi zkouset najit takovy radek pro vice predikatu paralelne?

    bool ret = false;
    auto length = predicates.size();
    auto row_count = data_table.size();

#pragma omp parallel for shared(ret)
    for (int i = 0; i < row_count; i++) {
        if (ret) continue;

        auto &row = data_table[i];
        auto satisfied = false;
        for (unsigned int j = 0; j < length; j++) {
            auto &predicate = predicates[j];

            if (predicate(row)) {
                satisfied = true;
                break;
            }
        }

        if (satisfied) {
            ret = true;
        }
    }

    return ret;
}


#endif //DATABASEQUERIES_QUERY_H
