#pragma once

// GridPoint {
//     Float: x
//     Float: y
//     GridPoint: next
// }
struct GridPoint {
    float x;
    float y;
    GridPoint* next;

    GridPoint(float x_, float y_) : x(x_), y(y_), next(nullptr) {}
};
