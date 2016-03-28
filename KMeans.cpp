//
// Created by Gavin on 3/26/2016.
//

#include <cassert>

#include "KMeans.h"
#include "Exceptions.h"


namespace Clustering {

    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        if (k == 0)
            throw ZeroClustersEx();

        std::ifstream file(filename);

        if (!file)
            throw DataFileOpenEx(filename);

        __maxIter = maxIter;
        __numNonempty = 1;
        __dimensionality = dim;
        __k = k;

        __clusters = new Cluster *[k];
        for (unsigned int i = 0; i < k; ++i) __clusters[i] = new Cluster(dim);

        file >> *(__clusters[0]);

        file.close();

        __initCentroids = new Point *[k];
        for (unsigned int i = 0; i < k; ++i) __initCentroids[i] = new Point(dim);

        __clusters[0]->pickCentroids(k, __initCentroids);
    }


    KMeans::~KMeans() {
        for (unsigned int i = 0; i < __k; ++i) {
            delete __initCentroids[i];
            delete __clusters[i];
        }
        delete [] __initCentroids;
        delete [] __clusters;
    }

    // accessors
    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }
    unsigned int KMeans::getNumIters() {
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }

    // element access (for testing, no bounds checking)
    Cluster &KMeans::operator[](unsigned int u) {
        return *(__clusters[u]);
    }
    const Cluster &KMeans::operator[](unsigned int u) const {
        return *(__clusters[u]);
    }

    // write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {
        for (unsigned int i = 0; i < kmeans.__k; ++i) {
            //std::cout << "Cluster ID: " << kmeans.__clusters[i]->getId() << std::endl;
            //os << *(kmeans.__clusters[i]);
            os << kmeans[i];
        }
    }

    // clustering functions
    void KMeans::run() {
        assert(__clusters[0]->getSize() > 0);

        unsigned int moves = 1;
        unsigned int iter = 0;

        bool kGreaterEqual = (__k >= __clusters[0]->getSize());

        for (unsigned int i = 1; i < __k; ++i) {
            Cluster::Move move(*(__initCentroids[i]), *(__clusters[0]), *(__clusters[i]));
            move.perform();
        }

        while (!kGreaterEqual && moves > 0 && iter < __maxIter) {
            moves = 0;

            for (unsigned int c = 0; c < __k; ++c) {
                for (unsigned int p = 0; p < __clusters[c]->getSize(); ++p) {
                    double distance = (*(__clusters[c]))[p].distanceTo(__clusters[c]->centroid.get());
                    unsigned int closestIndex = c;
                    for (unsigned int i = 0; i < __k; ++i) {
                        if (i != c && (*(__clusters[c]))[p].distanceTo(__clusters[i]->centroid.get()) < distance) {
                            distance = (*(__clusters[c]))[p].distanceTo(__clusters[i]->centroid.get());
                            closestIndex = i;
                        }
                    }

                    if (closestIndex != c) {
                        Cluster::Move move((*(__clusters[c]))[p], *(__clusters[c]), *(__clusters[closestIndex]));
                        move.perform();

                        moves++;
                    }
                }
            }

            for (unsigned int c = 0; c < __k; ++c) {
                if (!__clusters[c]->centroid.isValid())
                    __clusters[c]->centroid.compute();
            }

            iter++;
        }

        if (!kGreaterEqual)
            moves = 0;

        __numIter = iter;
        __numMovesLastIter = moves;
        __numNonempty = 0;
        for (unsigned int c = 0; c < __k; ++c) {
            if (__clusters[c]->getSize() > 0)
                __numNonempty++;
        }
        //Testing
        //std::cout << "Non empty clusters: " << __numNonempty << std::endl;
        /*if (__k == 4) {
            std::cout << "Clusters non empty" << std::endl;
            for (unsigned int i = 0; i < __k; ++i) {
                if (__clusters[i]->getSize() > 0) {
                    std::cout << *(__clusters[i]) << std::endl;
                }
            }
        }*/
    }

}