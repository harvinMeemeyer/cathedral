#include "core/circuit.h"
#include <iostream>

namespace Cathedral {

    Circuit::Circuit() : componentCounter(0) {}

    Circuit::~Circuit() {}

    void Circuit::addComponent(const std::string& type, double value, int node1, int node2) {
        std::string id = type + std::to_string(componentCounter++);
        components[id] = {id, type, value, node1, node2};
        std::cout << "Added component: " << id << " (" << type << ")" << std::endl;
    }

    void Circuit::removeComponent(const std::string& id) {
        if (components.erase(id)) {
            std::cout << "Removed component: " << id << std::endl;
        } else {
            std::cout << "Component not found: " << id << std::endl;
        }
    }

    void Circuit::listComponents() const {
        std::cout << "Circuit Components:" << std::endl;
        for (const auto& [id, component] : components) {
            std::cout << "- " << id << ": " << component.type 
                      << " (" << component.value << ")" 
                      << " connected to nodes " << component.node1 
                      << " and " << component.node2 << std::endl;
        }
    }

} // namespace Cathedral
