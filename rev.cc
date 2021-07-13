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
    const char* rev_file = nullptr;
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
    while ((c = getopt(argc, argv, "i:o:r:q:t:")) != -1) {
        switch (c) {
        case 'i': input_file = optarg; break;
        case 'o': output_file = optarg; break;
        case 'r': rev_file = optarg; break;
        case 'q': quad_file = optarg; break;
        case 't': tri_file = optarg; break;
        case '?':
            if (optopt == 'i') {
                std::cerr << "Option -i requires an argument.\n";
            }
            else if (optopt == 'o') {
                std::cerr << "Option -o requires an argument.\n";
            }
            else if (optopt == 'q') {
                std::cerr << "Option -q requires an argument.\n";
            }
            else if (optopt == 't') {
                std::cerr << "Option -t requires an argument.\n";
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
    std::unique_ptr<BipartiteGraph> G1 = reader1.read_graph();
    GraphReader reader2(input_file);
    std::unique_ptr<BipartiteGraph> G2 = reader2.read_graph();
    G1->calculateCosts();
    G2->calculateCosts();
    G1->rev(quad_file);
    G2->rev(tri_file);
    std::ofstream out;
    out.open(output_file, std::ios::out);
    out << "Input Parameters\n";
    out.close();
    G1->inputPara(output_file);
    out.open(output_file, std::ios::out | std::ios::app);
    out << "\nOutput\n";
    out << "LP_name, Size, Cost, Rank1, Rank2, Max-Dev, Avg-Dev\n";
    out.close();
    G1->outputPara(output_file, "Primal_Triple");
    G2->outputPara(output_file, "Primal_Quadruple");
    out.open(output_file, std::ios::out | std::ios::app);
    out << "\nAdditional output\n";
    out.close();
    G2->addPara(output_file);
    G1->lpcapout(output_file, "Primal_capacity_Triple");
    G2->lpcapout(output_file, "Primal_capacity_Quadruple");
    G1->matchingout(output_file, "Primal_capacity_Triple");
    G2->matchingout(output_file, "Primal_capacity_Quadruple");
    return 0;
}
