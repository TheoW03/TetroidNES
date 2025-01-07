#version 150

uniform sampler2D tex0;
uniform float mouse_x_offset; // 0.5
uniform float time;

uniform float density;
uniform float opacityScanline;
uniform float opacityNoise;
uniform float curvature;
uniform float vigantteWidth;
uniform vec2 Res;

uniform float brightness;
uniform float warp_brightness;

in vec4 Vertex_UV;
out vec4 FragColor;

float random (vec2 st) {
   //these are constants dont need to parameterized i dont think
   
   float noiseScale = 43758.5453123;
   float noiseY = 78.233;
   float noiseX = 12.9898;
    return fract(sin(dot(st.xy,
                         vec2(noiseX,noiseY)))*
        noiseScale);
}

vec2 curve(vec2 uv, float warp){
   vec2 dc = abs(0.5-uv);
    dc *= dc;
    
    uv.x -= 0.5; uv.x *= 1.0+(dc.y*(0.3*warp)); 
    uv.x += 0.5;
    uv.y -= 0.5; uv.y *= 1.0+(dc.x*(0.4*warp)); 
    uv.y += 0.5;
    return uv;
}

void main(){
   //TODO: Uniform these
   vec4 ntsc_color = vec4(1.2, 1.2, 1.4, 1.0);
   vec4 black_color = vec4(0.0,0.0,0.0,1.0);
   
   vec2 uv = gl_FragCoord.xy / Res;
   uv.y = (1.0 - uv.y);


   vec2 dc = abs(0.5-uv);
   dc *= dc;
   vec4 tc = texture(tex0, uv.xy ); //texture
   vec2 curve = uv * 2. - 1.;
   float offset = length(curve) / curvature;
   curve += curve * offset * offset;
   curve = curve * 0.5 + 0.5;
      
     
   vec2 vignetteThreshold = vigantteWidth / Res.xy;
   vec2 vignette = smoothstep(vec2(0), vignetteThreshold, 1. - abs(curve * 2. - 1.));
   //adjusts color and addes the warp affefct
   tc = tc * vignette.x * vignette.y;
   tc = pow(tc, ntsc_color) * brightness + warp_brightness;

   float count = Res.y * density;
   vec2 sl = vec2(sin(uv.y * count), cos(uv.y * count));
   vec4 scanlines = vec4(sl.x, sl.y, sl.x, 1.0);
   tc += tc * scanlines * opacityScanline;
   tc += tc * vec4(random(uv*time)) * opacityNoise;
   vec3 scanlinelerp =  mix(tc.rgb, black_color.xyz, opacityScanline);
   FragColor = vec4(scanlinelerp, 1.0);

} 