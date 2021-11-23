class kdtree2 {
	
public:

	class node {

	public:

		vec2 v; // value

		vec2 n; // min
		vec2 x; // max

		node* p; // parent
		node* l = nullptr; // left child
		node* r = nullptr; // right child

		node(vec2 v, node* p) : v(v), p(p), n(v), x(v) { }
	};

	int s;

	node* root = nullptr;

	kdtree2(vec2* vn, int s) : s(s) {
		root = build(vn, 0, s, 1);
	}

	static bool cmpx(const vec2& a, const vec2& b) {
		return a.x < b.x;
	}

	static bool cmpy(const vec2& a, const vec2& b) {
		return a.y < b.y;
	}

	static double min_rect_dist(const vec2& v, const vec2& n, const vec2& x) {
		bool midx = v.x > n.x && v.x < x.x;
		bool midy = v.y > n.y && v.y < x.y;
		if (midx && midy) return false;
		if (midx) return min(v.y - x.y, n.y - v.y);
		if (midy) return min(v.x - x.x, n.x - v.x);
		return min(min(v.dist(n), v.dist(x)), min(v.dist(vec2(n.x, x.y)), v.dist(vec2(x.x, n.y))));
	}

	static double max_rect_dist(const vec2& v, const vec2& n, const vec2& x) {
		return max(max(v.dist(n), v.dist(x)), max(v.dist(vec2(n.x, x.y)), v.dist(vec2(x.x, n.y))));
	}

	node* build(vec2* vn, int l, int r, bool ix, node* p = nullptr) {
		if (r <= l) return nullptr;
		int mid = (l + r) / 2;
		nth_element(vn + l, vn + mid, vn + r, ix ? cmpx : cmpy);
		node* n = new node(vn[mid], p);
		n->l = build(vn, l, mid, ix ^ 1, n);
		n->r = build(vn, mid + 1, r, ix ^ 1, n);
		if (n->l != nullptr) {
			n->n.x = min(n->n.x, n->l->n.x);
			n->n.y = min(n->n.y, n->l->n.y);
			n->x.x = max(n->x.x, n->l->x.x);
			n->x.y = max(n->x.y, n->l->x.y);
		}
		if (n->r != nullptr) {
			n->n.x = min(n->n.x, n->r->n.x);
			n->n.y = min(n->n.y, n->r->n.y);
			n->x.x = max(n->x.x, n->r->x.x);
			n->x.y = max(n->x.y, n->r->x.y);
		}
		return n;
	}

	void farthest(const vec2& v, node* n, double& d) const {
		if (n == nullptr || max_rect_dist(v, n->n, n->x) < d) return;
		double dist = v.dist(n->v);
		d = max(d, dist);
		farthest(v, n->l, d);
		farthest(v, n->r, d);
	}

	void nearest(const vec2& v, node* n, double& d) const {
		if (n == nullptr || min_rect_dist(v, n->n, n->x) > d) return;
		double dist = v.dist(n->v);
		d = min(d, dist);
		nearest(v, n->l, d);
		nearest(v, n->r, d);
	}

	void farthest_pair(node* n, double& d) const {
		if (n == nullptr) return;
		nearest(n->v, n, d);
		nearest_pair(n->l, d);
		nearest_pair(n->r, d);
	}

	void nearest_pair(node* n, double& d) const {
		if (n == nullptr) return;
		vec2 t = n->v;
		n->v.x += d * 2;
		nearest(t, n, d);
		n->v = t;
		nearest_pair(n->l, d);
		nearest_pair(n->r, d);
	}

	void k_farthest(const vec2& v, node* n,
		priority_queue<double, vector<double>, greater<double>>& pq) const {
		if (n == nullptr || max_rect_dist(v, n->n, n->x) < pq.top()) return;
		double dist = v.dist(n->v);
		if (dist > pq.top()) {
			pq.pop();
			pq.push(dist);
		}
		k_farthest(v, n->l, pq);
		k_farthest(v, n->r, pq);
	}

	void k_nearest(const vec2& v, node* n, priority_queue<double>& pq) const {
		if (n == nullptr || min_rect_dist(v, n->n, n->x) > pq.top()) return;
		double dist = v.dist(n->v);
		if (dist < pq.top()) {
			pq.pop();
			pq.push(dist);
		}
		k_nearest(v, n->l, pq);
		k_nearest(v, n->r, pq);
	}
};
