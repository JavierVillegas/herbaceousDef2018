#version 120
uniform vec2 XYoffset;
uniform vec2 XYScale;
uniform vec2 Cmass1;
uniform vec2 Cmass2;
uniform vec2 Cmass3;

varying vec2 TheRoots1;
varying vec2 TheRoots2;
varying vec2 TheRoots3;
void main()
{
    TheRoots1.x = XYoffset.x + XYScale.x*Cmass1.x;
    TheRoots1.y = XYoffset.y + XYScale.y*Cmass1.y;
    TheRoots2.x = XYoffset.x + XYScale.x*Cmass2.x;
    TheRoots2.y = XYoffset.y + XYScale.y*Cmass2.y;
    TheRoots3.x = XYoffset.x + XYScale.x*Cmass3.x;
    TheRoots3.y = XYoffset.y + XYScale.y*Cmass3.y;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
} 
