#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <tuple>
#include <vector>
#include <stdexcept>

#include "job.hpp"

template <typename T>
struct MaxHeap {
    std::vector<std::tuple<T, int>> data;

    void push(std::tuple<T, int> value) {
        data.push_back(value);

        for (int i = data.size() - 1; i > 0;) {
            int parent = (i - 1) / 2;
            if (std::get<1>(data[i]) > std::get<1>(data[parent])) {
                std::swap(data[i], data[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }
    std::tuple<T, int> pop() {
        if (data.empty()) {
            throw std::underflow_error("All jobs complete");
        }
        std::tuple<T, int> result = data[0];
        data[0] = data[data.size() - 1];
        data.pop_back();

        int current = 0;
        int size = data.size();

        while (current < size) {
            int left = current * 2 + 1;
            int right = current * 2 + 2;
            int maxChild = current;

            if (left < size && std::get<1>(data[left]) > std::get<1>(data[maxChild])) {
                maxChild = left;
            }
            if (right < size && std::get<1>(data[right]) > std::get<1>(data[maxChild])) {
                maxChild = right;
            }
            if (maxChild != current) {
                std::swap(data[current], data[maxChild]);
                current = maxChild;
            } else {
                break;
            }

        }
        return result;
    }

    bool empty () {
        return data.empty();
    }
};

template <typename T>
class PriorityQueue {
private:
    MaxHeap<T> heap;
public:
    void push(T value, int priority) {
        heap.push(std::make_tuple(value, priority));
    }
    T pop() {
        return std::get<0>(heap.pop());
    }
    bool empty() {
        return heap.empty();
    }
};


class JobQueue {
private:
    PriorityQueue<Job> order;
public:
    void push(Job job) {
        order.push(job, job.getPriority());
    }
    void runHighestPriority() {
        if (order.empty()) {
            throw std::underflow_error("All jobs complete");
        }
        Job task = order.pop();

        task.execute();
    }
};

#endif