vector<pair<double, int>> v;
// traverse all polygons, if polygon's x_min < x < x_max,
// calculate y coordinates of intersections and push {y0, 1}, {y1, -1},
// {y2, 1} ... {yn, -1} (y0 < y1 < ... < yn) to v.
int s = 0;
double last = 0, l = 0;
sort(v.begin(), v.end());
for (auto& i : v) {
	if (s == 0) last = i.first;
	s += i.second;
	if (s == 0) l += (i.first - last);
}
