#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in vec2 screenCoord;
in vec3 fragPosition;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

//custom uniform
uniform float iTime;

// Output fragment color
out vec4 finalColor;


// NOTE: Add your custom variables here

void main()  {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    const vec3 lightDir = vec3(-.6, -.4, -.2);

    // basic view
    //finalColor = texelColor * colDiffuse * fragColor;

    //Normals view
    //finalColor = vec4( fragNormal.xyz, 1);

    // basic lighting view
    finalColor = vec4( fragColor.xyz - dot(fragNormal, lightDir)*.1, 1);

    //ScreenSpace position
    //finalColor = vec4(screenCoord, 0, 1);
}