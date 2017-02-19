/** @file GuillotineBinPack.cpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the GUILLOTINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>
#include <iostream>
#include <limits>

#include <cassert>
#include <cstring>
#include <cmath>

//#include "GuillotineBinPack.h"
#include "minmax.h"
#using <mscorlib.dll>
//#using <Rect.cpp>
#include "Rect.cpp"

#ifdef _DEBUG
/// debug_assert is an assert that also requires debug mode to be defined.
#define debug_assert(x) assert(x)
#else
#define debug_assert(x)
#endif

namespace rbp {

using namespace System::Collections::Generic;
using namespace Types;

/// Specifies the different choice heuristics that can be used when deciding which of the free subrectangles
/// to place the to-be-packed rectangle into.
public enum class FreeRectChoiceHeuristic
    {
    RectBestAreaFit, ///< -BAF
    RectBestShortSideFit, ///< -BSSF
    RectBestLongSideFit, ///< -BLSF
    RectWorstAreaFit, ///< -WAF
    RectWorstShortSideFit, ///< -WSSF
    RectWorstLongSideFit ///< -WLSF
    };

/// Specifies the different choice heuristics that can be used when the packer needs to decide whether to
/// subdivide the remaining free space in horizontal or vertical direction.
public enum class GuillotineSplitHeuristic
    {
    SplitShorterLeftoverAxis, ///< -SLAS
    SplitLongerLeftoverAxis, ///< -LLAS
    SplitMinimizeArea, ///< -MINAS, Try to make a single big rectangle at the expense of making the other small.
    SplitMaximizeArea, ///< -MAXAS, Try to make both remaining rectangles as even-sized as possible.
    SplitShorterAxis, ///< -SAS
    SplitLongerAxis ///< -LAS
    };

/** GuillotineBinPack implements different variants of bin packer algorithms that use the GUILLOTINE data structure
to keep track of the free space of the bin where rectangles may be placed. */
public ref class GuillotineBinPack
    {
    int binWidth;
    int binHeight;

    /// Stores a list of all the rectangles that we have packed so far-> This is used only to compute the Occupancy ratio,
    /// so if you want to have the packer consume less memory, this can be removed.
    List<Rect^>^ usedRectangles;

    /// Stores a list of rectangles that represents the free area of the bin. This rectangles in this list are disjoint.
    List<Rect^>^ freeRectangles;

#ifdef _DEBUG
    /// Used to track that the packer produces proper packings.
    DisjointRectCollection^ disjointRects;
#endif


    /// Returns the internal list of disjoint rectangles that track the free area of the bin. You may alter this vector
    /// any way desired, as long as the end result still is a list of disjoint rectangles.
    List<Rect^>^ GetFreeRectangles() { return freeRectangles; }

    /// Returns the list of packed rectangles. You may alter this vector at will, for example, you can move a Rect^ from
    /// this list to the Free Rectangles list to free up space on-the-fly, but notice that this causes fragmentation.
    List<Rect^>^ GetUsedRectangles() { return usedRectangles; }

    /// The initial bin size will be (0,0). Call Init to set the bin size.
public: GuillotineBinPack()
        {
        Init(0, 0);
        }

    /// Initializes a new bin of the given size.
public: GuillotineBinPack(int width, int height)
        {
        Init(width, height);
        }

    /// (Re)initializes the packer to an empty bin of width x height units. Call whenever
    /// you need to restart with a new bin.
    void GuillotineBinPack::Init(int width, int height)
        {
        binWidth = width;
        binHeight = height;

#ifdef _DEBUG
        disjointRects = gcnew DisjointRectCollection();
#endif

        // Clear any memory of previously packed rectangles.
        usedRectangles = gcnew List<Rect^>();

        // We start with a single big free rectangle that spans the whole bin.
        Rect^ n = gcnew Rect();
        n->x = 0;
        n->y = 0;
        n->width = width;
        n->height = height;

        freeRectangles = gcnew List<Rect^>();
        freeRectangles->Add(n);
        }
    
    /// Inserts a list of rectangles into the bin.
    /// @param rects The list of rectangles to add. This list will be destroyed in the packing process.
    /// @param merge If true, performs Rectangle Merge operations during the packing process.
    /// @param rectChoice The free rectangle choice heuristic rule to use.
    /// @param splitMethod The free rectangle split heuristic rule to use.
    void GuillotineBinPack::Insert(List<RectSize^>^ rects, bool merge, FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod)
        {
        // Remember variables about the best packing choice we have made so far during the iteration process.
        int bestFreeRect = 0;
        int bestRect = 0;
        bool bestFlipped = false;

        // Pack rectangles one at a time until we have cleared the rects array of all rectangles.
        // rects will get destroyed in the process.
        while (rects->Count > 0)
            {
            // Stores the penalty score of the best rectangle placement - bigger=worse, smaller=better->
            int bestScore = INT_MAX;

            for (size_t i = 0; i < freeRectangles->Count; ++i)
                {
                for (size_t j = 0; j < rects->Count; ++j)
                    {
                    // If this rectangle is a perfect match, we pick it instantly.
                    if (rects[j]->width == freeRectangles[i]->width && rects[j]->height == freeRectangles[i]->height)
                        {
                        bestFreeRect = i;
                        bestRect = j;
                        bestFlipped = false;
                        bestScore = INT_MIN;
                        i = freeRectangles->Count; // Force a jump out of the outer loop as well - we got an instant fit.
                        break;
                        }
                    // If flipping this rectangle is a perfect match, pick that then.
                    else if (rects[j]->height == freeRectangles[i]->width && rects[j]->width == freeRectangles[i]->height)
                        {
                        bestFreeRect = i;
                        bestRect = j;
                        bestFlipped = true;
                        bestScore = INT_MIN;
                        i = freeRectangles->Count; // Force a jump out of the outer loop as well - we got an instant fit.
                        break;
                        }
                    // Try if we can fit the rectangle upright->
                    else if (rects[j]->width <= freeRectangles[i]->width && rects[j]->height <= freeRectangles[i]->height)
                        {
                        int score = ScoreByHeuristic(rects[j]->width, rects[j]->height, freeRectangles[i], rectChoice);
                        if (score < bestScore)
                            {
                            bestFreeRect = i;
                            bestRect = j;
                            bestFlipped = false;
                            bestScore = score;
                            }
                        }
                    // If not, then perhaps flipping sideways will make it fit?
                    else if (rects[j]->height <= freeRectangles[i]->width && rects[j]->width <= freeRectangles[i]->height)
                        {
                        int score = ScoreByHeuristic(rects[j]->height, rects[j]->width, freeRectangles[i], rectChoice);
                        if (score < bestScore)
                            {
                            bestFreeRect = i;
                            bestRect = j;
                            bestFlipped = true;
                            bestScore = score;
                            }
                        }
                    }
                }

            // If we didn't manage to find any rectangle to pack, abort.
            if (bestScore == INT_MAX)
                return;

            // Otherwise, we're good to go and do the actual packing.
            Rect^ newNode = gcnew Rect();
            newNode->x = freeRectangles[bestFreeRect]->x;
            newNode->y = freeRectangles[bestFreeRect]->y;
            newNode->width = rects[bestRect]->width;
            newNode->height = rects[bestRect]->height;


            if (bestFlipped)
                {
                newNode->width = newNode->width + newNode->height;
                newNode->height = newNode->width - newNode->height;
                newNode->width = newNode->width - newNode->height;
                }

            // Remove the free space we lost in the bin.
            SplitFreeRectByHeuristic(freeRectangles[bestFreeRect], newNode, splitMethod);
            freeRectangles->RemoveAt(bestFreeRect);

            // Remove the rectangle we just packed from the input list.
            rects->RemoveAt(bestRect);

            // Perform a Rectangle Merge step if desired.
            if (merge)
                MergeFreeList();

            // Remember the new used rectangle.
            usedRectangles->Add(newNode);

            // Check that we're really producing correct packings here.
            debug_assert(disjointRects->Add(newNode) == true);
            }
        }

    /// @return True if r fits inside freeRect (possibly rotated).
    bool Fits(RectSize^ r, Rect^ freeRect)
        {
        return (r->width <= freeRect->width && r->height <= freeRect->height) ||
            (r->height <= freeRect->width && r->width <= freeRect->height);
        }

    /// @return True if r fits perfectly inside freeRect, i.e. the leftover area is 0.
    bool FitsPerfectly(RectSize^ r, Rect^ freeRect)
        {
        return (r->width == freeRect->width && r->height == freeRect->height) ||
            (r->height == freeRect->width && r->width == freeRect->height);
        }

    /*
    // A helper function for GUILLOTINE-MAXFITTING. Counts how many rectangles fit into the given rectangle
    // after it has been split.
    void CountNumFitting(const Rect^ &freeRect, int width, int height, const std::vector<RectSize> &rects,
        int usedRectIndex, bool splitHorizontal, int &score1, int &score2)
    {
        const int w = freeRect->width - width;
        const int h = freeRect->height - height;

        Rect^ bottom;
        bottom->x = freeRect->x;
        bottom->y = freeRect->y + height;
        bottom->height = h;

        Rect^ right;
        right->x = freeRect->x + width;
        right->y = freeRect->y;
        right->width = w;

        if (splitHorizontal)
        {
            bottom->width = freeRect->width;
            right->height = height;
        }
        else // Split vertically
        {
            bottom->width = width;
            right->height = freeRect->height;
        }

        int fitBottom = 0;
        int fitRight = 0;
        for(size_t i = 0; i < rects->Count; ++i)
            if (i != usedRectIndex)
            {
                if (FitsPerfectly(rects[i], bottom))
                    fitBottom |= 0x10000000;
                if (FitsPerfectly(rects[i], right))
                    fitRight |= 0x10000000;

                if (Fits(rects[i], bottom))
                    ++fitBottom;
                if (Fits(rects[i], right))
                    ++fitRight;
            }

        score1 = min(fitBottom, fitRight);
        score2 = max(fitBottom, fitRight);
    }
    */
    /*
    // Implements GUILLOTINE-MAXFITTING, an experimental heuristic that's really cool but didn't quite work in practice.
    void GuillotineBinPack::InsertMaxFitting(std::vector<RectSize> &rects, std::vector<Rect> &dst, bool merge,
        FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod)
    {
        dst.clear();
        int bestRect = 0;
        bool bestFlipped = false;
        bool bestSplitHorizontal = false;

        // Pick rectangles one at a time and pack the one that leaves the most choices still open.
        while(rects->Count > 0 && freeRectangles->Count > 0)
        {
            int bestScore1 = -1;
            int bestScore2 = -1;

            ///\todo Different sort predicates.
            clb::sort::QuickSort(&freeRectangles[0], freeRectangles->Count, CompareRectShortSide);

            Rect^ &freeRect = freeRectangles[0];

            for(size_t j = 0; j < rects->Count; ++j)
            {
                int score1;
                int score2;

                if (rects[j]->width == freeRect->width && rects[j]->height == freeRect->height)
                {
                    bestRect = j;
                    bestFlipped = false;
                    bestScore1 = bestScore2 = INT_MAX;
                    break;
                }
                else if (rects[j]->width <= freeRect->width && rects[j]->height <= freeRect->height)
                {
                    CountNumFitting(freeRect, rects[j]->width, rects[j]->height, rects, j, false, score1, score2);

                    if (score1 > bestScore1 || (score1 == bestScore1 && score2 > bestScore2))
                    {
                        bestRect = j;
                        bestScore1 = score1;
                        bestScore2 = score2;
                        bestFlipped = false;
                        bestSplitHorizontal = false;
                    }

                    CountNumFitting(freeRect, rects[j]->width, rects[j]->height, rects, j, true, score1, score2);

                    if (score1 > bestScore1 || (score1 == bestScore1 && score2 > bestScore2))
                    {
                        bestRect = j;
                        bestScore1 = score1;
                        bestScore2 = score2;
                        bestFlipped = false;
                        bestSplitHorizontal = true;
                    }
                }

                if (rects[j]->height == freeRect->width && rects[j]->width == freeRect->height)
                {
                    bestRect = j;
                    bestFlipped = true;
                    bestScore1 = bestScore2 = INT_MAX;
                    break;
                }
                else if (rects[j]->height <= freeRect->width && rects[j]->width <= freeRect->height)
                {
                    CountNumFitting(freeRect, rects[j]->height, rects[j]->width, rects, j, false, score1, score2);

                    if (score1 > bestScore1 || (score1 == bestScore1 && score2 > bestScore2))
                    {
                        bestRect = j;
                        bestScore1 = score1;
                        bestScore2 = score2;
                        bestFlipped = true;
                        bestSplitHorizontal = false;
                    }

                    CountNumFitting(freeRect, rects[j]->height, rects[j]->width, rects, j, true, score1, score2);

                    if (score1 > bestScore1 || (score1 == bestScore1 && score2 > bestScore2))
                    {
                        bestRect = j;
                        bestScore1 = score1;
                        bestScore2 = score2;
                        bestFlipped = true;
                        bestSplitHorizontal = true;
                    }
                }
            }

            if (bestScore1 >= 0)
            {
                Rect^ newNode;
                newNode->x = freeRect->x;
                newNode->y = freeRect->y;
                newNode->width = rects[bestRect]->width;
                newNode->height = rects[bestRect]->height;
                if (bestFlipped)
                    std::swap(newNode->width, newNode->height);

                assert(disjointRects.Disjoint(newNode));
                SplitFreeRectAlongAxis(freeRect, newNode, bestSplitHorizontal);

                rects->RemoveAt(rects.begin() + bestRect);

                if (merge)
                    MergeFreeList();

                usedRectangles->Add(newNode);
    #ifdef _DEBUG
                disjointRects.Add(newNode);
    #endif
            }

            freeRectangles->RemoveAt(freeRectangles.begin());
        }
    }
    */

    /// Inserts a single rectangle into the bin. The packer might rotate the rectangle, in which case the returned
    /// struct will have the width and height values swapped.
    /// @param merge If true, performs free Rectangle Merge procedure after packing the new rectangle. This procedure
    ///		tries to defragment the list of disjoint free rectangles to improve packing performance, but also takes up 
    ///		some extra time.
    /// @param rectChoice The free rectangle choice heuristic rule to use.
    /// @param splitMethod The free rectangle split heuristic rule to use.
    Rect^ GuillotineBinPack::Insert(int width, int height, bool merge, FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod)
        {
        // Find where to put the new rectangle.
        int freeNodeIndex = 0;
        Rect^ newRect = FindPositionForNewNode(width, height, rectChoice, &freeNodeIndex);

        // Abort if we didn't have enough space in the bin.
        if (newRect->height == 0)
            return newRect;

        // Remove the space that was just consumed by the new rectangle.
        SplitFreeRectByHeuristic(freeRectangles[freeNodeIndex], newRect, splitMethod);
        freeRectangles->RemoveAt(freeNodeIndex);

        // Perform a Rectangle Merge step if desired.
        if (merge)
            MergeFreeList();

        // Remember the new used rectangle.
        usedRectangles->Add(newRect);

        // Check that we're really producing correct packings here.
        debug_assert(disjointRects->Add(newRect) == true);

        return newRect;
        }

    /// Computes the ratio of used/total surface area. 0.00 means no space is yet used, 1.00 means the whole bin is used.
    float GuillotineBinPack::Occupancy()
        {
        ///\todo The occupancy rate could be cached/tracked incrementally instead
        ///      of looping through the list of packed rectangles here.
        unsigned long usedSurfaceArea = 0;
        for (size_t i = 0; i < usedRectangles->Count; ++i)
            usedSurfaceArea += usedRectangles[i]->width * usedRectangles[i]->height;

        return (float)usedSurfaceArea / (binWidth * binHeight);
        }

    /// Returns the heuristic score value for placing a rectangle of size width*height into freeRect-> Does not try to rotate.
    // The following functions compute (penalty) score values if a Rect^ of the given size was placed into the 
    // given free rectangle. In these score values, smaller is better->
    int GuillotineBinPack::ScoreByHeuristic(int width, int height, Rect^ freeRect, FreeRectChoiceHeuristic rectChoice)
        {
        switch (rectChoice)
            {
            case FreeRectChoiceHeuristic::RectBestAreaFit: return ScoreBestAreaFit(width, height, freeRect);
            case FreeRectChoiceHeuristic::RectBestShortSideFit: return ScoreBestShortSideFit(width, height, freeRect);
            case FreeRectChoiceHeuristic::RectBestLongSideFit: return ScoreBestLongSideFit(width, height, freeRect);
            case FreeRectChoiceHeuristic::RectWorstAreaFit: return ScoreWorstAreaFit(width, height, freeRect);
            case FreeRectChoiceHeuristic::RectWorstShortSideFit: return ScoreWorstShortSideFit(width, height, freeRect);
            case FreeRectChoiceHeuristic::RectWorstLongSideFit: return ScoreWorstLongSideFit(width, height, freeRect);
            default: assert(false); return INT_MAX;
            }
        }

    int GuillotineBinPack::ScoreBestAreaFit(int width, int height, Rect^ freeRect)
        {
        return freeRect->width * freeRect->height - width * height;
        }

    int GuillotineBinPack::ScoreBestShortSideFit(int width, int height, Rect^ freeRect)
        {
        int leftoverHoriz = abs(freeRect->width - width);
        int leftoverVert = abs(freeRect->height - height);
        int leftover = min(leftoverHoriz, leftoverVert);
        return leftover;
        }

    int GuillotineBinPack::ScoreBestLongSideFit(int width, int height, Rect^ freeRect)
        {
        int leftoverHoriz = abs(freeRect->width - width);
        int leftoverVert = abs(freeRect->height - height);
        int leftover = max(leftoverHoriz, leftoverVert);
        return leftover;
        }

    int GuillotineBinPack::ScoreWorstAreaFit(int width, int height, Rect^ freeRect)
        {
        return -ScoreBestAreaFit(width, height, freeRect);
        }

    int GuillotineBinPack::ScoreWorstShortSideFit(int width, int height, Rect^ freeRect)
        {
        return -ScoreBestShortSideFit(width, height, freeRect);
        }

    int GuillotineBinPack::ScoreWorstLongSideFit(int width, int height, Rect^ freeRect)
        {
        return -ScoreBestLongSideFit(width, height, freeRect);
        }

    /// Goes through the list of free rectangles and finds the best one to place a rectangle of given size into.
    /// Running time is Theta(|freeRectangles|).
    /// @param nodeIndex [out] The index of the free rectangle in the freeRectangles array into which the new
    ///		rect was placed.
    /// @return A Rect^ structure that represents the placement of the new Rect^ into the best free rectangle.
    Rect^ GuillotineBinPack::FindPositionForNewNode(int width, int height, FreeRectChoiceHeuristic rectChoice, int *nodeIndex)
        {
        Rect^ bestNode = gcnew Rect();

        int bestScore = INT_MAX;

        /// Try each free rectangle to find the best one for placement.
        for (size_t i = 0; i < freeRectangles->Count; ++i)
            {
            // If this is a perfect fit upright, choose it immediately.
            if (width == freeRectangles[i]->width && height == freeRectangles[i]->height)
                {
                bestNode->x = freeRectangles[i]->x;
                bestNode->y = freeRectangles[i]->y;
                bestNode->width = width;
                bestNode->height = height;
                bestScore = INT_MIN;
                *nodeIndex = i;
                debug_assert(disjointRects->Disjoint(bestNode));
                break;
                }
            // If this is a perfect fit sideways, choose it.
            else if (height == freeRectangles[i]->width && width == freeRectangles[i]->height)
                {
                bestNode->x = freeRectangles[i]->x;
                bestNode->y = freeRectangles[i]->y;
                bestNode->width = height;
                bestNode->height = width;
                bestScore = INT_MIN;
                *nodeIndex = i;
                debug_assert(disjointRects->Disjoint(bestNode));
                break;
                }
            // Does the rectangle fit upright?
            else if (width <= freeRectangles[i]->width && height <= freeRectangles[i]->height)
                {
                int score = ScoreByHeuristic(width, height, freeRectangles[i], rectChoice);

                if (score < bestScore)
                    {
                    bestNode->x = freeRectangles[i]->x;
                    bestNode->y = freeRectangles[i]->y;
                    bestNode->width = width;
                    bestNode->height = height;
                    bestScore = score;
                    *nodeIndex = i;
                    debug_assert(disjointRects->Disjoint(bestNode));
                    }
                }
            // Does the rectangle fit sideways?
            else if (height <= freeRectangles[i]->width && width <= freeRectangles[i]->height)
                {
                int score = ScoreByHeuristic(height, width, freeRectangles[i], rectChoice);

                if (score < bestScore)
                    {
                    bestNode->x = freeRectangles[i]->x;
                    bestNode->y = freeRectangles[i]->y;
                    bestNode->width = height;
                    bestNode->height = width;
                    bestScore = score;
                    *nodeIndex = i;
                    debug_assert(disjointRects->Disjoint(bestNode));
                    }
                }
            }
        return bestNode;
        }

    /// Splits the given L-shaped free rectangle into two new free rectangles after placedRect has been placed into it.
    /// Determines the split axis by using the given heuristic.
    void GuillotineBinPack::SplitFreeRectByHeuristic(Rect^ freeRect, Rect^ placedRect, GuillotineSplitHeuristic method)
        {
        // Compute the lengths of the leftover area.
        const int w = freeRect->width - placedRect->width;
        const int h = freeRect->height - placedRect->height;

        // Placing placedRect into freeRect results in an L-shaped free area, which must be split into
        // two disjoint rectangles. This can be achieved with by splitting the L-shape using a single line.
        // We have two choices: horizontal or vertical.	

        // Use the given heuristic to decide which choice to make.

        bool splitHorizontal;
        switch (method)
            {
            case GuillotineSplitHeuristic::SplitShorterLeftoverAxis:
                // Split along the shorter leftover axis.
                splitHorizontal = (w <= h);
                break;
            case GuillotineSplitHeuristic::SplitLongerLeftoverAxis:
                // Split along the longer leftover axis.
                splitHorizontal = (w > h);
                break;
            case GuillotineSplitHeuristic::SplitMinimizeArea:
                // Maximize the larger area == minimize the smaller area.
                // Tries to make the single bigger rectangle.
                splitHorizontal = (placedRect->width * h > w * placedRect->height);
                break;
            case GuillotineSplitHeuristic::SplitMaximizeArea:
                // Maximize the smaller area == minimize the larger area.
                // Tries to make the rectangles more even-sized.
                splitHorizontal = (placedRect->width * h <= w * placedRect->height);
                break;
            case GuillotineSplitHeuristic::SplitShorterAxis:
                // Split along the shorter total axis.
                splitHorizontal = (freeRect->width <= freeRect->height);
                break;
            case GuillotineSplitHeuristic::SplitLongerAxis:
                // Split along the longer total axis.
                splitHorizontal = (freeRect->width > freeRect->height);
                break;
            default:
                splitHorizontal = true;
                assert(false);
            }

        // Perform the actual split.
        SplitFreeRectAlongAxis(freeRect, placedRect, splitHorizontal);
        }

    /// This function will add the two generated rectangles into the freeRectangles array. The caller is expected to
    /// remove the original rectangle from the freeRectangles array after that.
    /// Splits the given L-shaped free rectangle into two new free rectangles along the given fixed split axis.
    void GuillotineBinPack::SplitFreeRectAlongAxis(Rect^ freeRect, Rect^ placedRect, bool splitHorizontal)
        {
        // Form the two new rectangles.
        Rect^ bottom = gcnew Rect();
        bottom->x = freeRect->x;
        bottom->y = freeRect->y + placedRect->height;
        bottom->height = freeRect->height - placedRect->height;

        Rect^ right = gcnew Rect();
        right->x = freeRect->x + placedRect->width;
        right->y = freeRect->y;
        right->width = freeRect->width - placedRect->width;

        if (splitHorizontal)
            {
            bottom->width = freeRect->width;
            right->height = placedRect->height;
            }
        else // Split vertically
            {
            bottom->width = placedRect->width;
            right->height = freeRect->height;
            }

        // Add the new rectangles into the free rectangle pool if they weren't degenerate.
        if (bottom->width > 0 && bottom->height > 0)
            freeRectangles->Add(bottom);
        if (right->width > 0 && right->height > 0)
            freeRectangles->Add(right);

        debug_assert(disjointRects->Disjoint(bottom));
        debug_assert(disjointRects->Disjoint(right));
        }

    /// Performs a Rectangle Merge operation. This procedure looks for adjacent free rectangles and merges them if they
    /// can be represented with a single rectangle. Takes up Theta(|freeRectangles|^2) time.
    void GuillotineBinPack::MergeFreeList()
        {
#ifdef _DEBUG
        DisjointRectCollection test;
        for (size_t i = 0; i < freeRectangles->Count; ++i)
            assert(test.Add(freeRectangles[i]) == true);
#endif

        // Do a Theta(n^2) loop to see if any pair of free rectangles could me merged into one.
        // Note that we miss any opportunities to merge three rectangles into one. (should call this function again to detect that)
        for (size_t i = 0; i < freeRectangles->Count; ++i)
            for (size_t j = i + 1; j < freeRectangles->Count; ++j)
                {
                if (freeRectangles[i]->width == freeRectangles[j]->width && freeRectangles[i]->x == freeRectangles[j]->x)
                    {
                    if (freeRectangles[i]->y == freeRectangles[j]->y + freeRectangles[j]->height)
                        {
                        freeRectangles[i]->y -= freeRectangles[j]->height;
                        freeRectangles[i]->height += freeRectangles[j]->height;
                        freeRectangles->RemoveAt(j);
                        --j;
                        }
                    else if (freeRectangles[i]->y + freeRectangles[i]->height == freeRectangles[j]->y)
                        {
                        freeRectangles[i]->height += freeRectangles[j]->height;
                        freeRectangles->RemoveAt(j);
                        --j;
                        }
                    }
                else if (freeRectangles[i]->height == freeRectangles[j]->height && freeRectangles[i]->y == freeRectangles[j]->y)
                    {
                    if (freeRectangles[i]->x == freeRectangles[j]->x + freeRectangles[j]->width)
                        {
                        freeRectangles[i]->x -= freeRectangles[j]->width;
                        freeRectangles[i]->width += freeRectangles[j]->width;
                        freeRectangles->RemoveAt(j);
                        --j;
                        }
                    else if (freeRectangles[i]->x + freeRectangles[i]->width == freeRectangles[j]->x)
                        {
                        freeRectangles[i]->width += freeRectangles[j]->width;
                        freeRectangles->RemoveAt(j);
                        --j;
                        }
                    }
                }

#ifdef _DEBUG
        test.Clear();
        for (size_t i = 0; i < freeRectangles->Count; ++i)
            assert(test.Add(freeRectangles[i]) == true);
#endif
        }
    };
}
