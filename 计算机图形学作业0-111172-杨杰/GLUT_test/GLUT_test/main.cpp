//
//  main.cpp
//  GLUT_test
//
//  Created by Layer on 2019/9/2.
//  Copyright © 2019 Layer. All rights reserved.
//
#include "math.h"
#include <iostream>
#include "readobj.hpp"
#include <GLUT/GLUT.h>
using namespace std;

float matchange=0;//改变颜色
float positionx=1;//光源x
float positiony=1;//光源y
float rotateangle=0;//旋转角度
float rotatex=0;//旋转x
float rotatey=0;//旋转y
float rotatez=1;//选择z
float sizenum=1;//改变大小
bool colorchange=false;//自动变色
bool rotatechange=false;//自动旋转
bool allchange=false;//炫彩变换

vector<vector<float>>varr;//obj文件读取的v
vector<vector<int>>farr;//obj文件读取的f
int flag=0;//选择茶壶或者读取的obj文件

void CreateDisplayLists(){
    glNewList(1,GL_COMPILE);
    for (int i=0;i<farr.size();i++){
        glBegin(GL_TRIANGLES);
        glVertex3f(varr[farr[i][0]-1][0], varr[farr[i][0]-1][1], varr[farr[i][0]-1][2]);
        glVertex3f(varr[farr[i][1]-1][0], varr[farr[i][1]-1][1], varr[farr[i][1]-1][2]);
        glVertex3f(varr[farr[i][2]-1][0], varr[farr[i][2]-1][1], varr[farr[i][2]-1][2]);
        glEnd();
        }
        glEndList();
}

void init(void)
{

    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat position[] = {1.0, 1.0, 5.0, 0.0};
    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat local_view[] = {0.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
    
    glFrontFace(GL_CW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
}

//鼠标相机控制需要的参数
bool mouseLeftDown;//左键按下
bool mouseRightDown;//右键按下
bool mouseMiddleDown;//前一刻鼠标位置
float mouseX, mouseY;//前一刻鼠标位置
float cameraDistanceX;//右键物体平移
float cameraDistanceY;//右键物体平移
float cameraAngleX;//左键旋转角度
float cameraAngleY;//左键旋转角度

void display(void){
    glTranslatef(cameraDistanceX, cameraDistanceY, 0);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(8, 8, 0.0);
    
    GLfloat mat[4];//颜色改变
    mat[0] = matchange; mat[1] = 0.2; mat[2] = 0.9;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
    
    GLfloat position[4];//光线改变
    position[0]=positionx;position[1]=positiony;position[2]=5;position[3]=0;
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    
    glRotatef(rotateangle,rotatex,rotatey,rotatez);//旋转
    glScalef(sizenum,sizenum,sizenum);//缩放
    if(flag==0)//判断是茶壶还是读取到的obj文件
        glutSolidTeapot(3.0);
    else if(flag==2){
        for (int i=0;i<farr.size();i++){
            glBegin(GL_TRIANGLES);
            if(allchange){//炫彩的obj文件
                mat[0] = rand()/double(RAND_MAX);
                mat[1] = rand()/double(RAND_MAX);
                mat[2] = rand()/double(RAND_MAX);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
            }
            glVertex3f(varr[farr[i][0]-1][0], varr[farr[i][0]-1][1], varr[farr[i][0]-1][2]);
            glVertex3f(varr[farr[i][1]-1][0], varr[farr[i][1]-1][1], varr[farr[i][1]-1][2]);
            glVertex3f(varr[farr[i][2]-1][0], varr[farr[i][2]-1][1], varr[farr[i][2]-1][2]);
            glEnd();
        }
    }
    else if(flag==3){
        glCallList(1);
    }
    glPopMatrix();
    glFlush();
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(0.0, 16.0, 0.0, 16.0*(GLfloat)h/(GLfloat)w,
                -10.0, 10.0);
    else
        glOrtho(0.0, 16.0*(GLfloat)w/(GLfloat)h, 0.0, 16.0,
                -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27://ESC退出
            exit(0);
            break;
        case 99://c切换颜色
            if(matchange<=0.9) matchange+=0.1;
            else matchange=0;
            glutPostRedisplay();
            break;
        case 119://w方向上键
            cout<<key;
            positiony+=10;
            glutPostRedisplay();
            break;
        case 115://s方向下键
            cout<<key;
            positiony-=10;
            glutPostRedisplay();
            break;
        case 97://a方向左键
            cout<<key;
            positionx-=10;
            glutPostRedisplay();
            break;
        case 100://d方向右键
            cout<<key;
            positionx+=10;
            glutPostRedisplay();
            break;
        case 114://r旋转角度
            cout<<key;
            rotateangle+=5;
            glutPostRedisplay();
            break;
        case 116://t
            cout<<key;
            rotatex+=5;
            glutPostRedisplay();
            break;
        case 121://y
            cout<<key;
            rotatey+=5;
            glutPostRedisplay();
            break;
        case 117://u
            cout<<key;
            rotatez+=5;
            glutPostRedisplay();
            break;
        case 111://o缩小
            if(sizenum>=0)
                sizenum-=1;
            glutPostRedisplay();
            break;
        case 112://p放大
            sizenum+=1;
            glutPostRedisplay();
            break;
        case 113://q动态变色
            colorchange=!colorchange;
            glutPostRedisplay();
            break;
        case 101://e自动旋转
            rotatechange=!rotatechange;
            glutPostRedisplay();
            break;
        case 102://f炫彩
            allchange=!allchange;
            glutPostRedisplay();
            break;
        default:
            cout<<"Unhandled key press  "<<key<<"."<<endl;
            break;
    }
}

void SpecialKey(GLint key,GLint x,GLint y)
{
    if(key==GLUT_KEY_UP)
    {
        positiony+=10;
        glutPostRedisplay();
    }
    if(key==GLUT_KEY_LEFT)
    {
        positionx-=10;
        glutPostRedisplay();
    }
    if(key==GLUT_KEY_DOWN)
    {
        positiony-=10;
        glutPostRedisplay();
    }
    if(key==GLUT_KEY_RIGHT)
    {
        positionx+=10;
        glutPostRedisplay();
    }
}

void timerFunc(int value){
    if(colorchange){//自动变色
        if(matchange<=0.9) matchange+=0.1;
        else matchange=0;
    }
    if (rotatechange) {//自动旋转
        rotateangle+=5;
    }
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 1);
}

//鼠标缩放实现
void OnMouse(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){//放大
        sizenum+=1;
        glutPostRedisplay();
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN){//缩小
        sizenum-=1;
        glutPostRedisplay();
    }
    else if(button==GLUT_MIDDLE_BUTTON&&state==GLUT_DOWN){//旋转
        rotatechange=!rotatechange;
        glutPostRedisplay();
    }
}

