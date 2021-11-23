template <class type>

class mat {
	
public:
	
	int r = 0;
	int c = 0;
	type** mn;
	
	mat() { };
	
	mat(int r, int c) : r(r), c(c) {
		mn = new type*[r];
		mn[0] = new type[r * c];
		for (int i = 0; i < r; ++i) mn[i] = mn[0] + i * c;
		fill(mn[0], mn[0] + r * c, 0);
	}
	
	mat(int r, int c, type* v) : mat(r, c) {
		copy(v, v + r * c, mn[0]);
	}
	
	mat(const mat& mv) : mat(mv.r, mv.c, mv.mn[0]) { }
	
	type* operator[](int k) const {
		return mn[k];
	}
	
	mat operator+(const mat& v) const {
		mat mt(r, c);
		for (int i = 0; i < r * c; ++i) mt[0][i] = mn[0][i] + v[0][i];
		return mt;
	}
	
	void operator+=(const mat& v) const {
		for (int i = 0; i < r * c; ++i) mn[0][i] += v[0][i];
	}
	
	mat operator-(const mat& v) const {
		mat mt(r, c);
		for (int i = 0; i < r * c; ++i) mt[0][i] = mn[0][i] - v[0][i];
		return mt;
	}
	
	void operator-=(const mat& v) const {
		for (int i = 0; i < r * c; ++i) mn[0][i] -= v[0][i];
	}
	
	mat operator*(type v) const {
		mat mt(r, c);
		for (int i = 0; i < r * c; ++i) mt[0][i] = mn[0][i] * v;
		return mt;
	}
	
	void operator*=(type v) const {
		for (int i = 0; i < r * c; ++i) mn[0][i] *= v;
	}
	
	mat operator/(type v) const {
		mat mt(r, c);
		for (int i = 0; i < r * c; ++i) mt[0][i] = mn[0][i] / v;
		return mt;
	}
	
	void operator/=(type v) const {
		for (int i = 0; i < r * c; ++i) mn[0][i] /= v;
	}
	
	mat operator%(type v) const {
		mat mt(r, c);
		for (int i = 0; i < r * c; ++i) mt[0][i] = mn[0][i] % v;
		return mt;
	}
	
	void operator%=(type v) const {
		for (int i = 0; i < r * c; ++i) mn[0][i] %= v;
	}
	
	mat operator*(const mat& v) const {
		mat mt(r, v.c);
		for (int i = 0; i < r; ++i)
			for (int j = 0; j < c; ++j)
				for (int k = 0; k < v.c; ++k)
					mt[i][k] += mn[i][j] * v[j][k];
		return mt;
	}
	
	static mat identity(int v) {
		mat mt(v, v);
		for (int i = 0; i < v; ++i) mt[i][i] = 1;
		return mt;
	}
	
	mat transpose() const {
		return mat(c, r, mn[0]);
	}
	
	mat pow(unsigned v) const {
		mat mt = identity(r);
		mat bs = *this;
		while (v > 0) {
			if (v & 1) mt = mt * bs;
			bs = bs * bs;
			v >>= 1;
		}
		return mt;
	}
	
	type determinant() const {
		mat mt = mat(*this);
		for (int i = 0; i < r; ++i) {
			for (int j = i + 1; j < r; ++j) {
				double rt = mt[j][i] / mt[i][i];
				for (int k = i; k < r; ++k) mt[j][k] -= mt[i][k] * rt;
			}
		}
		type det = 1;
		for (int i = 0; i < r; ++i) det *= mt[i][i];
		return det;
	}
	
	int guass_elimination(mat& v) const {
		mat mt = mat(*this);
		for (int i = 0; i < r; ++i) {
			for (int j = i + 1; j < r; ++j) {
				double rt = mt[j][i] / mt[i][i];
				for (int k = i; k < c; ++k) mt[j][k] -= mt[i][k] * rt;
				for (int k = 0; k < v.c; ++k) v[j][k] -= v[i][k] * rt;
			}
		}
		int rnk = r;
		while (rnk > 0 && mt[rnk - 1][rnk - 1] == 0) --rnk;
		for (int i = 0; i < rnk; ++i) {
			for (int j = 0; j < v.c; ++j) v[i][j] /= mt[i][i];
			for (int j = c - 1; j >= i; --j) mt[i][j] /= mt[i][i];
		}
		for (int i = rnk - 1; i > 0; --i) {
			for (int j = 0; j < i; ++j) {
				double rt = mt[j][i] / mt[i][i];
				for (int k = 0; k < v.c; ++k) v[j][k] -= v[i][k] * rt;
			}
		}
		return rnk;
	}
	
	mat inverse() const {
		mat mt = identity(r);
		mat(*this).guass_elimination(mt);
		return mt;
	}
	
	mat conjugate_gradient(const mat& b) const {
		mat x = mat(r, 1);
		mat r = b - (*this) * x;
		mat p = mat(r);
		type rtr = 0;
		for (int i = 0; i < r.r; ++i) rtr += r[i][0] * r[i][0];
		for (int i = 0; i < 5; ++i) {
			mat ap = (*this) * p;
			type ptap = 0;
			for (int i = 0; i < p.r; ++i) ptap += p[i][0] * ap[i][0];
			type alpha = rtr / ptap;
			x = x + p * alpha;
			mat r1 = r - ap * alpha;
			type rtr1 = 0;
			for (int i = 0; i < r.r; ++i) rtr1 += r1[i][0] * r1[i][0];
			type beta = rtr1 / rtr;
			p = r1 + p * beta;
			r = r1;
			rtr = rtr1;
		}
		return x;
	}
};
