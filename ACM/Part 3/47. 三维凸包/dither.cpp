void dither(vec3 vtx, double a) {
	vtx.x += a * (2 * RND - 1);
	vtx.y += a * (2 * RND - 1);
	vtx.z += a * (2 * RND - 1);
}
