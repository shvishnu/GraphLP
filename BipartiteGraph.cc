#include "BipartiteGraph.h"
#include "Vertex.h"
#include "PartnerList.h"
#include <set>
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cstring>

BipartiteGraph::BipartiteGraph(const ContainerType& A, const ContainerType& B)
    : A_(A), B_(B)
{
    addEdges();
}

BipartiteGraph::~BipartiteGraph()
{}

const BipartiteGraph::ContainerType& BipartiteGraph::get_A_partition() const {
    return A_;
}

const BipartiteGraph::ContainerType& BipartiteGraph::get_B_partition() const {
    return B_;
}

bool BipartiteGraph::has_augmenting_path(const MatchedPairListType& M) const {
    // the second entry of the pair is true if
    // the vertex belongs to partition A, otherwise false
    std::queue<std::pair<VertexPtr, bool>> Q;
    std::set<VertexPtr> visited;

    // populate the queue with vertices from A
    // which can participate in an augmenting path
    // these are precisely the vertices that are unmatched
    for (auto it : get_A_partition()) {
        auto u = it.second;
        auto uit = M.find(u);

        if (uit == M.cend() or uit->second.size() == 0) {
            // u is a vertex from partition A
            Q.emplace(std::make_pair(u, true));

            // mark u as visited
            visited.emplace(u);
        }
    }

    while (not Q.empty()) {
        // remove a vertex from the queue
        auto it = Q.front();
        Q.pop();

        // get the preference list and the matched partners
        auto u = it.first;
        auto uit = M.find(u);
        auto& pl = u->get_preference_list();
        auto& partners = uit == M.end() ? PartnerList() : uit->second;

        if (it.second) {
            // u is a vertex belonging to partition A
            // go through the neigbours of this vertex
            for (auto i = pl.all_begin(), e = pl.all_end(); i != e; ++i) {
                auto v = pl.get_vertex(*i);

                // only add if this vertex has not been visited
                // and is not matched to u
                if (visited.find(v) == visited.end() and
                    partners.find(v) == partners.cend())
                {
                    // v is a vertex from partition B
                    Q.emplace(std::make_pair(v, false));

                    // we have seen this vertex
                    visited.emplace(v);
                }
            }
        } else {
            // u is a vertex from partition B
            // if u is matched to less than upper quota vertices
            // or u is unmatched |M(u)| = 0
            // this is an augmenting path
            if (partners.size() < u->get_upper_quota()) {
                return true;
            } else {
                // add all the matched neighbours
                for (auto i = partners.cbegin(), e = partners.cend(); i != e; ++i) {
                    auto v = partners.get_vertex(i);

                    // only add if this vertex has not been visited
                    if (visited.find(v) == visited.end()) {
                        // v is a vertex from partition A
                        Q.emplace(std::make_pair(v, true));

                        // we have seen this vertex
                        visited.emplace(v);
                    }
                }
            }
        }
    }

    return false;
}

void BipartiteGraph::checkValid() {
    for(auto ver: A_) {
        IdType a = ver.first;
        auto pl = ver.second->get_preference_list();
        for (auto i = pl.all_begin(), e = pl.all_end(); i != e; ++i) {
                auto b = pl.get_vertex(*i);
                bool  flag = 0;
                auto plb = b->get_preference_list();
                for (auto j = plb.all_begin(), e = plb.all_end(); j != e; ++j) {
                    if(plb.get_vertex(*j)->get_id() == a) {
                        flag=1;
                        break;
                    }
                }
                if(!flag) {
                    std::cerr << "Invalid edges" << std::endl;
                }
        }
    }
}

void BipartiteGraph::lpConstraints(const char* file_name) {
    objective(file_name);
    vertexConstraint(file_name);
    agentConstraint(file_name);
    programConstraint(file_name);
    lpBounds(file_name);
    lpGeneral(file_name);
}

