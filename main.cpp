#include <iostream>
#include <cstring>
#include <cmath>

#include "gnuplot_i.hpp"

using namespace std;

/*
 *	Interpolacja Lagrange'a dla funkcji:
 *	-  liniowa y = 13x - 9
 *	-  |x|
 * 	-  wielomian y = 3x^3 + 8x^2 - x + 1
 * 	-  trygonometryczna cos(x)
 * 	-  złożenia 13|x| - 9
 */

string funkcje[5]{
	"13*x-9",
	"abs(x)",
	"3*(x*x*x)+8*(x*x)-x+1",
	"cos(x)",
	"13*abs(x)-9"
};

double liniowa(double x);
double absolute(double x);
double wielomian(double x);
double trygo(double x);
double skladka(double x);


bool menu(int& c);
void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis);
void rys_wykres(string nazwa);
void clear_screen();
double interpolacja(int n, vector<double> y_axis, vector<double> x_axis, double X);
// void interpolacja_kuba(vector<double> x, vector<double> y, int n, vector<double>& wynik);

int main()
{
	int c;
	do{
		menu(c);
	}while(c!=0);
	return 0;
}

bool menu(int& c){
	
	clear_screen();

	cout << "Wybierz typ wykresu: \n" << "1. Liniowy ( y = 13x - 9)\n" << "2. Wartość bezwzględna z x [ |x| ]\n" 
		 << "3. Wielomian (3x^3 + 8x^2 - x + 1)\n" << "4. cos(x)\n" << "5. Złożenie funkcji (f[x] = 13*|x| - 9) \n"
		 << "[0] Wyjdź.\n" << "Twój wybór: ";


	int n;
	cin >> c;
	getchar();
	string nazwa = funkcje[c-1];
	if(c == 0){
		return 0;
	}
	rys_wykres(nazwa);
	double a,b;
	cout << "Podaj przedział interpolowania: ";
	cin >> a >> b;

	do{
		cout << "Podaj liczbę węzłów (co najmniej 3): ";
		cin >> n;
	}while(n<3);
	getchar();

	double odleglosc = abs(a-b)/n;

	vector<double> y_axis(n);
	vector<double> x_axis(n);
	vector<double> X(n);
	vector<double> Y(n);
	X[0]=x_axis[0]=a;
	for(int i=1;i<n;i++){
		X[i] = X[i-1]+odleglosc;
		x_axis[i] = x_axis[i-1]+odleglosc;
	}
	// double* wynik = new double[roz];
	switch(c){
		case 1:{
			for(int i=0;i<n;i++){
				y_axis[i] = liniowa(x_axis[i]);
			}
			for(int i=0;i<n;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,liniowa,Y,X);
			break;
		}
		case 2:{
			// rys_wykres(a,b, funkcje[c-1], absolute);
			for(int i=0;i<n;i++){
				y_axis[i] = absolute(x_axis[i]);
			}
			for(int i=0;i<n;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,absolute,Y,X);
			break;
		}
		case 3:{
			// rys_wykres(a,b,funkcje[c-1], wielomian);
			for(int i=0;i<n;i++){
				y_axis[i] = wielomian(x_axis[i]);
			}
			for(int i=0;i<n;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,wielomian,Y,X);
			break;
		}
		case 4:{
			// rys_wykres(a,b,funkcje[c-1], trygo);
			for(int i=0;i<n;i++){
				y_axis[i] = trygo(x_axis[i]);
			}
			for(int i=0;i<n;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,trygo,Y,X);
			break;
		}
		case 5:{
			// rys_wykres(a,b,funkcje [c-1], skladka);
			for(int i=0;i<n;i++){
				y_axis[i] = skladka(x_axis[i]);
			}
			for(int i=0;i<n;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,skladka,Y,X);
			break;
		}
		default: return 0;
	}
}

void clear_screen(){
	cout << "\x1B[2J\x1B[H"; // Na UNIXach czyści terminal
}

/*
void interpolacja_kuba(vector<double> x, vector<double> y, int n, vector<double>& wynik)
	{
		for (int i = 0 ; i < n ; i++)
		{
			wynik[i] = y[i];
			for (int k = 0 ; k < n ; k++)
			{
				if(x[i] != x[k])
				{
					wynik[i] *= (1.0 - x[k]);
					wynik[i] /= (x[i]- x[k]);
				}
			}
		}
		cout << endl;
		for (int i = 0 ; i < n ; i++)
		{
			cout << wynik[i] << '\t';
		}
		cout << endl;
	}
*/

double interpolacja(int n,vector<double> y_axis, vector<double> x_axis, double X){
	double wynik=0.0;
	double temp=1.0;
	for(int i=0;i<n;i++){
		temp = y_axis[i];
		// cout << temp << " " << x_axis[i] << " " << X  << " " << y_axis[i] << " ";
		for(int j=0;j<n;j++){
			if(i!=j){
				temp = temp*( X - x_axis[j] ) ;
				temp = temp/ (x_axis[i] - x_axis[j]);
				// cout << temp << " ";
			}
		}
		wynik += temp;
		// cout << wynik << " ";
	}
	return wynik;
}


void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis){

	Gnuplot wykres;
	wykres.set_grid();
	wykres.set_style("lines");
	wykres.set_xrange(-10,10);
	wykres.set_yrange(-10,10);
	wykres.set_title(nazwa);
	wykres.set_xlabel("Os x");
	wykres.set_ylabel("Os y");

	double temp=a, roz = (abs(a-b)*5)+1;
	vector<double> x(roz);
	vector<double> y(roz);

	for(int i=0;i<roz;i++){
		x[i] = temp;
		temp += 0.2;
	}

	for(int i=0;i<roz;i++){
		y[i] = fun(x[i]);
	}

	wykres.plot_xy(x,y,nazwa);

	wykres.set_style("points");
	wykres.plot_xy(x_axis,y_axis,nazwa+" interpolowane");
	wykres.set_style("lines");
	wykres.plot_xy(x_axis,y_axis,nazwa+"interpolowane");
	getchar();

}

void rys_wykres(string nazwa){

	Gnuplot wykres;
	wykres.set_grid();
	wykres.set_style("lines");
	wykres.set_xrange(-10,10);
	wykres.set_yrange(-10,10);
	wykres.set_title(nazwa);
	wykres.set_xlabel("Os x");
	wykres.set_ylabel("Os y");

	wykres.plot_equation(nazwa);
	getchar();
}

double liniowa(double x){
		return (13*x-9);
}
double absolute(double x){
		return abs(x);
}
double wielomian(double x){
		return (3*(x*x*x)+8*(x*x)-x+1);
}
double trygo(double x){
		return cos(x);
}
double skladka(double x){
		return (13*abs(x)-9);
}
