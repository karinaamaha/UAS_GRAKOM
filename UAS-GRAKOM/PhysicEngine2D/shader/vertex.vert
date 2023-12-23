layout(location = 0) in vec2 position;  // Menentukan koordinat untuk objek segitiga

uniform float translation;  // Variabel yang bisa diubah nilainya dari program utama

void main()
{
    gl_Position = vec4(position.x + translation, position.y, 0.0, 1.0);  // Menentukan posisi titik dalam ruang 3D
}
