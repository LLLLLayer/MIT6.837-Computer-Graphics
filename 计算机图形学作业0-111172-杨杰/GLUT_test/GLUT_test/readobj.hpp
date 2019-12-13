//
//  readobj.hpp
//  GLUT_test
//
//  Created by Layer on 2019/9/6.
//  Copyright © 2019 Layer. All rights reserved.
//

#ifndef readobj_hpp
#define readobj_hpp

#include <stdio.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class readobj{
private:
    vector<vector<float>> varr;//存放点的数组；
    vector<vector<int>> farr;//存放面的数组；
    string filename;
public:
    readobj(string name);
    void getnums();
    void display();
    vector<vector<float>> getvarr(){return varr;}
    vector<vector<int>> getfarr(){return farr;}
};

#endif /* readobj_hpp */
