#version 150
uniform mat4 modelViewProjectionMatrix;

in vec4 position;

uniform vec2 XYoffset;
uniform vec2 XYScale;
uniform vec2 Cmass;

void main()
{
    vec4 temppos= position;
    temppos.x = XYoffset.x + XYScale.x*temppos.x;
    temppos.y = XYoffset.y + XYScale.y*temppos.y;
    float Tx = XYoffset.x + XYScale.x*Cmass.x;
    float Ty = XYoffset.y + XYScale.y*Cmass.y;
    
    float px = temppos.x - Tx;
    float py = temppos.y - Ty;
//    SlopeSombra = -1.37;
//    ScaleXSombra = 1.0;
//    ScaleYSombra = 0.224;
//     temppos.x = (px*(2.0) +(0.04)*(15.0)*py) + Tx;
//     temppos.y = (0.04)*py + Ty;
	   vec4  prerot = position;
       prerot.x  = px*(.9)*cos(3.14159/2.0) + py*sin(3.14159/2.0) ;
	   prerot.y  = -px*(.9)*sin(3.14159/2.0) + py*cos(3.14159/2.0) ;

	   vec4  prerot2 = prerot;
	   prerot2.x = prerot.x;
	   prerot2.y = prerot.y*cos(0.3*3.14159/2.0) + prerot.z*sin(0.3*3.14159/2.0);
	   prerot2.z = -prerot.y*sin(0.3*3.14159/2.0) + prerot.z*cos(0.3*3.14159/2.0);

	   temppos = prerot2;
	   temppos.x = prerot2.x*cos(.4*3.14159/2.0) + prerot2.z*sin(.4*3.14159/2.0)+Tx;
	   temppos.y = prerot2.y +Ty;
	   temppos.z = -prerot2.x*sin(.4*3.14159/2.0) + prerot2.z*cos(.4*3.14159/2.0);// - .5*XYoffset.y;
       
    gl_Position = modelViewProjectionMatrix*temppos;
} 
