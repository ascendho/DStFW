#pragma once

#include <cmath>
#include <cstdlib>

// ─── Constructor ─────────────────────────────────────────────────────────────
// x_bin_width = (x_end – x_start) / num_x_bins
// y_bin_width = (y_end – y_start) / num_y_bins
inline Grid::Grid(int nx, int ny, float xs, float xe, float ys, float ye)
    : num_x_bins(nx), num_y_bins(ny),
      x_start(xs), x_end(xe),
      x_bin_width((xe - xs) / nx),
      y_start(ys), y_end(ye),
      y_bin_width((ye - ys) / ny),
      bins(nx, std::vector<GridPoint*>(ny, nullptr))
{}

// ─── Destructor ───────────────────────────────────────────────────────────────
inline Grid::~Grid() {
    for (int xi = 0; xi < num_x_bins; ++xi) {
        for (int yi = 0; yi < num_y_bins; ++yi) {
            GridPoint* cur = bins[xi][yi];
            while (cur) {
                GridPoint* next = cur->next;
                delete cur;
                cur = next;
            }
        }
    }
}

// ─── approx_equal ─────────────────────────────────────────────────────────────
// approx_equal(Float: x1, Float: y1, Float: x2, Float: y2):
//     IF abs(x1 – x2) > threshold: return False
//     IF abs(y1 – y2) > threshold: return False
//     return True
inline bool approx_equal(float x1, float y1, float x2, float y2) {
    if (std::abs(x1 - x2) > APPROX_THRESHOLD) return false;
    if (std::abs(y1 - y2) > APPROX_THRESHOLD) return false;
    return true;
}

// ─── GridInsert ────────────────────────────────────────────────────────────────
// GridInsert(Grid: g, Float: x, Float: y):
// ❶ Integer: xbin = Floor((x - g.x_start) / g.x_bin_width)
//    Integer: ybin = Floor((y - g.y_start) / g.y_bin_width)
// ❷ IF xbin < 0 OR xbin >= g.num_x_bins: return False
//    IF ybin < 0 OR ybin >= g.num_y_bins: return False
// ❸ GridPoint: next_point = g.bins[xbin][ybin]
//    g.bins[xbin][ybin] = GridPoint(x, y)
//    g.bins[xbin][ybin].next = next_point
// ❹ return True
inline bool GridInsert(Grid& g, float x, float y) {
    // ❶ Compute bin indices
    int xbin = static_cast<int>(std::floor((x - g.x_start) / g.x_bin_width));
    int ybin = static_cast<int>(std::floor((y - g.y_start) / g.y_bin_width));

    // ❷ Check that the point is within the grid
    if (xbin < 0 || xbin >= g.num_x_bins) return false;
    if (ybin < 0 || ybin >= g.num_y_bins) return false;

    // ❸ Prepend new point to front of the bin's linked list
    GridPoint* new_point = new GridPoint(x, y);
    new_point->next = g.bins[xbin][ybin];
    g.bins[xbin][ybin] = new_point;

    // ❹ Return success
    return true;
}

// ─── GridDelete ────────────────────────────────────────────────────────────────
// GridDelete(Grid: g, Float: x, Float: y):
// ❶ Integer: xbin = Floor((x - g.x_start) / g.x_bin_width)
//    Integer: ybin = Floor((y - g.y_start) / g.y_bin_width)
// ❷ IF xbin < 0 OR xbin >= g.num_x_bins: return False
//    IF ybin < 0 OR ybin >= g.num_y_bins: return False
// ❸ IF g.bins[xbin][ybin] == null: return False
// ❹ GridPoint: current = g.bins[xbin][ybin]
//    GridPoint: previous = null
//    WHILE current != null:
// ❺     IF approx_equal(x, y, current.x, current.y):
// ❻         IF previous == null: g.bins[xbin][ybin] = current.next
//            ELSE: previous.next = current.next
//            return True
// ❼     previous = current; current = current.next
//    return False
inline bool GridDelete(Grid& g, float x, float y) {
    // ❶ Compute bin indices
    int xbin = static_cast<int>(std::floor((x - g.x_start) / g.x_bin_width));
    int ybin = static_cast<int>(std::floor((y - g.y_start) / g.y_bin_width));

    // ❷ Check that the point is within the grid
    if (xbin < 0 || xbin >= g.num_x_bins) return false;
    if (ybin < 0 || ybin >= g.num_y_bins) return false;

    // ❸ Check if the bin is empty
    if (g.bins[xbin][ybin] == nullptr) return false;

    // ❹ Traverse the linked list to find the first matching point
    GridPoint* current  = g.bins[xbin][ybin];
    GridPoint* previous = nullptr;
    while (current != nullptr) {
        // ❺ Test approximate equality
        if (approx_equal(x, y, current->x, current->y)) {
            // ❻ Splice the node out of the list
            if (previous == nullptr) {
                g.bins[xbin][ybin] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            return true;
        }
        // ❼ Advance to next node
        previous = current;
        current  = current->next;
    }
    return false;
}
