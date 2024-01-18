//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

#include "Shape.h"
#include "Triangle.h"


//Constructor
Triangle::Triangle (string n, float xcent, float ycent, float x1, float y1, float x2, float y2, float x3, float y3) : Shape(n, xcent, ycent) {
    X1 = x1;
    Y1 = y1;
    X2 = x2;
    Y2 = y2;
    X3 = x3;
    Y3 = y3;
}

//Destructor
Triangle::~Triangle() {
    //Nothing
}

//Accessors
float Triangle::getx1() const {
    return X1;
}
float Triangle::gety1() const {
    return Y1;
}
float Triangle::getx2() const {
    return X2;
}
float Triangle::gety2() const {
    return Y2;
}
float Triangle::getx3() const {
    return X3;
}
float Triangle::gety3() const {
    return Y3;
}

//Mutators
void Triangle::setx1(float x) {
    X1 = x;
}
void Triangle::sety1(float y) {
    Y1 = y;
}
void Triangle::setx2(float x) {
    X2 = x;
}
void Triangle::sety2(float y) {
    Y2 = y;
}
void Triangle::setx3(float x) {
    X3 = x;
}
void Triangle::sety3(float y) {
    Y3 = y;
}

//Utility Methods
void Triangle::draw() const {
    cout << std::fixed;
    cout << std::setprecision(2);

    cout << "triangle: " << name << " "
         << x_centre << " " << y_centre
         << " " << X1 << " " << Y1
         << " " << X2 << " " << Y2
         << " " << X3 << " " << Y3
         << " " << computeArea() << endl;
}

float Triangle::computeArea() const {
    return abs(X1*(Y2-Y3) + X2*(Y3-Y1) + X3*(Y1-Y2))/2;
}

Shape* Triangle::clone() const {
    return (new Triangle(*this));
}