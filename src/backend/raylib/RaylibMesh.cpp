#include "backend/raylib/RaylibMesh.hpp"

shimera::RaylibMesh::RaylibMesh(Model& model) : m_model(model) {}

Model& shimera::RaylibMesh::model() {
    return m_model;
}