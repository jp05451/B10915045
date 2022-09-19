#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <vector>
using namespace std;

class Node
{
public:
	char variable;
	int else_edge, then_edge;
	bool isRedundant;
};

int nin = 0, nout = 0;
vector<Node> node;
vector<char> ilb;
vector<char> ob;
vector<bool> bit;

void ROBDD(ifstream &);
void Show(string, vector<Node>, int);
void Redundant(vector<Node> &, int);
void Save(string, vector<Node>, int);

int main(int argc, char *argv[])
{
	if (argc == 3)
	{ // robdd  input.pla  output.dot
		ifstream fin(argv[1]);
		if (!fin)
		{
			cout << "error!" << endl;
			return -1;
		}
		else
		{
			ROBDD(fin);
		}
	}
	else
	{
		cout << "error!" << endl;
		return -1;
	}

	Redundant(node, (int)pow(2, nin));
	Show("ROBDD", node, (int)pow(2, nin));
	Save(argv[2], node, (int)pow(2, nin));

	return 0;
}

void ROBDD(ifstream &fin)
{

	string str;

	while (!fin.eof())
	{
		fin >> str;

		if (str == ".i")
		{
			// cout << "check i!" << endl;
			fin >> nin;
			for (int i = 0; i < (int)pow(2, nin) + 1; i++)
			{
				Node temp;
				temp.isRedundant = false;

				if (i == 0)
				{
					temp.variable = '0';
				}
				else if (i == pow(2, nin))
				{
					temp.variable = '1';
				}
				else
				{
					temp.variable = 'n';
				}

				if (i >= 1 && i < pow(2, nin - 1))
				{
					temp.else_edge = i * 2;
					temp.then_edge = i * 2 + 1;
				}
				else if (i >= pow(2, nin - 1) && i < pow(2, nin))
				{
					temp.else_edge = 0;
					temp.then_edge = 0;
				}
				else
				{
					temp.else_edge = -1;
					temp.then_edge = -1;
				}
				node.push_back(temp);
			}
		}
		else if (str == ".o")
		{
			fin >> nout;
		}
		else if (str == ".ilb")
		{
			char temp_ilb;
			for (int i = 0; i < nin; i++)
			{
				fin >> temp_ilb;
				ilb.push_back(temp_ilb);
			}
			for (int i = 1; i < pow(2, nin); i++)
			{
				node[i].variable = ilb[(int)log2(i)];
			}
		}
		else if (str == ".ob")
		{
			char temp_ob;
			for (int i = 0; i < nout; i++)
			{
				fin >> temp_ob;
				ob.push_back(temp_ob);
			}
		}
		else if (str == ".p")
		{
			// cout << "check p!" << endl;
			int num_p;
			fin >> num_p;
			for (int i = 0; i < num_p; i++)
			{
				bit.clear();
				string terms;
				fin >> terms;
				reverse(terms.begin(), terms.end());
				for (int i = 0; i < pow(2, nin); i++)
				{
					bool temp_b = false;
					bit.push_back(temp_b);
				}
				for (int j = 0; j < (int)terms.length(); j++)
				{
					if (terms[j] == '1')
					{
						bit[j * 2 + 1] = true;
					}
					else if (terms[j] == '0')
					{
						bit[j * 2] = true;
					}
					else
					{
						bit[j * 2] = true;
						bit[j * 2 + 1] = true;
					}
				}
				fin >> terms;
				for (int i = (int)pow(2, nin - 1); i < (int)pow(2, nin); i++)
				{
					bool f = true;
					int temp_n = i * 2 - (int)pow(2, nin);
					for (int j = 0; j < nin; j++)
					{
						if (bit[temp_n % 2 + j * 2] == false)
						{
							f = false;
						}
						temp_n /= 2;
					}
					if (node[i].else_edge == 0 && f == true)
					{
						node[i].else_edge = (int)pow(2, nin);
					}
					f = true;
					temp_n = i * 2 + 1 - (int)pow(2, nin);
					for (int k = 0; k < nin; k++)
					{
						if (bit[temp_n % 2 + k * 2] == false)
						{
							f = false;
						}
						temp_n /= 2;
					}
					if (node[i].then_edge == 0 && f == true)
					{
						node[i].then_edge = (int)pow(2, nin);
					}
				}
			}
			/*
				for (int i = 0; i < pow(2, nin) +1; i++) {
					cout << " " << node[i].variable << " " << node[i].else_edge << " " << node[i].then_edge <<" " << node[i].isRedundant << endl;
				}
				cout << "------------" << endl;
			*/
		}
		else if (str == ".e")
		{

			break;
		}
	}
}

void Show(string name, vector<Node> node, int size)
{
	cout << "digraph " << name << " {" << endl;
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			cout << "\t{rank=same " << i << "}" << endl;
		}
	}
	cout << endl
		 << "\t0 [label=\"0\", shape=box];" << endl;
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			printf("\t%d [label=\"%c\"];\n", i, node[i].variable);
		}
	}
	printf("\t%d [label=\"1\", shape=box];\n", size);
	cout << endl;
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			printf("\t%d -> %d [label=\"0\", style=dotted]\n", i, node[i].else_edge);
			printf("\t%d -> %d [label=\"1\", style=solid]\n", i, node[i].then_edge);
		}
	}
	cout << "}" << endl;
}

void Redundant(vector<Node> &node, int size)
{
	bool f = false;
	for (int i = 1; i < size; i++)
	{
		if (node[i].isRedundant)
		{
			continue;
		}
		if (node[i].else_edge == node[i].then_edge)
		{
			node[i].isRedundant = true;
			f = true;
			for (int j = 1; j < size; j++)
			{
				if (node[j].else_edge == i)
				{
					node[j].else_edge = node[i].else_edge;
				}
				if (node[j].then_edge == i)
				{
					node[j].then_edge = node[i].else_edge;
				}
			}
		}
	}
	for (int i = 1; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (node[i].isRedundant)
			{
				continue;
			}
			if (node[j].isRedundant)
			{
				continue;
			}

			if (node[i].else_edge == node[j].else_edge && node[i].then_edge == node[j].then_edge && node[i].variable == node[j].variable)
			{
				node[j].isRedundant = true;
				f = true;
				for (int k = 1; k < size; k++)
				{
					if (node[k].else_edge == j)
					{
						node[k].else_edge = i;
					}
					if (node[k].then_edge == j)
					{
						node[k].then_edge = i;
					}
				}
			}
		}
	}
	if (f)
		Redundant(node, size);
}

void Save(string fileName, vector<Node> node, int size)
{
	ofstream fout(fileName);

	fout << "digraph "
		 << "ROBDD"
		 << " {" << endl;
	// rank
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			fout << "\t{rank=same " << i << "}" << endl;
		}
	}
	// label
	fout << endl
		 << "\t0 [label=\"0\", shape=box];" << endl;
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			fout << "\t" << i << " [label=\"" << node[i].variable << "\"];" << endl;
		}
	}
	fout << "\t" << size << " [label=\"1\", shape=box];" << endl;
	fout << endl;
	// style
	for (int i = 1; i < size; i++)
	{
		if (!node[i].isRedundant)
		{
			fout << "\t" << i << " -> " << node[i].else_edge << " [label=\"0\", style=dotted]" << endl;
			fout << "\t" << i << " -> " << node[i].then_edge << " [label=\"1\", style=solid]" << endl;
		}
	}
	fout << "}";

	fout.close();
}
