#shader vertex
#version 300 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 v_Color;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;

out vec3 o_Normal;
out vec3 o_Position;
out vec3 o_ViewPos;
out vec4 o_Color;

void main(){
    gl_Position = u_MVP * position;
    o_Normal = normalize(vec3(0.0, 0.0, 0.0));
    o_Position = normalize(position.xyz);
    o_ViewPos = (u_View * (u_Model * position)).xyz;
    o_Color = v_Color;
}

#shader fragment
#version 300 es

precision mediump float;
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform vec3 u_LightDir;

in vec3 o_Normal;
in vec3 o_Position;
in vec3 o_ViewPos;
in vec4 o_Color;

void main(){
    vec4 f_Color = vec4(0.3, 0.3, 0.8, 1.0);
    float ambientStrength = 0.4;
    
    vec3 lightColor = vec3(0.8, 0.8, 1.0);
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 lightDir = normalize(u_LightDir);
    vec3 xTan = dFdx(o_ViewPos);
    vec3 yTan = dFdy(o_ViewPos);
    vec3 norm = normalize(cross(xTan, yTan));

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * u_Color * o_Color;
    vec4 rimLight = (max(dot(norm, normalize(vec3(-1.0, -1.0, -1.0))), 0.0) * f_Color);

    vec4 result = (vec4(ambient, 1.0) + diffuse + rimLight) * u_Color * o_Color;
    color = result;
}
