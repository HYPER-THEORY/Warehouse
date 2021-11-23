class point_set {

public:

	int s;

	line* dls;
	vec2* pts;

	point_set() { }
	
	// left side of directed lines
	point_set(line* dls, int n) {
		sort(dls, dls + n, [](const line& a, const line& b) {
			double ang = a.dir.atan() - b.dir.atan();
			if (abs(ang) > 0.1) return ang < 0;
			return eq(a.dir ^ b.dir, 0) ? b.relative_location(a.pt) == 1 : (a.dir ^ b.dir) > 0;
		});
		deque<vec2> ps;
		deque<line> ls;
		ls.push_back(dls[0]);
		for (int i = 1; i < n; ++i) {
			if (dls[i].parallel(dls[i - 1])) continue;
			while (!ps.empty() && dls[i].relative_location(ps.back()) == -1) {
				ps.pop_back();
				ls.pop_back();
			}
			while (!ps.empty() && dls[i].relative_location(ps.front()) == -1) {
				ps.pop_front();
				ls.pop_front();
			}
			ps.push_back(ls.back().intersect_point(dls[i]));
			ls.push_back(dls[i]);
		}
		while (!ps.empty() && ls.front().relative_location(ps.back()) == -1) {
			ps.pop_back();
			ls.pop_back();
		}
		if (ps.empty()) ls.pop_back();
		else ps.push_back(ls.front().intersect_point(ls.back()));
		s = (int) ls.size();
		int i = 0;
		pts = new vec2[s];
		for (auto& pi : ps) pts[i++] = pi;
		i = 0;
		this->dls = new line[s];
		for (auto& li : ls) this->dls[i++] = li;
	}

	convex_polygon to_convex_polygon() const {
		convex_polygon cp;
		cp.s = s;
		cp.pts = pts;
		return cp;
	}
	
	bool hasKernel() const {
		return s > 3 || to_convex_polygon().area() > 0;
	}
};
