#ifndef ECS_H
#define ECS_H

#include <arty/core/result.h>
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace arty {

class Entity {
 public:
  using id_t = uint64_t;
  using Ptr = std::shared_ptr<Entity>;

  Entity(id_t id) : _id(id) {}

  static Entity create() {
    Entity ent(_next);
    _next++;
    return ent;
  }

  bool operator==(Entity const &e) const { return _id == e._id; }

  bool operator!=(Entity const &e) const { return !(*this == e); }

  bool operator<(Entity const &e) const { return _id < e._id; }

 private:
  id_t _id;
  static id_t _next;
};

class Component {};

class ComponentStorage {
 public:
  using type_t = std::string;
  using Ptr = std::shared_ptr<ComponentStorage>;

  type_t const &type() { return _type; }

  template <typename C>
  void set(Entity const &e, C const &val) {}

  template <typename C>
  Result get(Entity const &e, C *val) {
    return Result("nope");
  }

 private:
  type_t _type;
};

class Blackboard {
 public:
  using Ptr = std::shared_ptr<Blackboard>;

  void add(ComponentStorage::Ptr const &ptr);
  ComponentStorage::Ptr get(ComponentStorage::type_t const &type);

  bool exist(Entity const &id) const {
    return _entities.find(id) != _entities.end();
  }

  void create(Entity const &id) { _entities.insert(id); }

 private:
  std::set<Entity> _entities;
  std::unordered_map<ComponentStorage::type_t, ComponentStorage::Ptr>
      _components;
};

class System {
 public:
  using Ptr = std::shared_ptr<System>;
  virtual ~System() = 0;
  virtual Result process(Blackboard::Ptr const &board) = 0;

 private:
};

class Engine {
 public:
  Engine() : _systems(), _blackboard(new Blackboard) {}

  Result step() {
    Result r;
    for (auto const &system : _systems) {
      r = system->process(_blackboard);
      if (!r) {
        return r;
      }
    }
    return r;
  }

  template <typename T>
  Result attach_component(Entity::id_t const &id,
                          ComponentStorage::type_t const &type, T const &value,
                          bool create) {
    if (!_blackboard->exist(id)) {
      if (create) {
        _blackboard->create(id);
      } else {
        return Result("Entity doesn't exist");
      }
    }

    auto storage = _blackboard->get(type);
    if (storage) {
      storage->set(id, value);
    }
    return Result();
  }

  template <typename T>
  Result get(Entity const &id, ComponentStorage::type_t const &type,
             T *output) {
    if (!output) {
      return Result("null pointer givent");
    }
    auto storage = _blackboard->get(type);
    if (!storage) {
      return Result("unknown component type");
    }
    return storage->get(id, output);
  }

 private:
  std::vector<System::Ptr> _systems;
  Blackboard::Ptr _blackboard;
};

class StringComponent {
 public:
  StringComponent() = default;
  StringComponent(std::string const &val) : _val(val) {}

 private:
  std::string _val;
};

template <typename C>
class DenseComponentStorage : public ComponentStorage {
 public:
  DenseComponentStorage(ComponentStorage::type_t const &type) : _type(type) {}

  void set(Entity const &e, C const &val) { _data[e] = val; }

 private:
  ComponentStorage::type_t _type;
  std::vector<C> _data;
};

};  // namespace arty

#endif  // ECS_H
