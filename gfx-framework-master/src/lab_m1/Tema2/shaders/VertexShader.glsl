#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;
uniform float HP;
uniform vec3 Damage_source;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out float damage;

void main()
{
    // TODO(student): Send output to fragment shader
    damage = HP;
    frag_color = Color;
    // TODO(student): Compute gl_Position
    vec3 new_pos = v_position;
    
    if(1.0 - HP > 0.1)
    {
        vec3 world_pos = (Model * vec4(v_position,1)).xyz;
        vec3 dir = normalize(world_pos - Damage_source);
        float dist = (distance(world_pos, Damage_source));
        new_pos = v_position - (1 / (dist + 0.5)) * dir * 0.75f;
    }
    gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
