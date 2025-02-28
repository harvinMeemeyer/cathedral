# Cathedral

Cathedral is an open-source alternative to Cadence, designed for digital and analog circuit design, simulation, and verification. This project aims to provide engineers, researchers, and hobbyists with an accessible, flexible, and feature-rich EDA (Electronic Design Automation) tool.

## Features
- **Schematic Capture**: Intuitive GUI for designing circuits
- **Simulation Support**: Integration with SPICE-based simulators and custom solvers
- **Layout Editor**: Supports multi-layer design and verification
- **HDL Integration**: Verilog and VHDL support for RTL design and verification
- **Open-Source Libraries**: Community-driven component models and design tools
- **Cross-Platform**: Runs on Linux, Windows, and macOS

## Technology Stack
Cathedral is primarily written in **C++** for high-performance computation and simulation, with additional scripting and UI elements in **Python** and **Qt/GTK**.

## Installation
### Prerequisites
Ensure you have the following dependencies installed:
- C++ compiler (GCC/Clang/MSVC)
- Python 3.x (for scripting and automation)
- GTK/Qt (for GUI)
- SPICE (e.g., ngspice, Xyce)
- Verilog/VHDL compilers (Icarus Verilog, GHDL)

### Build Instructions
```sh
git clone https://github.com/your-repo/cathedral.git
cd cathedral
mkdir build && cd build
cmake ..
make
./install.sh  # Or follow manual setup instructions
```

## Usage
### Running Cathedral
```sh
./cathedral
```

### Loading a Project
```sh
./cathedral my_project
```

## Contribution
We welcome contributions! If you are interested in improving Cathedral, follow these steps:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature-branch`)
3. Commit changes (`git commit -m 'Add new feature'`)
4. Push to your fork and create a pull request

## License
Cathedral is released under the MIT License.

## Contact
For discussions, issues, and feature requests, please open an issue on GitHub or reach out to the developer community.


