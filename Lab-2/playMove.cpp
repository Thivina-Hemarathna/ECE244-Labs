//
//  main.cpp
//  TicTacTo
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file


#include <iostream>
#include <string>
using namespace std;

#include "globals.h"
#include "GameState.h"

// Function prototype for playMove
void playMove(GameState&);
void gameResult (int row,int col,GameState& game_state);

// The main function
int main() {

    /**********************************************************************************/
    /* Create an initialized game state object                                        */
    /**********************************************************************************/
    GameState game_state;

    // Read two integers from the user that represent the row and column
    // the player would like to place an X or an O in
    // You can assume there will be no formatting errors in the input

    int row;
    int col;
    while (!game_state.get_gameOver()) {
        cout << "Enter row and column of a grid cell: ";
        cin >> row >> col;
        
        // Check that the read row and column values are valid grid coordinates
        if ( (row < 0) || (row > 2) || (col < 0) || (col > 2) ) {
            cout << "Invalid board coordinates " << row << " " << col << endl << endl;
           continue;
        }
        // The coordinates are valid; set the selectedRow and selectedColumn
        // members of the game state to the read values
        // Again, the corresponding mutators of GameState must be first
        // implemented before this works
            
        // ECE244 Student: add your code here
        if (game_state.get_gameBoard(row,col)==Empty){
            game_state.set_moveValid(true);
            game_state.set_selectedRow(row);
            game_state.set_selectedColumn(col);
        }
        else {
            game_state.set_moveValid(false);
        }
        // Call playMove
        playMove(game_state);   
        gameResult(row,col,game_state);
        // ECE244 Student: add your code here
            
        // Print the GameState object, as prescribed in the handout
            
        // ECE244 Student: add your code here
        cout << "Selected row " << game_state.get_selectedRow() << " and column " << game_state.get_selectedColumn() << endl;
        cout << "Game state after playMove: " << endl;
        cout << "Board: " << endl;
        
        for(int row=0;row<3;row++){
            for(int col=0;col<3;col++){
                if (game_state.get_gameBoard(row,col)==X){
                    cout<< "X";
                }
                else if (game_state.get_gameBoard(row,col)==O){
                    cout<< "O";
                } else { cout<< "B";}
                if (col!=2){
                    cout<< " ";
                }
            }
            cout<<endl;
        }
        
        if (game_state.get_moveValid()==true){
            cout<<"moveValid: true"<< endl;
        }
        else {
            cout<<"moveValid: false"<<endl;
        }
        if (game_state.get_gameOver()==true){
            cout<<"gameOver: true"<<endl;
            cout<<"winCode: "<<game_state.get_winCode()<< endl;
        }
        else {
            cout<<"gameOver: false"<<endl;
            cout<<"winCode: "<<game_state.get_winCode()<<endl;
        }
    }
    return 0;
}

void gameResult(int row, int col, GameState& game_state){
    
    // for 3 consecutive along columns.
    for (int row=0;row<3;row++){
        int X_value=0;
        int O_value=0;
        for (int col=0;col<3;col++){
            if (game_state.get_gameBoard(row,col)==X){
                X_value++;
            }
            else if (game_state.get_gameBoard(row,col)==O){
                O_value++;
                
            }
            if (X_value>2||O_value>2){
                game_state.set_winCode(row+1);
                game_state.set_gameOver(true);
            }
        }
    }
    
    //for 3 consecutive along rows.
    for (int col=0;col<3;col++){
        int X_value=0;
        int O_value=0;
        for (int row=0;row<3;row++){
            if(game_state.get_gameBoard(row,col)==X){
                X_value++;
            }
            else if (game_state.get_gameBoard(row,col)==O){
                O_value++;
            }
            if (X_value>2||O_value>2){
                game_state.set_winCode(col+4);
                game_state.set_gameOver(true);
            }
        }
    }
    
    //for 3 consecutive on (0,0)to(2,2)
    if (game_state.get_gameBoard(0,0)!=Empty && game_state.get_gameBoard(0,0)==game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1)==game_state.get_gameBoard(2,2)){
        game_state.set_gameOver(true);
        game_state.set_winCode(7);
    }
    
    //for 3 consecutive on (2,0)to (0,2)
    if (game_state.get_gameBoard(2,0)!=Empty && game_state.get_gameBoard(2,0)==game_state.get_gameBoard(1,1) && game_state.get_gameBoard(1,1)==game_state.get_gameBoard(0,2)){
        game_state.set_gameOver(true);
        game_state.set_winCode(8);
    }
    
    // for draw
    
    bool state=true;
    for(int row=0;row<3;row++){
        for(int col=0;col<3;col++){
            if (game_state.get_gameBoard(row,col)==Empty){
                 state=false;}    
    }
    }
    if (game_state.get_gameOver()!=true){
        game_state.set_gameOver(state);
    }
}