#version 150

uniform sampler2DRect TrunkTextu;
in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
    float TW = 500.0;
    vec2 st = texCoordVarying;
    vec4 colorBase = texture(TrunkTextu, st);
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
    
    outputColor = vec4(colorBase.w*colorBase.xyz*vec3(ilufact*.813,ilufact*.822,ilufact*0.67),colorBase.w);
}
