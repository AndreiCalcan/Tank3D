#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform float Time;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
   vec2 shifted = texcoord;
   shifted.x += Time * 0.1f;
   vec4 col = texture2D(texture_1, shifted);
   vec4 col2 = texture2D(texture_2, shifted);
   col = mix(col, col2, 0.5f);
   float alpha = col.a;
   if (alpha < 0.5f) {
    discard;
    }
    // TODO(student): Calculate the out_color using the texture2D() function.
    out_color = col;
}
