//
// Created by Haokun.hu on 2018/11/1.
// ref: https://coolshell.cn/articles/8239.html
//

#ifndef HODIS_LOCK_FREE_QUEUE_H
#define HODIS_LOCK_FREE_QUEUE_H

#include <atomic>
#include <exception>

template <typename T>

class LockFreeQueue {

    LockFreeQueue();

    ~LockFreeQueue();

    void EnQueue(T&& element) {
        Node * newNode = new Node(element, nullptr);

        Node* p;
        do {
            p = tail_;
        } while (!std::atomic_compare_exchange_weak(p->next, nullptr, newNode));

        std::atomic_compare_exchange_weak(tail_, p, newNode);
    }

    T DeQueue() {
        Node* p;
        do {
            p = head_;
            if (p->next == nullptr) {
                // TODO handle empty queue
                throw std::exception();
            }
        } while (!std::atomic_compare_exchange_weak(head_, p, p->next));
        return p->next->element;
    }

private:

    struct Node {
        T element;
        Node* next = nullptr;
    };

    Node* head_ = new Node();
    Node* tail_ = new Node();


};


#endif //HODIS_LOCK_FREE_QUEUE_H
