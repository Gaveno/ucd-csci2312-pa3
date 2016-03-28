//
// Created by Gavin on 2/14/2016.
//

#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

#include "Cluster.h"
#include "Exceptions.h"

namespace Clustering {

    unsigned int Cluster::__idGenerator = 0;
    const char POINT_CLUSTER_ID_DELIM = ':';

    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __c(c), __p(d) { // needs ref to cluster
        __dimensions = d;
        if (__c.__size == 0) {
            __valid = false;
            toInfinity();
        }
    }

    // getters/setters
    const Point Cluster::Centroid::get() const { // doesn't check for validity
        return __p;
    }
    void Cluster::Centroid::set(const Point &p) { // sets to valid
        __valid = true;
        __p = p;
    }
    bool Cluster::Centroid::isValid() const {
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }

    // functions
    void Cluster::Centroid::compute() {
        double avg;

        if (__c.__size > 0) {
            for (unsigned int d = 0; d < __dimensions; ++d) {
                avg = 0;
                for (unsigned int i = 0; i < __c.__size; ++i) {
                    avg += (__c[i])[d];
                }
                __p[d] = avg / __c.__size;
            }
        }
        else {
            toInfinity();
        }

        __valid = true;
    }
    bool Cluster::Centroid::equal(const Point &p) const {
        bool eq = true;
        for (unsigned int i = 0; i < __dimensions; ++i) {
            if (__p[i] != p[i]) {
                eq = false;
                break;
            }
        }

        return eq;
    }
    void Cluster::Centroid::toInfinity() {
        for (unsigned int i = 0; i < __dimensions; ++i) {
            __p[i] = std::numeric_limits<double>::max();
        }
    }

    Cluster::Cluster(unsigned int d) : centroid(d, *this) {
        __dimensionality = d;
        __size = 0;
        __points = nullptr;
        __id = __idGenerator;
        ++__idGenerator;
    }

    Cluster::Cluster(const Cluster &origin) : Cluster(origin.__dimensionality) {
        for (int i = 0; i < origin.getSize(); ++i)
        {
            add(origin[i]);
        }

        __id = origin.__id;

        centroid.compute();
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

        __id = origin.__id;
        centroid.compute();

        return *this;
    }

