#include "MathUtils.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "Utils.h"
#include <stdlib.h>
#include <cmath>
#include <stdexcept>
using std::runtime_error;
#include <map>
using std::map;

void MathUtils::checkSystemDimensions(vector<vector<double> >& a, vector<double>& b)
{
    if (b.size() != a.size())
        throw std::runtime_error("[MathUtils::solveSystemOfLinearEquations]: A and b must have the same number of rows");
    unsigned long n = a.size();
    BOOST_FOREACH(vector<double>& row, a)
        if (row.size() != n)
            runtime_error("[MathUtils::solveSystemOfLinearEquations]: A must be a square matrix");
}

vector<double> MathUtils::solveSystemOfLinearEquations(vector<vector<double> > a, vector<double> b)
{
    if (!a.size()) throw std::runtime_error("[MathUtils::solveSystemOfLinearEquations] called with empty input");

    // used to solve
    //      A.x = b
    // where:
    //      A is an n x n matrix
    //      x and b are n x 1 vectors
    MathUtils::checkSystemDimensions(a,b);
    unsigned long n = a.size();

    // perform Gaussian elimination
    for (unsigned long i = 0; i < n - 1; ++i)
    {
        // eliminate everything in column i, below row i
        MathUtils::eliminate(a, b, i);
    }

    vector<double> x(n);

    // now solve, from the bottom up
    for (long index = n - 1; index >= 0; --index)
    {
        double otherVarSum = 0.0;
        for (unsigned long column = index +1; column < n; ++column)
        {
            otherVarSum += a.at(index).at(column) * x.at(column);
        }
        double val = b.at(index) - otherVarSum;
        double denominator = a.at(index).at(index);
        if (!denominator)
            throw std::runtime_error("[MathUtils::solveSystemOfLinearEquations] Cannot divide by zero ==> no unique solution. Is system identified? Have you included too many variables?");

        double coefficient = val / denominator;
        x.at(index) = coefficient;
    }
    return x;
}

// void MathUtils::eliminate(TOtMatrix& a, TOtMatrix& b, int index)
void MathUtils::eliminate(vector<vector<double> >& a, vector<double>& b, unsigned long index)
{
    unsigned long n = a.size();
    if (a.at(index).at(index) == 0)
    {
        // find a non-zero value in the rows below.
        bool found = false;
        for (unsigned long row = index + 1; row < n; ++row)
        {
            if (a.at(row).at(index) != 0)
            {
                // found a non-zero value
                found = true;
                for (unsigned long column = index; column < n; ++column)
                {
                    a.at(index).at(column) = a.at(index).at(column) + a.at(row).at(column);
                }
                b.at(index) = b.at(index) + b.at(row);
                break;
            }
        }
        if (!found)
            throw std::runtime_error("Could not eliminate on index " + boost::lexical_cast<std::string>(index));
    }

    // now eliminate in all the rows below index
    for (unsigned long row = index + 1; row < n; ++row)
    {
        double factor = a.at(row).at(index) / a.at(index).at(index);
        if (factor != 0)
        {
            for (unsigned long column = index; column < n; ++column)
            {
                a.at(row).at(column) = a.at(row).at(column) - factor * a.at(index).at(column);
            }
            b.at(row) = b.at(row) - factor * b.at(index);
        }
    }
}

