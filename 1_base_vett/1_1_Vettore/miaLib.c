#include <iostream>
#include "miaLib.h"
using namespace std;

//implementazone delle funzioni dichiarate nella mia libreria in miaLib.h

void leggiVettore(int vettore[], const int n){
	
	for(int i=0; i<n; i++){
		cout<<"inserisci elemento nel vetore in pos "<<i <<" : ";
		cin>>vettore[i];
	}

}

void stampaVettore(int vettore[], const int n){

	for(int i=0; i<n; i++){
		cout<<"stampo vetore in pos "<<i <<" : "<< vettore[i]<<"\n";;
	}

}
