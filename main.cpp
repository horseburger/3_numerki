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


void menu();
void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis);
void rys_wykres(string nazwa);
void clear_screen();
void interpolacja(int n, double(*fun)(double), double x[],double X, vector<double> y_axis);

int main()
{
	menu();
	return 0;
}

void menu(){
	
	clear_screen();

	cout << "Wybierz typ wykresu: \n" << "1. Liniowy ( y = 13x - 9)\n" << "2. Wartość bezwzględna z x [ |x| ]\n" 
		 << "3. Wielomian (3x^3 + 8x^2 - x + 1)\n" << "4. cos(x)\n" << "5. Złożenie funkcji (f[x] = 13*|x| - 9) \n"
		 << "[q] Wyjdź.\n" << "Twój wybór: ";


	int c,n;
	cin >> c;
	getchar();
	rys_wykres(funkcje[c-1]);
	double a,b;
	cout << "Podaj przedział interpolowania: ";
	cin >> a >> b;

	do{
		cout << "Podaj liczbę węzłów (co najmniej 3): ";
		cin >> n;
	}while(n<3);
	getchar();

	double odleglosc = abs(a-b)/n;

	double* x = new double[n];
	x[0]= a;
	for(int i=1;i<n;i++){
		x[i] = x[i-1]+odleglosc;
	}

	vector<double> y_axis(n);
	vector<double> x_axis(n);

	switch(c){
		case 1:{
			// rys_wykres(a,b,funkcje[c-1], liniowa);
			// interpolacja(n,liniowa,x,0,y_axis);
			rys_wykres(a,b,funkcje[c-1],liniowa,y_axis,x_axis);
			break;
		}
		case 2:{
			// rys_wykres(a,b, funkcje[c-1], absolute);
			break;
		}
		case 3:{
			// rys_wykres(a,b,funkcje[c-1], wielomian);
			break;
		}
		case 4:{
			// rys_wykres(a,b,funkcje[c-1], trygo);
			break;
		}
		case 5:{
			// rys_wykres(a,b,funkcje [c-1], skladka);
			break;
		}
	}
}

void clear_screen(){
	cout << "\x1B[2J\x1B[H"; // Na UNIXach czyści terminal
}

void interpolacja(int n, double(*fun)(double),double x[],double X,vector<double> y_axis){
	double tmp=1;
	// double* x = new double[n];
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			tmp *= (X-x[j])/(x[i]-x[j]);
		}
		y_axis[i] = fun(x[i])*tmp;
	}
}	// n to liczba węzłów, fun to funkcja wybrana przez użytkownika, x to tablica osi X, X to wartość x dla której badamy
	// funkcję, koniec to wartość końcowa wszystkich punktów

void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis){

	Gnuplot wykres;
	wykres.set_grid();
	wykres.set_style("lines");
	wykres.set_xrange(-10,10);
	wykres.set_yrange(-10,10);
	wykres.set_title(nazwa);
	wykres.set_xlabel("Os x");
	wykres.set_ylabel("Os y");

	double tmp=a, roz = (abs(a-b)*5)+1;
	vector<double> x(roz);
	vector<double> y(roz);

	for(int i=0;i<roz;i++){
		x[i] = tmp;
		tmp += 0.2;
	}

	for(int i=0;i<roz;i++){
		y[i] = fun(x[i]);
	}

	wykres.plot_xy(x,y,nazwa);
	getchar();

	wykres.set_style("points");
	wykres.plot_xy(x_axis,y_axis,nazwa+" interpolowane");
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

	wykres.plot_equation("13*x-9");
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
