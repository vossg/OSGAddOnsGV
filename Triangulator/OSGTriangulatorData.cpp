/* ************************************************************************* *
 *                                                                           *
 *   This file (OSGTriangulatorData.cpp) is a wrapper for                    *
 *                                                                           *
 *       'Triangle': A Two-Dimensional Quality Mesh Generator and            *
 *                   Delaunay Triangulator.                                  *
 *                                                                           *
 *                   by Jonathan Richard Shewchuk                            *
 *                   (see triangle.c and triangle.h)                         *
 *                                                                           *
 *   This file contains the configuration needed for a proper compilation    *
 *   of 'Triangle' to work as a library (i.e. without a 'main()' function)   *
 *   as well as some specific code for OpenSG.                               *
 *                                                                           *
 *   Triangulator(Data) was written by Martin Spindler (2007-08-08) and      *
 *   does not belong to the 'Triangle' - package by J.R. Shewchuk.           *
 *                                                                           *
 * ************************************************************************* */


/* ------------------------------------------------------------------------- *
 *                     CONFIGURATION of 'Triangle'                           *
 *      (see triangle.c and triangle.h for a more detailed description)      *
 *                                                                           *
 *   ANSI_DECLARATORS reference:                                             *
 *      [1] http://www.thescripts.com/forum/thread566983.html                *
 * ------------------------------------------------------------------------- */

#define ANSI_DECLARATORS  // do NOT remove this (see [1])
#define TRILIBRARY        // needed to compile as a library, i.e. no  main()


/* The 'LINUX' and 'CPU86' options are used to improve floating-point        *
 * precision with gcc and MSVC compilers and might introduce some unexpected *
 * 'Floating point exceptions'. It's safest to leave them commented.         *
 * - martin 2007-08-13                                                       */
//#define LINUX
//#define CPU86

/* Some additional options to increase the performance of Triangle           */
#define SELF_CHECK         // comment that to increase performance
#define NO_TIMER           // timer only works with unix
// #define REDUCED
// #define CDT_ONLY

/* ------------------------------------------------------------------------- *
 * ---                     include of triangle.c                         --- *
 * ------------------------------------------------------------------------- */

/* ************************************************************************* *
 * ***                         ToNKy lib stuff                           *** *
 * ************************************************************************* */

#include <OSGTriangulatorData.h>

#include "triangle.c" // also includes "triangle.h"

OSG_USING_NAMESPACE

/* ------------------------------------------------------------------------- *
 * ---                       TriangulatorData                            --- *
 * ------------------------------------------------------------------------- */

TriangulatorData::TriangulatorData(bool isInput) :
    data(new triangulateio),
    isInputFlag(isInput),
    point2index()
{
    initialize();
}

TriangulatorData::~TriangulatorData(void)
{
    reset();
    delete this->data;
}

// --- points --------------------------------------------------
void TriangulatorData::setPointNumber(unsigned int num)
{
    assert(this->data->numberofpoints == 0);
    this->data->numberofpoints = num;
    this->data->pointlist      = (REAL *)malloc(num * 2 * sizeof(REAL));
}

void TriangulatorData::setPoint(const Pnt2f& pnt, unsigned int idx)
{
    assert(int(idx) < this->data->numberofpoints);
    idx += idx; // mimic: idx = 2*idx
    this->data->pointlist[idx  ] = REAL(pnt.x());
    this->data->pointlist[idx+1] = REAL(pnt.y());
}

unsigned int TriangulatorData::getPointNumber(void) const
{
    return this->data->numberofpoints;
}

Pnt2f TriangulatorData::getPoint(unsigned int idx) const
{
    assert(int(idx) < this->data->numberofpoints);
    idx += idx; // mimic: idx = 2*idx
    return Pnt2f(this->data->pointlist[idx], this->data->pointlist[idx+1]);
}

// --- pointmarkers --------------------------------------------

void TriangulatorData::setPointMarker(int marker, unsigned int idx)
{
    unsigned int num = this->data->numberofpoints;
    assert(num > 0 && 
           "numberofpoints is not initialized. Call setPointNumber() first!");
    assert(idx < num);

    if (this->data->pointmarkerlist == NULL) {
        this->data->pointmarkerlist = (int*)malloc(num * sizeof(int));
        for (unsigned int i=0; i<num; i++) this->data->pointmarkerlist[i] = 0;
    }
    this->data->pointmarkerlist[idx] = marker;
}


