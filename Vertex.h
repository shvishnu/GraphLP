#ifndef VERTEX_H
#define VERTEX_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "PreferenceList.h"
#include "PartnerList.h"
#include "TDefs.h"

class Vertex {
private:
    IdType id_;                 // id of this vertex
    IdType cloned_for_id_;      // id of the vertex this was cloned for
    int index;
    unsigned lower_quota_;           // least number of partners to this vertex
    unsigned upper_quota_;           // maximum number of partners to this vertex
    unsigned cost;                  // cost of hospital
    bool dummy_;                // is this a dummy vertex
    PreferenceList pref_list_;  // preference list according to priority
    std::vector<std::string> P;
    PartnerList part_list_;
public:
    Vertex(const IdType& id, int i);
    Vertex(const IdType& id, unsigned upper_quota, int i);
    Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota, int i);
    Vertex(const IdType& id, unsigned lower_quota, unsigned upper_quota, int i, bool dummy);
    Vertex(const IdType& id, const IdType& cloned_for_id, unsigned lower_quota,
           unsigned upper_quota, int i, bool dummy);
    void partnerAppend(IdType);
    virtual ~Vertex();

    const IdType& get_id() const;
    const IdType& get_cloned_for_id() const;
    unsigned get_lower_quota() const;
    unsigned get_upper_quota() const;
    unsigned get_cost() const;
    void setCost(unsigned);
    int get_index() const;
    bool is_dummy() const;
    PartnerList& get_partner_list();
    PreferenceList& get_preference_list();
    const PreferenceList& get_preference_list() const;
};

/// print this vertex in the format v: p1, ..., pk
std::ostream& operator<<(std::ostream& out, VertexPtr v);

#endif
