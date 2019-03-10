#include <queue>
#include <omp.h>
#include "bfs.h"
//#include "concurrent_set.h"
#include <vector>
#include <unordered_set>
#include <mutex>
#include <thread>
#include <bits/unordered_map.h>
#include <langinfo.h>

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi cesty v grafu.
// V teto metode nemusite prilis optimalizovat pametove naroky, a vhodnym algo-
// ritmem tak muze byt napriklad pouziti prohledavani do sirky (breadth-first
// search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi cesty.

std::mutex m;

bool contains (std::unordered_set<unsigned long long int> & set, unsigned long long int & item)
{
    std::unique_lock<std::mutex> mlock(m);
    return (set.find(item) != set.end());
}

void insert(std::unordered_set<unsigned long long int> & set, unsigned long long int & item)
{
    std::unique_lock<std::mutex> mlock(m);
    set.insert(item);
}

std::shared_ptr<const state> bfs_help(std::vector<std::shared_ptr<const state>> &level,std::unordered_set <unsigned long long int>  &history) {
    if(level.empty()) return nullptr;
    std::vector<std::vector<std::shared_ptr<const state>>> new_level;
    new_level.resize(omp_get_max_threads());
    std::shared_ptr<const state> ret = nullptr;
    bool found = false;
    auto sz = level.size();

#pragma omp parallel for
        for (int i = 0; i < sz; ++i) {
            auto tmp = level[i]->next_states();
            for (auto st : tmp) {
                auto id =st->get_identifier();
                if (!contains(history, id)) {
                    if (st->is_goal()) {
                        //printf("found it\n");
                        found = true;
                        ret = st;
                        #pragma omp cancel for
                    }
                    insert(history,id);
                    new_level[omp_get_thread_num()].push_back(st);
                    if (found) {
                        #pragma omp cancel for
                    }

                }
            }
            if (found) {
                #pragma omp cancel for
            }
            //new_level[omp_get_thread_num()].insert(new_level[omp_get_thread_num()].end(), tmp.begin(), tmp.end());

        }
    if(found) return ret;
    for (int j = 1; j < omp_get_max_threads(); ++j) {
        new_level[0].insert(new_level[0].end(),new_level[j].begin(),new_level[j].end());
    }

    return bfs_help(new_level[0],history);
}

std::shared_ptr<const state> bfs(std::shared_ptr<const state> root){
    if(root->is_goal()) return root;
    auto level = root->next_states();
    std::unordered_set <unsigned long long int> history;
    auto id =root->get_identifier();
    insert(history,id);
    for (auto st : level) {
        if (st->is_goal()) {
            printf("found it\n");
            return st;
        }
        id = st->get_identifier();
        insert(history,id);
    }
    return bfs_help(level,history);
}

//std::shared_ptr<const state> bfs(std::shared_ptr<const state> root){
//    //print_queue(q);
//    auto cur = root;
//    std::queue<std::shared_ptr<const state>> q;
//    //printf("I am %d\n", omp_get_thread_num());
//    std::shared_ptr<const state> ret = nullptr;
//
//        do {
//            std::vector<std::shared_ptr<const state>> next_states;
//            next_states = cur->next_states();
//
//        for (auto st : next_states) {
//            if(st->is_goal()){
//                printf("found it\n");
//                return st;
//            }
//            q.push(st);
//        }
//        cur = q.front(); q.pop();
//    } while (!q.empty());
//    printf("didn't find it\n");
//    return ret;
//
//}

//std::shared_ptr<const state> bfs(std::shared_ptr<const state> root) {
//
//    std::queue<std::shared_ptr<const state>> q;
//    auto cur = root;
//    std::vector<std::queue<std::shared_ptr<const state>>> queues;
//    queues.resize(omp_get_max_threads());
//    std::shared_ptr<const state> ret = nullptr;
//    do {
//        auto next_states = cur->next_states();
//
//        for (auto st : next_states) {
//            if(st->is_goal()){
//                printf("found it\n");
//                return st;
//            }
//            q.push(st);
//        }
//        if(q.size() > omp_get_max_threads()) break;
//        cur = q.front(); q.pop();
//        if(q.empty()) return nullptr;
//    }while(true);
//
//    int sz = q.size();
//    printf("size is %d\n",q.size());
//
//    for (int i = 0; i < sz; ++i) {
//        printf("%d going to %d\n",i,i% omp_get_max_threads());
//        queues[(i % omp_get_max_threads())].push(q.front());
//        q.pop();
//    }
//    ret = nullptr;
//
//    std::vector<std::thread> threads;
//
//        for (int j = 0; j < omp_get_max_threads(); j++) {
//            //if (ret != nullptr)continue;
//            threads.push_back(std::thread(bfs_help,queues[j], &ret));
//
//        }
//    for (int j = 0; j < omp_get_max_threads(); j++) {
//        //if (ret != nullptr)continue;
//        threads[j].join();
//    }
//
//    return ret;
//
//}





