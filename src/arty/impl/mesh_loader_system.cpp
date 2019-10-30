#include <arty/impl/mesh_loader_system.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace arty {

Result Loader::load(const std::string &path, Mesh *out) {
  if (!out) {
    return error("given empty pointer");
  }
  if (path.size() == 0) {
    return error("given empty path");
  }
  Mesh tmp;
  std::cout << "loading mesh: " << path << std::endl;
  std::ifstream file;
  file.open(path);
  if (!file.is_open()) {
    return error("error opening file: " + path);
  }

  std::string line;
  while (std::getline(file, line)) {
    std::cout << line << std::endl;
    if (line.empty()) {
      continue;
    }
    // remove comments
    if (line[0] == '#') {
      continue;
    }
    std::istringstream ss(line);
    std::string header;
    ss >> header;
    if (header.compare("v") == 0) {
      float x, y, z;
      ss >> x >> y >> z;
      tmp.vertices.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("vt") == 0) {
      float u, v;
      ss >> u >> v;
      tmp.uvs.emplace_back(Vec2f{u, v});
    } else if (header.compare("vn") == 0) {
      float x, y, z;
      ss >> x >> y >> z;
      tmp.normals.emplace_back(Vec3f{x, y, z});
    } else if (header.compare("f") == 0) {
      std::size_t count = 0;
      std::string element;
      while (std::getline(ss, element, ' ')) {
        if (element.size() == 0) {
          continue;
        }
        std::istringstream index(element);
        std::string sub;
        while (std::getline(index, sub, '/')) {
          count++;
          uint16_t ind;
          std::istringstream convert(sub);
          convert >> ind;
          tmp.indices.push_back(ind);
        }
      }
      if (count / 3 == 4) {
        tmp.type = FaceType::QUAD;
        //  We need to produce triangles, cause I can't render them
      } else {
        tmp.type = FaceType::TRIANGLE;
      }
    } else {
      std::cout << "unhandled: " << header << std::endl;
    }
  }
  std::cout << "loaded! vertex: " << tmp.vertices.size()
            << " normals: " << tmp.normals.size() << " uvs: " << tmp.uvs.size()
            << " indices: " << tmp.indices.size() << std::endl;
  std::cout << "start optimizing mesh" << std::endl;

  std::unordered_map<uint16_t, uint16_t> mapping;
  for (std::size_t i = 0; i < tmp.indices.size() - 2; i += 3) {
    uint16_t v, vt, vn;
    v = tmp.indices[i];
    vt = tmp.indices[i + 1];
    vn = tmp.indices[i + 2];

    if (mapping.count(v) == 0) {
      uint16_t id = out->vertices.size();
      out->vertices.push_back(tmp.vertices[v]);
      out->uvs.push_back(tmp.uvs[vt]);
      out->normals.push_back(tmp.normals[vn]);
      out->indices.push_back(id);
      mapping[v] = id;
    } else {
      out->indices.push_back(mapping[v]);
    }
  }
  out->type = TRIANGLE;
  if (tmp.type == QUAD) {
    std::cout << "converting quads to triangles" << std::endl;
    // We need to reorganized again to transform quads into triangles
    auto cpy = out->indices;
    out->indices.clear();
    for (std::size_t i = 0; i < cpy.size() - 3; i += 4) {
      out->indices.push_back(cpy[i]);
      out->indices.push_back(cpy[i + 1]);
      out->indices.push_back(cpy[i + 2]);
      out->indices.push_back(cpy[i]);
      out->indices.push_back(cpy[i + 2]);
      out->indices.push_back(cpy[i + 3]);
    }
  }
  std::cout << "mesh is ready! vertex: " << out->vertices.size()
            << " normals: " << out->normals.size()
            << " uvs: " << out->uvs.size()
            << " indices: " << out->indices.size() << std::endl;
  return ok();
}

}  // namespace arty