void BipartiteGraph::objective(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out);
    out << "minimize\n";
    std::string s;
    int i = 0;
    for(auto ver: B_) {
        IdType b = ver.first;
        int pl = ver.second->get_cost();
        s+= std::to_string(pl);
        s+="y";
        s+= std::to_string(++i);
        s+=" + ";
    }
    s = s.substr(0, s.size()-2);
    out << s << std::endl;
}

void BipartiteGraph::vertexConstraint(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "st\n";

    for(auto edg: edgeIndex) {
        std::string s;
        int i = edg.second;
        IdType a = edg.first.first;
        IdType b = edg.first.second;
        //cout << a << b << endl;
        auto& apl = A_[a]->get_preference_list();
        auto& bpl = B_[b]->get_preference_list();

        for (auto i = apl.begin(), e = apl.end(); i != e; ++i) {
            if(apl.is_ranked_better(apl.get_vertex(i), B_[b])) {
                IdType p = apl.get_vertex(i)->get_id();
                //std::cout << a << p << b << std::endl;
                auto& ppl = B_[p]->get_preference_list();
                for (auto j = ppl.begin(), e = ppl.end(); j != e; ++j) {
                    if(ppl.is_ranked_better(A_[a], ppl.get_vertex(j))) {
                        s += "x";
                        //s += std::to_string(i);
                        s += std::to_string(A_[a]->get_index());
                        s += "_";
                        s += std::to_string(B_[b]->get_index());
                        s += " + x";
                        //s += std::to_string(edgeIndex[{ppl.get_vertex(j)->get_id(), p}]);
                        s += std::to_string(ppl.get_vertex(j)->get_index());
                        s += "_";
                        s += std::to_string(apl.get_vertex(i)->get_index());
                        //std::cout <<ppl.get_vertex(j)->get_id()  << p << b << std::endl;
                        s += " <= 1";
                        out << s << std::endl;
                        s.clear();
                    }
                }
            }
        }
        // Add s to output file
    }
    out.close();
}

void BipartiteGraph::agentConstraint(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    for(auto ver: A_) {
        std::string s;
        IdType a = ver.first;
        auto& pl = ver.second->get_preference_list();
        for (auto i = pl.all_begin(), e = pl.all_end(); i != e; ++i) {
                auto v = pl.get_vertex(*i);
                IdType b = v->get_id();
                s += "x";
                //s += std::to_string(edgeIndex[{a, b}]);
                s += std::to_string(ver.second->get_index());
                s += "_";
                s += std::to_string(v->get_index());
                s += " + ";
        }
        s = s.substr(0, s.size()-2);
        s+="= 1\n";
        out << s ;
    }
}

void BipartiteGraph::programConstraint(const char* file_name) {
    std::ofstream out;
    int i = 0;
    out.open(file_name, std::ios::out | std::ios::app);
    for(auto ver: B_) {
        std::string s;
        IdType b = ver.first;
        auto& pl = ver.second->get_preference_list();
        for (auto i = pl.all_begin(), e = pl.all_end(); i != e; ++i) {
                auto v = pl.get_vertex(*i);
                IdType a = v->get_id();
                s += "x";
               // s += std::to_string(edgeIndex[{a, b}]);
                s += std::to_string(A_[a]->get_index());
                s += "_";
                s += std::to_string(B_[b]->get_index());
                s += " + ";
        }
        s = s.substr(0, s.size()-2);
        s+="- y";
        s+= std::to_string(++i);
        s+= " <= 0";
        out << s << std::endl;
    }
}

void BipartiteGraph::lpBounds(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "bounds\n";
    for(auto i: edgeIndex) {
        //std::cout << "edges Wroking";
        std::string s, a, b;
        a = i.first.first;
        b = i.first.second;
        s += "x";
        //s += std::to_string(i+1);
        s += std::to_string(A_[a]->get_index());
        s += "_";
        s += std::to_string(B_[b]->get_index());
        s += " <= 1";
        out << s << std::endl;
    }
    out.close();
}

