#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "structs.h"

class Model
{
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    const char* m_fileName{};

public:
    Model(const char* fileName) : m_fileName(fileName) {};
    void LoadModelIndexed();

    inline std::vector<Vertex> GetVertices() { return m_vertices; }
    inline std::vector<uint32_t> GetIndices() { return m_indices; }
};

#endif // !MODEL_H
