#version 120
uniform sampler2DRect tex0;

varying vec2 TheRoots1;
varying vec2 TheRoots2;
varying vec2 TheRoots3;

void main()
{
    vec2 st = gl_TexCoord[0].st;
    vec4 colorBase = texture2DRect(tex0, st);

    vec2 error1 = TheRoots1 - st;
    vec2 error2 = TheRoots2 - st;
    vec2 error3 = TheRoots3 - st;
    
    float dista1 = dot(error1/vec2(4.0,.5),error1/vec2(4.0,.5));
    float dista2 = dot(error2/vec2(4.0,.5),error2/vec2(4.0,.5));
    float dista3 = dot(error3/vec2(4.0,.5),error3/vec2(4.0,.5));
    float Thefac = 1.0;
    if ((dista1 <100.0) && (dista1<dista2) && (dista1<dista3) ) {
        Thefac = 1.0 -(100.0 -dista1)/100.0;
        gl_FragColor = vec4(colorBase.x*(.6 + Thefac*.4),
                            colorBase.y*(.6 + Thefac*.4),
                            colorBase.z*(.7 + Thefac*.3),
                            1.0);
    }
    else if ((dista2 <200.0) && (dista2<dista1) && (dista2<dista3) ) {
        Thefac = 1.0 -(200.0 -dista2)/200.0;
        gl_FragColor = vec4(colorBase.x*(.7 + Thefac*.3),
                            colorBase.y*(.6 + Thefac*.4),
                            colorBase.z*(.9 + Thefac*.1),
                            1.0);
    }

    else if ((dista3 <200.0) && (dista3<dista1) && (dista3<dista2) ) {
        Thefac = 1.0 -(200.0 -dista3)/200.0;
        gl_FragColor = vec4(colorBase.x*(.7 + Thefac*.3),
                            colorBase.y*(.6 + Thefac*.4),
                            colorBase.z*(.9 + Thefac*.1),
                            1.0);
    }
    
    else{
        gl_FragColor = vec4(colorBase.x*(.7 + Thefac*.3),
                            colorBase.y*(.6 + Thefac*.4),
                            colorBase.z*(.9 + Thefac*.1),
                            1.0);
    }
    

    }
