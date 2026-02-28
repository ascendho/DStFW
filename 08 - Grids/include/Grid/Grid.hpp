#pragma once

#include <vector>
#include "GridPoint.hpp"

// Grid {
//     Integer: num_x_bins
//     Integer: num_y_bins
//     Float: x_start
//     Float: x_end
//     Float: x_bin_width
//     Float: y_start
//     Float: y_end
//     Float: y_bin_width
//     Matrix of GridPoints: bins
// }
struct Grid {
    int num_x_bins;
    int num_y_bins;
    float x_start;
    float x_end;
    float x_bin_width;
    float y_start;
    float y_end;
    float y_bin_width;
    std::vector<std::vector<GridPoint*>> bins;

    Grid(int nx, int ny, float xs, float xe, float ys, float ye);
    ~Grid();
};

// approx_equal 用于浮点数比较的阈值
static constexpr float APPROX_THRESHOLD = 1e-5f;

bool approx_equal(float x1, float y1, float x2, float y2);

bool GridInsert(Grid& g, float x, float y);
bool GridDelete(Grid& g, float x, float y);

#include "Grid.tpp"
