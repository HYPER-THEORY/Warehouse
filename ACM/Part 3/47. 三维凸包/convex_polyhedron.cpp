class convex_polyhedron {
	
public:
	
	int s;
	vec3* vtxs;
	list<int*> faces;
	list<vec3> norms;
	
	convex_polyhedron() {}
	
	convex_polyhedron(vec3* vtxs, int s) : vtxs(vtxs) {
		add(new int[3] {0, 1, 2});
		add(new int[3] {2, 1, 0});
		for (int i = 3; i < s; ++i) {
			set<pair<int, int>> ls;
			auto fi = faces.begin();
			auto ni = norms.begin();
			while (fi != faces.end()) {
				if (*ni * (vtxs[i] - vtxs[**fi]) > 0) {
					for (int k = 0; k < 3; ++k) {
						int u = (*fi)[k];
						int v = (*fi)[(k + 1) % 3];
						if (ls.count({v, u})) ls.erase({v, u});
						else ls.insert({u, v});
					}
					delete[] *fi;
					fi = faces.erase(fi);
					ni = norms.erase(ni);
				} else {
					++fi;
					++ni;
				}
			}
			for (auto& l : ls) add(new int[3] {i, l.first, l.second});
		}
	}
	
	void add(int* f) {
		faces.push_back(f);
		norms.push_back((vtxs[f[1]] - vtxs[f[0]]) ^ (vtxs[f[2]] - vtxs[f[0]]));
	}
	
	double surface_area() const {
		double sum = 0;
		for (auto& n : norms) sum += n.mag();
		return sum / 2;
	}
};
