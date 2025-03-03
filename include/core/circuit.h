#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <string>
#include <unordered_map>

namespace Cathedral {

    // Basic circuit element structure
    struct CircuitComponent {
        std::string id;
        std::string type;  // e.g., "Resistor", "Capacitor"
        double value;  // e.g., Resistance in Ohms, Capacitance in Farads
        int node1, node2;  // Nodes it connects to
    };

    class Circuit {
    public:
        Circuit();
        ~Circuit();

        void addComponent(const std::string& type, double value, int node1, int node2);
        void removeComponent(const std::string& id);
        void listComponents() const;
        
    private:
        std::unordered_map<std::string, CircuitComponent> components;
        int componentCounter;
    };

} // namespace Cathedral

#endif // CIRCUIT_H
