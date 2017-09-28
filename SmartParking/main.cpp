#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
#include <fstream>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	//int haar = haarCascade();
	//cout << haar << endl;
	// SETUP STEP: Load an image to use for parking slot definition and set parking spaces
	const int nVagas = 14;
	String slotDatabase = "slotDatabase.txt";
	String pathMap = "images/mapa_vazio.png";
	Vaga posVaga[nVagas]; // posições de cada vaga, centro, largura, altura e inclinação
	Mat vaga_mapa[nVagas], histograma_mapa_h[nVagas];
	Mat vaga[nVagas], histograma_h[nVagas];
	//salvarLocalVagas(pathMap,nVagas,slotDatabase);
	carregarParametros(slotDatabase, posVaga, nVagas);
	//mostrarMapeamento(pathMap, posVaga,nVagas);
	Mat mapa = imread(pathMap, 1);
	// variavel para salvar os resultados
	ofstream contorno; // variavel para auxilio da gravação de dados
	contorno.open("c_26-Set-05_15.csv"); // abrindo arquivo onde os dados serão salvos

	ofstream hist_hue; // variavel para auxilio da gravação de dados
	hist_hue.open("h_26-Set-05_15.csv"); // abrindo arquivo onde os dados serão salvos

	Mat src;
	Mat cinza, hsv;
	vector<Mat> channels;
	Point2f posicao;
	int inclinacao;
	float correlacao_cinza[nVagas], correlacao_h[nVagas];
	int nContorno_mapa[nVagas];
	int nContorno[nVagas];
	for (int i = 0; i < nVagas; i++) {
		posicao = posVaga[i].posicao;
		inclinacao = posVaga[i].inclinacao;
		vaga_mapa[i] = pegarImagemCortada(mapa, posicao,
				Size2f(posVaga[i].largura, posVaga[i].altura), inclinacao);
		// numero de contornos de cada vaga do estacionamento vazio (EV)
		nContorno_mapa[i] = numeroContornos(vaga_mapa[i]);
		// histograma de cada vaga apenas do Hue do EV
		cvtColor(vaga_mapa[i], hsv, COLOR_BGR2HSV);
		split(hsv, channels); // separei os canais H, S e V
		histograma_mapa_h[i] = histCalc(channels[0], 180);
	}

	VideoCapture capture;
	while (1) {
		capture.open("/home/bianor/Dropbox/gravs/26-Set-05_15.mp4");
		if (!capture.isOpened()) {
			cout << "ERRO, VIDEO NAO ENCONTRADO\n";
			getchar();
			return -1;
		}
		while (capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(
				CV_CAP_PROP_FRAME_COUNT) - 1) {
			capture.read(src);
			// loop para processar as 14 vagas

			for (int i = 0; i < nVagas; i++) {
				posicao = posVaga[i].posicao;
				inclinacao = posVaga[i].inclinacao;
				vaga[i] = pegarImagemCortada(src, posicao,
						Size2f(posVaga[i].largura, posVaga[i].altura),
						inclinacao);
				// numero de contornos de cada vaga do estacionamento vazio (EV)
				nContorno[i] = numeroContornos(vaga[i]);
				contorno << nContorno[i] << " ";
				// histograma de cada vaga apenas do Hue do EV
				cvtColor(vaga[i], hsv, COLOR_BGR2HSV);
				split(hsv, channels); // separei os canais H, S e V
				histograma_h[i] = histCalc(channels[0], 180);
				// comparacao entre histogramas hue
				correlacao_h[i] = compareHist(histograma_mapa_h[i],
						histograma_h[i], HISTCMP_CORREL);
				hist_hue << correlacao_h[i] << " ";

			}
			contorno << "\n";
			hist_hue << "\n";
		}
		contorno.close();
		hist_hue.close();
		capture.release();
		imshow("vaga", vaga[5]);
		waitKey(0);
		cout << "PAROU";
	}

}