// --- segments ------------------------------------------------
void TriangulatorData::setSegmentNumber(unsigned int num)
{
    assert(this->data->numberofsegments == 0);
    this->data->numberofsegments = num;
    this->data->segmentlist      = (int *)malloc(num * 2 * sizeof(int));
}

void TriangulatorData::setSegment(int startIdx, int endIdx, unsigned int idx)
{
    assert(int(idx) < this->data->numberofsegments);
    idx += idx; // mimic: idx = 2*idx
    this->data->segmentlist[idx  ] = startIdx;
    this->data->segmentlist[idx+1] = endIdx;
}

int TriangulatorData::getSegment(int startIdx, int endIdx) const
{
    if (this->data->numberofsegments == 0) return -1; // segment not found

    int idx = 0;

    for (int i=0; i<this->data->numberofsegments; i++) {
        int firstIdx  = this->data->segmentlist[idx++];
        int secondIdx = this->data->segmentlist[idx++];
        
        if (firstIdx == startIdx && secondIdx == endIdx  ) return i;
        if (firstIdx == endIdx   && secondIdx == startIdx) return i;
    }
    return -1; // segment not found
}

// --- segmentmarkers --------------------------------------------

void TriangulatorData::setSegmentMarker(int marker, unsigned int idx)
{
    unsigned int num = this->data->numberofsegments;
    assert(num > 0 && 
           "numberofsegments is not initialized. "
           "Call setSegmentNumber() first!");
    assert(idx < num);

    if (this->data->segmentmarkerlist == NULL) {
        this->data->segmentmarkerlist = (int*)malloc(num * sizeof(int));
        for (unsigned int i=0; i<num; i++) this->data->segmentmarkerlist[i] = 1;
    }
    this->data->segmentmarkerlist[idx] = marker;
}

int TriangulatorData::getSegmentMarker(unsigned int idx) const
{
    assert(int(idx) < this->data->numberofsegments);
    return this->data->segmentmarkerlist[idx];
}

int TriangulatorData::getSegmentMarker(int startIdx, int endIdx) const
{
    int idx = getSegment(startIdx, endIdx);

    if (idx < 0) return -1; // not found
    return getSegmentMarker(idx);
}

// --- holes ---------------------------------------------------
void TriangulatorData::setHoleNumber(unsigned int num)
{
    assert(this->data->numberofholes == 0);
    this->data->numberofholes = num;
    this->data->holelist      = (REAL *)malloc(num * 2 * sizeof(REAL));
}

void TriangulatorData::setHole(const Pnt2f& hole, unsigned int idx)
{
    assert(int(idx) < this->data->numberofholes);
    idx += idx; // mimic: idx = 2*idx
    this->data->holelist[idx  ] = REAL(hole.x());
    this->data->holelist[idx+1] = REAL(hole.y());
}

// --- triangles -----------------------------------------------
unsigned int TriangulatorData::getTriangleNumber(void) const
{
    return this->data->numberoftriangles;
}

unsigned int TriangulatorData::getCornerNumber(void) const
{
    return this->data->numberofcorners;
}

int TriangulatorData::getTrianglePointIndex(unsigned int triIdx, 
                                            unsigned int cornIdx) const
{
    assert(int(triIdx)  < this->data->numberoftriangles);
    assert(int(cornIdx) < this->data->numberofcorners);
    int idx;

    idx = this->data->trianglelist[triIdx*this->data->numberofcorners+cornIdx];

    assert(idx >= 0);
    assert(idx < this->data->numberofpoints);

    return idx;
}

Pnt2f TriangulatorData::getTrianglePoint(unsigned int triIdx, 
                                         unsigned int cornIdx) const
{
    return getPoint(getTrianglePointIndex(triIdx, cornIdx));
}

// --- neighbours ----------------------------------------------

bool TriangulatorData::hasNeighbours(void) const
{
    return (this->data->neighborlist != NULL);
}

int TriangulatorData::getNeighbourIndex(unsigned int triIdx, 
                                        unsigned int sideIdx) const
{
    assert(this->data->neighborlist != NULL);
    assert(int(triIdx)  < this->data->numberoftriangles);
    assert(sideIdx < 3);

    int idx = this->data->neighborlist[triIdx * 3 + sideIdx];

    assert(idx >= -1);
    assert(idx < this->data->numberoftriangles);
    return idx;
}

// -------------------------------------------------------------

struct triangulateio* TriangulatorData::getPtr()
{
    return this->data;
}

