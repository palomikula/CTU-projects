// V teto domaci uloze se Vas budeme snazit presvedcit, ze vykon vasi implementace zavisi do znacne miry na podobe
// vstupnich dat. Pri navrhu efektivnich algoritmu byste se tedy meli rozhodovat i na zaklade datove sady.
//
// Vasim ukolem je doimplementovat 4 ruzne metody na pocitani sumy kazdeho vstupniho vektoru ("data").
// V kazde metode budete pouzitivat OpenMP, ale pokazde trochu jinym zpusobem. Rychlost vypoctu Vasi implementace
// budeme porovnavat s nasi referencni implementaci. Vychazet muzete z metody sumsOfVectors_sequential implementovane
// v _executor/Executor.cpp.

#include <iostream>
#include <numeric>
#include <algorithm>
#include <omp.h>
#include "SumsOfVectors.h"
#include <numeric>

void
sumsOfVectors_omp_per_vector(const vector<vector<int8_t>> &data, vector<long> &solution, unsigned long minVectorSize) {
    // V teto metode si vyzkousite paralelizaci na urovni vektoru. Naimplementujte paralelni pristup
    // k vypocteni sum jednotlivych vektoru (sumu vektoru data[i] ulozte do solution[i]). V teto
    // funkci zpracovavejte jednotlive vektory sekvencne a paralelizujte nalezeni maxima v jednom
    // konkretnim vektoru. Tento pristup by mel byt zejmena vhodny, pokud mate maly pocet vektoru
    // velke delky.
    unsigned long len = data.size();
    for (unsigned long i = 0; i < len; i++) {
        long acc = 0;
        #pragma omp parallel reduction(+:acc)
        {
            const int threads = omp_get_num_threads();
            const int thread_id = omp_get_thread_num();
            unsigned long chunk_size = 1 + data[i].size() / threads;

            unsigned long begin = chunk_size * thread_id;
            unsigned long end = std::min(chunk_size * (thread_id + 1), data[i].size());

            for (unsigned long j = begin; j < end; j++) {
                acc += data[i][j];
            }
        }
        solution[i] = acc;
    }
}

void sumsOfVectors_omp_static(const vector<vector<int8_t>> &data, vector<long> &solution,
                              unsigned long minVectorSize) {
    // Pokud vektory, ktere zpracovavame nejsou prilis dlouhe, ale naopak jich musime zpracovat
    // velky pocet, muzeme zparalelizovat vnejsi for smycku, ktera prochazi pres jednotlive
    // vektory. Vyuzijte paralelizaci pres #pragma omp parallel for se statickym schedulingem.
    unsigned long num_vectors = data.size();
    #pragma omp parallel for schedule(static)
    for (unsigned long i = 0; i < num_vectors; i++) {
        long acc = 0;

        const unsigned long vector_size = data[i].size();
        for (unsigned long j = 0; j < vector_size; j++) {
            acc += data[i][j];
        }

        solution[i] = acc;
    }
}

void sumsOfVectors_omp_dynamic(const vector<vector<int8_t>> &data, vector<long> &solution,
                               unsigned long minVectorSize) {
    // Na cviceni jsme si ukazali, ze staticky scheduling je nevhodny, pokud praci mezi
    // jednotliva vlakna nedokaze rozdelit rovnomerne. V teto situaci muze byt vhodnym
    // resenim pouzitim dynamickeho schedulingu. Ten je rovnez vhodny v situacich, kdy
    // nevime predem, jak dlouho budou jednotlive vypocty trvat. Dani za to je vyssi
    // rezie pri zjistovani indexu 'i', ktery ma vlakno v dane chvili zpracovavat.

    unsigned long num_vectors = data.size();
    #pragma omp parallel for schedule(dynamic)
    for (unsigned long i = 0; i < num_vectors; i++) {
        long sum = 0;

        const unsigned long vector_size = data[i].size();
        for (unsigned long j = 0; j < vector_size; j++) {
            sum += data[i][j];
        }

        solution[i] = sum;
    }
}

void sumsOfVectors_omp_shuffle(const vector<vector<int8_t>> &data, vector<long> &solution,
                               unsigned long minVectorSize) {
    // Dalsi moznosti, jak se vyhnout problemum pri pouziti statickeho schedulingu (tj.,
    // zejmena nevyvazenemu rozdeleni prace) je predzpracovani dat. V teto funkci zkuste
    // data pred zparalelizovanim vnejsi for smycky (se statickym schedulingem) nejprve
    // prohazet. To samozrejme predpoklada, ze cas potrebny na predzpracovani je radove
    // mensi, nez zisk, ktery ziskame nahrazenim dynamickeho schedulingu za staticky.
    //
    // Tip: Abyste se vyhnuli kopirovani vektoru pri "prohazovani" (ktere muze byt velmi
    // drahe!), muzete prohazovat pouze pointery na vektory. Alternativou je vytvorit si
    // nejprve pole nahodne serazenych indexu a ty pak pouzit pro pristup k poli.
    // Uzitecnymi metodami mohou byt metody std::iota(...) (ktera Vam vygeneruje posloupnost
    // indexu) a std::random_shuffle(...), ktera zajisti nahodne prohazeni prvku.

    vector<unsigned long> indices(data.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::random_shuffle(indices.begin(), indices.end());
    const unsigned long num_vectors = indices.size();

#pragma omp parallel for schedule(static)
    for(unsigned long i = 0; i < num_vectors; i++){
        unsigned long index = indices[i];
        const unsigned long vector_size = data[index].size();

        long acc = 0;
        for (unsigned long j = 0; j < vector_size; j++) {
            acc += data[index][j];
        }
        solution[index] = acc;
    }
}