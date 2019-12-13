//
//  readobj.cpp
//  GLUT_test
//
//  Created by Layer on 2019/9/6.
//  Copyright © 2019 Layer. All rights reserved.
//

#include "readobj.hpp"

readobj::readobj(string name){
    filename=name;
    getnums();
}

void readobj::getnums(){
    fstream file(filename.c_str());
    if(!file.is_open())
        cout<<"Open filed!"<<endl;
    else{
        char type;
        float a,b,c;
        while(!file.eof()){
            file>>type>>a>>b>>c;
            //cout<<type<<" "<<a<<" "<<b<<" "<<c<<endl;
            if(type=='v'){
                vector<float>temp{a,b,c};
                varr.push_back(temp);
            }
            else if(type=='f'){
            vector<int>temp{(int)a,(int)b,(int)c};
            farr.push_back(temp);
            }
        }
   }
    file.close();
}
