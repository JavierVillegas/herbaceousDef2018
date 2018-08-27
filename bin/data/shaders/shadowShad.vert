#version 120
uniform vec2 XYoffset;
uniform vec2 XYScale;
uniform vec2 Cmass;

void main()
{
    vec4 temppos= gl_Vertex;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
    float Tx = XYoffset.x + XYScale.x*Cmass.x;
    float Ty = XYoffset.y + XYScale.y*Cmass.y;
    
    float px = temppos.x - Tx;
    float py = temppos.y - Ty;
//    SlopeSombra = -1.37;
//    ScaleXSombra = 1.0;
//    ScaleYSombra = 0.224;
     temppos.x = (px*(2.0) +(0.04)*(15.0)*py) + Tx;
     temppos.y = (0.04)*py + Ty;
    
    
    gl_Position = gl_ModelViewProjectionMatrix*temppos;
} 
