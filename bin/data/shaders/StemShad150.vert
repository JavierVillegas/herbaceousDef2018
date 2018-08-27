#version 150
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

uniform vec2 XYoffset;
uniform vec2 XYScale;

out vec2 texCoordVarying;

void main()
{
    vec4 temppos= position;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
      texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix*temppos;
  
} 
