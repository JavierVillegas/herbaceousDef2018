#version 120
uniform vec2 XYoffset;
uniform vec2 XYScale;

void main()
{
    vec4 temppos= gl_Vertex;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
    
    gl_Position = gl_ModelViewProjectionMatrix*temppos;
} 
