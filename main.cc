#include "GraphReader.h"
#include "BipartiteGraph.h"
#include "PartnerList.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int c = 0;
    bool A_proposing = true;
    const char* input_file = nullptr;
    const char* output_file = nullptr;
    const char* quad_file = nullptr;
    const char* tri_file = nullptr;

    opterr = 0;
    // choose the proposing partition using -A and -B
    // -s, -p, and -m flags compute the stable, max-card popular and pop among
    // max-card matchings respectively
    // -r and -h compute the resident and hopsital heuristic for an HRLQ instance
    // -c computes the many-to-one popular matching
    // -i is the path to the input graph, -o is the path where the matching
    // computed should be stored
    while ((c = getopt(argc, argv, "i:o:q:t:")) != -1) {
        switch (c) {
        case 'i': input_file = optarg; break;
        case 'o': output_file = optarg; break;
        case 'q': quad_file = optarg; break;
        case 't': tri_file = optarg; break;
        case '?':
            if (optopt == 'i') {
                std::cerr << "Option -i requires an argument.\n";
            }
            else if (optopt == 'o') {
                std::cerr << "Option -o requires an argument.\n";
            }
            else {
                std::cerr << "Unknown option: " << (char)optopt << '\n';
            }
            break;
        default: break;
        }
    }

    if (not input_file or not output_file) {
        // do not proceed if file names are not valid
    }
    GraphReader reader1(input_file);
    GraphReader reader2(input_file);
    std::unique_ptr<BipartiteGraph> GQuad = reader1.read_graph();
    std::unique_ptr<BipartiteGraph> GTri = reader2.read_graph();

    try {
        GQuad->checkValid();
    }
    catch (std::string s) {
        std::cout << s;
        return 0;
    }
    GQuad->calculateCosts();
    GTri->calculateCosts();
    GQuad->lpConstraints(quad_file);
    GTri->tripConstraints(tri_file);
    std::cout << "Done";
    return 0;
}
