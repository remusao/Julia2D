#version 150 core

out vec4 color;
in vec2 pixPos;

uniform int iter;
uniform vec2 mu;
uniform float zoom;

void main()
{
    vec2 z;

    z.x = (pixPos.x - 0.5) * zoom;
    z.y = (pixPos.y - 0.5) * zoom;

    int i;
    for (i = 0; i < iter; ++i)
    {
        float x = (z.x * z.x - z.y * z.y) + mu.x;
        float y = (z.y * z.x + z.x * z.y) + mu.y;

        if ((x * x + y * y) > 4.0)
            break;
        z.x = x;
        z.y = y;
    }
    float c = (float(i) / float(iter));
    color = vec4(c, c, c, 1.0);
}
