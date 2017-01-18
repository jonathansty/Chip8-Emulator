#version 150

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D tex;
void main()
{
  float col2 = texture(tex,TexCoord).r;

  outColor = vec4(col2,col2,col2,1.0f);
  //outColor = texture(tex2,TexCoord);
}
