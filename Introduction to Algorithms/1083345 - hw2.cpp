#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct edge
{
	int u;
	int v;
	int w;

	bool operator<(const edge& r) { return w < r.w; }
};

vector<edge> pairs;
vector<int> vp;
vector<int> vr;

void make_set(int v)
{
	vp[v] = v;
	vr[v] = 0;
}

int find_set(int v)
{
	if (v != vp[v])
		vp[v] = find_set(vp[v]);
	return vp[v];
}

void link(int x, int y)
{
	if (vr[x] > vr[y])
		vp[y] = x;
	else if (vr[x] < vr[y])
		vp[x] = y;
	else
		vp[y] = x, ++vr[y];
}

void Union(int u, int v)
{
	link(find_set(u), find_set(v));
}

void kruscal(vector<edge>& A,int n)
{
	for (int i = 0; i < n; ++i)
		make_set(i);

	sort(pairs.begin(), pairs.end());

	for (int i = 0; i < pairs.size(); ++i)
		if (find_set(pairs[i].u) != find_set(pairs[i].v))
		{
			A.push_back(pairs[i]);
			Union(pairs[i].u, pairs[i].v);
		}
}

int main()
{
	int n, pairN, u, v, w;
	while (cin >> n >> pairN)
	{
		if (!n && !pairN)
			break;

		//initialize
		pairs.clear();
		vp.clear(), vp.assign(n, 0);
		vr.clear(), vr.assign(n, 0);

		//input
		bool ZERO = 0;
		for (int i = 0; i < pairN; ++i)
		{
			cin >> u >> v >> w;
			if (w == 0)
				ZERO = 1;
			pairs.push_back({ u,v,w });
		}
		if (ZERO)
		{
			cout << 0 << endl;
			continue;
		}

		//kruscal
		vector<edge> A;
		kruscal(A, n);

		//calculate
		int cost = 1;
		for (int i = 0; i < A.size(); ++i)
		{
			cost *= A[i].w;
			cost %= 65537;
		}

		//output
		cout << cost << endl;
	}
	return 0;
}