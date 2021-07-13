#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include <map>
#include <memory>
#include <ostream>
#include <vector>
#include "TDefs.h"

class BipartiteGraph {
public:
    typedef std::map<IdType, VertexPtr> ContainerType;
    typedef ContainerType::iterator Iterator;

private:
    /// the partitions A and B
    ContainerType A_;
    ContainerType B_;
    std::map<std::pair<IdType, IdType>, int> edgeIndex;
    std::vector<std::pair<int, int>> edges;
    int siz;
    int cost;
    int maxDev;
    float avgDev;
    void addEdges();
    /// Quadruple lp constraints
    void agentConstraint(const char* file_name);
    void programConstraint(const char* file_name);
    void lpBounds(const char* file_name);
    void lpGeneral(const char* file_name);
    void vertexConstraint(const char*);
    void objective(const char*);

    /// Triple lp constraints
    void tripObjective(const char*);
    void tripvertexconstraints(const char*);
    void tripGeneral(const char*);
    
    int calcSize();
    void calcMaxdev();
    void getSize();
    void calcCost();


public:
    BipartiteGraph(const ContainerType& A, const ContainerType& B);
    virtual ~BipartiteGraph();
    void rev(const char* file_name);
    void calculateCosts();          // Calculates hospital costs for all vertices in B
    const ContainerType& get_A_partition() const;
    const ContainerType& get_B_partition() const;
    bool has_augmenting_path(const MatchedPairListType& M) const;
    void lpConstraints(const char*);
    void tripConstraints(const char*);
    void checkValid();
    void verifyStableMatching();
    friend std::ostream& operator<<(std::ostream& out,
                                    const std::unique_ptr<BipartiteGraph>& G);
        /// input parameters
    void inputPara(const char* file_name);
    void outputPara(const char* file_name, std::string);
    void addPara(const char* file_name);
    void lpcapout(const char* file_name, std::string);
    void matchingout(const char* file_name, std::string);
};

#endif