vector<int> MathUtils::identifyLinearlyDependentMatrixRows(vector<vector<double> >& matrix)
{
    // this method identifies rows which need to be removed to achieve full row rank.
    // rows which can be fully eliminated (as a linear combination of other rows) are what we're looking for.

    unsigned long rows = matrix.size();
    unsigned long cols = matrix.front().size();
    // vlcMessage.Write(string("We have " + ToString(rows) + " rows.").c_str());
    // vlcMessage.Write(string("We have " + ToString(cols) + " cols.").c_str());

    BOOST_FOREACH(vector<double>& row, matrix)
    {
        if (row.size() != cols)
            throw runtime_error("[MathUtils::identifyLinearlyDependentMatrixRows] Matrix is not rectangular");
    }

    unsigned long numberOfColumnsEliminated = 0;
    map<int, int> rowHasBeenUsed;

    for (unsigned long row = 0; row < rows; ++row)
    {
        double rowAbsSum = Utils::vectorSum(Utils::vectorAbs(matrix.at(row)));
        // vlcMessage.Write(string("We have initial row abs sum " + ToString(rowAbsSum) + " for row " + ToString(row)).c_str());
    }
    
    unsigned long columnToEliminateWith = -1;
    while (numberOfColumnsEliminated < rows-1 && columnToEliminateWith < cols - 1)
    {
        columnToEliminateWith += 1;

        unsigned long rowToEliminateWith = -1;
        double eliminationCellValue;
        for (rowToEliminateWith = 0; rowToEliminateWith < rows; ++rowToEliminateWith)
        {
            if (rowHasBeenUsed[rowToEliminateWith] > 0)
                continue;

            eliminationCellValue = matrix.at(rowToEliminateWith).at(columnToEliminateWith);
            
            if (eliminationCellValue > 0.001)
                break;
            else if (eliminationCellValue > 0)
            {
                // first check it's not TINY and we're dealing with precision issues
                double rowAbsSum = Utils::vectorSum(Utils::vectorAbs(matrix.at(rowToEliminateWith)));
                if (rowAbsSum > 0.01)
                    break; // if it's less than 0.001 it may as well be zero, so it's linearly dependent
            }
        }

        if (rowToEliminateWith == rows)
            continue; // didn't find a non-zero value in the column to eliminate, so keep going!

        // vlcMessage.Write(string("Eliminating column " + ToString(columnToEliminateWith)).c_str());
        // vlcMessage.Write(string("Eliminating using row " + ToString(rowToEliminateWith)).c_str());

        for (unsigned long rowToEliminate = 0; rowToEliminate < rows; ++rowToEliminate)
        {
            if (rowToEliminate == rowToEliminateWith || rowHasBeenUsed[rowToEliminate] > 0)
                continue;

            double cellValueToEliminate = matrix.at(rowToEliminate).at(columnToEliminateWith);
            if (cellValueToEliminate == 0.0)
                continue; // nothing to do!

            // vlcMessage.Write(string("Eliminating row " + ToString(rowToEliminate)).c_str());

            double ratio = cellValueToEliminate / eliminationCellValue;
            // vlcMessage.Write(ToString(ratio).c_str());

            for (unsigned long columnToEliminate = columnToEliminateWith; columnToEliminate < cols; ++columnToEliminate)
            {
                double increment = ratio * matrix.at(rowToEliminateWith).at(columnToEliminate);
                //vlcMessage.Write(string("Subtracting " + ToString(increment) + " from value " + ToString(matrix.at(rowToEliminate).at(columnToEliminate))).c_str());
                matrix.at(rowToEliminate).at(columnToEliminate) -= increment;
            }
        }
        rowHasBeenUsed[rowToEliminateWith] = 1;
        numberOfColumnsEliminated += 1;
    }

    vector<int> linearlyDependentRows;

    // now check which rows are *basically* zero!
    for (unsigned long row = 0; row < rows; ++row)
    {
        double rowAbsSum = Utils::vectorSum(Utils::vectorAbs(matrix.at(row)));
        // vlcMessage.Write(string("We have row abs sum " + ToString(rowAbsSum) + " for row " + ToString(row)).c_str());
        if (rowAbsSum < 0.001)
            linearlyDependentRows.push_back(row);
    }

    return linearlyDependentRows;
}

double MathUtils::convertRandomNumberToExtremeValue(long double random_number)
{
    if (random_number < std::numeric_limits<double>::epsilon() || random_number > 1.0 - std::numeric_limits<double>::epsilon())
        throw std::runtime_error("Can't have a random number not on the range (0.0, 1.0)");

    return -::log(-::log(random_number));
}
