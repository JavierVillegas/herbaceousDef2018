#version 120
uniform sampler2D tex;
uniform vec2 XYDim;

varying float TheAngle;
varying vec2 Hojacord;
void main()
{
    //gl_FragColor =vec4(gl_TexCoord[0].s,gl_TexCoord[0].t,0.0,1.0);
    
    
    vec2 SunPos = vec2(XYDim.x,XYDim.y/4.0);
    float distaTosun =distance(Hojacord,SunPos);
    
    mat2 RotMat = mat2(cos(TheAngle),sin(TheAngle),
                       -sin(TheAngle),cos(TheAngle));
    vec2 NewCords = RotMat*(gl_TexCoord[0].st-vec2(0.5)) + vec2(0.5);
    vec4 thecolor = texture2D(tex,NewCords);

    float facto = pow(1.0 - distaTosun/XYDim.x,.4);//.4
    // con LeavesNew.pnga
//    gl_FragColor = vec4(thecolor.r*.4*facto,
//                        thecolor.g*.3*facto,
//                       thecolor.b*.2*facto,
//                        thecolor.a);
    gl_FragColor = vec4(thecolor.r*.7*facto,
                        thecolor.r*.7*facto,
                        thecolor.b*.2,
                        thecolor.a*.8);
//    gl_FragColor = vec4(thecolor.g*.9*facto,
//                        thecolor.g*.7*facto,
//                        thecolor.b*.2,
//                        thecolor.a);
    // con newHoja.png
//    gl_FragColor = vec4(thecolor.r*1.5,
//                        thecolor.g*0.4,
//                        2.0*thecolor.b*thecolor.b*0.3,
//                        thecolor.a);

}