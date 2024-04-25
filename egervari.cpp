#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

struct Edge
{
	int i;
	int j;
	int value;
};

vector<vector<int>> matrixRead();
void matrixOut(vector<vector<int>>& matrix);
int maxNumFind(vector<vector<int>>& matrix);
void inverseMatrix(vector<vector<int>>& matrix);
void matrixReduction(vector<vector<int>>& matrix);
vector<vector<int>> nullMatrixFind(vector<vector<int>>& matrix);
vector<vector<int>> selectedNullMatrixFind(vector<vector<int>>& condMatrix, vector<int>& selected);
void egervariReduction(vector<vector<int>>& matrix, vector<int>& notSelected_i, vector<int>& notSelected_j);
vector<Edge> chainCreate(vector<vector<int>>& matrixB, vector<Edge>& chain, vector<vector<int>>& chainMatrix);
bool chainInverse(vector<vector<int>>& matrixB, Edge& startEdge, int cycles);
void egervari(vector<vector<int>>& matrix, vector<vector<int>>& matrixB, vector<int>& selected, int cycles);

//-------------------------------------------------------------

int main()
{
	vector<vector<int>> matrix, matrixCopy, matrixM, matrixB;
	matrix = matrixRead();
	matrixCopy = matrix;

	vector<int> selected(matrix.size(), -1);

	cout << "Columns: vacancies\nRows:\t candidates\n\n";

	cout << "Matrix A:\n";
	matrixOut(matrix);

	inverseMatrix(matrixCopy);

	cout << "Matrix A':\n";
	matrixOut(matrixCopy);

	matrixReduction(matrixCopy);

	cout << "Matrix A'(1&2):\n";
	matrixOut(matrixCopy);

	matrixM = nullMatrixFind(matrixCopy);
	matrixB = selectedNullMatrixFind(matrixM, selected);

	cout << "Matrix M:\n";
	matrixOut(matrixM);
	cout << "Matrix B:\n";
	matrixOut(matrixB);


	egervari(matrixCopy, matrixB, selected, 0);

	char key;
	cin >> key;
}

//-------------------------------------------------------------

vector<vector<int>> matrixRead()
{
	ifstream file;

	vector<vector<int>> matrix;

	file.open("matrix.txt");
	if (file.is_open())
	{
		vector<int> row;
		string s1, s2;

		while (getline(file, s1, '\n'))
		{
			stringstream line(s1);

			while (getline(line, s2, ' '))
				row.push_back(stoi(s2));

			matrix.push_back(row);
			row.clear();
		}
	}

	return matrix;
}

void matrixOut(vector<vector<int>>& matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[0].size(); j++)
			cout << matrix[i][j] << "\t";
		cout << "\n";
	}
	cout << "\n";
}


int maxNumFind(vector<vector<int>>& matrix)
{
	int maxNum = 0;

	for (int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix[0].size(); j++)
			if (matrix[i][j] > maxNum)
				maxNum = matrix[i][j];

	return maxNum;
}

void inverseMatrix(vector<vector<int>>& matrix)
{
	int maxNum = maxNumFind(matrix);

	for (int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix[0].size(); j++)
			matrix[i][j] = maxNum - matrix[i][j];
}

void matrixReduction(vector<vector<int>>& matrix)
{
	int minNum;

	for (int i = 0; i < matrix.size(); i++)
	{
		minNum = INT_MAX;

		for (int j = 0; j < matrix[0].size(); j++)
			if (matrix[i][j] < minNum)
				minNum = matrix[i][j];

		if (minNum == 0)
			continue;

		for (int j = 0; j < matrix[0].size(); j++)
			matrix[i][j] = matrix[i][j] - minNum;
	}

	for (int j = 0; j < matrix[0].size(); j++)
	{
		minNum = INT_MAX;

		for (int i = 0; i < matrix.size(); i++)
			if (matrix[i][j] < minNum)
				minNum = matrix[i][j];

		if (minNum == 0)
			continue;

		for (int i = 0; i < matrix.size(); i++)
			matrix[i][j] = matrix[i][j] - minNum;
	}
}

