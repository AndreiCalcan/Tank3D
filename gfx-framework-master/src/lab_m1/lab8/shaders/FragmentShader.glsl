#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[2];
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlight;
uniform float cut_off;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


float point_light_contribution(vec3 light_pos, vec3 light_color, vec3 light_dir)
{
	// TODO(student): Define ambient, diffuse and specular light components
    vec3 L = normalize(light_pos - world_position );
    vec3 V = normalize(eye_position - world_position );
    vec3 H = normalize( L + V );

    float ambient_light = 0.25 * material_kd;
    float diffuse_light = material_kd * max (dot(world_normal,L), 0);
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);
    }

    float obj_color;
    float light_att_factor;
    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if (spotlight == 1)
    {
        float spot_light = dot(-L, light_dir);
        float spot_light_limit = cos(cut_off);
        if (spot_light > cos(cut_off))
        {
	        // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	        // se calculeaza atenuarea luminii
            
 
            // Quadratic attenuation
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);

        }
        else
        {
            light_att_factor = 0;   
        }
    }
    else
    {
        float d = distance(world_position, light_pos);
        light_att_factor = 1 / pow(d, 2);
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.      
    obj_color = ambient_light + light_att_factor * (diffuse_light + specular_light);
    return obj_color;
}



void main()
{
    float color_sum = 0;
    for (int i = 0; i < 1; i++)
    {
       color_sum += point_light_contribution(light_position[i], object_color, light_direction[i]);
    }

    // TODO(student): Write pixel out color
    if(color_sum < 0.3)
    {
        color_sum = 0.3;
    }
    else if(color_sum > 0.3 && color_sum < 0.9995)
    {
        color_sum = 0.7;
    }
    else if(color_sum >= 0.9995)
    {
        color_sum = 0.9995;
    }
    out_color = vec4(color_sum * object_color, 1);

}
