#version 150

uniform sampler2D tex0;
uniform float mouse_x_offset; // 0.5
in vec4 Vertex_UV;
out vec4 FragColor;

void main(){
   
   vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0);
   uv.y = (1.0 - uv.y);
   vec4 tc = texture(tex0, uv.xy );
   FragColor = tc;
} 