vector<vector<int>> nullMatrixFind(vector<vector<int>>& matrix)
{
	vector<vector<int>> condMatrix = matrix;

	for (int i = 0; i < matrix.size(); i++)
		for (int j = 0; j < matrix[0].size(); j++)
			if (condMatrix[i][j] > 0)
				condMatrix[i][j] = 0;
			else
				condMatrix[i][j] = 1;

	return condMatrix;
}

vector<vector<int>> selectedNullMatrixFind(vector<vector<int>>& condMatrix, vector<int>& selected)
{
	vector<vector<int>> sCondMatrix = condMatrix;
	bool flagCol, flagRow;

	for (int i = 0; i < condMatrix.size(); i++)
	{
		flagRow = false;

		for (int j = 0; j < condMatrix[0].size(); j++)
		{
			flagCol = false;

			for (int k = 0; k < i; k++)
				if (sCondMatrix[k][j] == 1)
					flagCol = true;

			if (sCondMatrix[i][j] == 1)
			{
				if (flagCol || flagRow)
					sCondMatrix[i][j] = -1;
				else
					flagRow = true;
			}

		}
	}

	for (int i = 0; i < condMatrix.size(); i++)
		for (int j = 0; j < condMatrix[0].size(); j++)
			if (sCondMatrix[i][j] == 1)
			{
				selected[i] = j;
				continue;
			}

	return sCondMatrix;
}


void egervariReduction(vector<vector<int>>& matrix, vector<int>& notSelected_i, vector<int>& notSelected_j)
{
	int minAlpha = INT_MAX;

	for (int i = 0; i < notSelected_i.size(); i++)
		for (int j = 0; j < matrix[0].size(); j++)
		{
			int num = matrix[notSelected_i[i]][j];

			if (num < minAlpha && num != 0)
				minAlpha = num;
		}

	for (int j = 0; j < notSelected_j.size(); j++)
		for (int i = 0; i < matrix.size(); i++)
		{
			int num = matrix[i][notSelected_j[j]];

			if (num < minAlpha && num != 0)
				minAlpha = num;
		}


	for (int i = 0; i < notSelected_i.size(); i++)
		for (int j = 0; j < matrix[0].size(); j++)
			matrix[notSelected_i[i]][j] -= minAlpha;


	for (int j = 0; j < notSelected_j.size(); j++)
		for (int i = 0; i < matrix.size(); i++)
			matrix[i][notSelected_j[j]] += minAlpha;
}

vector<Edge> chainCreate(vector<vector<int>>& matrixB, vector<Edge>& chain, vector<vector<int>>& chainMatrix)
{
	Edge nextEdge;

	if (chain[0].value == -1)
	{
		for (int i = 0; i < matrixB.size(); i++)
			if (matrixB[i][chain[0].j] == 1)
			{
				if (chainMatrix[chain[0].i][chain[0].j] > 0)
					i += (chainMatrix[chain[0].i][chain[0].j] - 1);

				if (chainMatrix[i][chain[0].j] == 1)
				{
					Edge cycleStartEdge;
					cycleStartEdge.i = i;
					cycleStartEdge.j = chain[0].j;

					for (int k = 0; k < chain.size(); k++)
						if (chain[0].i != cycleStartEdge.i || chain[0].j != cycleStartEdge.j)
							chain.erase(chain.begin());
						else
							break;
				}
				else
				{
					nextEdge.i = i;
					nextEdge.j = chain[0].j;
					nextEdge.value = 1;

					chain.insert(chain.begin(), nextEdge);
				}

				chainMatrix[i][chain[0].j]++;

				chainCreate(matrixB, chain, chainMatrix);

				break;
			}
	}
	else
	{
		for (int j = 0; j < matrixB[0].size(); j++)
			if (matrixB[chain[0].i][j] == -1)
			{
				if (chainMatrix[chain[0].i][chain[0].j] > 0)
					j += (chainMatrix[chain[0].i][chain[0].j] - 1);

				if (chainMatrix[chain[0].i][j] == 1)
				{
					Edge cycleStartEdge;
					cycleStartEdge.i = chain[0].i;
					cycleStartEdge.j = j;

					for (int k = 0; k < chain.size(); k++)
						if (chain[0].i != cycleStartEdge.i || chain[0].j != cycleStartEdge.j)
							chain.erase(chain.begin());
						else
							break;
				}
				else
				{
					nextEdge.i = chain[0].i;
					nextEdge.j = j;
					nextEdge.value = -1;

					chain.insert(chain.begin(), nextEdge);
				}

				chainMatrix[chain[0].i][j]++;

				chainCreate(matrixB, chain, chainMatrix);

				break;
			}
	}

	return chain;
}