    unsigned int Cluster::getSize() const {
        return __size;
    }

    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }

    unsigned int Cluster::getId() const {
        return __id;
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
        if (__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());

        // Empty list, adding first
        if (__size == 0) {
            ++__size;
            __points = new LNode(p, nullptr);
            centroid.setValid(false);
        }
        else { // Non-empty list
            // Make sure point does not already exist
            if (contains(p))
                return; // Point already exists, exit

            centroid.setValid(false);
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
        if (__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());

        if (contains(p)) {
            centroid.setValid(false);
            // Point is in list
            LNodePtr next;
            LNodePtr prev = nullptr;

            next = __points;

            while (next != nullptr) {
                if (next->point.getId() == p.getId()) {
                    // Found point
                    if (prev == nullptr) {
                        // First element
                        if (__size > 1)
                            __points = next->next;
                        else
                            __points = nullptr;

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

    bool Cluster::contains(const Point &p) const {
        if (__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());

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

    // Centroid functions
    void Cluster::pickCentroids(unsigned int k, Point **pointArray) { // pick k initial centroids
        if (k >= __size) {
            for (unsigned int i = 0; i < __size; ++i) {
                *(pointArray[i]) = (*this)[i];
            }
            if (k > __size) {
                for (unsigned int i = __size; i < k; ++i) {
                    //pointArray[i] = new Point(__dimensionality);
                    for (unsigned int d = 0; d < __dimensionality; ++d) {
                        pointArray[i]->setValue(d, std::numeric_limits<double>::max());
                        //(pointArray[i])[d] = std::numeric_limits<double>::max();
                    }
                }
            }
        }
        else {
            if (k > 100) {
                for (unsigned int i = 0; i < k; ++i) {
                    *(pointArray[i]) = (*this)[i];
                }
            }
            else {
                *(pointArray[0]) = __points->point;
                // a is index of point k
                // b is index of cluster
                // c is index of distance between pointArray and next point
                for (unsigned int a = 1; a < k; ++a) {
                    double avgD = 0;
                    unsigned int furIndx = 0;
                    for (unsigned int b = 0; b < __size; ++b) {
                        double nextD = 0;
                        bool used = false;

                        // Average distance between current point and pointArray
                        for (unsigned int c = 0; c < a; ++c) {
                            nextD += ((*this)[b]).distanceTo(*(pointArray[c]));
                            if ((*this)[b] == *(pointArray[c]))
                                used = true;
                        }
                        nextD /= a;

                        // New furthest point
                        if (nextD > avgD && !used) {
                            avgD = nextD;
                            furIndx = b;
                        }
                    }
                    *(pointArray[a]) = (*this)[furIndx];
                    //pointArray[a] = new Point((*this)[furIndx]);
                }
            }
        }
    }

    // Operators
    // Members: Sub-script
    const Point &Cluster::operator[](unsigned int index) const {
        if (__size == 0)
            throw EmptyClusterEx();

        if (index >= __size)
            throw OutOfBoundsEx(__size, index);

        LNodePtr cursor = __points;

        for (int i = 0; i < index; ++i) {
            cursor = cursor->next;
        }
        return cursor->point;
    }

    // Members: Compound assignment (Point argument)
    Cluster &Cluster::operator+=(const Point &p) {
        /*if (__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());*/
        add(p);

        return *this;
    }
    Cluster &Cluster::operator-=(const Point &p) {
        /*if (__dimensionality != p.getDims())
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());*/

        remove(p);

        return *this;
    }

    // Members: Compound assignment (Cluster argument)
    Cluster &Cluster::operator+=(const Cluster &rhs) { // union
        if (__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, rhs.__dimensionality);

        for (int i = 0; i < rhs.getSize(); ++i) {
            add(rhs[i]);
        }

        return *this;
    }
    Cluster &Cluster::operator-=(const Cluster &rhs) { // (asymmetric) difference
        if (__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, rhs.__dimensionality);

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
            out << cluster[i] << " " << POINT_CLUSTER_ID_DELIM << " " << cluster.__id << std::endl;
            //std::cout << cluster[i] << std::endl;
        }

        return out;
    }
    std::istream &operator>>(std::istream &in, Cluster &cluster) {
        while (!in.eof()) {
            //Point p(cluster.__dimensionality);

            std::string str;
            std::getline(in, str);

            if (str.length() > cluster.__dimensionality) {
                Point p(cluster.__dimensionality);
                std::stringstream ss(str);

                try {
                    ss >> p;
                    cluster.add(p);
                }
                catch (DimensionalityMismatchEx& e) {
                    std::cerr << "Caught an exception of type: " << e << std::endl;
                    p.rewindIdGen();
                }
                catch (...) {
                    std::cerr << "Caught an unknown exception" << std::endl;
                    p.rewindIdGen();
                }
            }
        }


        return in;
    }

    // Friends: Comparison
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);

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
        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);

        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) { // (asymmetric) difference
        if (lhs.__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);

        Cluster sub(lhs);
        sub -= rhs;
        return sub;
    }

    // Move Inner class
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to)
    : __p(p), __from(from), __to(to) {
        //perform();
    }

    void Cluster::Move::perform() {
        if (__from.contains(__p)) {
            __to.add(__from.remove(__p));

            //__to.centroid.setValid(false);
            //__from.centroid.setValid(false);
        }

        if (__to.__size == 0) {
            __to.centroid.toInfinity();
        }

        if (__from.__size == 0) {
            __from.centroid.toInfinity();
        }
    }
}