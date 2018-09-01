#version 150
uniform sampler2D tex;

uniform vec2 XYDim;

in float TheAngle;
in vec2 Hojacord;


out vec4 outputColor;
void main()
{
    //gl_FragColor =vec4(gl_TexCoord[0].s,gl_TexCoord[0].t,0.0,1.0);
    
    
	vec2 dirSun = vec2(0.8, 0.2);
	dirSun = normalize(dirSun);
    
    mat2 RotMat = mat2(cos(TheAngle),sin(TheAngle),
                       -sin(TheAngle),cos(TheAngle));
    vec2 NewCords = RotMat*(gl_PointCoord-vec2(0.5)) + vec2(0.5);
    vec4 thecolor = texture(tex,NewCords);

  float facto = 5.0*dot(dirSun, gl_PointCoord - vec2(0.5));
  if (facto < 0.0){facto = 0.0;}
    // con LeavesNew.pnga
//    gl_FragColor = vec4(thecolor.a*thecolor.r*.4*facto,
//                        thecolor.a*thecolor.g*.3*facto,
//                       thecolor.a*thecolor.b*.2*facto,
//                        1.0);
	if(thecolor.a > 0.0){
	    outputColor = vec4(thecolor.r + 0.1*facto,
                        thecolor.r + 0.1*facto,
                        thecolor.b,
                        1.0);
    //outputColor = vec4(thecolor.r*.7*facto,
    //                    thecolor.r*.7*facto,
    //                    thecolor.b*.2,
    //                    1.0);
						}

	else{
	outputColor = vec4(thecolor.r*.7*facto,
                        thecolor.r*.7*facto,
                        thecolor.b*.2,
                        thecolor.a*.9);
	}

    // outputColor = vec4(thecolor.r,
                        // thecolor.g,
                        // thecolor.b,
                        // 1.0);						
						
						
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