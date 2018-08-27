#version 150
uniform sampler2DRect justTree;
uniform vec2 steps;
in vec2 texCoordVarying;
out vec4 outputColor;
void main()
{
    vec2 st = texCoordVarying;							
    vec4 colorBase = texture(justTree, st);
  
  if (colorBase.w > 0.0){
	vec4 acum = colorBase;
	int count = 1;
    vec4 colorBaseleft = texture(justTree, st + vec2(-steps.x,0.0));
	if(colorBaseleft.w > 0.0){acum+=colorBaseleft; count++;}

    vec4 colorBaseright = texture(justTree, st + vec2(steps.x,0.0));
	if(colorBaseright.w > 0.0){acum+=colorBaseright; count++;}
    
	vec4 colorBaseup = texture(justTree, st + vec2(0.0,steps.y));
    if(colorBaseup.w > 0.0){acum+=colorBaseup; count++;}

	vec4 colorBasedown = texture(justTree, st + vec2(0.0,-steps.y));
	if(colorBasedown.w > 0.0){acum+=colorBasedown; count++;}
    
	vec4 colorBaseleftUp = texture(justTree, st + vec2(-steps.x,steps.y));
	if(colorBaseleftUp.w > 0.0){acum+=colorBaseleftUp; count++;}

    vec4 colorBaserightUp = texture(justTree, st + vec2(steps.x,steps.y));
	if(colorBaserightUp.w > 0.0){acum+=colorBaserightUp; count++;}

    vec4 colorBaseleftDown = texture(justTree, st + vec2(-steps.x,-steps.y));
	if(colorBaseleftDown.w > 0.0){acum+=colorBaseleftDown; count++;}

    vec4 colorBaserightDown = texture(justTree, st + vec2(steps.x,-steps.y));
	if(colorBaserightDown.w > 0.0){acum+=colorBaserightDown; count++;}
    
    vec4 theAv = acum/float(count);
    outputColor = vec4(theAv.xyz,theAv.w);
  }
  else{
    outputColor = colorBase;
  }
}
