#include<iostream>
#include<string>
#include<vector>

using namespace std;

int LCS(string& a, string& b);

int main()
{
	string a, b;
	while (cin >> a >> b)
		cout<<LCS(a, b)<<'\n';
	return 0;
}

int LCS(string& a, string& b)
{
	vector<vector<int>> v(a.length(), vector<int>(b.length(), 0));
	for (int i = 0; i < a.length(); ++i)
		for (int j = 0; j < b.length(); ++j)
			if (i == 0 || j == 0)
				v[i][j] = (a[i] == b[j] ? 1 : (i > 0 ? v[i - 1][j] : (j > 0 ? v[i][j - 1] : 0)));
			else if (a[i] == b[j])
				v[i][j] = max(max(v[i - 1][j], v[i][j - 1]), v[i - 1][j - 1] + 1);
			else
				v[i][j] = max(v[i - 1][j], v[i][j - 1]);
	return v[a.length() - 1][b.length() - 1];
}