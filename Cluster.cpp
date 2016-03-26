//
// Created by Gavin on 2/14/2016.
//

#include <cassert>
#include <sstream>
#include <iomanip>
#include <string>

#include "Cluster.h"

namespace Clustering {

    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {
    }

    Cluster::Cluster() {
        __size = 0;
        __points = nullptr;
    }

    Cluster::Cluster(const Cluster &origin) : Cluster() {
        for (int i = 0; i < origin.getSize(); ++i)
        {
            add(origin[i]);
        }
    }

    Cluster &Cluster::operator=(const Cluster &origin) {
        if (__size > 0) {
            LNodePtr destroy;
            LNodePtr cursor = __points;

            while (cursor != nullptr) {
                destroy = cursor;
                cursor = cursor->next;

                delete destroy;
            }
            __size = 0;
        }

        for (int i = 0; i < origin.getSize(); ++i)
        {
            add(origin[i]);
        }

        return *this;
    }

    int Cluster::getSize() const {
        return __size;
    }

    Cluster::~Cluster() {
        LNodePtr current;
        while (__points != nullptr) {
            current = __points;
            __points = current->next;

            delete current;
        }
    }

    // Set functions: They allow calling c1.add(c2.remove(p));
    void Cluster::add(const Point &p) {
        // Empty list, adding first
        if (__size == 0) {
            ++__size;
            __points = new LNode(p, nullptr);
        }
        else { // Non-empty list
            // Make sure point does not already exist
            if (contains(p))
                return; // Point already exists, exit

            // next and previous pointers
            LNodePtr next;
            LNodePtr prev;

            next = __points;    // Start of list
            prev = nullptr;

            // find insert position
            while (next != nullptr) {
                if (p < next->point) {
                    // Insert here
                    if (prev == nullptr) {
                        // First element in list
                        __points = new LNode(p, next);

                        ++__size;

                        return;  // Add complete, exit
                    }
                    else {
                        // Not first element in list
                        prev->next = new LNode(p, next);

                        ++__size;

                        return;  // Add complete, exit
                    }
                }

                prev = next;
                next = next->next;

            }

            // No insert found, add to end
            prev->next = new LNode(p, nullptr);

            ++__size;
        }
    }

    const Point &Cluster::remove(const Point &p) {
        if (contains(p)) {
            // Point is in list
            LNodePtr next;
            LNodePtr prev = nullptr;

            next = __points;

            while (next != nullptr) {
                if (next->point == p) {
                    // Found point
                    if (prev == nullptr) {
                        // First element
                        __points = next->next;

                        delete next;

                        --__size;

                        break;
                    }
                    else {
                        // not first
                        prev->next = next->next;
                        delete next;

                        --__size;

                        break;
                    }
                }

                prev = next;
                next = next->next;
            }
        }

        return p;
    }

    bool Cluster::contains(const Point &p) {
        LNodePtr next = __points;

        while (next != nullptr) {
            //  Check if ID's of the pointers match
            if (next->point.getId() == p.getId())
                return true;    // True, contains point
            else
                next = next->next;
        }

        // End of loop, does not contain point
        return false;
    }

    // Operators
    // Members: Sub-script
    const Point &Cluster::operator[](unsigned int index) const {
        assert (__size > 0);
        LNodePtr cursor = __points;

        for (int i = 0; i < index; ++i) {
            cursor = cursor->next;
        }
        return cursor->point;
    }

    // Members: Compound assignment (Point argument)
    Cluster &Cluster::operator+=(const Point &p) {
        add(p);

        return *this;
    }
    Cluster &Cluster::operator-=(const Point &p) {
        remove(p);

        return *this;
    }

    // Members: Compound assignment (Cluster argument)
    Cluster &Cluster::operator+=(const Cluster &rhs) { // union
        for (int i = 0; i < rhs.getSize(); ++i) {
            add(rhs[i]);
        }

        return *this;
    }
    Cluster &Cluster::operator-=(const Cluster &rhs) { // (asymmetric) difference
        for (int i = 0; i < rhs.getSize(); ++i) {
            remove(rhs[i]);
        }

        return *this;
    }

    // Private Member functions
    void Cluster::__del() {
        // stub
    }

    void Cluster::__cpy(LNodePtr pts) {
        // stub
    }

    bool Cluster::__in(const Point &p) const {
        // stub
    }

    // Operators Friends
    // Friends: IO
    std::ostream &operator<<(std::ostream &out, const Cluster &cluster) {
        out << std::setprecision(20);
        for (int i = 0; i < cluster.getSize(); ++i) {
            out << cluster[i] << std::endl;
            //std::cout << cluster[i] << std::endl;
        }

        return out;
    }
    std::istream &operator>>(std::istream &in, Cluster &cluster) {
        while (!in.eof()) {
            Point p(1);

            std::string str;
            std::getline(in, str);

            if (str.length() > 0) {
                std::stringstream ss(str);

                ss >> p;

                cluster.add(p);
            }
        }


        return in;
    }

    // Friends: Comparison
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.getSize() != rhs.getSize())
            return false;

        for (int i = 0; i < lhs.getSize(); ++i) {
            if (lhs[i] != rhs[i])
                return false;
        }

        return true;
    }
    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        return !(lhs == rhs);
    }

    // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &lhs, const Point &rhs) {
        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Point &rhs) {
        Cluster sub(lhs);
        sub -= rhs;
        return sub;
    }

    // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) { // union
        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) { // (asymmetric) difference
        Cluster sub(lhs);
        sub -= rhs;
        return sub;
    }


}