//鼠标状态获取
void mouse(int button, int state, int x, int y){
    mouseX = x;
    mouseY = y;
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }
    
    else if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
    
    //滚轮缩放模型
    else if (state == GLUT_UP && button == GLUT_MIDDLE_BUTTON)
    {
        sizenum++;
        glutPostRedisplay();
    }
    
    else if (state == GLUT_UP && button == GLUT_MIDDLE_BUTTON)
    {
        sizenum--;
        glutPostRedisplay();
    }
}

void mouseMotion(int x, int y){
    cameraAngleX = cameraAngleY = 0;
    cameraDistanceX = cameraDistanceY = 0;
    
    if (mouseLeftDown){
        cameraAngleY += (x - mouseX) * 0.1;
        cameraAngleX += (y - mouseY) * 0.1;
        mouseX = x;
        mouseY = y;
    }
    if (mouseRightDown){
        cameraDistanceX = (x - mouseX) * 0.002;
        cameraDistanceY = -(y - mouseY) * 0.002;
        mouseY = y;
        mouseX = x;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    /*
     请根据显示内容选择屏蔽flag值
     esc：退出
     c：更换颜色
     w、s、a、dh或者键盘上下左右：改变光源位置
     r：旋转
     t、y、u：改变旋转轴
     o、p：缩小放大
     f：读取的兔子的炫彩展示
     e：自动旋转
     q：自动变色
     鼠标左键：旋转
     鼠标右键：移动
     鼠标滑轮：放大缩小
     */
    flag=0;//显示茶壶
//    flag=1;//显示读取文件
    flag=3;//vbo
    
    
    readobj test("/Users/layer/Downloads/ComputerGraphics/编程作业0/作业0/bunny_1k.obj");
    varr=test.getvarr();
    farr=test.getfarr();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(300,150);
    glutCreateWindow("hello world");
    if(flag==3)
        CreateDisplayLists();
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc(SpecialKey);
    //glutMouseFunc(OnMouse);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(33, timerFunc, 1);
    glutMainLoop();
    
    
    return 0;
}


