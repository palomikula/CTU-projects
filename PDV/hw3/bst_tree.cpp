#include <functional>
#include <atomic>
#include "bst_tree.h"


void bst_tree::insert(long long data) {

    node * new_node = new node(data);
    node * next = nullptr;
    node * cur = root;

    if (cur == nullptr) {
        root = new_node;
        return;
    }


    while (true) {
        if (cur->data < data) {
            next = cur->right;
            if (next == nullptr){
                if(cur->right.compare_exchange_strong(next, new_node)) {
                    return;
                }

            }else
                cur = cur->right;

        } else {
            next = cur->left;
            if (next == nullptr){
                if(cur->left.compare_exchange_strong(next, new_node)) {
                    return;
                }
            }else
                cur = cur->left;

        }
    }

}

bst_tree::~bst_tree() {
    // Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym
    // uzlum
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            cleanup(n->right);

            delete n;
        }
    };
    cleanup(root);
}
