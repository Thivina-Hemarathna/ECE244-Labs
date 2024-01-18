//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
bool keyword(string s);
bool typeword(string s);
void database(int n);
void error(char err, string s);

int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input

    while ( !cin.eof () ) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;

        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        
     if (!lineStream.fail()){   

        //maxShapes

        if (command == "maxShapes"){
            int value;
            lineStream >> value;
            //peek()=-1 for eof
            //peek()=32 for space
            if (!lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                //check for extra arguments
                string arg;
                lineStream >> arg;

                if (lineStream.fail() && lineStream.eof()){
                    //clear existing array
                    while (shapesArray != NULL){
                        for (int i = 0; i < shapeCount; i++) {
                            delete shapesArray[i];
                            shapesArray[i] = NULL;     
                        }
                        delete [] shapesArray;          
                        shapesArray = NULL;             
                        shapeCount = 0;
                    }
                    //new array parameters
                    max_shapes = value;
                    shapesArray = new Shape*[max_shapes];
                    database(max_shapes);
                }
                else {
                    error('H',"");
                }
            }
                else {
                    if (lineStream.eof()){
                        error('I',"");
                    }
                    else{
                        error('B',"");
                    }
                }

            }

            //create

        else if (command == "create"){
            string name;
            lineStream >> name;
            
            if (!lineStream.fail()){

                bool name_exists = false;

                    //shape name is a keyword
                name_exists=keyword(name);
                if (name_exists){
                    error('C',"");
                }
                else {
                    name_exists=typeword(name);
                    if (name_exists){
                        error('C',"");
                    }
                }
                    
                    //shape name already exists
                    for (int i = 0; i < shapeCount ; i++){
                        if (shapesArray[i] != NULL){
                            if ( name == (shapesArray[i]->getName())){
                                name_exists = true;
                                error('D',name);
                                break;
                            }
                        }
                    }
                
                    if(!name_exists){
                        string type;
                        lineStream >> type;

                        //check if valid type
                        if(!lineStream.fail()){
                            bool type_exists = false;
                            
                            type_exists=typeword(type);
                            //valid shape type
                            if (type_exists){
                                int x_loc;
                                lineStream >> x_loc;

                                if ((x_loc>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                                    int y_loc;
                                    lineStream >> y_loc;

                                     if ((y_loc>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                                        int x_size;
                                        lineStream >> x_size;

                                         if ((x_size>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                                            int y_size;
                                            lineStream >> y_size;
                                             //extra args
                                             if ((y_size>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                                                string arg;
                                                lineStream >> arg;

                                                if (lineStream.fail() && lineStream.eof()){
                                                    //check circle parameters
                                                    if( type == "circle" && (y_size != x_size)){
                                                        error('G',"");
                                                        goto skip1;
                                                    }

                                                    if (shapeCount != max_shapes){
                                                        Shape* Shp = new Shape(name,type,x_loc,x_size,y_loc,y_size);
                                                        shapesArray[shapeCount] = Shp;
                                                        shapeCount = shapeCount + 1;
                                                        cout << "Created ";
                                                        Shp->draw();
                                                        cout<<endl;
                                                    }
                                                    else{
                                                        error('Z',"");
                                                    }
                                                }
                                                else{
                                                    error('H',"");
                                                }
                                }
                                else{
                                    if(lineStream.fail()){
                                        if( lineStream.eof()){
                                        error('I',"");
                                    }
                                    else{
                                        error('B',"");
                                    }} else
                                    if( y_size < 0){
                                        error('G',"");
                                    }
                                    else{
                                        error('B',"");
                                    }
                                }
                            }
                                else{
                                    if(lineStream.fail()){
                                        if (lineStream.eof()){
                                        error('I',"");
                                    }
                                    else{
                                        error('B',"");
                                    } } else if( x_size < 0){
                                        error('G',"");
                                    }
                                    else{
                                        error('B',"");
                                    }
                                }
                            }
                            else{
                                if(lineStream.fail()){
                                    if(lineStream.eof()){
                                    error('I',"");
                                }
                                else{
                                    error('B',"");
                                }}
                                else if( y_loc < 0){
                                    error('G',"");
                                }
                                else{
                                    error('B',"");
                                }
                             }
                        }
                        else{
                            if(lineStream.fail()){
                                if( lineStream.eof()){
                                error('I',"");
                            }
                            else{
                                error('B',"");
                            }} else
                            if( x_loc < 0){
                                error('G',"");
                            }
                            else{
                                error('B',"");
                            }
                        }
                    }
                else {
                    error('F',"");
                }
            }
            else{
                if (lineStream.eof()){
                    error('I',"");
                }
            }
                    }
            }
            else  if(lineStream.eof()){
                    error('I',"");
                }
                     
        }

        //move

        else if (command == "move"){
            string name;
            lineStream >> name;

            if(!lineStream.fail()){
                //search for shape
                Shape* mov_shp=NULL;
                for (int i = 0; i < shapeCount; i++) {
                    if (shapesArray[i] != NULL) {
                        if (((shapesArray[i])->getName()) == name) {
                            mov_shp = shapesArray[i];//shape found
                         }
                    }
                 }
                //not found
                if (mov_shp==NULL){
                    error('E',name);
                    goto skip1;
                }
                
                int x_loc;
                lineStream >> x_loc;

                    if((x_loc>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                        int y_loc;
                        lineStream >> y_loc;

                        if ((y_loc>=0) && !lineStream.fail() && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                            //extra arguments
                            string arg;
                            lineStream >> arg;

                         if (lineStream.fail() && lineStream.eof()){
                            mov_shp->setXlocation(x_loc);
                            mov_shp->setYlocation(y_loc);
                            cout <<"Moved " << mov_shp->getName()<<" to "<<mov_shp->getXlocation()<<" "<<mov_shp->getYlocation() << endl;
                        }
                        else{
                            error('H',"");
                        }                      
                    }
                    else {
                        if (lineStream.fail()){
                            if (lineStream.eof()){
                            error('I',"");
                        }else {
                            error('B',"");
                        }}
                        if (y_loc < 0){
                            error('G',"");
                        }
                        else{
                            error('B',"");
                        }
                    }
                }
                else {
                if (lineStream.fail()){
                    if ( lineStream.eof()){
                     error('I',"");
                }else {
                     error('B',"");
                }}
                if (x_loc < 0){
                     error('G',"");
                }
                else{
                     error('B',"");
                }
            }    
        }
        else {
            if(lineStream.eof()){
                error('I',"");
            }
        }
     }

     //rotate
       else if (command == "rotate"){
        
        string name;
        lineStream >> name;

        if(!lineStream.fail()){
            //find the shape name
            Shape* rot_shp=NULL;
                for (int i = 0; i < shapeCount; i++) {
                    if (shapesArray[i] != NULL) {
                        if (((shapesArray[i])->getName()) == name) {
                            rot_shp = shapesArray[i];//shape found
                         }
                    }
                 }
                //not found
                if (rot_shp==NULL){
                    error('E',name);
                    goto skip1;
                }

                int rotate;
                lineStream >> rotate;
                 if (!lineStream.fail() && rotate>=0 && rotate<=360 && (lineStream.peek() == -1 || lineStream.peek() == 32)){
                    //extra arguments
                    string arg;
                    lineStream >> arg;

                    if (lineStream.fail() && lineStream.eof()){
                        rot_shp->setRotate(rotate);

                        cout<<"Rotated "<<rot_shp->getName()<<" by "<<rotate<<" degrees"<<endl;
                    }
                    else{
                        error('H',"");
                    }
                 }
                else {
                    if (lineStream.fail()){
                        if( lineStream.eof()){
                        error('I',"");
                    }
                    else{
                        error('B',"");
                    }
                    }
                    if (rotate<0 || rotate>360){
                        error('G',"");
                    }
                    else{
                        error('B',"");                     
                    }
                }
            }
        else if(lineStream.eof()){
                error('I',"");
            }
       }

       //draw
       else if(command == "draw"){

        string name;
        lineStream >> name;

        if(!lineStream.fail()){
            //extra arguments
            string arg;
            lineStream >> arg;

            //find shape
             Shape* draw_shp=NULL;
                for (int i = 0; i < shapeCount; i++) {
                    if (shapesArray[i] != NULL) {
                        if (((shapesArray[i])->getName()) == name) {
                            draw_shp = shapesArray[i];//shape found
                            break;
                         }
                    }
                 }
                //name=all jump to all
             if (name == "all"){
                 goto all1;
             }
             //not found
                    if (draw_shp == NULL){
                    error('E',name);
                    goto skip1;
                    } 
             
        

                //check for end of args
                if(lineStream.fail() && lineStream.eof()){
                    cout<<"Drew ";
                    draw_shp->draw();
                    cout<<endl;
                }
                else{
                    error('H',"");
                }
             all1:
            if (name == "all"){
                if(lineStream.fail() && lineStream.eof()){
                    cout<<"Drew all shapes"<< endl;
                    for (int i = 0; i < shapeCount; i++) {
                        if (shapesArray[i] != NULL) {
                         shapesArray[i]->draw();
                        }
                    }
                }
                else{
                    error('H',"");
                }
            }    
        }
        else{
            error('I',"");
        }
    } 

    //delete
    else if(command == "delete"){
        string name;
        lineStream >> name;
        if(!lineStream.fail()){
            //extra arguments
            string arg;
            lineStream >> arg;

            Shape* del_shp=NULL;
            
            for (int i= 0; i<shapeCount; i++){
                if (shapesArray[i] != NULL){
                    if (((shapesArray[i])->getName()) == name) {
                            del_shp = shapesArray[i];
                            break;
                }
            }
            }
            //not found
            if(name != "all"){
            if (del_shp==NULL){
                    error('E',name);
                    goto skip1;
                }
            }
            if(lineStream.fail() && lineStream.eof()){
                for (int i = 0; i < shapeCount; i++) {
                    if (shapesArray[i] != NULL) {
                        if (((shapesArray[i])->getName()) == name) {
                            del_shp = shapesArray[i];//shape found
                             cout << "Deleted shape " << name << endl;
                            delete shapesArray[i];
                            shapesArray[i]=NULL;
                            break;
                         }
                    }
                 }
                
            }
            else {
                error('H',"");
            } if (name == "all"){
                    if (lineStream.fail() && lineStream.eof()){
                        for (int i=0; i<shapeCount; i++){
                        delete shapesArray[i];
                        shapesArray[i]=NULL;
                    }
                    cout<<"Deleted: all shapes"<<endl;
                    }
                    else if (!lineStream.eof()){
                    error('H',"");
                    }
                }
                
        }
        else {
            error('I',"");
        }
    }
    else{
        error('A',"");
        }
 }
        
        // Once the command has been processed, prompt for the
        // next command
        skip1:
        cout << "> ";          // Prompt for input
        getline(cin, line);
        
    }  // End input loop until EOF.
    
    return 0;
}

bool keyword(string s){
    for (int i=0 ; i < NUM_KEYWORDS; i++){
        if (s == keyWordsList[i]){
            return true;
        }
    }
    return false;
}
bool typeword(string s){
    for (int i=0; i < NUM_TYPES; i++){
        if (s == shapeTypesList[i]){
            return true;
        }
    }
    return false;
}
void database(int n){
    cout << "New database: max shapes is "<<n<< endl;
    return;
}

void error(char err, string s){
    char code = err;
    string name = s;

    switch(code){
        case 'A':
            cout << "Error: invalid command" << endl;
            return;
        case 'B':
            cout << "Error: invalid argument" << endl;
            return;
        case 'C':
            cout << "Error: invalid shape name" << endl;
            return;
        case 'D':
            cout << "Error: shape "<<name<<" exists"<< endl;
            return;
        case 'E':
            cout << "Error: shape "<<name<<" not found" << endl;
            return;
        case 'F':
            cout<< "Error: invalid shape type" << endl;
            return;
        case 'G':
            cout<< "Error: invalid value" << endl;
            return;
        case 'H':
            cout<< "Error: too many arguments" << endl;
            return;
        case 'I':
            cout<< "Error: too few arguments" << endl;
            return;
        default:
            cout<< "Error: shape array is full" << endl;
            return;
    }
    return;
}
