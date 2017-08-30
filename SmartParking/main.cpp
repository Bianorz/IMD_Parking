#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	// SETUP STEP: Load an image to use for parking slot definition and set parking spaces
	const int nVagas = 14;
	String slotDatabase = "slotDatabase.txt";
	String pathMap = "images/mapa.png";
	Vaga posVaga[nVagas]; // posições de cada vaga, centro, largura, altura e inclinação
	Mat vaga[nVagas];
	//salvarLocalVagas(pathMap,nVagas,slotDatabase);
	carregarParametros(slotDatabase, posVaga, nVagas);
	//mostrarMapeamento(pathMap, posVaga,nVagas);
	Mat mapa = imread(pathMap, 1);
	Point2f posicao;
	int inclinacao;
	for (int i = 0; i <= nVagas; i++) {
		posicao = posVaga[i].posicao;
		inclinacao = posVaga[1].inclinacao;
		vaga[i] = pegarImagemCortada(mapa, posicao,
		Size2f(posVaga[i].largura, posVaga[i].altura), inclinacao);
	}

}
