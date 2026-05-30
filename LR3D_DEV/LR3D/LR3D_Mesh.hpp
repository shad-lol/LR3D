#pragma once
#ifndef LR3D_MESH_HPP
#define LR3D_MESH_HPP

namespace LR3D {
    struct Mesh2D {
        std::vector<LR3D::Vec2f> vertices;
        std::vector<int> indices;
    };

    struct Mesh3D {
        std::vector<Vec3f> vertices;
        std::vector<int> indices;
    };
}
#endif
