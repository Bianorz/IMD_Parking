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
	Mat src;
	Point2f posicao;
	int inclinacao;
	int nContorno = 0;
	for (int i = 0; i < nVagas; i++) {
		posicao = posVaga[i].posicao;
		inclinacao = posVaga[1].inclinacao;
		vaga[i] = pegarImagemCortada(mapa, posicao,
				Size2f(posVaga[i].largura, posVaga[i].altura), inclinacao);
		nContorno = numeroContornos(vaga[i]);
		cout << "contornos encontrados: " << nContorno << " vaga: " << i + 1
				<< endl;
	}
	VideoCapture capture;
	while (1) {
		capture.open("/home/bianor/Videos/est01.mp4");
		if (!capture.isOpened()) {
			cout << "ERRO, VIDEO NAO ENCONTRADO\n";
			getchar();
			return -1;
		}
		while (capture.get(CV_CAP_PROP_POS_FRAMES)
				< capture.get(CV_CAP_PROP_FRAME_COUNT) - 1) {
			capture.read(src);
			mostrarMapeamento(src,posVaga,nVagas);
			namedWindow("real", CV_WINDOW_NORMAL);
			imshow("real", src);

		}
		capture.release();
	}

}
