#include "vao.hpp"

#include <iostream>

// uint createAskewVAO()
// {   

//     float vertices[] = {
//          0.9,  0.7,  0.0,
//         -0.3,  0.7,  0.0,
//         -0.9, -0.7,  0.0,
//          0.3, -0.7,  0.0,
//     };

//     uint indices[] = {
//         0, 1, 3,
//         1, 2, 3,
//     };

//     float colors[] = {
//          0.3,  0.7,  0.2,  1.0,
//          0.3,  0.7,  0.2,  1.0,
//          0.3,  0.7,  0.2,  1.0,
//          0.3,  0.7,  0.2,  1.0,
//     };

//     return createTrianglesVAO(vertices, indices, colors, 2);;
// }


uint createTrianglesVAO(float* vertices, uint* indices, float* colors, float* normals, uint triangles_num)
{   
    uint indices_num = kIndicesPerTriangle * triangles_num;
    uint vertices_num = 0;
    for (uint i = 0; i < indices_num; i++) {
        if (indices[i] >= vertices_num) vertices_num = indices[i] + 1;
    }

    // Set up VAO
    uint array = 0;
    glGenVertexArrays(1, &array);
    glBindVertexArray(array);
    printGLError();

    // Set up VBO
    uint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    printGLError();

    // Load vertex data into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * kFloatsPerVertex * vertices_num, vertices, GL_STATIC_DRAW);
    printGLError();

    // Set up vertex attribute
    glVertexAttribPointer(0, kFloatsPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    printGLError();

    // Set up index buffer
    uint indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    printGLError();

    // Load index data into buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices_num, indices, GL_STATIC_DRAW);
    printGLError();

    // Set up color buffer
    uint colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    printGLError();

    // Load color data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * kFloatsPerColor * vertices_num, colors, GL_STATIC_DRAW);
    printGLError();

    // Set up vertex attribute
    glVertexAttribPointer(1, kFloatsPerColor, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    printGLError();

    // Set up normal buffer
    uint normalBuffer = 0;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    printGLError();

    // Load normal data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * kFloatsPerVertex * vertices_num, normals, GL_STATIC_DRAW);
    printGLError();

    // Set up vertex attribute
    glVertexAttribPointer(2, kFloatsPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    printGLError();

    return array;
}



uint createExtrusionVAO(float* vertices, uint* indices, float* colors, uint triangles_num, float height, uint* out_triangles_num)
{   
    uint* line_indices = new uint[kIndicesPerLine * kIndicesPerTriangle * triangles_num];

    for (uint i = 0; i < triangles_num; i++) {
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i]     = indices[kIndicesPerTriangle * i];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 1] = indices[kIndicesPerTriangle * i + 1];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 2] = indices[kIndicesPerTriangle * i + 1];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 3] = indices[kIndicesPerTriangle * i + 2];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 4] = indices[kIndicesPerTriangle * i + 2];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 5] = indices[kIndicesPerTriangle * i];
    }

    uint lines_num = kIndicesPerTriangle * triangles_num;

    for (uint i = 0; i < lines_num; i++) {
        for (uint j = i + 1; j < lines_num; j++) {
            // Remove repeated lines
            if (line_indices[kIndicesPerLine * i] == line_indices[kIndicesPerLine * j] 
                && line_indices[kIndicesPerLine * i + 1] == line_indices[kIndicesPerLine * j + 1]) {
                removeLine(line_indices, &lines_num, j);
                j--;
                continue;
            }

            // Opposite lines cancel each other out (are inside shape)
            if (line_indices[kIndicesPerLine * i] == line_indices[kIndicesPerLine * j + 1] 
                && line_indices[kIndicesPerLine * i + 1] == line_indices[kIndicesPerLine * j]) {
                removeLine(line_indices, &lines_num, j);
                removeLine(line_indices, &lines_num, i);
                i--;
                break;
            }
        }
    }

    uint line_indices_num = kIndicesPerLine * lines_num;
    uint line_vertices_num = 0;
    for (uint i = 0; i < line_indices_num; i++) {
        if (line_indices[i] >= line_vertices_num) line_vertices_num = line_indices[i] + 1;
    }

    // 2 extrusion vertices per line vertex
    uint extrusion_vertices_num = 2 * line_vertices_num;
    float* extrusion_vertices = new float[kFloatsPerVertex * extrusion_vertices_num];
    for (uint i = 0; i < line_vertices_num; i++) {
        uint extrusion_vertices_i = kFloatsPerVertex * 2 * i;
        uint vertices_i = kFloatsPerVertex * i;
        extrusion_vertices[extrusion_vertices_i]     = vertices[vertices_i];
        extrusion_vertices[extrusion_vertices_i + 1] = vertices[vertices_i + 1];
        extrusion_vertices[extrusion_vertices_i + 2] = vertices[vertices_i + 2];
        extrusion_vertices[extrusion_vertices_i + 3] = vertices[vertices_i + 0];
        extrusion_vertices[extrusion_vertices_i + 4] = vertices[vertices_i + 1];
        extrusion_vertices[extrusion_vertices_i + 5] = vertices[vertices_i + 2] - height;
    }

    // for (uint i = 0; i < lines_num; i++) {
    //     std::cout << i << ':' << line_indices[kIndicesPerLine * i] << ' ' << line_indices[kIndicesPerLine * i + 1] << '\n'; 
    // }
    // std::cout << '\n';

    // for (uint i = 0; i < extrusion_vertices_num; i++) {
    //     std::cout << i << ':' << extrusion_vertices[kFloatsPerVertex * i] << ' ' << extrusion_vertices[kFloatsPerVertex * i + 1] << ' ' << extrusion_vertices[kFloatsPerVertex * i + 2] << '\n'; 
    // }
    // std::cout << '\n';

    // 2 extrusion triangles per line
    uint* extrusion_indices = new uint[kIndicesPerTriangle * 2 * lines_num];
    // one normal per vertex, same number of floats
    float* extrusion_normals = new float[kFloatsPerVertex * extrusion_vertices_num];
    for (uint i = 0; i < lines_num; i++) {
        uint extrusion_indices_i = kIndicesPerTriangle * 2 * i;
        uint line_indices_i = kIndicesPerLine * i;
 
        uint extrusion_index_start     = 2 * line_indices[line_indices_i];
        uint extrusion_index_start_ex  = extrusion_index_start + 1;
        uint extrusion_index_end       = 2 * line_indices[line_indices_i + 1];
        uint extrusion_index_end_ex    = extrusion_index_end + 1;
        
        extrusion_indices[extrusion_indices_i]     = extrusion_index_start;
        extrusion_indices[extrusion_indices_i + 1] = extrusion_index_start_ex;
        extrusion_indices[extrusion_indices_i + 2] = extrusion_index_end;
        extrusion_indices[extrusion_indices_i + 3] = extrusion_index_start_ex;
        extrusion_indices[extrusion_indices_i + 4] = extrusion_index_end_ex;
        extrusion_indices[extrusion_indices_i + 5] = extrusion_index_end;

        // for (uint j = 0; j < 6; j++) {
        //     std::cout << extrusion_indices[extrusion_indices_i + j] << ' ';
        // }
        // std::cout << '\n';

        uint extrusion_normals_i = kFloatsPerVertex * extrusion_index_start;
        float line_start_x = extrusion_vertices[kFloatsPerVertex * extrusion_index_start];
        float line_start_y = extrusion_vertices[kFloatsPerVertex * extrusion_index_start + 1];
        float line_end_x   = extrusion_vertices[kFloatsPerVertex * extrusion_index_end];
        float line_end_y   = extrusion_vertices[kFloatsPerVertex * extrusion_index_end + 1];
        extrusion_normals[extrusion_normals_i]     = line_end_y - line_start_y;
        extrusion_normals[extrusion_normals_i + 1] = line_start_x - line_end_x;
        extrusion_normals[extrusion_normals_i + 2] = 0.0f;
        extrusion_normals[extrusion_normals_i + 3] = extrusion_normals[extrusion_normals_i];
        extrusion_normals[extrusion_normals_i + 4] = extrusion_normals[extrusion_normals_i + 1];
        extrusion_normals[extrusion_normals_i + 5] = 0.0f;
    }
    // std::cout << '\n';

    // for (uint i = 0; i < 2 * lines_num; i++) {
    //     std::cout << i << ':' << extrusion_indices[kIndicesPerTriangle * i] << ' ' << extrusion_indices[kIndicesPerTriangle * i + 1] << ' ' << extrusion_indices[kIndicesPerTriangle * i + 2] << '\n'; 
    // }
    // std::cout << '\n';

    // one color per vertex 
    float* extrusion_colors = new float[kFloatsPerColor * extrusion_vertices_num];
    for (uint i = 0; i < line_vertices_num; i++) {
        uint extrusion_colors_i = kFloatsPerColor * 2 * i;
        uint colors_i = kFloatsPerColor * i;
        extrusion_colors[extrusion_colors_i]     = colors[colors_i];
        extrusion_colors[extrusion_colors_i + 1] = colors[colors_i + 1];
        extrusion_colors[extrusion_colors_i + 2] = colors[colors_i + 2];
        extrusion_colors[extrusion_colors_i + 3] = colors[colors_i + 3];
        extrusion_colors[extrusion_colors_i + 4] = colors[colors_i];
        extrusion_colors[extrusion_colors_i + 5] = colors[colors_i + 1];
        extrusion_colors[extrusion_colors_i + 6] = colors[colors_i + 2];
        extrusion_colors[extrusion_colors_i + 7] = colors[colors_i + 3];
    }

    *out_triangles_num = 2 * lines_num;

    return createTrianglesVAO(extrusion_vertices, extrusion_indices, extrusion_colors, extrusion_normals, *out_triangles_num);
}

