## CSCI 2312: Programming Assignment 3

_exceptions, inner classes, k-means clustering_

* * *

### Goals

1. Extend the design of the Cluster, and implement a new class, KMeans, which performs K-means clustering on multidimensional real-valued data.
2. Learn implement and use exceptions to fortify your program's interface.
3. Practice input/output within the streams abstraction.
4. Complete the design and implementation of a fully functional C++ program.
5. Continue using git and Github.
6. Develop good coding style.

### Synopsis

PA3 brings you to the culmination of your work with points, asking you to implement the K-means algorithm for clustering multidimensional real-valued points. Most of the functionality of your `Point` and `Cluster` classes will be used as is, with some key extensions to `Cluster`. Extensive exception coverage will fortify the program's interface. The actual clustering algorithm will be implemented in a new class, `KMeans`. You have to write four files, <tt>Point.cpp</tt>, <tt>Cluster.cpp</tt>, <tt>KMeans.cpp</tt>, and <tt>Exceptions.cpp</tt>. See the [Detailed Instructions](https://github.com/ivogeorg/ucd-csci2312-pa3/blob/master/README.md#detailed-instructions) at the bottom of this file.

PA3 is in the test-driven-development (TDD) style, just like [PA2](https://github.com/ivogeorg/ucd-csci2312-pa2). It has 392 tests that your implementation should pass for full points. The tests from PA2 are a subset of the PA3 test suite. Start by adding your original <tt>Point.cpp</tt> and <tt>Cluster.cpp</tt> files and creating and implementing the <tt>Exceptions.cpp</tt> file, which should be fairly straightforward. When you pass all the `Point` and `Cluster` tests, with all the `Exceptions` checking, uncomment and move on to the `KMeans` tests. (Once again, don't forget to `git add` whatever files you add to your project.) There are some much larger data files contained in the repository than for PA2.

PA3 is commensurate to PA2. You should expect to write about 400 lines of code and spend about 15-25 hours on it, depending on how well you did on PA2. Once again, it is advisable that you start early and do it in stages. If you get stuck on anything for more than **one hour**, seek help.

### Submission

You don't need to submit anything. Once you fork the repository (this is your **remote** repository on Github, aka **origin**), you will clone it to your development machine (this is your local repository), and start work on it. Commit your changes to your local repository often and push them up to the remote repository occasionally. Make sure you push at least once before the due date. At the due date, your remote repository will be cloned and tested automatically by the grading script. _**Note:** Your code should be in the **master** branch of your remote repository._

### Grading

An autograding script will run the test suite against your files. Your grade will be based on the number of tests passed. (E.g. if your code passes 3 out of 6 test cases, your score will be 50% and the grade will be the corresponding letter grade in the course's grading scale). The test suite for PA3 has __392__ tests. **Note:** The testing and grading will be done with fresh original copies of all the provided files. In the course of development, you can modify them, if you need to, but your changes will not be used. Only your <tt>Point.cpp</tt>, <tt>Cluster.cpp</tt>, <tt>KMeans.cpp</tt>, and <tt>Exceptions.cpp</tt> files will be used.

### Compiler

Your program should run on **GCC 4.9.0** or later, or **Clang 3.3** or later. No other compilers are supported.

### Due Date

The assignment is due on **Wed, Mar 30, at 23:59 Mountain time**. No late work. The last commit to your PA3 repository before the deadline will be graded.

### Honor Code

Free Github repositories are public so you can look at each other's code. Please, don't do that. You can discuss any programming topics and the assignments in general but sharing of solutions diminishes the individual learning experience of many people. Assignments might be randomly checked for plagiarism and a plagiarism claim may be raised against you.

### Use of libraries

For this assignment, no external libraries should be used, except for the Standard Library minus the Standard Template Library (STL). You should reuse your own implementation of the linked list from PA2. You are encouraged to use STL classes in your implementation of `KMeans::pickCentroids` and `Kmeans::run`. We will use the STL more extensively in PA4-PA5.

### Coding style

Familiarize yourself with and start following [coding style guidelines](http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cppstyle.html). There are others on the Web. Pick one and be consistent. _**Note:** If you stumble on the Google C++ Style Guide, be advised that it has been heavily criticized by many leading C++ programmers. I don't advise you to follow it, especially the more advanced features. This Guide is for entry-level coders at Google who need to be able to work with their legacy code. It is not advisable for new projects and novice programmers._

### References

Operator overloading [guidelines](http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html).

A very good [C++ tutorial](http://www.learncpp.com/), including many topics we are covering.

Two websites with C++ Reference, [here](http://en.cppreference.com/w/) and [here](http://www.cplusplus.com/).

### Detailed Instructions

#### Exceptions

1. All the exceptions that you need to implement are declared in the <tt>Exceptions.h</tt> file. They are very simple classes, which hold some environment data, if any, to report back to the handler when thrown. They also hold their `name` so that the test suite handlers can make sure the correct exception is thrown in expected anomalous situations. They also print themselves to a stream, so an informative message can be reported and/or logged.

2. `OutOfBoundsEx`.

  1. Attributes: `__current` is the _size_ of the current object, `__rhs` is the offending argument.
  2. Thrown by `Point::setValue`, `Point::getValue`, `Point::operator[]`, `Cluster::operator[]`.
  3. Caught by user code (test suite).

3. `DimensionalityMismatchEx`.

  1. Attributes: `__current` is the _dimensionality_ of the current object, `__rhs` is the offending argument.
  2. Thrown by `Point::distanceTo`, `Point::operator+=`, `Point::operator-=`, `Point::operator<` (and the other comparison operators through it), `Point::operator=`, `Point::operator==` (and the `operator!=` through it), `Point::operator>>`; `Cluster::add` and `Cluster::remove` (and, through them, any other functions or operators which use them), `Cluster::operator+=` and `Cluster::operator-=` (for both `Point` and `Cluster` right-hand sides) (and, through them, the correspoinding simple-arithmetic operators), `Cluster::operator==` (and the `operator!=` through it).
  3. Caught by user code (test suite) and by `Cluster::operator>>` when reading in a data file.

4. `ZeroClustersEx`.
 
  1. Attributes: none.
  2. Thrown by `KMeans::KMeans` constructor when k=0.
  3. Caught by user code (test suite).

5. `DataFileOpenEx`.

  1. Attributes: the data file's `filename`.
  2. Thrown by `KMeans::KMeans` constructor when the file with such a name cannot be found or can otherwise not be opened.
  3. Caught by user code (test suite).

6. `ZeroDimensionsEx`.

  1. Attributes: none.
  2. Thrown by `Point::Point` constructor when d=0. Note that this exception can be thrown by both `Cluster::Cluster` and `KMeans::KMeans` but, since the initialization of both `Cluster` and `KMeans` involves initialization of at least one `Point`, and since this is a fatal condition after which the clustering program cannot proceed, throwing it from `Point::Point` only is _minimalistic design_.
  3. Caught by user code (test suite).

7. `EmptyClusterEx`.

  1. Attributes: none.
  2. Thrown by `Cluster::operator[]` when the cluster is empty.
  3. Caught by user code (test suite).

#### Point class

1. Throw the indicated exceptions from the indicated methods in the [Exceptions section](https://github.com/ivogeorg/ucd-csci2312-pa3/blob/master/README.md#exceptions).

2. Define and initialize the `static const char POINT_VALUE_DELIM` to `','`. Use it in your `Point::operator>>`.

3. Define the `static void Point::rewindIdGen` method. Use it to rewind the `Point` id generator when a `DimensionalityMismatchEx` is caught in `Cluster::operator>>` when a `Point` cannot read itself. This will ensure the `Point` is-s are sequential and there are no gaps in the sequence due to data file reading problems.

4. Define the `const Point::operator[]`. It is needed by the `KMeans` class.

#### Centroid class

1. This class is a _private inner_ class of class `Cluster`. This means that:

  1. Objects of class `Centroid` can **only** be declared within the scope of class `Cluster` (declarations and definitions).
  2. An object of class `Centroid` has _private access_ to an object of class `Cluster`. For this to be useful, a reference to a `Cluster` object has to be passed as an argument to the `Centroid::Centroid` constructor. See the `Cluster` header.

2. Implement the methods of the inner class `Centroid`. All implementations will be identified by the `Cluster::Centroid::` qualifier.

3. `Cluster::Centroid::equal` only compares the `double` values and ignores the _id_ of the `Point` argument.

4. `Cluster::Centroid::compute` uses the private access to its `Cluster` to compute the _average of its points_.

5. `Cluster::Centroid::set` and `Cluster::Centroid::compute` set the centroid to _valid_.

6. An "infinite" `Point` is used to indicate the centroid of an empty `Cluster`. For the purposes of this assignment, this is defined as a `Point` with all values set to `std::numeric_limits<double>::max()`. 

7. `Cluster::Centroid::toInfinity` sets the centroid to an _infinite point_.

#### Move class

1. This class is a very simple _public inner_ class of class `Cluster`. This means that objects of class `Move` can be declared in any scope by using the `Cluster::Move` type.

2. This class is used to encapsulate the _indivisible_ operation of adding a `Point` from one `Cluster` after removing it from another. In our clustering paradigm, all `Point` objects have to "belong" to `Cluster` objects, and do not make sense outside of them.

3. Implement the `Cluster::Move::perform` method and use it in the implementation of `KMeans::run`. 

  **Note:** This method should invalidate the centroids of both clusters.

#### Cluster class

1. Throw the indicated exceptions from the indicated methods in the [Exceptions section](https://github.com/ivogeorg/ucd-csci2312-pa3/blob/master/README.md#exceptions).

2. Define and initialize the `static const char POINT_CLUSTER_ID_DELIM` to `':'`. Use it in your `Cluster::operator<<`.

3. Initialize the new `public Centroid` member.

  **Note:** You can now use calls like `c7.centroid.isValid();` in your code.

  **Note:** A `Cluster` constructed with a _copy ctor_ has to compute its centroid. Notice that `Centroid::Centroid(const Centroid &)` is **explicitly deleted** disallowing copying of `Centoid` objects. _(This is due to the fact that the private `Cluster` reference in the `Centroid` is explicitly constant and cannot be reassigned. C++ references can only be assigned upon initialization (i.e. in a ctor).)_

  **Note:** A `Cluster` assigned to with a `operator=` cannot assignconstructed with a _copy ctor_ has to set its centroid to the centroid of the argument `Cluster` and also set it to valid. Notice that `Centroid::operator=(const Centroid &)` is **explicitly deleted** disallowing copying and assignment of `Centoid` objects.

4. When a `KMeans` object is initializing itself (i.e. in the `KMeans::KMeans` constructor), it creates `k` number of clusters (in a dynamic array) and uses the first `Cluster` array element to read the specified data file in. Then it picks `k` number of centroids from the `Point`s of this `Cluster` to set as initial centroids of all `k` `Cluster` array elements. Implement the `Cluster::pickCentroids` method.

  1. The arguments to this method are `k`, the number of clusters, and an already allocated array of `Point`s where to put the selected centroids.

  **Note:** The caller, which in this case is the `KMeans::KMeans` constructor, is respoinsible for allocation of this array. The double pointer `Point **` is because `Point` has no default constructor and we cannot declare a static array of `Point`s. `KMeans::KMeans` constructor or `KMeans::~KMeans` destructor should deallocate this array.
  
  2. When **k >= __size** (the size of the `Cluster`) the first `k` elements of the array should be set to the `Point`s of the `Cluster` and the rest should be set to _infinite points_. See the [Centroid section](https://github.com/ivogeorg/ucd-csci2312-pa3/blob/master/README.md#centoid-class) for an explanation of _infinite points_. The `Point::Point(const Point &)` operator should be used to set the centroids.

  3. When **k < __size** `k` points should be picked from the cluster to set as initial centroids. The algorithm is up to you and you should start by just picking the first `k` points to get things going. Eventually, you might need to pick your centroids better to be able to finish each K-means clustering test **within 20 iterations**. _Hint: The best performance is achieved by pickeing points that are far away from each other. You can loop through `k` iterations of an algorithm that picks the farthest point from the set of points already picked._

  4. This is a great use case for the `std::set` class.

5. Implement cluster id-s through the `static unsigned int` id generator, just like you did for `Point`.

6. Modify `Cluster::operator<<` to output the cluster id after each point, as follows:

   ```
   2.30000, 5.60000, 0.00000, 5.60000, 7.90000 : 6093
   ```
  
#### KMeans class

1. The `KMeans` class does three things:

  1. Initializes itself.
  2. Runs.
  3. Outputs the results to a file.

1. Throw the indicated exceptions from the indicated methods in the [Exceptions section](https://github.com/ivogeorg/ucd-csci2312-pa3/blob/master/README.md#exceptions).

2. Implement the `KMeans::KMeans` constructor. This is where all the initialization and anomaly checking has to happen. Either the constructor throws an exception or after its execution the `KMeans::run` method can safely be called. If any exceptions are to be thrown, any already allocated memory should be deallocated before the `throw` statement.

3. Implement the getters and simple getters. The variables are as follows:

  1. `__maxIter` is the maximum number of iterations the algorithm can run. This is a constructor parameter and has to be observed. Note that this is a **termination condition** for the K-means algorithm.

  2. `__numIter` is the actual number of iterations the algorithm ran. Note that this can be less than `__maxIter` as the points may stop moving between clusters before `__maxIter` iterations have been performed. Note that this is another **termination condition** for the K-means algorithm.

  3. `__numNonemptyClusters` is the number of non-empty clusters before or after the K-means algorithm has run. Note that before running, it's equal to 1, if the initialization in the `KMeans::KMeans` constructor was successful.

  4. `__numMovesLastIter` is the number of moves that were performed during the last iteration of the algorithm. Note that this can be larger than zero if `__maxIter` was set to a number smaller than the minimum iterations necessary for the point movement to stop.

4. Implement the `KMeans::run` method. This is the heart of the algorithm. Here it is _in pseudocode_:

  1. Check if the centroids have been assigned. You can use `assert`.

  2. Initialize locals `moves = 100` a `iter = 0`. These will be used for the termination condition.

  3. `while (moves > 0 and iter < __maxIter)`

    * `moves = 0` // restart the count for the new iteration
    * `for (c : k clusters)`
      * `for (p : cluster points)`
        * Find closest centroid
        * If it is **not** the centroid of `c`
          * Move `p` to the other cluster. Use a local `Cluster::Move`
          * `moves ++`
    * Recompute all invalidated centroids.
    * `iter ++`

  4. Set the metadata:
  
    * `__numIter = iter`
    * `__numMovesLastIter = moves`
    * Count non-empty clusters in `nonempty`
    * `__numNonempty = nonempty`

5. Implement `KMeans::operator<<`. This should write out the results to a file. A file output stream is supplied by the caller. Only nonempty clusters should be written out, in no particular order, in the following format:

   ```
   48.9819, 47.4258, 52.0498 : 10440
   49.7811, 49.1227, 51.7090 : 10440
   50.2934, 48.7180, 51.8880 : 10440
   50.4146, 48.4508, 51.4070 : 10440
   51.4925, 48.3226, 51.9171 : 10440
   13.1473, 45.7105, 46.8457 : 10441
   12.6223, 51.8332, 48.9889 : 10442
   13.4091, 51.1114, 51.8555 : 10442
   13.4349, 49.4999, 51.6522 : 10442
   13.4117, 47.1157, 51.4740 : 10443
   13.6196, 45.1573, 52.3948 : 10443
   13.8807, 47.5944, 55.9948 : 10444
   15.6523, 46.7538, 55.6698 : 10444
   14.6511, 48.6109, 45.2570 : 10445
   14.7062, 48.2472, 47.5892 : 10445
   14.7980, 48.4135, 47.7455 : 10445
   15.7419, 48.8970, 47.0186 : 10445
   14.7341, 46.1872, 42.4663 : 10446
   14.7637, 47.3621, 49.4916 : 10447
   14.9784, 49.5267, 49.5483 : 10447
   15.1096, 50.0164, 48.2296 : 10447
   15.7378, 49.5870, 50.3862 : 10447
   15.7588, 50.3209, 49.7688 : 10447
   16.7001, 49.3881, 50.4152 : 10447
   15.1601, 49.5503, 53.4565 : 10448
   15.9976, 49.1623, 51.5173 : 10448
   16.0302, 50.3806, 52.0563 : 10448
   16.4739, 49.4711, 52.6637 : 10448
   15.2441, 56.2196, 48.3763 : 10449
   15.9123, 54.1316, 48.0258 : 10449
   17.3475, 54.4616, 48.3485 : 10449
   15.3573, 43.8995, 47.8981 : 10450
   15.5129, 49.0171, 42.9764 : 10451
   16.1129, 48.3082, 43.9640 : 10451
   17.0414, 48.9724, 44.4231 : 10451
   ```
