#include <bits/stdc++.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

// Constants
const double INF = 1e18;  // A large value representing infinity
const double EARTH_RADIUS_KM = 6371.0;  // Earth's radius in kilometers

// Type aliases for convenience
using ll = long long;
using ld = long double;

// Structures to represent Nodes and Ways
struct Node {
    string name;
    ld latitude;
    ld longitude;
};

struct Way {
    string name;
    vector<ll> node_ids;
};

// Function to convert degrees to radians
ld toRadians(const ld degree) {
    return degree * M_PI / 180.0;
}

// Function to calculate the haversine distance between two geographic coordinates
ld computeDistance(ld lat1, ld lon1, ld lat2, ld lon2) {
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);

    ld dlat = lat2 - lat1;
    ld dlon = lon2 - lon1;

    ld a = pow(sin(dlat / 2), 2) +
           cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);

    ld c = 2 * asin(sqrt(a));
    return EARTH_RADIUS_KM * c;
}

// Function to find the shortest distance between two nodes using Dijkstra's algorithm
void findShortestPath(
    unordered_map<ll, vector<pair<ll, ld>>>& adjacency_list,
    unordered_map<ll, Node>& nodes,
    ll source_id,
    ll destination_id
) {
    // Initialize distances to all nodes as infinity
    unordered_map<ll, ld> distances;
    for (const auto& node_pair : nodes) {
        distances[node_pair.first] = INF;
    }
    distances[source_id] = 0.0;

    // Min-priority queue to select the node with the smallest distance
    set<pair<ld, ll>> active_nodes;
    active_nodes.insert({0.0, source_id});

    while (!active_nodes.empty()) {
        auto [current_distance, current_id] = *active_nodes.begin();
        active_nodes.erase(active_nodes.begin());

        // If we have reached the destination, we can stop
        if (current_id == destination_id) {
            break;
        }

        // Update distances to adjacent nodes
        for (const auto& [neighbor_id, edge_distance] : adjacency_list[current_id]) {
            ld new_distance = current_distance + edge_distance;
            if (new_distance < distances[neighbor_id]) {
                active_nodes.erase({distances[neighbor_id], neighbor_id});
                distances[neighbor_id] = new_distance;
                active_nodes.insert({new_distance, neighbor_id});
            }
        }
    }

    // Output the result
    if (distances[destination_id] == INF) {
        cout << "\nNo path found between the given nodes.\n";
    } else {
        cout << "\nShortest distance between nodes " << source_id
             << " and " << destination_id << ": "
             << distances[destination_id] << " km\n";
    }
}

