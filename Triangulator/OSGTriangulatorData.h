#ifndef _Contrib_Triangulator_h_
#define _Contrib_Triangulator_h_

#include "OSGContribTriangulator.h"
#include "OSGMemoryObject.h"
#include "OSGVector.h"
#include <map>

struct triangulateio;

OSG_BEGIN_NAMESPACE

class OSG_CONTRIBTRIANGULATOR_DLLMAPPING TriangulatorData : public MemoryObject
{
    typedef std::map<Pnt2f, int> PointIndexMap;

 public:
    TriangulatorData(bool isInput=true);
    ~TriangulatorData(void);

    virtual void reset(void);

    // --- points ---
    void setPointNumber(unsigned int num);
    void setPoint(const Pnt2f& pnt, unsigned int idx);
    unsigned int getPointNumber(void) const;
    Pnt2f getPoint(unsigned int idx) const;

    // --- pointmarkers ---
    void setPointMarker(int marker, unsigned int idx);

    // --- holes ---
    void setHoleNumber(unsigned int num);
    void setHole(const Pnt2f& hole, unsigned int idx);

    // --- segments ---
    void setSegmentNumber(unsigned int num);
    void setSegment(int startIdx, int endIdx, unsigned int idx);
    int getSegment(int startIdx, int endIdx) const;

    // --- segmentmarkers ---
    void setSegmentMarker(int marker, unsigned int idx);
    int getSegmentMarker(unsigned int idx) const;
    int getSegmentMarker(int startIdx, int endIdx) const;

    // --- triangles ---
    unsigned int getTriangleNumber(void) const;
    unsigned int getCornerNumber(void) const;
    int getTrianglePointIndex(unsigned int triIdx, unsigned int cornIdx) const;
    Pnt2f getTrianglePoint(unsigned int triIdx, unsigned int cornIdx) const;

    // --- neighbours ---
    bool hasNeighbours(void) const;
    int getNeighbourIndex(unsigned int triIdx, unsigned int sideIdx) const;

   /* --- Avoiding of point duplicates ------------------------------------- *
    *  The follwoing methods ensure that data.pointlist does not contain any *
    *  point duplicates. Please keep in mind that the "no point duplicates"  *
    *  property can only be achieved by strictly using the additonal         *
    *  accessor methods following next.                                      *
    * ---------------------------------------------------------------------- */

    // guarantees, that exactly one instance of pnt is contained in pointlist
    bool addUniquePoint(const Pnt2f& pnt);

    unsigned int getUniquePointNumber(void) const;

    // should only be used once, after adding all points
    void shrinkToUniquePointNumber(void);

    bool isInitializedWithUniquePoints(void) const;
    void initializeWithUniquePoints(void) const;

    void setSegment(const Pnt2f& start, const Pnt2f& end, unsigned int idx);
    void setPointMarker(int marker, const Pnt2f& pnt);

    int getSegmentByPoints(const Pnt2f& start, const Pnt2f& end) const;
    int getSegmentMarkerByPoints(const Pnt2f& start, const Pnt2f& end) const;

    // *** some stuff for debugging *****************************************
    void report(bool showTriangles=true, bool showSegments=true,
                bool showEdges=true, bool showHoles=true) const;
    bool isWithinBound(Real32 ll, Real32 bb, Real32 rr, Real32 tt) const;

    struct triangulateio* getPtr();

 protected:
    void initialize();

 private:
    triangulateio* data;
    bool                  isInputFlag;
    mutable PointIndexMap point2index; // for 'no point duplicates' constraint

 public:
    static void tesselate(const std::string& options,
                          TriangulatorData& input,
                          TriangulatorData& output, 
                          TriangulatorData& voronoiOutput); 

 private:
    /* --- not implemented ----------------------------------------------- */
    TriangulatorData(const TriangulatorData& other);
    const TriangulatorData& operator=(const TriangulatorData& other);
};


OSG_END_NAMESPACE

#endif /* _Contrib_Triangulator_h_ */
