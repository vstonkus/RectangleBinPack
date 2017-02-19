/** @file Rect.cpp
	@author Jukka Jylänki

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>

//#include "Rect.h"
#using <mscorlib.dll>

namespace Types {

    using namespace System;

    public ref struct RectSize
        {
        int width;
        int height;
        };

    public ref struct Rect
        {
        int x;
        int y;
        int width;
        int height;
        };

    //Sita uzkomentavau, galima ideti i struct klase.
    /// Returns true if a is contained in b->
    //bool IsContainedIn(Rect^ a, Rect^ b)
    //{
    //	return a->x >= b->x && a->y >= b->y 
    //		&& a->x+a->width <= b->x+b->width 
    //		&& a->y+a->height <= b->y+b->height;
    //}

/*
#include "clb/Algorithm/Sort.h"

/// Performs a lexicographic compare on (rect short side, rect long side).
/// @return -1 if the smaller side of a is shorter than the smaller side of b, 1 if the other way around.
///   If they are equal, the larger side length is used as a tie-breaker->
///   If the rectangles are of same size, returns 0.
int CompareRectShortSide(const Rect &a, const Rect &b)
{
	using namespace std;

	int smallerSideA = min(a->width, a->height);
	int smallerSideB = min(b->width, b->height);

	if (smallerSideA != smallerSideB)
		return clb::sort::TriCmp(smallerSideA, smallerSideB);

	// Tie-break on the larger side.
	int largerSideA = max(a->width, a->height);
	int largerSideB = max(b->width, b->height);

	return clb::sort::TriCmp(largerSideA, largerSideB);
}
*/
/*
/// Performs a lexicographic compare on (x, y, width, height).
int NodeSortCmp(const Rect &a, const Rect &b)
{
	if (a->x != b->x)
		return clb::sort::TriCmp(a->x, b->x);
	if (a->y != b->y)
		return clb::sort::TriCmp(a->y, b->y);
	if (a->width != b->width)
		return clb::sort::TriCmp(a->width, b->width);
	return clb::sort::TriCmp(a->height, b->height);
}
*/

public ref class DisjointRectCollection
    {
    System::Collections::Generic::List<Rect^>^ rects;

    public: DisjointRectCollection ()
    {
        rects = gcnew System::Collections::Generic::List<Rect^>();
    }

    bool Add(Rect^ r)
        {
        // Degenerate rectangles are ignored.
        if (r->width == 0 || r->height == 0)
            return true;

        if (!Disjoint(r))
            return false;
        rects->Add(r);
        return true;
        }

    void Clear()
        {
        rects = gcnew System::Collections::Generic::List<Rect^>();
        }

    bool Disjoint(Rect^ r)
        {
        // Degenerate rectangles are ignored.
        if (r->width == 0 || r->height == 0)
            return true;

        for (size_t i = 0; i < rects->Count; ++i)
            if (!Disjoint(rects[i], r))
                return false;
        return true;
        }

    static bool Disjoint(Rect^ a, Rect^ b)
        {
        if (a->x + a->width <= b->x ||
            b->x + b->width <= a->x ||
            a->y + a->height <= b->y ||
            b->y + b->height <= a->y)
            return true;
        return false;
        }
    };

}