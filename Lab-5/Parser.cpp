//
//  Parser.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

// This is the main function of the program. It should include the
// command parser functions for the various shapes. One example for
// the circle shape is given, which you should mimic for the other
// shapes

// The main function does three key things
//      1. It creates a database for up to 1000 shapes and up to 10 shape types
//      2. It registers your command parser call-back functions with the database
//      3. It invokes the command parser method of the database

#include <iostream>
#include <sstream>
#include <iomanip>

#include "Shape.h"
#include "ShapesDB.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
using namespace std;

#define MAX_SHAPES 1000
#define MAX_SHAPE_TYPES 10

// Create a database instance; making it global to simplify student code
ShapesDB sdb(MAX_SHAPES,MAX_SHAPE_TYPES);

// ECE244 Student: add your parser function prototypes here
Shape* CommandCircle(stringstream& line);
Shape* CommandTriangle(stringstream& line);
Shape* CommandRectangle(stringstream& line);

int main () {
    
    sdb.registerShapeType("circle", &CommandCircle);
    sdb.registerShapeType("triangle", &CommandTriangle);
    sdb.registerShapeType("rectangle", &CommandRectangle);
        
    // Invoke the parser of the DB
    // No new commands should be registered after this
    sdb.parseCommands();
    
    // Done
    return (0);
}

Shape* CommandCircle(stringstream& line) {
    string name;
    float xcent;
    float ycent;
    float radius;
    
    line >> name;
    line >> xcent;
    line >> ycent;
    line >> radius;
    
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    Shape* createShape = (Shape*) new Circle(name, xcent, ycent, radius);
    cout << "created circle" << endl;
    return createShape;
}

Shape* CommandTriangle(stringstream& line) {
    string name;
    float xcent;
    float ycent;
    float x1, x2, x3;
    float y1, y2, y3;
    
    line >> name;
    line >> x1;
    line >> y1;
    line >> x2;
    line >> y2;
    line >> x3;
    line >> y3;
    
    xcent = (x1 + x2 + x3) / 3;
    ycent = (y1 + y2 + y3) / 3;
    
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    Shape* createShape = (Shape*) new Triangle(name, xcent, ycent, x1, y1, x2, y2, x3, y3);
    cout << "created triangle" << endl;
    return createShape;
}

Shape* CommandRectangle(stringstream& line) {
    string name;
    float xcent;
    float ycent;
    float w, h;
    
    line >> name;
    line >> xcent;
    line >> ycent;
    line >> w;
    line >> h;
    
    if (line.fail()) {
        cout << "Error: invalid input" << endl;
        return nullptr;
    }
    if (sdb.shapeExists(name)) {
        cout << "Error: a shape with the name " << name << " already exists" << endl;
        return nullptr;
    }
    if (sdb.isReserved(name)) {
        cout << "Error: " << name << " is a reserved word" << endl;
        return nullptr;
    }
    
    Shape* createShape = (Shape*) new Rectangle(name, xcent, ycent, w, h);
    cout << "created rectangle" << endl;
    return createShape;
}