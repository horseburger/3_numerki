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
void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis,vector<double> x_nodes,
				vector<double> y_node);
void rys_wykres(string nazwa);
void clear_screen();
double interpolacja(int n, vector<double> y_axis, vector<double> x_axis, double X);
double interpolacja_wspolczynniki(int n,vector<double> y_axis, vector<double> x_axis,double wsp[]);

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
	int roz = (abs(a-b)*10)+1;
	vector<double> X(roz);
	vector<double> Y(roz);
	X[0]=x_axis[0]=a;
	for(int i=1;i<n;i++){
		x_axis[i] = x_axis[i-1]+odleglosc;
	}
	double tmp = a;
	for(int i=0;i<roz;i++){
		X[i] = tmp;
		tmp += 0.1;
	}
	double* wsp = new double[n];
	switch(c){
		case 1:{
			for(int i=0;i<n;i++){
				y_axis[i] = liniowa(x_axis[i]);
			}
			for(int i=0;i<roz;i++){
				Y[i] = interpolacja(n,y_axis,x_axis, X[i]);
			}
			rys_wykres(a,b,nazwa,liniowa,Y,X,x_axis,y_axis);
			break;
		}
		case 2:{
			for(int i=0;i<n;i++){
				y_axis[i] = absolute(x_axis[i]);
			}
			for(int i=0;i<roz;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,absolute,Y,X,x_axis,y_axis);
			break;
		}
		case 3:{
			for(int i=0;i<n;i++){
				y_axis[i] = wielomian(x_axis[i]);
			}
			for(int i=0;i<roz;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,wielomian,Y,X,x_axis,y_axis);
			break;
		}
		case 4:{
			for(int i=0;i<n;i++){
				y_axis[i] = trygo(x_axis[i]);
			}
			for(int i=0;i<roz;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,trygo,Y,X,x_axis,y_axis);
			break;
		}
		case 5:{
			for(int i=0;i<n;i++){
				y_axis[i] = skladka(x_axis[i]);
			}
			for(int i=0;i<roz;i++){
				Y[i] = interpolacja(n,y_axis,x_axis,X[i]);
			}
			rys_wykres(a,b,nazwa,skladka,Y,X,x_axis,y_axis);
			break;
		}
		default: return 0;
	}
	delete [] wsp;
}

void clear_screen(){
	cout << "\x1B[2J\x1B[H"; // Na UNIXach czyści terminal
}


double interpolacja(int n,vector<double> y_axis, vector<double> x_axis, double X){
	double wynik=0.0;
	double temp=1.0;
	for(int i=0;i<n;i++){
		temp = y_axis[i];
		for(int j=0;j<n;j++){
			if(i!=j){
				temp = temp*( X - x_axis[j] ) ;
				temp = temp/ (x_axis[i] - x_axis[j]);
			}
		}
		wynik += temp;
	}
	return wynik;
}



void rys_wykres(double a, double b, string nazwa, double(*fun)(double),vector<double> y_axis, vector<double> x_axis, vector<double> x_nodes,
				vector<double> y_nodes){

	Gnuplot wykres;
	wykres.set_grid();
	wykres.set_style("lines");
	wykres.set_xrange(-10,10);
	wykres.set_yrange(-10,10);
	wykres.set_title(nazwa);
	wykres.set_xlabel("Os x");
	wykres.set_ylabel("Os y");

	double temp=a, roz = (abs(a-b)*10)+1;
	vector<double> x(roz);
	vector<double> y(roz);

	for(int i=0;i<roz;i++){
		x[i] = temp;
		temp += 0.1;
	}

	for(int i=0;i<roz;i++){
		y[i] = fun(x[i]);
	}

	wykres.plot_xy(x,y,nazwa);

	wykres.set_style("points");
	wykres.plot_xy(x_nodes,y_nodes,"Wezly");
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
