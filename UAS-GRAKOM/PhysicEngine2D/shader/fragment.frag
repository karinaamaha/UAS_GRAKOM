uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;

out vec4 fragColor;

void main()
{
    fragColor = mix(color1, color2, 0.5); 
}
