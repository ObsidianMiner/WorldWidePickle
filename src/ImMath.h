#pragma once
#include <imgui/imgui.h>
// Add ImVec2 + ImVec2
inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
    return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

// Subtract ImVec2 - ImVec2
inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
    return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Multiply ImVec2 * scalar
inline ImVec2 operator*(const ImVec2& lhs, float scalar) {
    return ImVec2(lhs.x * scalar, lhs.y * scalar);
}

// Divide ImVec2 / scalar
inline ImVec2 operator/(const ImVec2& lhs, float scalar) {
    return ImVec2(lhs.x / scalar, lhs.y / scalar);
}

// Compound assignment: +=
inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

// Compound assignment: -=
inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}