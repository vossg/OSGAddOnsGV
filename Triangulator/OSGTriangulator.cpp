/* ************************************************************************* *
 *                                                                           *
 *   This file (OSGTriangulator.cpp) is a wrapper for                        *
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
 *   The Triangulator and TriangulatorData classes were written by           *
 *   Martin Spindler (2007-08-08) and do not belong to the                   *
 *   'Triangle' - package by J.R. Shewchuk.                                  *
 *                                                                           *
 * ************************************************************************* */

#include <OSGTriangulator.h>

OSG_USING_NAMESPACE

Triangulator::Triangulator(void) :
    input(true),          // isInput=true
    output(false),        // isInput=false
    voronoiOutput(false), // isInput=false
    isDoneFlag(false),
    optionString("pzeq")
{
    setComputeNeighbours(false);
    setComputeConvexHull(false);
    setSilent(false);
}

Triangulator::~Triangulator(void)
{
}

TriangulatorData& Triangulator::reset(void)
{
    this->input.reset();
    this->output.reset();
    this->voronoiOutput.reset();
    this->isDoneFlag = false;
    return this->input;
}

TriangulatorData& Triangulator::triangulate(void)
{
    if (this->isDoneFlag) return this->output;

    TriangulatorData::tesselate(this->optionString,
                                this->input, 
                                this->output,
                                this->voronoiOutput);

    this->isDoneFlag = true;
    return this->output;
}

void Triangulator::setComputeNeighbours(bool flag)
{
    setOption(flag, "n");
}

bool Triangulator::getComputeNeighbours(void) const
{
    return getOption("n");
}

void Triangulator::setComputeConvexHull(bool flag)
{
    setOption(flag, "c");
}

bool Triangulator::getComputeConvexHull(void) const
{
    return getOption("c");
}

void Triangulator::setSilent(bool flag)
{
    setOption(flag, "Q");
}

bool Triangulator::getSilent(void) const
{
    return getOption("Q");
}

const std::string&  Triangulator::getOptionString(void) const
{
    return this->optionString;
}

/* ************************************************************************* *
 * ***                        protected methods                          *** *
 * ************************************************************************* */

void Triangulator::setOption(bool flag, const std::string& str)
{
    std::string::size_type pos = this->optionString.find(str, 0);

    if (pos == std::string::npos) { // does NOT contain str
        if (flag) this->optionString += str;
    }
    else { // does contain str
        if (!flag) this->optionString.erase(pos, str.length());
    }
}

bool Triangulator::getOption(const std::string& str) const
{
    std::string::size_type pos = this->optionString.find(str, 0);

    return (pos != std::string::npos);
}

/* ************************************************************************* *
 * ***                        some setups for testing                    *** *
 * ************************************************************************* */

// point duplicates might introduce a segmentation fault!!!
TriangulatorData& Triangulator::resetWithDuplicates(void)
{
    reset();

    this->input.setPointNumber(7);
    this->input.setPoint(Pnt2f(0.2f, 0.6f), 0);
    this->input.setPoint(Pnt2f(0.2f, 0.8f), 1);
    this->input.setPoint(Pnt2f(0.4f, 0.8f), 2);
    this->input.setPoint(Pnt2f(0.4f, 0.6f), 3);
    this->input.setPoint(Pnt2f(0.2f, 0.6f), 4); // point duplicate
    this->input.setPoint(Pnt2f(0.3f, 0.8f), 5);
    this->input.setPoint(Pnt2f(0.4f, 0.6f), 6); // point duplicate

    this->input.setSegmentNumber(7);
    this->input.setSegment(0, 1,  0);
    this->input.setSegment(1, 2,  1);
    this->input.setSegment(2, 3,  2);
    this->input.setSegment(3, 0,  3);
    this->input.setSegment(4, 5,  4);
    this->input.setSegment(5, 6,  5);
    this->input.setSegment(6, 4,  6);

    this->input.setHoleNumber(1);
    this->input.setHole(Pnt2f(0.3f, 0.666666667f), 0);

    return input;
}


// no point duplicates, i.e. no segmentation fault
TriangulatorData& Triangulator::resetWithoutDuplicates(void)
{
    reset();

    this->input.setPointNumber(5);
    this->input.setPoint(Pnt2f(0.2f, 0.6f), 0);
    this->input.setPoint(Pnt2f(0.2f, 0.8f), 1);
    this->input.setPoint(Pnt2f(0.4f, 0.8f), 2);
    this->input.setPoint(Pnt2f(0.4f, 0.6f), 3);
    this->input.setPoint(Pnt2f(0.3f, 0.8f), 4);

    this->input.setSegmentNumber(7);
    this->input.setSegment(0, 1,  0);
    this->input.setSegment(1, 2,  1);
    this->input.setSegment(2, 3,  2);
    this->input.setSegment(3, 0,  3);
    this->input.setSegment(0, 4,  4);
    this->input.setSegment(4, 3,  5);
    this->input.setSegment(3, 0,  6);

    this->input.setHoleNumber(1);
    this->input.setHole(Pnt2f(0.3f, 0.666666667f), 0);

    return input;
}