void TriangulatorData::initialize()
{
    this->data->pointlist             = NULL;
    this->data->pointattributelist    = NULL;
    this->data->pointmarkerlist       = NULL;
    this->data->trianglelist          = NULL;
    this->data->triangleattributelist = NULL;
    this->data->trianglearealist      = NULL;
    this->data->neighborlist          = NULL;
    this->data->segmentlist           = NULL;
    this->data->segmentmarkerlist     = NULL;
    this->data->holelist              = NULL;
    this->data->regionlist            = NULL;
    this->data->edgelist              = NULL;
    this->data->edgemarkerlist        = NULL;
    this->data->normlist              = NULL;

    this->data->numberofpoints             = 0;
    this->data->numberofpointattributes    = 0;
    this->data->numberoftriangles          = 0;
    this->data->numberofcorners            = 0;
    this->data->numberoftriangleattributes = 0;
    this->data->numberofsegments           = 0;
    this->data->numberofholes              = 0;
    this->data->numberofregions            = 0;
    this->data->numberofedges              = 0;

    this->point2index.clear();
}

void TriangulatorData::reset(void)
{
    free(this->data->pointlist);
    free(this->data->pointattributelist);
    free(this->data->pointmarkerlist);
    free(this->data->trianglelist);
    free(this->data->triangleattributelist);
    free(this->data->trianglearealist);
    free(this->data->neighborlist);
    free(this->data->segmentlist);
    free(this->data->segmentmarkerlist);
    if (this->isInputFlag) {
        free(this->data->holelist);
        free(this->data->regionlist);
    }
    free(this->data->edgelist);
    free(this->data->edgemarkerlist);
    free(this->data->normlist);

    initialize();
}

// --- No point duplicates ---------------------------------------------------

// returns false, if pnt could not be added and pnt not already present
bool TriangulatorData::addUniquePoint(const Pnt2f& pnt)
{
    if (this->point2index.count(pnt) > 0) return true; // pnt was already added

    int idx = this->point2index.size();

    if (idx < int(getPointNumber())) {
        setPoint(pnt, idx);
        this->point2index[pnt] = idx;
    }
    else
        return false;

    return true;
}

unsigned int TriangulatorData::getUniquePointNumber(void) const
{
    return this->point2index.size();
}

void TriangulatorData::shrinkToUniquePointNumber(void)
{
    unsigned int oldPointNum = getPointNumber();
    unsigned int newPointNum = this->point2index.size();
    if (newPointNum == oldPointNum) return;

    assert(newPointNum > 0);
    assert(newPointNum < oldPointNum);
    this->data->numberofpoints = newPointNum;
}

bool TriangulatorData::isInitializedWithUniquePoints(void) const
{
    if (this->data->numberofpoints == 0) return true;

    if (this->point2index.size() == 0) initializeWithUniquePoints();

    return (Int32(this->point2index.size()) == this->data->numberofpoints);
}

void TriangulatorData::initializeWithUniquePoints(void) const
{
    this->point2index.clear();

    for (int i=0; i<this->data->numberofpoints; i++) {
        Pnt2f pnt = getPoint(i);
        if (this->point2index.count(pnt) == 0)
            this->point2index[pnt] = i;
    }
}

void TriangulatorData::setPointMarker(int marker, const Pnt2f& pnt)
{
    assert(this->point2index.count(pnt) == 1);
    setPointMarker(marker, this->point2index[pnt]);
}

void TriangulatorData::setSegment(const Pnt2f& start, const Pnt2f& end,
                                  unsigned int idx)
{
    assert(int(idx) < this->data->numberofsegments);
    idx += idx; // mimic: idx = 2*idx

    assert(this->point2index.count(start) == 1);
    assert(this->point2index.count(end)   == 1);

    this->data->segmentlist[idx  ] = this->point2index[start];
    this->data->segmentlist[idx+1] = this->point2index[end];
}

int TriangulatorData::getSegmentByPoints(const Pnt2f& start,
                                         const Pnt2f& end) const
{
    int startCnt = this->point2index.count(start);
    int endCnt   = this->point2index.count(end);

    if (startCnt == 0) return -1;
    if (endCnt   == 0) return -1;

    assert(startCnt == 1 && "'no point duplicates' constraint does not hold!");
    assert(endCnt   == 1 && "'no point duplicates' constraint does not hold!");

    return getSegment(this->point2index[start], this->point2index[end]);
}

int TriangulatorData::getSegmentMarkerByPoints(const Pnt2f& start,
                                               const Pnt2f& end) const
{
    int idx = getSegmentByPoints(start, end);

    if (idx < 0) return -1; // not found
    return getSegmentMarker(idx);
}