void BipartiteGraph::lpGeneral(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "general\n";
    std::string s, a, b;
    for(auto i: edgeIndex) {
        //std::cout << "edges Working";
        a = i.first.first;
        b = i.first.second;
        s += "x";
        //s += std::to_string(i+1);
        s += std::to_string(A_[a]->get_index());
        s += "_";
        s += std::to_string(B_[b]->get_index());
        s += " ";
    }
    int i = 0;
    for(auto y: B_) {
        s += "y";
        s += std::to_string(++i);
        s += " ";
    }
    out << s << std::endl;
    out << "end\n";
    out.close();
}

void BipartiteGraph::tripObjective(const char* file_name) {
    std::cout << "Called"<<std::endl;
    std::ofstream out;
    out.open(file_name, std::ios::out);
    out << "minimize\n";
    std::string s;
    int i = 0;
    for(auto e: edgeIndex) {
        auto u = e.first.first;
        auto v = e.first.second;
        s+=std::to_string(B_[v]->get_cost());
        s+="x";
        s+=std::to_string(A_[u]->get_index());
        s+="_";
        s+=std::to_string(B_[v]->get_index());
        s+=" + ";
    }
    s = s.substr(0, s.size()-2);
    out << s << std::endl;
}

void BipartiteGraph::tripvertexconstraints(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "st\n";
    std::string s;
    for(auto e: edgeIndex) {
        auto r = e.first.first;
        auto h = e.first.second;
        auto hpl = B_[h]->get_preference_list();
        for (auto i = hpl.begin(), g = hpl.end(); i != g; ++i) {
            auto rp = hpl.get_vertex(i);
            if(hpl.is_ranked_better(A_[r], rp)) {
                s.clear();
                auto rppl = A_[r]->get_preference_list();
                for (auto j = rppl.begin(), f = rppl.end(); j != f; ++j) {
                    auto hp = rppl.get_vertex(j);
                    if(rppl.is_ranked_better(hp, B_[h]) || hp->get_id() == h) {
                        s+="x";
                        s+=std::to_string(A_[r]->get_index());
                        s+="_";
                        s+=std::to_string(hp->get_index());
                        s+=" + ";
                    }
                }
                s = s.substr(0, s.size()-3);
                if(s.size()) {
                    s+=" - ";
                    s+="x";
                    s+=std::to_string(rp->get_index());
                    s+="_";
                    s+=std::to_string(B_[h]->get_index());
                    s+=" >= 0";
                    out << s << std::endl;
                }
            }
        }
    }
}

void BipartiteGraph::tripConstraints(const char* file_name) {
    tripObjective(file_name);
    tripvertexconstraints(file_name);
    agentConstraint(file_name);
    lpBounds(file_name);
    tripGeneral(file_name);
}

void BipartiteGraph::tripGeneral(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "general\n";
    std::string s, a, b;
    for(auto i: edgeIndex) {
        //std::cout << "edges Working";
        a = i.first.first;
        b = i.first.second;
        s += "x";
        //s += std::to_string(i+1);
        s += std::to_string(A_[a]->get_index());
        s += "_";
        s += std::to_string(B_[b]->get_index());
        s += " ";
    }
    out << s << std::endl;
    out << "end\n";
    out.close();
}

void BipartiteGraph::calculateCosts() {
    std::vector<std::pair<unsigned, VertexPtr>> upperQuotas;
    for(auto b: B_) {
        upperQuotas.push_back({b.second->get_upper_quota(), b.second});
    }
    sort(upperQuotas.begin(), upperQuotas.end(), std::greater<std::pair<unsigned, VertexPtr>>());
    int cost = 0;
    int cur = 0;
    int uq=0;
    for(auto i: upperQuotas) {
        if(cur==0) {
            uq = i.second->get_upper_quota();
        }
        else {
            if(i.second->get_upper_quota()==uq) {
            }
            else {
                cost++;
                uq = i.second->get_upper_quota();
            }
        }
        i.second->setCost(cost);
        cur++;
    }
}

