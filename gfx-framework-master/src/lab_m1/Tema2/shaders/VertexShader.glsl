#version 330

// Input
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
out vec3 frag_color;
out float damage;

void main()
{
    damage = HP;
    frag_color = Color;
    vec3 new_pos = v_position;
    
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    if(1.0 - HP > 0.1)
    {
        vec3 world_pos = (Model * vec4(v_position,1)).xyz;
        vec3 dir = normalize(world_pos - Damage_source);
        float dist = (distance(world_pos, Damage_source));
        new_pos = world_pos + ((1 - HP)  / (dist + 9.5f)) * dir * 4.0f;
        gl_Position = Projection * View *vec4(new_pos, 1.0);
    }   
}
