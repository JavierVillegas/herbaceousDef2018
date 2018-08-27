#version 150

uniform sampler2DRect TrunkTextu;
in vec2 texCoordVarying;
out vec4 outputColor;

void main()
{
  
  float ilufact =0.35;

  vec2 st = texCoordVarying;
  vec4 colorBase = texture(TrunkTextu, st);
//  ilufact = 4.0*(st.x/450.0-6.0/8.0) +0.5;
//  ilufact = pow(ilufact,1.5)*0.8;
  outputColor = vec4(colorBase.xyz*vec3(ilufact,ilufact,ilufact),1.0);
 //   outputColor = vec4(colorBase.xyz,1.0);
}
