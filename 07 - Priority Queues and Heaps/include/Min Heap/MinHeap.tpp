#pragma once

// MinHeapInsert(MinHeap: heap, Type: value):
//     IF heap.last_index == heap.array_size - 1:
//         Increase Heap size.
//     heap.last_index = heap.last_index + 1
//     heap.array[heap.last_index] = value
//     # Swap the new node up the heap.
//     Integer: current = heap.last_index
//     Integer: parent = Floor(current / 2)
//  1  WHILE parent >= 1 AND (heap.array[parent] > heap.array[current]):  ← only change
//         swap(heap.array[parent], heap.array[current])
//         current = parent
//         parent = Floor(current / 2)
template<typename T>
void MinHeapInsert(Heap<T>& heap, const T& value) {
    if (heap.last_index == heap.array_size - 1) {
        int new_size = heap.array_size * 2;
        heap.array.resize(static_cast<std::size_t>(new_size));
        heap.array_size = new_size;
    }
    heap.last_index = heap.last_index + 1;
    heap.array[static_cast<std::size_t>(heap.last_index)] = value;

    int current = heap.last_index;
    int parent  = current / 2;
    // 1 Min-heap: bubble up when parent > current
    while (parent >= 1 &&
           heap.array[static_cast<std::size_t>(parent)] >
           heap.array[static_cast<std::size_t>(current)]) {
        T temp = heap.array[static_cast<std::size_t>(parent)];
        heap.array[static_cast<std::size_t>(parent)]  = heap.array[static_cast<std::size_t>(current)];
        heap.array[static_cast<std::size_t>(current)] = temp;
        current = parent;
        parent  = current / 2;
    }
}

// MinHeapRemoveMin(Heap: heap):
//     IF heap.last_index == 0:
//         return null
//     Type: result = heap.array[1]
//     heap.array[1] = heap.array[heap.last_index]
//     heap.array[heap.last_index] = null
//     heap.last_index = heap.last_index - 1
//     Integer: i = 1
//     WHILE i <= heap.last_index:
//         Integer: swap = i
//  1      IF 2*i <= heap.last_index AND heap.array[swap] > heap.array[2*i]:  ← changed
//             swap = 2*i
//  2      IF 2*i+1 <= heap.last_index AND heap.array[swap] > heap.array[2*i+1]:  ← changed
//             swap = 2*i+1
//         IF i != swap:
//             swap heap.array[i] and heap.array[swap]
//             i = swap
//         ELSE:
//             break
//     return result
template<typename T>
std::optional<T> MinHeapRemoveMin(Heap<T>& heap) {
    if (heap.last_index == 0) {
        return std::nullopt;
    }
    T result = heap.array[1];
    heap.array[1] = heap.array[static_cast<std::size_t>(heap.last_index)];
    heap.array[static_cast<std::size_t>(heap.last_index)] = T{};
    heap.last_index = heap.last_index - 1;

    int i = 1;
    while (i <= heap.last_index) {
        int swap_idx = i;
        // 1 Check left child — swap toward smaller
        if (2 * i <= heap.last_index &&
            heap.array[static_cast<std::size_t>(swap_idx)] >
            heap.array[static_cast<std::size_t>(2 * i)]) {
            swap_idx = 2 * i;
        }
        // 2 Check right child
        if (2 * i + 1 <= heap.last_index &&
            heap.array[static_cast<std::size_t>(swap_idx)] >
            heap.array[static_cast<std::size_t>(2 * i + 1)]) {
            swap_idx = 2 * i + 1;
        }
        if (i != swap_idx) {
            T temp = heap.array[static_cast<std::size_t>(i)];
            heap.array[static_cast<std::size_t>(i)]        = heap.array[static_cast<std::size_t>(swap_idx)];
            heap.array[static_cast<std::size_t>(swap_idx)] = temp;
            i = swap_idx;
        } else {
            break;
        }
    }
    return result;
}