void removeLine(uint* line_indices, uint* lines_num, uint index) {
    line_indices[kIndicesPerLine * index]     = line_indices[kIndicesPerLine * (*lines_num - 1)];
    line_indices[kIndicesPerLine * index + 1] = line_indices[kIndicesPerLine * (*lines_num - 1) + 1];
    *lines_num -= 1;
}




uint createBottomVAO(float* vertices, uint* indices, float* colors, float* normals, uint triangles_num, float height)
{   
    uint* bottom_indices = new uint[kIndicesPerTriangle * triangles_num];
    uint vertices_num = 0;
    for (uint i = 0; i < triangles_num; i++) {
        bottom_indices[kIndicesPerTriangle * i]     = indices[kIndicesPerTriangle * i];
        bottom_indices[kIndicesPerTriangle * i + 1] = indices[kIndicesPerTriangle * i + 2];
        bottom_indices[kIndicesPerTriangle * i + 2] = indices[kIndicesPerTriangle * i + 1];
        for (uint j = 0; j < 3; j++) {
            if (indices[kIndicesPerTriangle * i + j] >= vertices_num) vertices_num = indices[kIndicesPerTriangle * i + j] + 1;
        }
    }

    float* bottom_vertices = new float[kFloatsPerVertex * vertices_num];
    float* bottom_normals = new float[kFloatsPerVertex * vertices_num]; 
    for (uint i = 0; i < vertices_num; i++) {
        bottom_vertices[kFloatsPerVertex * i]     = vertices[kFloatsPerVertex * i];
        bottom_vertices[kFloatsPerVertex * i + 1] = vertices[kFloatsPerVertex * i + 1];
        bottom_vertices[kFloatsPerVertex * i + 2] = vertices[kFloatsPerVertex * i + 2] - height;
        
        bottom_normals[kFloatsPerVertex * i]     = -normals[kFloatsPerVertex * i];
        bottom_normals[kFloatsPerVertex * i + 1] = -normals[kFloatsPerVertex * i + 1];
        bottom_normals[kFloatsPerVertex * i + 2] = -normals[kFloatsPerVertex * i + 2];
    }

    return createTrianglesVAO(bottom_vertices, bottom_indices, colors, bottom_normals, triangles_num);
}

