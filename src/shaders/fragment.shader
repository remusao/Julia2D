#version 150 core

out vec4 color;
in vec4 pixPos;

uniform int iter;
uniform vec2 mu;

void main()
{
    vec2 z;

    z.x = (pixPos.x - 0.5) / pixPos.z;
    z.y = (pixPos.y - 0.5) / pixPos.z;

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