/* --- just for testing -------------------------------------------------- */

void TriangulatorData::report(bool showTriangles,
                              bool showSegments,
                              bool showEdges,
                              bool showHoles) const
{
    int numPointAttr = this->data->numberofpointattributes;
    int numCorners   = this->data->numberofcorners;
    int numTriAttr   = this->data->numberoftriangleattributes;
    int i, j;

    std::cerr << "---------- start report ----------------------" << std::endl;
    for (i = 0; i < this->data->numberofpoints; i++) {
        printf("Point %4d:", i);
        for (j = 0; j < 2; j++) {
            printf("  %.6g", this->data->pointlist[i * 2 + j]);
        }
        if (numPointAttr > 0) {
            printf("   attributes");
        }
        for (j = 0; j < numPointAttr; j++) {
            printf("  %.6g", this->data->pointattributelist[i*numPointAttr+j]);
        }
        if (this->data->pointmarkerlist != NULL) {
            printf("   marker %d\n", this->data->pointmarkerlist[i]);
        } else {
            printf("\n");
        }
    }
    printf("\n");

    if (showTriangles) {
        for (i = 0; i < this->data->numberoftriangles; i++) {
            printf("Triangle %4d points:", i);
            for (j = 0; j < numCorners; j++) {
                printf("  %4d", this->data->trianglelist[i*numCorners+j]);
            }
            if (numTriAttr > 0) {
                printf("   attributes");
            }
            for (j = 0; j < numTriAttr; j++) {
                printf("  %.6g", 
                       this->data->triangleattributelist[i*numTriAttr+j]);
            }
            if (this->data->neighborlist) {
                printf("      neighbors:");
                for (j = 0; j < 3; j++) {
                    printf("  %4d", this->data->neighborlist[i * 3 + j]);
                }
            }
            printf("\n");
        }
        printf("\n");
    }

    if (showSegments) {
        for (i = 0; i < this->data->numberofsegments; i++) {
            printf("Segment %4d points:", i);
            for (j = 0; j < 2; j++) {
                printf("  %4d", this->data->segmentlist[i * 2 + j]);
            }
            if (this->data->segmentmarkerlist != NULL) {
                printf("   marker %d\n", this->data->segmentmarkerlist[i]);
            } else {
                printf("\n");
            }
        }
        printf("\n");
    }

    if (showEdges) {
        for (i = 0; i < this->data->numberofedges; i++) {
            printf("Edge %4d points:", i);
            for (j = 0; j < 2; j++) {
                printf("  %4d", this->data->edgelist[i * 2 + j]);
            }
            if (this->data->normlist && (this->data->edgelist[i*2+1] == -1)) {
                for (j = 0; j < 2; j++) {
                    printf("  %.6g", this->data->normlist[i * 2 + j]);
                }
            }
            if (this->data->edgemarkerlist != NULL) {
                printf("   marker %d\n", this->data->edgemarkerlist[i]);
            } else {
                printf("\n");
            }
        }
        printf("\n");
    }
    if (showHoles) {
        for (i=0; i<this->data->numberofholes; i++) {
            std::cerr << "Hole    " << i << " point:    "
                      << this->data->holelist[2*i]   << "  "
                      << this->data->holelist[2*i+1] << std::endl;
        }
        std::cerr << std::endl;
    }
    std::cerr << "---------- end report ------------------------" << std::endl;
}

bool TriangulatorData::
isWithinBound(Real32 ll, Real32 bb, Real32 rr, Real32 tt) const
{
    bool isOK = true;
    for (int i=0; i<this->data->numberofpoints; i++) {
        Real32 xx = this->data->pointlist[i*2];
        Real32 yy = this->data->pointlist[i*2+1];
        if ( xx < ll || xx > rr || yy < bb || yy > tt) {
            if (isOK) {
                std::cerr << "TriangulatorData: not within bound ("
                          << ll << "," << bb << ") - ("
                          << rr << "," << tt << "):" 
                          << std::endl;
            }
            std::cerr << "pointlist[" << i << "] = (" << xx << ","
                      << yy << ")" << std::endl;
            isOK = false;
        }
    }
    return isOK;
}



void TriangulatorData::tesselate(const std::string& options,
                                 TriangulatorData& input,
                                 TriangulatorData& output, 
                                 TriangulatorData& vorOutput)
{
    triangulate((char*)options.c_str(),
                (struct triangulateio*)input.getPtr(),
                (struct triangulateio*)output.getPtr(),
                (struct triangulateio*)vorOutput.getPtr());
}
