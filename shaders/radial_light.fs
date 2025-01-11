#version 330           // Adjust version if needed

// Our uniforms (variables passed from the CPU code):
uniform vec2 lightPos;  // Torch position in *screen* coordinates
uniform float radius;   // How far the torch reaches
uniform float intensity; // Brightness at the center

// Varyings from raylib's default vertex shader
in vec2 fragTexCoord;   // [0..1] texture coordinates
in vec4 fragColor;      // Not often needed here

// The texture(s) we read from
uniform sampler2D texture0;

// Final color we output
out vec4 finalColor;

void main()
{
    // Grab whatever is in the render texture at this pixel:
    vec4 color = texture(texture0, fragTexCoord);

    // Convert fragTexCoord from [0..1] range into actual screen coords.
    // If your screen is 1280x720, for example:
    vec2 screenCoord = vec2(fragTexCoord.x * 1280.0,
                            fragTexCoord.y * 720.0);

    // Distance from this pixel to the torch center:
    float dist = distance(screenCoord, lightPos);

    // Basic “1 - distance/radius” falloff. 
    // Then clamp so it won’t go below 0 or above 1.
    float factor = 1.0 - (dist / radius);
    factor = clamp(factor, 0.0, 1.0);

    // Multiply by intensity if you want the center brighter or dimmer.
    factor *= intensity;

    // Multiply the original color by our factor:
    finalColor = vec4(color.rgb * factor, color.a);
}
