# OpenStreetMap Parser and Analyzer in C++

This project is a C++ application that parses OpenStreetMap (OSM) data to extract nodes and ways, providing functionalities to search, analyze, and compute distances and paths between map elements. The application demonstrates the use of XML parsing, graph algorithms, and geospatial computations.

---

## Table of Contents

- [Motivation](#motivation)
- [Features](#features)
- [Requirements](#requirements)
- [Project Structure](#project-structure)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)
  - [Use Case 1: Search Nodes and Ways](#use-case-1-search-nodes-and-ways)
  - [Use Case 2: Find K-Closest Nodes](#use-case-2-find-k-closest-nodes)
  - [Use Case 3: Shortest Path Between Nodes](#use-case-3-shortest-path-between-nodes)
- [References](#references)
- [License](#license)

---

## Motivation

OpenStreetMap (OSM) is a collaborative project to create a free editable map of the world. OSM data is rich and complex, and working with it requires parsing XML data and understanding geospatial concepts. This project aims to:

- Provide a practical example of parsing and analyzing OSM data using C++.
- Demonstrate the implementation of algorithms for geospatial computations.
- Offer a tool that can be extended for more advanced mapping and routing applications.

---

## Features

- **OSM Data Parsing**: Extracts nodes and ways from an OSM XML file.
- **Search Functionality**: Allows searching for nodes and ways by name.
- **Distance Calculations**: Computes the "crow fly" (great-circle) distance between nodes.
- **K-Closest Nodes**: Finds the k-nearest nodes to a given node.
- **Shortest Path Computation**: Calculates the shortest path between two nodes using Dijkstra's algorithm.

---

## Requirements

- **C++ Compiler**: Compatible with C++11 or higher (e.g., GCC, Clang).
- **RapidXML Library**: A fast XML parser used for parsing OSM data.
- **OSM Data File**: An OSM XML file (`map.osm`) containing map data to be analyzed.

---

## Project Structure

```plaintext
├── osm_parser.cpp   # Main C++ source code
├── map.osm          # OpenStreetMap data file (XML format)
└── rapidxml.hpp     # RapidXML library header
```

---

## Installation and Setup

1. Clone the Repository:

   ```bash
   git clone https://github.com/your-username/osm-parser.git
   cd osm-parser
   ```

2. Obtain the Required Files:

   - **map.osm**: Download an OSM data file for your area of interest from [OpenStreetMap](https://www.openstreetmap.org/).
   - **rapidxml.hpp**: Download the RapidXML header file from [RapidXML Documentation](http://rapidxml.sourceforge.net/). Place it in the project directory.

3. Compile the Program:

   Use the following command to compile the program:

   ```bash
   g++ -std=c++11 osm_parser.cpp -o osm_parser
   ```

   If `rapidxml.hpp` is in a different directory, include the path:

   ```bash
   g++ -std=c++11 -I/path/to/rapidxml osm_parser.cpp -o osm_parser
   ```

---

## Usage

Run the compiled program:

```bash
./osm_parser
```

Follow the on-screen prompts to interact with the application.

### Use Case 1: Search Nodes and Ways

**Purpose**: Search for map elements by name.

**Instructions**:
1. When prompted, enter a search string.
2. The program will display all nodes and ways whose names contain the search string.

**Example**:

```plaintext
Enter a name to search for in nodes and ways: Library
```

---

### Use Case 2: Find K-Closest Nodes

**Purpose**: Find the k-nearest nodes to a specified node using the great-circle distance.

**Instructions**:
1. Enter the node ID for which you want to find the closest nodes.
2. Enter the value of `k` (number of closest nodes to find).
3. The program will display the `k` closest nodes with their distances.

**Example**:

```plaintext
Enter the node ID to find its closest nodes: 123456789
Enter the number of closest nodes to find (k): 5
```

---

### Use Case 3: Shortest Path Between Nodes

**Purpose**: Compute the shortest path between two nodes through connected ways.

**Instructions**:
1. Enter the source node ID.
2. Enter the destination node ID.
3. The program will calculate and display the shortest path distance.

**Example**:

```plaintext
Enter the source node ID: 123456789
Enter the destination node ID: 987654321
```

---

## References

- [RapidXML Documentation](http://rapidxml.sourceforge.net/)
- [Haversine Formula: GeeksforGeeks Article](https://www.geeksforgeeks.org/program-distance-two-points-earth/)
- [OpenStreetMap](https://www.openstreetmap.org/)

---

## License

This project is licensed under the MIT License - see the LICENSE file for details.
