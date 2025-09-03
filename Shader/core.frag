#version 330 core

in vec3 ourColor;      // color que viene del VBO
out vec4 color;        // salida final del fragment shader

// Uniforms para permitir color fijo
uniform bool useSolidColor;  // true = usar solidColor, false = usar ourColor
uniform vec3 solidColor;     // color fijo (ej: rojo para ojos)

void main()
{
    vec3 c = useSolidColor ? solidColor : ourColor;
    color = vec4(c, 1.0);
}
