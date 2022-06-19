#pragma once

#include "Vertex.h"
#include <vector>
#include "tiny_obj_loader.h"
#include <iostream>

static std::vector<Vertex> loadObj(const char* fileName,std::vector<int>& indices) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files
    std::vector<Vertex> objVertex;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(fileName, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        Vertex v{};
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                float tx= 0, ty = 0;
                // access to vertex
                
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                float vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                float vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
  

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }

                // Optional: vertex colors
               /* tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];*/
                
            }
            index_offset += fv;
           
            // per-face material
            shapes[s].mesh.material_ids[f];
        }
        std::cout << attrib.vertices.size() << std::endl;
        for (long i = 0; i < attrib.vertices.size() ; i+=3) {
            objVertex.push_back({ { attrib.vertices[i],attrib.vertices[i+1],attrib.vertices[i+2] }, { 1.0,1.0 } , {255,255,255,1} });
        }
        std::cout << objVertex.size() << std::endl;
        for (long i = 0; i < shapes[s].mesh.indices.size(); i++)
        {
            indices.push_back(shapes[s].mesh.indices[i].vertex_index);
        }

        return objVertex;
    }

}