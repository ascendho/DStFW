#pragma once

#include <optional>
#include "Grid/GridPoint.hpp"
#include "Grid/Grid.hpp"

// euclidean_dist(Float: x1, Float: y1, Float: x2, Float: y2):
//     return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
float euclidean_dist(float x1, float y1, float x2, float y2);

// MinDistToBin(Grid: g, Integer: xbin, Integer: ybin, Float: x, Float: y):
//   Returns the minimum possible Euclidean distance from point (x,y)
//   to any point inside the given bin. Returns Inf for invalid bins.
float MinDistToBin(const Grid& g, int xbin, int ybin, float x, float y);

// GridLinearScanNN(Grid: g, Float: x, Float: y):
//   Nearest-neighbor search by scanning all bins with min-distance pruning.
//   Returns pointer to closest GridPoint, or nullptr if the grid is empty.
GridPoint* GridLinearScanNN(const Grid& g, float x, float y);

// GridCheckBin(Grid: g, Integer: xbin, Integer: ybin,
//              Float: x, Float: y, Float: threshold):
//   Returns the closest point in the given bin that is strictly closer than
//   threshold, or nullptr if no such point exists.
GridPoint* GridCheckBin(const Grid& g, int xbin, int ybin,
                        float x, float y, float threshold);

// GridSearchExpanding(Grid: g, Float: x, Float: y):
//   Nearest-neighbor search using an expanding (diamond-shaped) search
//   that starts at the bin containing (x,y) and expands outward.
//   Returns pointer to closest GridPoint, or nullptr if the grid is empty.
GridPoint* GridSearchExpanding(const Grid& g, float x, float y);

#include "NearestNeighbor.tpp"
