#version 120
uniform sampler2DRect TrunkTextu;

void main()
{
    float TW = 500.0;
    vec2 st = gl_TexCoord[0].st;
    vec4 colorBase = texture2DRect(TrunkTextu, st);
    //test
    float ilufact =1.0;
    
    // central tree
    if ((st.x>5.0/8.0*TW)&&(st.x<7.0/8.0*TW)) {
        ilufact = 4.0*(st.x/TW-6.0/8.0) +0.5;

    }
    // left tree
    else if ((st.x>3.0/8.0*TW)&&(st.x<5.0/8.0*TW)) {
        ilufact = 4.0*(st.x/TW-4.0/8.0) +0.5;

    }
    // right tree
    else if ((st.x>0.0)&&(st.x<1.0/8.0*TW)) {
        ilufact = 8.0*(st.x/TW-1.0/16.0) +0.5;

    }
    
    ilufact = pow(ilufact,1.5)*0.8;
    
    gl_FragColor = vec4(colorBase.xyz*vec3(ilufact*.813,ilufact*.822,ilufact*0.67),1.0);
}
