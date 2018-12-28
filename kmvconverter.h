/*
 * program name:KmVConverter.h/KmVConverter.cpp
 * program date:2018.12.28
 * program auth:James Lisle
 * program desc:Simple class to: (1) load a dataset that correlates degree Kelvin to milliVolt
 *                               (2) provides 2 public methods that translate between Kelvin and milliVolts in both directions
 *
 *                               - Uses binary search to locate data point or where data point should be located
 *                               - Uses linear interpolation when an exact match is not found [y = y1 + (y2 - y1)((x - x1)/(x2 - x1))]
 *                               - Assumes data is sorted
 *                               - Requires input to exist in dataset
 *
 *                               - currently does not consider significant digits
 *
*/

#ifndef KMVCONVERTER_H
#define KMVCONVERTER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;





class KmVConverter
{
public:
    KmVConverter(char* fileName);
    ~KmVConverter();

    double convertmVtoK(double mVolts);
    double convertKtomV(double Kelvin);

    enum ConvUnit {Kelvin=0, mVolt=1};

private:

    int KmVLookup(int &high, int &low, ConvUnit convUnit, double v);
    int KmVLookupDesc(int &low, int &high, ConvUnit convUnit, double value);
    double KTomVInterpolation(int low, int high, double target);
    double mVToKInterpolation(int low, int high, double target);
    void loadKmVXRefTable(char* fileName);
    bool validateInput(double value, ConvUnit convUnit);


    double **dataStore;
    char *dataFileName;
    int recordCnt;




};

#endif // KMVCONVERTER_H
