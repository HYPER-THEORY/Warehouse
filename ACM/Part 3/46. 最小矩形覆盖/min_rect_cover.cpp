double min_rect_cover(const convex_polygon& cp, line& l1, line& l2, line& l3, line& l4) {
	double opti = numeric_limits<double>::max();
	vec2 orig, norm;
	vec2 vtx1, vtx2, vtx3, vtx4;
	for (int i = 0; i < cp.s; ++i) {
		vec2 dir = cp.pts[i] - cp.pts[cp.next(i)];
		vec2 v1, v2, v3;
		double d1 = 0, d2 = 0, d3 = 0;
		for (int j = 0; j < cp.s; ++j) {
			double d = (cp.pts[i] - cp.pts[j]) * dir;
			if (d > d1) {
				d1 = d;
				v1 = cp.pts[j];
			}
			if (d < d2) {
				d2 = d;
				v2 = cp.pts[j];
			}
			double c = (cp.pts[i] - cp.pts[j]) ^ dir;
			if (c < d3) {
				d3 = c;
				v3 = cp.pts[j];
			}
		}
		double area = -d3 * (d1 - d2) / dir.mag_2();
		if (area < opti) {
			opti = area;
			orig = dir;
			vtx1 = v1;
			vtx2 = v2;
			vtx3 = v3;
			vtx4 = cp.pts[i];
		}
	}
	norm = vec2(-orig.y, orig.x);
	l1 = line(vtx1, norm);
	l2 = line(vtx2, norm);
	l3 = line(vtx3, orig);
	l4 = line(vtx4, orig);
	return opti;
}