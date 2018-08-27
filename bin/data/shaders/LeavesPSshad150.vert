#version 150
uniform mat4 modelViewProjectionMatrix;

in vec4 position;


uniform vec2 XYoffset;
uniform vec2 XYScale;

out float TheAngle;
out vec2 Hojacord;


void main()
{
	  gl_PointSize  = 25.0;
    vec4 temppos= position;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
    TheAngle = temppos.z;
    Hojacord = temppos.xy;
    temppos.z = 0.0;
  //  gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = modelViewProjectionMatrix*temppos;
} 

