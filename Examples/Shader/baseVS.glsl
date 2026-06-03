#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// raylib default
uniform mat4 matModel;
uniform mat4 matNormal;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec2 screenCoord;

uniform float iTime;

// NOTE: Add your custom variables here

void main()  {
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragPosition = vertexPosition;

    fragNormal = vertexNormal.xyz;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition+vec3(sin(vertexPosition.z*2-iTime*3), 1, 0), 1.0);
    screenCoord = vec2(gl_Position.xy);
}