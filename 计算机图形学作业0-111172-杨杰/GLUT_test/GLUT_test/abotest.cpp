//
//  abotest.cpp
//  GLUT_test
//
//  Created by Layer on 2019/9/8.
//  Copyright © 2019 Layer. All rights reserved.
//

/*
#include<stdio.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <iostream>

GLfloat afVertex[][3]=
{
    {-0.75,-0.5,0.0},
    {0.75,-0.5,0.0},
    {0.0,0.75,0.0}
};

const char* cpVShader = {
    "#version 150\n"
    ""
    "in vec3 vPos;"
    ""
    "void main() {"
    "  gl_Position = vec4(vPos,1);"
    "}"
};

const char* cpFShader = {
    "#version 150\n"
    ""
    "out vec4 fColor;"
    ""
    "void main() {"
    "  fColor = vec4( 1, 1, 1, 1 );"
    "}"
};


GLuint uVbo;
GLuint uPos;
GLuint uProgram;

bool AttachProgram( GLuint uProgram, const char* cpShader, GLenum eShaderType )
{
    GLuint  uShader;
    GLint   iCompleted;
    
    uShader = glCreateShader( eShaderType );
    glShaderSource( uShader, 1, &cpShader, NULL );
    glCompileShader( uShader );
    glGetShaderiv( uShader, GL_COMPILE_STATUS, &iCompleted );
    
    if ( !iCompleted )
    {
        GLint  len;
        char*  msg;
        glGetShaderiv( uShader, GL_INFO_LOG_LENGTH, &len );
        msg = (char*) malloc( len );
        glGetShaderInfoLog( uShader, len, &len, msg );
        fprintf( stderr, "Vertex shader compilation failure:\n%s\n", msg );
        free( msg );
        return false;
    }else
    {
        glAttachShader( uProgram, uShader );
        return true;
    }
}
void initGL()
{
    
    glGenBuffers(1,&uVbo );
    glBindBuffer(GL_ARRAY_BUFFER,uVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(afVertex), afVertex, GL_STATIC_DRAW );
    
    uProgram = glCreateProgram();
    AttachProgram( uProgram, cpVShader , GL_VERTEX_SHADER );
    AttachProgram( uProgram, cpFShader , GL_FRAGMENT_SHADER );
    glLinkProgram( uProgram );
    
    uPos = glGetAttribLocation( uProgram, "vPos" );
    
    glClearColor( 0.0, 0.0, 1.0, 1.0 );
    
}

void display()
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    glUseProgram(uProgram);
    glBindBuffer( GL_ARRAY_BUFFER, uVbo );
    glVertexAttribPointer( uPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray( uPos );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    
    glutSwapBuffers();
}

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
}

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}



//int main( int argc, char* argv[] )
//{
//    glutInit( &argc, argv );
//    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
//
//    glutCreateWindow( argv[0] );
//    glewInit();
//
//    initGL();
//
//    glutDisplayFunc( display );
//    glutReshapeFunc( reshape );
//    glutKeyboardFunc( keyboard );
//    glutMainLoop();
//}
*/
