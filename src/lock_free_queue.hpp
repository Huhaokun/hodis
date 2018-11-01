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
public:

    LockFreeQueue();

    ~LockFreeQueue();

    void EnQueue(T&& element) {
        Node * newNode = new Node(element, nullptr);

        Node* p;

        // poll tail until it's the real tail
        do {
            p = tail_;
        } while (!std::atomic_compare_exchange_weak(p->next, nullptr, newNode));

        std::atomic_compare_exchange_weak(head_->next, nullptr, newNode);
        std::atomic_compare_exchange_weak(tail_, p, newNode);

    }

    T DeQueue() {
        Node* p;

        // poll head until it's the real head
        do {
            p = head_;
            if (p->next == nullptr) {
                // TODO use <either> or <optional> to handle empty queue
                throw std::out_of_range("empty queue");
            }
        } while (!head_.compare_exchange_weak(p, p->next));
        return p->next->element;
    }

private:

    struct Node {
        T element;
        Node* next = nullptr;
    };

    std::atomic<Node*> head_;
    std::atomic<Node*> tail_;


};


#endif //HODIS_LOCK_FREE_QUEUE_H
