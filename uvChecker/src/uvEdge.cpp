#include "uvEdge.h"
#include "uvUtils.h"
#include <float.h>
#include <iostream>
#include <math.h>
#include <maya/MGlobal.h>

UvEdge::UvEdge()
{
}

UvEdge::UvEdge(UvPoint beginPt, UvPoint endPt, unsigned int index)
{
    this->begin = beginPt;
    this->end = endPt;
    this->index = index;
    this->beginIndex = beginPt.index;
    this->endIndex = endPt.index;
}

UvEdge::~UvEdge()
{
}

bool UvEdge::operator==(const UvEdge& rhs) const
{
    return this->index == rhs.index;
}

bool UvEdge::operator>(const UvEdge& rhs) const
{
    return this->index > rhs.index;
}

bool UvEdge::operator>=(const UvEdge& rhs) const
{
    return this->index >= rhs.index;
}

bool UvEdge::operator<(const UvEdge& rhs) const
{
    return this->index < rhs.index;
}

bool UvEdge::operator<=(const UvEdge& rhs) const
{
    return this->index <= rhs.index;
}

bool UvEdge::isIntersected(UvEdge& otherEdge, bool& isParallel)
{

    // Check edge index if they have shared UV index
    bool isConnected;
    if (this->beginIndex == otherEdge.beginIndex || this->beginIndex == otherEdge.endIndex) {
        isConnected = true;
    } else if (this->endIndex == otherEdge.beginIndex || this->endIndex == otherEdge.endIndex) {
        isConnected = true;
    } else {
        isConnected = false;
    }

    float area1 = UvUtils::getTriangleArea(
        this->begin.u,
        this->begin.v,
        otherEdge.begin.u,
        otherEdge.begin.v,
        this->end.u,
        this->end.v);

    float area2 = UvUtils::getTriangleArea(
        this->begin.u,
        this->begin.v,
        otherEdge.end.u,
        otherEdge.end.v,
        this->end.u,
        this->end.v);

    if ((area1 == 0 && area2 == 0) && isConnected == true) {
        return false;
    }

    float area3 = UvUtils::getTriangleArea(
        otherEdge.begin.u,
        otherEdge.begin.v,
        this->begin.u,
        this->begin.v,
        otherEdge.end.u,
        otherEdge.end.v);

    float area4 = UvUtils::getTriangleArea(
        otherEdge.begin.u,
        otherEdge.begin.v,
        this->end.u,
        this->end.v,
        otherEdge.end.u,
        otherEdge.end.v);

    if (area1 == 0.0 && area2 == 0.0) {
        // if area of the two triangles are 0, two lines are parallel on a same
        // line
        float u_min;
        float u_max;
        float v_min;
        float v_max;

        // Get u_min and u_max
        if (otherEdge.begin.u > otherEdge.end.u) {
            u_max = otherEdge.begin.u;
            u_min = otherEdge.end.u;
        } else {
            u_max = otherEdge.end.u;
            u_min = otherEdge.begin.u;
        }

        // Get v_min and v_max
        if (otherEdge.begin.v < otherEdge.end.v) {
            v_min = otherEdge.begin.v;
            v_max = otherEdge.end.v;
        } else {
            v_min = otherEdge.end.v;
            v_max = otherEdge.begin.v;
        }

        // If two lines are in vertical line
        if (this->begin.u == this->end.u) {
            if (v_min < this->begin.v && this->begin.v < v_max) {
                isParallel = true;
                return true;
            } else if (v_min < this->end.v && this->end.v < v_max) {
                isParallel = true;
                return true;
            } else {
                return false;
            }
        }

        if (u_min < this->begin.u && this->begin.u < u_max) {
            // parallel edges overlaps
            return true;
        } else if (u_min < this->end.u && this->end.u < u_max) {
            // parallel edges overlaps
            return true;
        } else {
            return false;
        }
    }

    float ccw1;
    float ccw2;
    // If two edges are connected, at least 2 area of 4 triangles should be 0,
    // therefore, ccw1 and 2 need to be 0.
    if (isConnected) {
        ccw1 = 0;
        ccw2 = 0;
    } else {
        ccw1 = area1 * area2;
        ccw2 = area3 * area4;
    }

    if (ccw1 < 0 && ccw2 < 0) {
        return true;
    } else
        return false;
}

void UvEdge::setCrossingPointX(float Y)
{
    float& x1 = this->begin.u;
    float& y1 = this->begin.v;
    float& x2 = this->end.u;
    float& y2 = this->end.v;

    if (y2 == y1) {
        this->crossingPointX = this->begin.u;
    } else {
        float X = ((Y - y1) * (x2 - x1)) / (y2 - y1) + x1;
        this->crossingPointX = X;
    }
}

bool UvEdgeComparator::operator()(const UvEdge& rhs1, const UvEdge& rhs2) const
{
    if (rhs1.crossingPointX == rhs2.crossingPointX) {
        return rhs1.end.u < rhs2.end.u;

    } else {
        return rhs1.crossingPointX < rhs2.crossingPointX;
    }
}
