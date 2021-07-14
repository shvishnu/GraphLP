#include "Vertex.h"
#include <sstream>

Vertex::Vertex(const IdType& id, int i)
    : Vertex(id, id, 0, 1, i, false)
{}

Vertex::Vertex(const IdType& id, unsigned upper_quota, int i)
    : Vertex(id, id, 0, upper_quota, i, false)
{}

Vertex::Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota, int i)
    : Vertex(id, id, lower_quota, upper_quota, i, false)
{}

Vertex::Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota, int i, bool dummy)
    : Vertex(id, id, lower_quota, upper_quota, i, false)
{}

Vertex::Vertex(const IdType& id, const IdType& cloned_for_id,
               unsigned lower_quota, unsigned upper_quota, int i, bool dummy)
    : id_(id), cloned_for_id_(cloned_for_id),
      lower_quota_(lower_quota), upper_quota_(upper_quota),
      dummy_(dummy)
{index = i;}

Vertex::~Vertex()
{}

IdType const& Vertex::get_id() const {
    return id_;
}

IdType const& Vertex::get_cloned_for_id() const {
    return cloned_for_id_;
}

unsigned Vertex::get_lower_quota() const {
    return lower_quota_;
}

unsigned Vertex::get_upper_quota() const {
    return upper_quota_;
}

unsigned Vertex::get_cost() const {
    return cost;
}

void Vertex::setCost(unsigned n) {
    cost = n;
}

int Vertex::get_index() const {
    return index;
}

bool Vertex::is_dummy() const {
    return dummy_;
}

void Vertex::partnerAppend(IdType s) {
    P.push_back(s);
}

PreferenceList& Vertex::get_preference_list() {
    return pref_list_;
}

PreferenceList const& Vertex::get_preference_list() const {
    return pref_list_;
}

PartnerList& Vertex::get_partner_list(){
    return part_list_;
}

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v) {
    std::stringstream stmp;
    stmp << v->get_id() << ": "; // print vertex id
    stmp << v->get_preference_list() << '\n';
    return out << stmp.str();
}
