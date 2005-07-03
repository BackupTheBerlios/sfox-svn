uniform samplerCube texCube;
varying vec3 R;

void main (void)
{
  gl_FragColor = textureCube(texCube, R);
}
