#ifndef __Contrib_OSGTriangulator_h__
#define __Contrib_OSGTriangulator_h__

#include <OpenSG/OSGContribTriangulator.h>
#include <OpenSG/OSGMemoryObject.h>
#include <OpenSG/OSGVector.h>
#include <OpenSG/OSGTriangulatorIO.h>
#include <string>

/* ************************************************************************* *
 * ***                    How to use the Triangulator?                   *** *
 * ************************************************************************* *
 *                                                                           *
 *   Before triangulate() can be called, the Triangulator needs to be fed    *
 *   with some information. This is normally achieved by calling one of      *
 *   the reset(...) functions.                                               *
 *                                                                           *
 *   The typical usage scenario of the Triangulator looks like that:         *
 *                                                                           *
 *          Triangulator tri;                                                *
 *                                                                           *
 *          TriangulatorIO& input = tri.reset(...);                          *
 *          ... // add points to input here                                  *
 *          TriangultorIO& output = tri.triangulate()                        *
 *          ... // use output to whatever purpose you like to do             *
 *                                                                           *
 *   The most general reset() function is reset(void), which simply sets     *
 *   all fields of the input, output, and voronoiOutput to NULL/0.           *
 *                                                                           *
 * ************************************************************************* */


/* ************************************************************************* *
 * ***                   A note about point duplicates                   *** *
 * ************************************************************************* *
 *                                                                           *
 *   The 'Triangle' lib can cause segmentation faults when configured        *
 *   for tesselating a point cloud with point duplicates. In any             *
 *   circumstance, try NOT to use point duplicates in your input at all.     *
 *                                                                           *
 *   To make life easier, TrinangulatorIO provides some helper functions     *
 *   ensuring the 'no point duplicates' - constraint. Namely, these          *
 *   functions are:                                                          *
 *                                                                           *
 *         TriangulatorIO::addUniquePoint(const Pnt2f& pnt);                 *
 *                                                                           *
 * ************************************************************************* */

OSG_BEGIN_NAMESPACE

class OSG_CONTRIBTRIANGULATOR_DLLMAPPING Triangulator : public MemoryObject
{
 public:
    Triangulator(void);
    ~Triangulator(void);

    void setComputeNeighbours(bool flag);
    bool getComputeNeighbours(void) const ;

    void setComputeConvexHull(bool flag);
    bool getComputeConvexHull(void) const;

    void setSilent(bool flag); // prevent all output from 'Triangle' lib
    bool getSilent(void) const;

    const std::string& getOptionString(void) const;

    TriangulatorIO& triangulate(void);

    // --- general reset, sets all internal fields to NULL/0 --------
    TriangulatorIO& reset(void); // returns the input

    // --- example setups -------------------------------------------
    TriangulatorIO& resetWithDuplicates(void);    // might create a segfault!
    TriangulatorIO& resetWithoutDuplicates(void); // stable (no segfault)

 protected:
    void setOption(bool flag, const std::string& str);
    bool getOption(const std::string& str) const;

 protected:
    TriangulatorIO input;
    TriangulatorIO output;
    TriangulatorIO voronoiOutput; // not really used yet

 private:
    bool           isDoneFlag;
    std::string    optionString;  // configuring Triangle's triangulate(..)

 private:
    /* --- not implemented ----------------------------------------------- */
    Triangulator(const Triangulator& other);
    const Triangulator& operator=(const Triangulator& other);
};

OSG_END_NAMESPACE

#endif /* __Contrib_OSGTriangulator_h__ */
