#ifndef VAO_HPP
#define VAO_HPP
#pragma once

#include "program.hpp"

typedef struct {
    uint array, array_bottom, array_ex;
    uint triangles_num, triangles_ex_num;
    bool has_bottom, has_extrusion;
    glm::mat4 transform;
} Model;

const uint kIndicesPerTriangle = 3;
const uint kIndicesPerLine     = 2;
const uint kFloatsPerVertex    = 3;
const uint kFloatsPerColor     = 4;

uint createTrianglesVAO(float* vertices, uint* indices, float* colors, float* normals, uint triangles_num);

uint createExtrusionVAO(float* vertices, uint* indices, float* colors, uint triangles_num, float height, uint* out_triangles_num);

void removeLine(uint* line_indices, uint* lines_num, uint index);

uint createBottomVAO(float* vertices, uint* indices, float* colors, float* normals, uint triangles_num, float height);

Model createFlatModel(float* vertices2d, uint* indices, float* color, uint triangles_num, float height, bool has_bottom);

void renderModel(Model model, uint uniform_location);


#endif
