#pragma once

// HeapInsert(Heap: heap, Type: value):
//  1  IF heap.last_index == heap.array_size - 1:
//         Increase Heap size.  (array doubling)
//  2  heap.last_index = heap.last_index + 1
//     heap.array[heap.last_index] = value
//     # Swap the new node up the heap.
//  3  Integer: current = heap.last_index
//     Integer: parent = Floor(current / 2)
//  4  WHILE parent >= 1 AND (heap.array[parent] < heap.array[current]):
//  5      swap(heap.array[parent], heap.array[current])
//         current = parent
//         parent = Floor(current / 2)
template<typename T>
void HeapInsert(Heap<T>& heap, const T& value) {
    // 1 若已满则扩容
    if (heap.last_index == heap.array_size - 1) {
        int new_size = heap.array_size * 2;
        heap.array.resize(static_cast<std::size_t>(new_size));
        heap.array_size = new_size;
    }
    // 2 追加到末尾
    heap.last_index = heap.last_index + 1;
    heap.array[static_cast<std::size_t>(heap.last_index)] = value;

    // 3 上浮
    int current = heap.last_index;
    int parent  = current / 2;
    // 4 当父节点存在且父节点 < 当前节点时（最大堆：父节点应 >= 子节点）
    while (parent >= 1 &&
           heap.array[static_cast<std::size_t>(parent)] <
           heap.array[static_cast<std::size_t>(current)]) {
        // 5 交换
        T temp = heap.array[static_cast<std::size_t>(parent)];
        heap.array[static_cast<std::size_t>(parent)]  = heap.array[static_cast<std::size_t>(current)];
        heap.array[static_cast<std::size_t>(current)] = temp;
        current = parent;
        parent  = current / 2;
    }
}

// HeapRemoveMax(Heap: heap):
//  1  IF heap.last_index == 0:
//         return null
//  2  Type: result = heap.array[1]
//     heap.array[1] = heap.array[heap.last_index]
//     heap.array[heap.last_index] = null   (cleared)
//     heap.last_index = heap.last_index - 1
//     # Bubble the new root down.
//     Integer: i = 1
//  3  WHILE i <= heap.last_index:
//         Integer: swap = i
//  4      IF 2*i <= heap.last_index AND heap.array[swap] < heap.array[2*i]:
//             swap = 2*i
//  5      IF 2*i+1 <= heap.last_index AND heap.array[swap] < heap.array[2*i+1]:
//             swap = 2*i+1
//  6      IF i != swap:
//             swap heap.array[i] and heap.array[swap]
//             i = swap
//         ELSE:
//             break
//     return result
template<typename T>
std::optional<T> HeapRemoveMax(Heap<T>& heap) {
    // 1 空堆检查
    if (heap.last_index == 0) {
        return std::nullopt;
    }
    // 2 保存根节点，将最后一个元素提升为根节点，收缩堆
    T result = heap.array[1];
    heap.array[1] = heap.array[static_cast<std::size_t>(heap.last_index)];
    heap.array[static_cast<std::size_t>(heap.last_index)] = T{};  // 清除
    heap.last_index = heap.last_index - 1;

    // 3 下沉
    int i = 1;
    while (i <= heap.last_index) {
        int swap_idx = i;
        // 4 检查左子节点
        if (2 * i <= heap.last_index &&
            heap.array[static_cast<std::size_t>(swap_idx)] <
            heap.array[static_cast<std::size_t>(2 * i)]) {
            swap_idx = 2 * i;
        }
        // 5 检查右子节点
        if (2 * i + 1 <= heap.last_index &&
            heap.array[static_cast<std::size_t>(swap_idx)] <
            heap.array[static_cast<std::size_t>(2 * i + 1)]) {
            swap_idx = 2 * i + 1;
        }
        // 6 执行交换或跳出
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

// UpdateMaxValue(Heap: heap, Integer: index, Type: value):
//     Type: old_value = heap.array[index]
//     heap.array[index] = value
//     IF old_value < value:
//         Bubble the element up (as in insert)
//     ELSE:
//         Drop the element down (as in remove-max)
template<typename T>
void UpdateMaxValue(Heap<T>& heap, int index, const T& value) {
    T old_value = heap.array[static_cast<std::size_t>(index)];
    heap.array[static_cast<std::size_t>(index)] = value;

    if (old_value < value) {
        // 上浮
        int current = index;
        int parent  = current / 2;
        while (parent >= 1 &&
               heap.array[static_cast<std::size_t>(parent)] <
               heap.array[static_cast<std::size_t>(current)]) {
            T temp = heap.array[static_cast<std::size_t>(parent)];
            heap.array[static_cast<std::size_t>(parent)]  = heap.array[static_cast<std::size_t>(current)];
            heap.array[static_cast<std::size_t>(current)] = temp;
            current = parent;
            parent  = current / 2;
        }
    } else {
        // 下沉
        int i = index;
        while (i <= heap.last_index) {
            int swap_idx = i;
            if (2 * i <= heap.last_index &&
                heap.array[static_cast<std::size_t>(swap_idx)] <
                heap.array[static_cast<std::size_t>(2 * i)])
                swap_idx = 2 * i;
            if (2 * i + 1 <= heap.last_index &&
                heap.array[static_cast<std::size_t>(swap_idx)] <
                heap.array[static_cast<std::size_t>(2 * i + 1)])
                swap_idx = 2 * i + 1;
            if (i != swap_idx) {
                T temp = heap.array[static_cast<std::size_t>(i)];
                heap.array[static_cast<std::size_t>(i)]        = heap.array[static_cast<std::size_t>(swap_idx)];
                heap.array[static_cast<std::size_t>(swap_idx)] = temp;
                i = swap_idx;
            } else {
                break;
            }
        }
    }
}