int main() {
    /*
     * References:
     * - Parsing XML: https://rapidxml.sourceforge.net/
     * - Haversine formula: https://www.geeksforgeeks.org/program-distance-two-points-earth/
     */

    // Data structures to store nodes and ways
    unordered_map<ll, Node> nodes_map;
    unordered_map<ll, Way> ways_map;

    // Set output formatting for floating-point numbers
    cout << fixed << setprecision(8);

    // Read and parse the map.osm file
    ifstream file("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');  // Null-terminate the buffer

    // Parse the XML content
    xml_document<> doc;
    doc.parse<0>(&buffer[0]);
    xml_node<>* osm = doc.first_node("osm");

    // Parse nodes from the XML
    for (xml_node<>* node_element = osm->first_node("node"); node_element; node_element = node_element->next_sibling()) {
        if (node_element->first_attribute("id") &&
            node_element->first_attribute("lat") &&
            node_element->first_attribute("lon")) {

            // Extract node attributes
            ll id = stoll(node_element->first_attribute("id")->value());
            ld lat = stold(node_element->first_attribute("lat")->value());
            ld lon = stold(node_element->first_attribute("lon")->value());
            string name;

            // Search for the name in the node's tags
            for (xml_node<>* tag = node_element->first_node("tag"); tag; tag = tag->next_sibling()) {
                if (strcmp(tag->first_attribute("k")->value(), "name") == 0) {
                    name = tag->first_attribute("v")->value();
                    break;
                }
            }

            // Create a Node object and add it to the map
            nodes_map[id] = Node{name, lat, lon};
        }
    }

    // Parse ways from the XML
    for (xml_node<>* way_element = osm->first_node("way"); way_element; way_element = way_element->next_sibling()) {
        if (way_element->first_attribute("id") && way_element->first_node("nd")) {

            // Extract way attributes
            ll id = stoll(way_element->first_attribute("id")->value());
            string name;
            vector<ll> node_ids;

            // Extract node references in the way
            for (xml_node<>* nd = way_element->first_node("nd"); nd; nd = nd->next_sibling()) {
                if (nd->first_attribute("ref")) {
                    ll node_id = stoll(nd->first_attribute("ref")->value());
                    node_ids.push_back(node_id);
                }
            }

            // Search for the name in the way's tags
            for (xml_node<>* tag = way_element->first_node("tag"); tag; tag = tag->next_sibling()) {
                if (strcmp(tag->first_attribute("k")->value(), "name") == 0) {
                    name = tag->first_attribute("v")->value();
                    break;
                }
            }

            // Create a Way object and add it to the map
            ways_map[id] = Way{name, node_ids};
        }
    }

    // ---------------------- Use Case 1 ----------------------
    cout << "\n------------------------------\n";
    cout << "USE CASE 1: Search Nodes and Ways\n";
    cout << "------------------------------\n";

    // Output the total number of nodes and ways
    cout << "\nTotal number of nodes: " << nodes_map.size() << endl;
    cout << "Total number of ways: " << ways_map.size() << endl;

    // Prompt the user for a search string
    cout << "\nEnter a name to search for in nodes and ways: ";
    string search_query;
    cin >> search_query;

    // Containers to store matching nodes and ways
    unordered_map<ll, Node> matching_nodes;
    unordered_map<ll, Way> matching_ways;

    // Search for matching nodes
    for (const auto& [id, node] : nodes_map) {
        if (!node.name.empty() && node.name.find(search_query) != string::npos) {
            matching_nodes[id] = node;
        }
    }

    // Search for matching ways
    for (const auto& [id, way] : ways_map) {
        if (!way.name.empty() && way.name.find(search_query) != string::npos) {
            matching_ways[id] = way;
        }
    }

    // Output matching nodes
    cout << "\nNodes matching '" << search_query << "': " << matching_nodes.size() << endl;
    if (matching_nodes.empty()) {
        cout << "No matching nodes found.\n";
    } else {
        cout << "ID\tLatitude\tLongitude\tName\n";
        for (const auto& [id, node] : matching_nodes) {
            cout << id << "\t" << node.latitude << "\t" << node.longitude << "\t" << node.name << endl;
        }
    }

    // Output matching ways
    cout << "\nWays matching '" << search_query << "': " << matching_ways.size() << endl;
    if (matching_ways.empty()) {
        cout << "No matching ways found.\n";
    } else {
        cout << "ID\tName\n";
        for (const auto& [id, way] : matching_ways) {
            cout << id << "\t" << way.name << endl;
        }
    }

    // ---------------------- Use Case 2 ----------------------
    cout << "\n------------------------------\n";
    cout << "USE CASE 2: Find K-Closest Nodes\n";
    cout << "------------------------------\n";

    // Prompt the user for a node ID and k value
    ll node_id;
    ll k;
    cout << "\nEnter the node ID to find its closest nodes: ";
    cin >> node_id;
    cout << "Enter the number of closest nodes to find (k): ";
    cin >> k;

    // Check if the node exists
    auto node_it = nodes_map.find(node_id);
    if (node_it == nodes_map.end()) {
        cout << "\nNode ID " << node_id << " does not exist.\n";
    } else {
        // Compute distances to all other nodes
        vector<pair<ld, ll>> distances;
        const Node& reference_node = node_it->second;
        for (const auto& [id, node] : nodes_map) {
            if (id != node_id) {
                ld distance = computeDistance(
                    reference_node.latitude, reference_node.longitude,
                    node.latitude, node.longitude
                );
                distances.emplace_back(distance, id);
            }
        }

        // Sort the distances
        sort(distances.begin(), distances.end());

        // Output the k closest nodes
        cout << "\nThe " << k << " closest nodes to node " << node_id << " are:\n";
        cout << "ID\tDistance (km)\tLatitude\tLongitude\n";
        for (size_t i = 0; i < min(k, static_cast<ll>(distances.size())); ++i) {
            ll id = distances[i].second;
            const Node& node = nodes_map[id];
            cout << id << "\t" << distances[i].first << "\t" << node.latitude << "\t" << node.longitude << endl;
        }
    }

    // ---------------------- Use Case 3 ----------------------
    cout << "\n------------------------------\n";
    cout << "USE CASE 3: Shortest Path Between Nodes\n";
    cout << "------------------------------\n";

    // Prompt the user for two node IDs
    ll source_id, destination_id;
    cout << "\nEnter the source node ID: ";
    cin >> source_id;
    cout << "Enter the destination node ID: ";
    cin >> destination_id;

    // Check if both nodes exist
    if (nodes_map.find(source_id) == nodes_map.end()) {
        cout << "\nSource node ID " << source_id << " does not exist.\n";
    } else if (nodes_map.find(destination_id) == nodes_map.end()) {
        cout << "\nDestination node ID " << destination_id << " does not exist.\n";
    } else {
        // Build the adjacency list from ways
        unordered_map<ll, vector<pair<ll, ld>>> adjacency_list;
        for (const auto& [way_id, way] : ways_map) {
            const vector<ll>& node_ids = way.node_ids;
            for (size_t i = 0; i + 1 < node_ids.size(); ++i) {
                ll u = node_ids[i];
                ll v = node_ids[i + 1];
                if (nodes_map.count(u) && nodes_map.count(v)) {
                    const Node& node_u = nodes_map[u];
                    const Node& node_v = nodes_map[v];
                    ld distance = computeDistance(node_u.latitude, node_u.longitude, node_v.latitude, node_v.longitude);
                    adjacency_list[u].emplace_back(v, distance);
                    adjacency_list[v].emplace_back(u, distance);
                }
            }
        }

        // Find and output the shortest path
        findShortestPath(adjacency_list, nodes_map, source_id, destination_id);
    }

    cout << "\n------------------------------\n";
    cout << "\tThank you!\n";
    cout << "------------------------------\n";

    return 0;
}