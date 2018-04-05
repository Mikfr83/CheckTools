#ifndef __UVSHELL__
#define __UVSHELL__

#include "uvEdge.h"
#include "uvPoint.h"
#include <set>
#include <unordered_set>
#include <vector>

struct hash_edge {
    size_t operator()(const UvEdge& edge) const
    {
        return std::hash<std::string>{}(edge.stringID);
    }
};

class UvShell {
public:
    UvShell();
    ~UvShell();

    int shellIndex;
    float uMax;
    float uMin;
    float vMax;
    float vMin;

    std::vector<UvPoint> uvPoints;
    std::vector<float> uVector;
    std::vector<float> vVector;
    std::unordered_set<int> polygonIDs;
    std::vector<int> borderUvPoints;
    std::unordered_set<UvEdge, hash_edge> unordered_edgeSet;

    bool operator==(const UvShell& rhs) const;
    inline bool operator!=(const UvShell& rhs) const
    {
        return !(*this == rhs);
    }

private:
};

#endif /* defined(__UVSHELL_H__) */