Model createFlatModel(float* vertices2d, uint* indices, float* color, uint triangles_num, float height, bool has_bottom)
{   
    uint indices_num = kIndicesPerTriangle * triangles_num;
    uint vertices_num = 0;
    for (uint i = 0; i < indices_num; i++) {
        if (indices[i] >= vertices_num) vertices_num = indices[i] + 1;
    }

    float* vertices = new float[kFloatsPerVertex * vertices_num];
    float* colors   = new float[kFloatsPerColor  * vertices_num];
    float* normals  = new float[kFloatsPerVertex * vertices_num];
    for (uint i = 0; i < vertices_num; i++) {
        // 2 floats per vertices for 2d vertices
        vertices[kFloatsPerVertex * i]     = vertices2d[2 * i];
        vertices[kFloatsPerVertex * i + 1] = vertices2d[2 * i + 1];
        vertices[kFloatsPerVertex * i + 2] = 0.5f * height;

        colors[kFloatsPerColor * i]     = color[0];
        colors[kFloatsPerColor * i + 1] = color[1];
        colors[kFloatsPerColor * i + 2] = color[2];
        colors[kFloatsPerColor * i + 3] = color[3];

        normals[kFloatsPerVertex * i]     = 0.0f;
        normals[kFloatsPerVertex * i + 1] = 0.0f;
        normals[kFloatsPerVertex * i + 2] = 1.0f;
    }

    Model model;
    model.array = createTrianglesVAO(vertices, indices, colors, normals, triangles_num);
    model.triangles_num = triangles_num;
    if (has_bottom) {
        model.has_bottom = true;
        model.array_bottom = createBottomVAO(vertices, indices, colors, normals, triangles_num, height);
    } 
    if (height > 0) {
        model.has_extrusion = true;
        model.array_ex = createExtrusionVAO(vertices, indices, colors, triangles_num, height, &(model.triangles_ex_num));
    }
    return model;
}

void renderModel(Model model, uint uniform_location)
{
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(model.transform));

    glBindVertexArray(model.array);
    glDrawElements(GL_TRIANGLES, kIndicesPerTriangle * model.triangles_num, GL_UNSIGNED_INT, 0);
    printGLError();

    // if (model.has_bottom) {
    //     glBindVertexArray(model.array_bottom);
    //     // glDrawElements(GL_TRIANGLES, kIndicesPerTriangle * model.triangles_num, GL_UNSIGNED_INT, 0);
    //     printGLError();
    // }

    // if (model.has_extrusion) {
    //     glBindVertexArray(model.array_ex);
    //     // glDrawElements(GL_TRIANGLES, kIndicesPerTriangle * model.triangles_ex_num, GL_UNSIGNED_INT, 0);
    //     printGLError();
    // }
}
