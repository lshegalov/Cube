// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

#include <iostream>
#include <chrono>

#include "Simulation.h"
#include "Polyhedron.h"

int main()
{
    std::cout << "Hello World!\n";

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    

    Simulation simulation;
    
    int nNumberOfSteps = 50000;

    std::string sErrorMsg;

    Results results;

    if ( ! simulation.runOnCube(nNumberOfSteps, OUT results, OUT sErrorMsg))
    {
        std::cerr << "Failed :" << sErrorMsg <<std::endl;
    }
    std::cout << "Completed OK" << std::endl;

    std::cout << "# Of Steps=" << results.nNumberOfSteps << std::endl;

    std::cout << "# Of Polygons=" << results.lNumberOfPolygons << std::endl;

    std::cout << "Total # Of Sides = " << results.nTotalNumberOfSides << std::endl;
    std::cout << "Total # Of Edges = " << results.nTotalNumberOfEdges << std::endl;
        

    std::cout << "Mean # Of Edges Per Polygon=" << results.dAverageNumberOfEdgesPerPolygons << std::endl;
    std::cout << "Mean # Of Sides Per Polygon=" << results.dAverageNumberOfSidesPerPolygons << std::endl;
    
    for (std::map<int, size_t>::const_iterator edgeNumIter = results.distributionPerEdges.cbegin();
        edgeNumIter != results.distributionPerEdges.cend(); edgeNumIter++)
    {
        size_t nNumberOfPolygonPerEdge = edgeNumIter->second;

        double dProportion = (1. * nNumberOfPolygonPerEdge) / results.lNumberOfPolygons;
        dProportion *= 100;

        std::cout << "Edges:=" << edgeNumIter->first << "  Number Of Polygons= " << dProportion << "%" << std::endl;
    }

    std::cout << std::endl;

    for (std::map<int, size_t>::const_iterator edgeNumIter = results.distributionPerSides.cbegin();
        edgeNumIter != results.distributionPerSides.cend(); edgeNumIter++)
    {
        size_t nNumberOfPolygonPerSide = edgeNumIter->second;

        double dProportion = (1. * nNumberOfPolygonPerSide) / results.lNumberOfPolygons;
        dProportion *= 100;

        std::cout << "Sides:=" << edgeNumIter->first << "  Number Of Polygons= " << dProportion << "%" << std::endl;
    }
    
    std::cout << std::endl;

    // per # of steps
    for (std::map<int, double>::const_iterator stepIter = results.dMeanNumberOfSidesPerPolygons.cbegin();
        stepIter != results.dMeanNumberOfSidesPerPolygons.cend(); stepIter++)
    {
        int nStep = stepIter->first;
        double dMeanNumberOfSidesPerPolygons = stepIter->second;

        std::cout << "Steps:=" << nStep << "  Mean Number Of Sides Per Polygon= " << dMeanNumberOfSidesPerPolygons << std::endl;
    }

    // per # of steps
    for (std::map<int, double>::const_iterator stepIter = results.dMeanNumberOfEdgesPerPolygons.cbegin();
        stepIter != results.dMeanNumberOfEdgesPerPolygons.cend(); stepIter++)
    {
        int nStep = stepIter->first;
        double dMeanNumberOfEdgesPerPolygons = stepIter->second;

        std::cout << "Steps:=" << nStep << "  Mean Number Of Edges Per Polygon= " << dMeanNumberOfEdgesPerPolygons << std::endl;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

}
/*
Hello World!
Completed OK
# Of Steps = 50000
# Of Polygons = 50006
Total # Of Sides = 200024
Total # Of Edges = 223797
Mean # Of Edges Per Polygon = 4.4754
Mean # Of Sides Per Polygon = 4
Edges: = 3  Number Of Polygons = 28.7785 %
Edges : = 4  Number Of Polygons = 30.3684 %
Edges : = 5  Number Of Polygons = 20.1916 %
Edges : = 6  Number Of Polygons = 11.5806 %
Edges : = 7  Number Of Polygons = 5.44535 %
Edges : = 8  Number Of Polygons = 2.21773 %
Edges : = 9  Number Of Polygons = 0.945886 %
Edges : = 10  Number Of Polygons = 0.33196 %
Edges : = 11  Number Of Polygons = 0.103988 %
Edges : = 12  Number Of Polygons = 0.0259969 %
Edges : = 13  Number Of Polygons = 0.00599928 %
Edges : = 14  Number Of Polygons = 0.00199976 %
Edges : = 18  Number Of Polygons = 0.00199976 %

Sides : = 3  Number Of Polygons = 36.9096 %
Sides : = 4  Number Of Polygons = 36.7336 %
Sides : = 5  Number Of Polygons = 18.1358 %
Sides : = 6  Number Of Polygons = 6.28925 %
Sides : = 7  Number Of Polygons = 1.59181 %
Sides : = 8  Number Of Polygons = 0.283966 %
Sides : = 9  Number Of Polygons = 0.0519938 %
Sides : = 10  Number Of Polygons = 0.00399952 %

Steps : = 1000  Mean Number Of Sides Per Polygon = 4
Steps : = 2000  Mean Number Of Sides Per Polygon = 4
Steps : = 3000  Mean Number Of Sides Per Polygon = 4
Steps : = 4000  Mean Number Of Sides Per Polygon = 4
Steps : = 5000  Mean Number Of Sides Per Polygon = 4
Steps : = 6000  Mean Number Of Sides Per Polygon = 4
Steps : = 7000  Mean Number Of Sides Per Polygon = 4
Steps : = 8000  Mean Number Of Sides Per Polygon = 4
Steps : = 9000  Mean Number Of Sides Per Polygon = 4
Steps : = 10000  Mean Number Of Sides Per Polygon = 4
Steps : = 11000  Mean Number Of Sides Per Polygon = 4
Steps : = 12000  Mean Number Of Sides Per Polygon = 4
Steps : = 13000  Mean Number Of Sides Per Polygon = 4
Steps : = 14000  Mean Number Of Sides Per Polygon = 4
Steps : = 15000  Mean Number Of Sides Per Polygon = 4
Steps : = 16000  Mean Number Of Sides Per Polygon = 4
Steps : = 17000  Mean Number Of Sides Per Polygon = 4
Steps : = 18000  Mean Number Of Sides Per Polygon = 4
Steps : = 19000  Mean Number Of Sides Per Polygon = 4
Steps : = 20000  Mean Number Of Sides Per Polygon = 4
Steps : = 21000  Mean Number Of Sides Per Polygon = 4
Steps : = 22000  Mean Number Of Sides Per Polygon = 4
Steps : = 23000  Mean Number Of Sides Per Polygon = 4
Steps : = 24000  Mean Number Of Sides Per Polygon = 4
Steps : = 25000  Mean Number Of Sides Per Polygon = 4
Steps : = 26000  Mean Number Of Sides Per Polygon = 4
Steps : = 27000  Mean Number Of Sides Per Polygon = 4
Steps : = 28000  Mean Number Of Sides Per Polygon = 4
Steps : = 29000  Mean Number Of Sides Per Polygon = 4
Steps : = 30000  Mean Number Of Sides Per Polygon = 4
Steps : = 31000  Mean Number Of Sides Per Polygon = 4
Steps : = 32000  Mean Number Of Sides Per Polygon = 4
Steps : = 33000  Mean Number Of Sides Per Polygon = 4
Steps : = 34000  Mean Number Of Sides Per Polygon = 4
Steps : = 35000  Mean Number Of Sides Per Polygon = 4
Steps : = 36000  Mean Number Of Sides Per Polygon = 4
Steps : = 37000  Mean Number Of Sides Per Polygon = 4
Steps : = 38000  Mean Number Of Sides Per Polygon = 4
Steps : = 39000  Mean Number Of Sides Per Polygon = 4
Steps : = 40000  Mean Number Of Sides Per Polygon = 4
Steps : = 41000  Mean Number Of Sides Per Polygon = 4
Steps : = 42000  Mean Number Of Sides Per Polygon = 4
Steps : = 43000  Mean Number Of Sides Per Polygon = 4
Steps : = 44000  Mean Number Of Sides Per Polygon = 4
Steps : = 45000  Mean Number Of Sides Per Polygon = 4
Steps : = 46000  Mean Number Of Sides Per Polygon = 4
Steps : = 47000  Mean Number Of Sides Per Polygon = 4
Steps : = 48000  Mean Number Of Sides Per Polygon = 4
Steps : = 49000  Mean Number Of Sides Per Polygon = 4
Steps : = 50000  Mean Number Of Sides Per Polygon = 4
Steps : = 1000  Mean Number Of Edges Per Polygon = 4.46123
Steps : = 2000  Mean Number Of Edges Per Polygon = 4.48903
Steps : = 3000  Mean Number Of Edges Per Polygon = 4.48736
Steps : = 4000  Mean Number Of Edges Per Polygon = 4.48577
Steps : = 5000  Mean Number Of Edges Per Polygon = 4.48282
Steps : = 6000  Mean Number Of Edges Per Polygon = 4.47536
Steps : = 7000  Mean Number Of Edges Per Polygon = 4.48202
Steps : = 8000  Mean Number Of Edges Per Polygon = 4.48514
Steps : = 9000  Mean Number Of Edges Per Polygon = 4.48046
Steps : = 10000  Mean Number Of Edges Per Polygon = 4.48271
Steps : = 11000  Mean Number Of Edges Per Polygon = 4.48228
Steps : = 12000  Mean Number Of Edges Per Polygon = 4.48168
Steps : = 13000  Mean Number Of Edges Per Polygon = 4.48324
Steps : = 14000  Mean Number Of Edges Per Polygon = 4.48151
Steps : = 15000  Mean Number Of Edges Per Polygon = 4.47828
Steps : = 16000  Mean Number Of Edges Per Polygon = 4.47707
Steps : = 17000  Mean Number Of Edges Per Polygon = 4.47754
Steps : = 18000  Mean Number Of Edges Per Polygon = 4.47856
Steps : = 19000  Mean Number Of Edges Per Polygon = 4.4809
Steps : = 20000  Mean Number Of Edges Per Polygon = 4.47951
Steps : = 21000  Mean Number Of Edges Per Polygon = 4.48148
Steps : = 22000  Mean Number Of Edges Per Polygon = 4.47941
Steps : = 23000  Mean Number Of Edges Per Polygon = 4.4797
Steps : = 24000  Mean Number Of Edges Per Polygon = 4.47767
Steps : = 25000  Mean Number Of Edges Per Polygon = 4.47952
Steps : = 26000  Mean Number Of Edges Per Polygon = 4.48097
Steps : = 27000  Mean Number Of Edges Per Polygon = 4.47971
Steps : = 28000  Mean Number Of Edges Per Polygon = 4.47836
Steps : = 29000  Mean Number Of Edges Per Polygon = 4.47894
Steps : = 30000  Mean Number Of Edges Per Polygon = 4.47867
Steps : = 31000  Mean Number Of Edges Per Polygon = 4.47884
Steps : = 32000  Mean Number Of Edges Per Polygon = 4.47804
Steps : = 33000  Mean Number Of Edges Per Polygon = 4.47837
Steps : = 34000  Mean Number Of Edges Per Polygon = 4.47753
Steps : = 35000  Mean Number Of Edges Per Polygon = 4.47686
Steps : = 36000  Mean Number Of Edges Per Polygon = 4.4765
Steps : = 37000  Mean Number Of Edges Per Polygon = 4.47657
Steps : = 38000  Mean Number Of Edges Per Polygon = 4.47706
Steps : = 39000  Mean Number Of Edges Per Polygon = 4.47744
Steps : = 40000  Mean Number Of Edges Per Polygon = 4.47763
Steps : = 41000  Mean Number Of Edges Per Polygon = 4.47886
Steps : = 42000  Mean Number Of Edges Per Polygon = 4.47788
Steps : = 43000  Mean Number Of Edges Per Polygon = 4.47675
Steps : = 44000  Mean Number Of Edges Per Polygon = 4.47709
Steps : = 45000  Mean Number Of Edges Per Polygon = 4.47629
Steps : = 46000  Mean Number Of Edges Per Polygon = 4.47498
Steps : = 47000  Mean Number Of Edges Per Polygon = 4.47456
Steps : = 48000  Mean Number Of Edges Per Polygon = 4.47511
Steps : = 49000  Mean Number Of Edges Per Polygon = 4.47511
Steps : = 50000  Mean Number Of Edges Per Polygon = 4.4754
Time difference = 1934[s]

D : \AlexLevin\src\ConsoleApplication1\x64\Release\CubeSplit.exe(process 8636) exited with code 0.
Press any key to close this window . . .
*/


























































