void BipartiteGraph::addEdges() {
    int numEdges = 0;
    for(auto ver: A_) {
        IdType a = ver.first;
        auto& pl = (ver.second)->get_preference_list();
        for(auto i = pl.all_begin(), e = pl.all_end(); i != e; i++) {
            IdType b = (pl.get_vertex(*i))->get_id();
            edgeIndex[{a, b}] = ++numEdges;
        }
    }
}

void BipartiteGraph::rev(const char* file_name) {
    //std::cout << "REv callled";
    std::ifstream in;
    siz = 0;
    in.open(file_name, std::ios::in);
    //std::cout << edgeIndex.size();
    int a, b, p;
    std::string u, v;
    VertexPtr x, y;
    std::vector<std::pair<VertexPtr, VertexPtr>> unmatched;
    for(int i=0; i<edgeIndex.size(); i++) {
        in >> a >> b >> p;
        //std::cout << a << " " << b << " " << p << std::endl;
        for(auto m: A_) {
            if(m.second->get_index()==a) {
                u = m.first;
                x = m.second;
                break;
            }
        }
        for(auto m: B_) {
            if(m.second->get_index()==b) {
                v = m.first;
                y = m.second;
                break;
            }
        }
        auto &pl = A_[u]->get_preference_list();

        int pos1 = 0;

        for(auto i = pl.all_begin(), e = pl.all_end(); i != e; i++) {
            pos1++;
            IdType b = (pl.get_vertex(*i))->get_id();
            if(b==v) {
                break;
            }
        }
        if(p) {
            siz++;
            //std::cout << u<<","<<v << "," << pos1 <<std::endl;
            auto& xpl = x->get_partner_list();
            auto& ypl = y->get_partner_list();
            auto px = x->get_preference_list();
            auto py = y->get_preference_list();
            auto &pl = B_[v]->get_preference_list();

            int pos2 = 0;

            for(auto i = pl.all_begin(), e = pl.all_end(); i != e; i++) {
                pos2++;
                IdType a = (pl.get_vertex(*i))->get_id();
                if(a==u) {
                    break;
                }
            }
            xpl.add_partner({py.get_rank(pos1), y});
            ypl.add_partner({px.get_rank(pos2), x});
        }
        else {
            unmatched.push_back({x, y});
        }
    }
    /* for(auto pp: unmatched) {
        x = pp.first;
        y = pp.second;
        std::cout << x->get_id() << y->get_id() << std::endl;
        auto& xpl = x->get_partner_list();
        auto& ypl = y->get_partner_list();
        auto px = x->get_preference_list();
        auto py = y->get_preference_list();
        auto yleastit = ypl.get_least_preferred();
        auto xleastit = xpl.get_least_preferred();
        auto ylr = ypl.get_vertex(yleastit);
        auto xlr = xpl.get_vertex(xleastit);
        //if( px.is_ranked_better(y, xlr) && py.is_ranked_better(x, ylr)) std::cerr << "Not a Stable Matching" << std::endl;
    } */
    //calculateCosts();
    calcCost();
    calcMaxdev();
}

void BipartiteGraph::calcCost() {
    cost = 0;
    for(auto i: B_) {
        cost += i.second->get_cost()*i.second->get_partner_list().size();
    }
}

void BipartiteGraph::calcMaxdev() {
    maxDev = 0;
    avgDev = 0;
    for(auto i: B_) {
        maxDev = std::max(maxDev, int(i.second->get_partner_list().size()-i.second->get_upper_quota()));
        avgDev += int(i.second->get_partner_list().size() - i.second->get_upper_quota());
        std::cout << maxDev << " " << avgDev << std::endl;
    }
    std::cout << B_.size()<<std::endl;
    avgDev /= B_.size();
}

void BipartiteGraph::inputPara(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "R = " << A_.size() <<  ", H = " << B_.size();
    int m = 1e9;
    for(auto i: A_) {
        m = std::min(m, int(i.second->get_preference_list().size()));
    }
    out << ", LR = " << m;
    m=1e9;
    for(auto i: B_) {
        m = std::min(m, int(i.second->get_preference_list().size()));
    }
    out << ", LH = " << m;
    m=0;
    for(auto i: B_) {
        m = std::max(m, int(i.second->get_cost()));
    }
    out << ", UC = " << m<<std::endl;
}

