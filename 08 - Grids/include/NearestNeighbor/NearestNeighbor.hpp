#pragma once

#include <optional>
#include "Grid/GridPoint.hpp"
#include "Grid/Grid.hpp"

// euclidean_dist(Float: x1, Float: y1, Float: x2, Float: y2):
//     return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
float euclidean_dist(float x1, float y1, float x2, float y2);

// MinDistToBin(Grid: g, Integer: xbin, Integer: ybin, Float: x, Float: y):
//   返回点 (x,y) 到给定单元格内任意点的最小可能欧几里得距离。
//   对于无效的单元格返回 Inf。
float MinDistToBin(const Grid& g, int xbin, int ybin, float x, float y);

// GridLinearScanNN(Grid: g, Float: x, Float: y):
//   通过扫描所有单元格并进行最小距离剪枝来执行最近邻搜索。
//   返回指向最近 GridPoint 的指针，若网格为空则返回 nullptr。
GridPoint* GridLinearScanNN(const Grid& g, float x, float y);

// GridCheckBin(Grid: g, Integer: xbin, Integer: ybin,
//              Float: x, Float: y, Float: threshold):
//   返回给定单元格中严格小于阈值距离的最近点，
//   若不存在这样的点则返回 nullptr。
GridPoint* GridCheckBin(const Grid& g, int xbin, int ybin,
                        float x, float y, float threshold);

// GridSearchExpanding(Grid: g, Float: x, Float: y):
//   使用从包含 (x,y) 的单元格开始向外扩展的菱形搜索
//   进行最近邻搜索。
//   返回指向最近 GridPoint 的指针，若网格为空则返回 nullptr。
GridPoint* GridSearchExpanding(const Grid& g, float x, float y);

#include "NearestNeighbor.tpp"
