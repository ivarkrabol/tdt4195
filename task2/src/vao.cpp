#include "vao.hpp"

#include <iostream>

uint createAskewVAO()
{   

    float vertices[] = {
         0.9,  0.7,  0.0,
        -0.3,  0.7,  0.0,
        -0.9, -0.7,  0.0,
         0.3, -0.7,  0.0,
    };

    uint indices[] = {
        0, 1, 3,
        1, 2, 3,
    };

    float colors[] = {
         0.3,  0.7,  0.2,  1.0,
         0.3,  0.7,  0.2,  1.0,
         0.3,  0.7,  0.2,  1.0,
         0.3,  0.7,  0.2,  1.0,
    };

    return createTrianglesVAO(vertices, indices, colors, 2);;
}


uint createTrianglesVAO(float* vertices, uint* indices, float* colors, uint triangles_num)
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
    glBufferData(GL_ARRAY_BUFFER, vertices_num * kFloatsPerVertex * sizeof(float), vertices, GL_STATIC_DRAW);
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

    // Load index data into index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_num * sizeof(int), indices, GL_STATIC_DRAW);
    printGLError();

    // Set up color buffer
    uint colorBuffer = 0;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    printGLError();

    // Load color data into color buffer
    glBufferData(GL_ARRAY_BUFFER, vertices_num * kFloatsPerColor * sizeof(float), colors, GL_STATIC_DRAW);
    printGLError();

    // Set up vertex attribute
    glVertexAttribPointer(1, kFloatsPerColor, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    printGLError();

    return array;
}



uint createExtrusionVAO(float* vertices, uint* indices, float* colors, uint* triangles_num, float height)
{   
    uint* line_indices = new uint[kIndicesPerLine * kIndicesPerTriangle * *triangles_num];

    for (uint i = 0; i < *triangles_num; i++) {
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i]     = indices[kIndicesPerTriangle * i];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 1] = indices[kIndicesPerTriangle * i + 1];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 2] = indices[kIndicesPerTriangle * i + 1];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 3] = indices[kIndicesPerTriangle * i + 2];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 4] = indices[kIndicesPerTriangle * i + 2];
        line_indices[kIndicesPerLine * kIndicesPerTriangle * i + 5] = indices[kIndicesPerTriangle * i];
    }

    uint lines_num = kIndicesPerTriangle * *triangles_num;

    std::cout << lines_num << "\n\n";
    for (uint k = 0; k < lines_num; k++) {                            
        std::cout << line_indices[kIndicesPerLine * k] << ' ' << line_indices[kIndicesPerLine * k + 1] << '\n';
    }
    std::cout << '\n';

    for (uint i = 0; i < lines_num; i++) {
        // for (uint j = i + 1; j < lines_num; j++) {
        //     // Remove repeated lines
        //     if (line_indices[kIndicesPerLine * i] == line_indices[kIndicesPerLine * j] 
        //         && line_indices[kIndicesPerLine * i + 1] == line_indices[kIndicesPerLine * j + 1]) {

        //         removeLine(line_indices, &lines_num, j);

        //         std::cout << lines_num << "\n\n";
        //         for (uint k = 0; k < lines_num; k++) {                            
        //             std::cout << line_indices[kIndicesPerLine * k] << ' ' << line_indices[kIndicesPerLine * k + 1] << '\n';
        //         }
        //         std::cout << '\n';

        //         j--;
        //         continue;
        //     }

        //     // Opposite lines neutralize each other
        //     if (line_indices[kIndicesPerLine * i] == line_indices[kIndicesPerLine * j + 1] 
        //         && line_indices[kIndicesPerLine * i + 1] == line_indices[kIndicesPerLine * j]) {
                
        //         // std::cout << line_indices[kIndicesPerLine * i] << ' ' << line_indices[kIndicesPerLine * i + 1] << ", ";
        //         // std::cout << line_indices[kIndicesPerLine * j] << ' ' << line_indices[kIndicesPerLine * j + 1] << '\n';

        //         removeLine(line_indices, &lines_num, j);
        //         removeLine(line_indices, &lines_num, i);

        //         std::cout << lines_num << "\n\n";
        //         for (uint k = 0; k < lines_num; k++) {                            
        //             std::cout << line_indices[kIndicesPerLine * k] << ' ' << line_indices[kIndicesPerLine * k + 1] << '\n';
        //         }
        //         std::cout << '\n';

        //         i--;
        //         break;
        //     }
        // }
    }

    uint line_indices_num = kIndicesPerLine * lines_num;
    uint line_vertices_num = 0;
    for (uint i = 0; i < line_indices_num; i++) {
        if (line_indices[i] >= line_vertices_num) line_vertices_num = line_indices[i] + 1;
    }

    // 2 extrusion vertices per line vertex
    float* extrusion_vertices = new float[kFloatsPerVertex * 2 * line_vertices_num];
    for (uint i = 0; i < line_vertices_num; i++) {
        uint extrusion_vertices_i = kFloatsPerVertex * 2 * i;
        uint vertices_i = kFloatsPerVertex * i;
        extrusion_vertices[extrusion_vertices_i]     = vertices[vertices_i];
        extrusion_vertices[extrusion_vertices_i + 1] = vertices[vertices_i + 1];
        extrusion_vertices[extrusion_vertices_i + 2] = vertices[vertices_i + 2] + height;
        extrusion_vertices[extrusion_vertices_i + 3] = vertices[vertices_i + 0];
        extrusion_vertices[extrusion_vertices_i + 4] = vertices[vertices_i + 1];
        extrusion_vertices[extrusion_vertices_i + 5] = vertices[vertices_i + 2];
    }

    // 2 extrusion triangles per line
    uint* extrusion_indices = new uint[kIndicesPerTriangle * 2 * lines_num];
    for (uint i = 0; i < lines_num; i++) {
        uint extrusion_indices_i = kIndicesPerTriangle * 2 * i;
        uint line_indices_i = kIndicesPerLine * i;
        extrusion_indices[extrusion_indices_i]     = 2 * line_indices[line_indices_i];
        extrusion_indices[extrusion_indices_i + 1] = 2 * line_indices[line_indices_i] + 1;
        extrusion_indices[extrusion_indices_i + 2] = 2 * line_indices[line_indices_i + 1];
        extrusion_indices[extrusion_indices_i + 3] = 2 * line_indices[line_indices_i + 1];
        extrusion_indices[extrusion_indices_i + 4] = 2 * line_indices[line_indices_i] + 1;
        extrusion_indices[extrusion_indices_i + 5] = 2 * line_indices[line_indices_i + 1] + 1;
    }

    // 2 extrusion vertices per line vertex
    float* extrusion_colors = new float[kFloatsPerColor * 2 * line_vertices_num];
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

    *triangles_num = 2 * lines_num;

    return createTrianglesVAO(extrusion_vertices, extrusion_indices, extrusion_colors, *triangles_num);
}

void removeLine(uint* line_indices, uint* lines_num, uint index) {
    line_indices[kIndicesPerLine * index]     = line_indices[kIndicesPerLine * (*lines_num - 1)];
    line_indices[kIndicesPerLine * index + 1] = line_indices[kIndicesPerLine * (*lines_num - 1) + 1];
    *lines_num--;
}
