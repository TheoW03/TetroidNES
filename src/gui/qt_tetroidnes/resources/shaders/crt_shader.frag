#version 150

uniform sampler2D tex0;
uniform float mouse_x_offset; // 0.5
in vec4 Vertex_UV;
out vec4 FragColor;

void main(){
   float density = 1.3;
   float opacityScanline = .2;
   vec2 Res = vec2(800.0, 600.0);

   vec2 uv = gl_FragCoord.xy / Res;
   uv.y = (1.0 - uv.y);

   vec4 tc = texture(tex0, uv.xy );
   float count = Res.y * density;
   vec2 sl = vec2(sin(uv.y * count), cos(uv.y * count));
   vec4 scanlines = vec4(sl.x, sl.y, sl.x, 1.0);
   tc += tc * scanlines * opacityScanline;
   FragColor = tc;
} 