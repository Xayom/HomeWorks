#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <c++0x_warning.h>
#include <random>

using namespace std;

const int dsize = 64;
int cR;
int bA = 0, bE10 = 0, dE10 = 0;

struct lc
{
	int value = 0, next = 0;
};

int aOc = 0, Aor = 0, xSh = 0, ySh = 0, sX = 64, sY = 64;
lc ** fd;

void encurecapacity(int x, int y)
{
	if (x > sX || sY < y){
		lc** tp = new lc*[sX];
		for (int i = 0; i < sX; i++){
			tp[i] = new lc[sY];
			for (int j = 0; j < sY; j++)
				tp[i][j] = fd[i][j];
		}
		delete[] fd;
		int nx, ny;
		nx = sX;
		ny = sY;
		while (x >= nx)
			nx *= 2;
		while (y >= ny)
			ny *= 2;
		fd = new lc*[nx];
		for (int i = 0; i < nx; i++){
			fd[i] = new lc[ny];
			if (i < sX)
			for (int j = 0; j < ny; j++)
				fd[i][j] = tp[i][j];
		}
		delete[] tp;
		sX = nx;
		sY = ny;
	}
}

int ReturnCellvalue(int x, int y)
{
	if ((sX > x && x > 0) && (sY > y && y > 0))
			return fd[x][y].value;
    return 0;
}

int ReturnSum(int x, int y)
{
	int s = ReturnCellvalue(x + 1, y - 1) + ReturnCellvalue(x + 1, y) + ReturnCellvalue(x + 1, y + 1) + ReturnCellvalue(x, y + 1);
    s += ReturnCellvalue(x - 1, y - 1) + ReturnCellvalue(x - 1, y) + ReturnCellvalue(x - 1, y + 1) + ReturnCellvalue(x, y - 1);
	return s;
}

void check()
{
	int s = 0;
	for (int i = 0; i < sX; i++)
		for (int j = 0; j < sY; j++)
		{
			s = ReturnSum(i, j);
			if (fd[i][j].value == 0 && s == 3)
            {
				bA++;
				bE10++;
			}
			if (fd[i][j].value == 1 && (s > 3 || s < 2))
				dE10++;
			if ((s == 3) || ((s == 2) && (fd[i][j].value == 1)))
			{
				fd[i][j].next = 1;
				xSh = i == 0 ? 1 : 0;
				ySh = j == 0 ? 1 : 0;
				int nx = i == sX - 1 ? sX * 2 : sX;
				int ny = j == sX - 1 ? sY * 2 : sY;
				if (nx != sX || ny != sY)
					encurecapacity (nx, ny);
			}
		}
}

void generateR()
{
	xSh = 0;
	ySh = 0;
	check();
	for (int i = 0; i < sX; i++)
		for (int j = 0; j < sY; j++)
        {
			fd[i + xSh][j + ySh].value = fd[i][j].next;
			fd[i][j].next = 0;
		}
}

int main()
{
	srand(time(0));
	fd = new lc*[dsize];
	for (int i = 0; i < dsize; i++)
		fd[i] = new lc[dsize];

	cout << "Type height and width" << endl;
	int sX = 0;
	int sY= 0;
	cin >> sX >> sY;
	encurecapacity(sX, sY);
	cout << "Type amount of rounds" << endl;
	cin >> Aor;
	char ans = 'a';
	cout << "Do you want to type all cells by yourself(y) or generate by Random generator(n)?" << endl;
    cin >> ans;
	cout << "Type amount of generated cells" << endl;
	cin >> aOc;
	if (ans == 'y')
    {
		cout << "Type x and y of cell" << endl;
		int x = 0;
		int y = 0;
		for (int i = 0; i < aOc; i++)
		{
			cin >> x >> y;
			encurecapacity(y + 1, x + 1);
			fd[y][x].value = 1;
		}
	}

	if (ans == 'n')
	for (int i = 0; i < aOc; i++)
    {
		int x = rand() % sX + 1;
		int y = rand() % sY+ 1;
		encurecapacity(x + 1, y + 1);
		fd[x][y].value = 1;
	}
	cout << endl;

	for (cR = 1; cR <= Aor; cR++)
    {
		generateR();
		if (cR % 10 == 0){
			cout << "Cells born from " << (cR / 10 - 1) * 10 << " to " << (cR / 10) * 10 << ": " << bE10 << endl;
			if (dE10 != 0)
				cout << "Average born / dead ratio from " << (cR / 10 - 1) * 10 << " to " << (cR / 10) * 10 << ": " << (double) bE10 / dE10 << endl;
			else
				cout << "Average born / dead ratio from " << (cR / 10 - 1) * 10 << " to " << (cR / 10) * 10 << ": INFINITE(nobody died)" << endl;
			bE10 = 0;
			dE10 = 0;
			cout << endl;
		}
	}
	cout << "Cells born: " << bA << endl;

	int cx = 0; int cy = 0;
	cout << "Press l, r, u, d to see another 60 * 60 block in specified destination" << endl;
	while (true)
    {
		cout << "Current left upper position: x =" << cx << " y =" << cy << endl;
		for (int i = cy; i < cy + 60; i++){
			for (int j = cx; j < cx + 60; j++)
				cout << ReturnCellvalue(i, j);
			cout << endl;
		}

		ans = 'p';
		while (!(ans == 'l' || ans == 'r' || ans == 'd' || ans == 'u'))
			cin >> ans;
		switch (ans)
		{
            case 'd':
                cy += 60;
                break;
            case 'r':
                cx += 60;
                break;
            case 'l':
                if (cx >= 60)
                    cx -= 60;
                else
                    cout << "Its pointless, there is nothing" << endl;
                break;
            case 'u':
                if (cy >= 60)
                    cy -= 60;
                else
                    cout << "Its pointless, there is nothing" << endl;
			break;
		}
	}
	return 0;
}
