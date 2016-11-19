#ifndef VAO_HPP
#define VAO_HPP
#pragma once

#include "program.hpp"

const uint kIndicesPerTriangle = 3;
const uint kIndicesPerLine     = 2;
const uint kFloatsPerVertex    = 3;
const uint kFloatsPerColor     = 4;

uint createTrianglesVAO(float* vertices, uint* indices, float* colors, uint triangles_num);

uint createExtrusionVAO(float* vertices, uint* indices, float* colors, uint* triangles_num, float height);

void removeLine(uint* line_indices, uint* lines_num, uint index);

#endif
