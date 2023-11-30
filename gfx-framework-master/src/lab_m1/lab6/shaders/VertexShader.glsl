#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 1) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_color_normal;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color + sin(Time);
    frag_color_normal = v_normal;
    vec3 pos = v_position + cos(Time);
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