bool chainInverse(vector<vector<int>>& matrixB, Edge& startEdge, int cycles)
{
	vector<vector<int>> visited(matrixB.size(), vector<int>(matrixB[0].size(), 0));

	vector<Edge> chain;
	chain.push_back(startEdge);

	chainCreate(matrixB, chain, visited);

	if (chain.size() % 2 == 1)
	{
		for (int i = 0; i < chain.size(); i++)
			chain[i].value = -chain[i].value;


		for (int i = 0; i < chain.size(); i++)
		{
			matrixB[chain[i].i][chain[i].j] = chain[i].value;
		}

		cout << "Result\n";
		cout << "Matrix B`" << cycles + 1 << ":\n";
		matrixOut(matrixB);

		return true;
	}

	return false;
}

void egervari(vector<vector<int>>& matrix, vector<vector<int>>& matrixB, vector<int>& selected, int cycles)
{
	vector<int> notSelected_i, notSelected_j;
	bool flag;

	for (int k = 0; k < selected.size(); k++)
		if (selected[k] == -1)
		{
			for (int j = 0; j < matrixB[0].size(); j++)
				if (matrixB[k][j] == -1)
				{
					flag = false;

					for (int m = 0; m < notSelected_j.size(); m++)
						if (notSelected_j[m] == j)
						{
							flag = true;
							break;
						}

					if (!flag)
						notSelected_j.push_back(j);

					for (int i = 0; i < matrixB.size(); i++)
						if (matrixB[i][j] == 1)
						{
							flag = false;

							for (int m = 0; m < notSelected_i.size(); m++)
								if (notSelected_i[m] == i)
								{
									flag = true;
									break;
								}

							if (!flag)
								notSelected_i.push_back(i);
						}
				}
		}


	vector<int> selectedCopy(matrix.size(), -1);
	vector<vector<int>> mtrxM, mtrxB;
	vector<vector<int>> matrixCopy = matrix;
	egervariReduction(matrixCopy, notSelected_i, notSelected_j);

	matrixReduction(matrixCopy);

	cout << "Egervari alg. " << cycles + 1 << "\n";
	cout << "Matrix A'" << cycles + 1 << ":\n";
	matrixOut(matrixCopy);

	mtrxM = nullMatrixFind(matrixCopy);
	mtrxB = selectedNullMatrixFind(mtrxM, selectedCopy);

	cout << "Matrix M" << cycles + 1 << ":\n";
	matrixOut(mtrxM);
	cout << "Matrix B" << cycles + 1 << ":\n";
	matrixOut(mtrxB);


	Edge startEdge;

	for (int i = 0; i < matrix.size(); i++)
		if (selectedCopy[i] == -1)
		{
			startEdge.value = -1;
			startEdge.i = i;

			for (int j = 0; j < matrix[0].size(); j++)
				if (mtrxB[i][j] == -1)
				{
					startEdge.j = j;
					break;
				}
		}

	bool stopFlag;
	stopFlag = chainInverse(mtrxB, startEdge, cycles);

	if (stopFlag)
		return;


	cycles++;
	egervari(matrixCopy, mtrxB, selected, cycles);
}