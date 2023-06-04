/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   cell.h
 * Author: MSCS
 *
 * Created on 12 March 2020, 8:39 AM
 */

#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include <iostream>
#define OB "E8"  

#ifndef _SLP_CL_EG 
   #define _SLP_CL_EG
   #define C(x) #x
   #ifdef _WIN32
     	#define SLP(t) Sleep(t*1000)
     	#include <windows.h>
     	#define CL C(cls)
   #endif
   #ifdef unix
      #define SLP(t) sleep(t)
      #include <unistd.h>
      #define CL C(clear)
   #endif
#endif

using namespace std;
int getAdjacentIndex(const int& i);
class cell{
    char marble;
    vector<cell*> adjacent;//six directions: north-east clockwise to north-west
    string location;
    vector<int> distanceToEdge; //how far is the boundary from this cell.

public:
    char getMarble() const;
    void setMarble(const char&);//set marble to either O, @ or + or #.
    cell* getAdjacent(const int&) const;
    void setAdjacent(cell*,const int&);//only set each neighbour once.
    string getLocation() const;
    static bool validateLocation(const string& l);
    void setLocation(const string& l);
    void setDistanceToEdge(const int&);
    int getDistanceToEdge(const int&) const;
    cell();//set all 6 neighbours to null.
};

#endif /* CELL_H */

