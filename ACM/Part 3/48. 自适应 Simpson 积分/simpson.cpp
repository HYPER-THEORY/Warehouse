double simpson(double l, double r, double (&f)(double)) {
	return (f(l) + f(r) + f((l + r) / 2) * 4) * (r - l) / 6;
}

double adaptive_simpson(double l, double r, double s, double p, double (&f)(double)) {
	double mid = (l + r) / 2;
	double sl = simpson(l, mid, f), sr = simpson(mid, r, f);
	if (abs(sl + sr - s) < p) return sl + sr;
	return adaptive_simpson(l, mid, sl, p, f) + adaptive_simpson(mid, r, sr, p, f);
}
