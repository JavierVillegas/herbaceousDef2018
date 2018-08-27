#version 120
uniform vec2 XYoffset;
uniform vec2 XYScale;

varying float TheAngle;
varying vec2 Hojacord;

void main()
{
	  gl_PointSize  = 16.0;
    vec4 temppos= gl_Vertex;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
    TheAngle = temppos.z;
    Hojacord = temppos.xy;
    temppos.z = 0.0;
  //  gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix*temppos;
} 