void BipartiteGraph::outputPara(const char* file_name, std::string lpname) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    int rank1=0, rank2=0;
    for(auto i: A_) {
        auto pl = i.second->get_preference_list();
        auto b = i.second->get_partner_list().get_least_preferred()->second;
        if(pl.get_vertex(0) == b) rank1++;
        else if(pl.get_vertex(1) == b) rank2++;
    }
    out << lpname << ", " << siz << ", " << cost << ", " << rank1 << ", " << rank2 << ", " << maxDev << ", " << avgDev << "\n";
}

void BipartiteGraph::addPara(const char* file_name) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << "Hospital ";
    std::string s="";
    for(auto i: B_) {
        s+=i.second->get_id();
        s+=", ";
    }
    s = s.substr(0, s.size()-2);
    out << s << std::endl;

    out << "Capacity ";
    s="";
    for(auto i: B_) {
        s+=std::to_string(i.second->get_upper_quota());
        s+=", ";
    }
    s = s.substr(0, s.size()-2);
    out << s << std::endl;

    out << "Cost ";
    s="";
    for(auto i: B_) {
        s+=std::to_string(i.second->get_cost());
        s+=", ";
    }
    s = s.substr(0, s.size()-2);
    out << s << std::endl;
}

void BipartiteGraph::lpcapout(const char* file_name, std::string lpname) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    std::string s="";
    for(auto i: B_) {
        auto pl = i.second->get_partner_list();
        s+=std::to_string(pl.size());
        s+=", ";
    }
    s=s.substr(0, s.size()-2);
    out << lpname << ", " << s << std::endl;
}

void BipartiteGraph::matchingout(const char* file_name, std::string lpname) {
    std::ofstream out;
    out.open(file_name, std::ios::out | std::ios::app);
    out << lpname << std::endl;
    for(auto a: A_) {
        PartnerList::Iterator it = a.second->get_partner_list().get_least_preferred();
        auto b = a.second->get_partner_list().get_vertex(it);
        auto &pl = a.second->get_preference_list();

        int pos1 = 0;

        for(auto i = pl.all_begin(), e = pl.all_end(); i != e; i++) {
            pos1++;
            VertexPtr v = (pl.get_vertex(*i));
            if(v==b) {
                break;
            }
        }
        out << a.first<<","<<b->get_id() << "," << pos1 <<std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const std::unique_ptr<BipartiteGraph>& G) {
    auto print_vertex = [](const VertexPtr& v, std::stringstream& stmp) {
        int lq = v->get_lower_quota();
        int uq = v->get_upper_quota();

        stmp << v->get_id();
        if (lq > 0 and uq > 0) {
            stmp << ' ' << '(' << lq << ',' << uq << ')';
        } else if (uq > 1) {
            stmp << ' ' << '(' << uq << ')';
        }
    };

    std::stringstream stmp;

    // print the partitions first
    stmp << "@PartitionA\n";
    for (const auto& it : G->get_A_partition()) {
        print_vertex(it.second, stmp);
        stmp << ", ";
    }
    stmp.seekp(-2, stmp.cur);
    stmp << ";\n@End\n";

    stmp << "\n@PartitionB\n";
    for (const auto& it : G->get_B_partition()) {
        print_vertex(it.second, stmp);
        stmp << ", ";
    }
    stmp.seekp(-2, stmp.cur);
    stmp << ";\n@End\n";

    // and then the preference lists
    stmp << "\n@PreferenceListsA\n";
    for (const auto& it : G->get_A_partition()) {
        stmp << it.first << ": " << it.second->get_preference_list() << '\n';
    }
    stmp << "@End\n";

    stmp << "\n@PreferenceListsB\n";
    for (const auto& it : G->get_B_partition()) {
        stmp << it.first << ": " << it.second->get_preference_list() << '\n';
    }
    stmp << "@End";

    return out << stmp.str();
}

