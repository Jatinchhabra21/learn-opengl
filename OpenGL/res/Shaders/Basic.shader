#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform float u_Offset;
layout(location = 1) out vec4 position_color;

void main()
{
   gl_Position = vec4(position.x-u_Offset, position.y - u_Offset, position.z, position.w);
   position_color = position;
}
        
#shader fragment        
#version 330 core

layout(location = 0) out vec4 color;
layout (location = 1) in vec4 position_color;

uniform vec4 u_Color;

void main()
{
   color = position_color;
}