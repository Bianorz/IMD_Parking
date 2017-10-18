// Universidade Federal do Rio Grande do Norte
// Disciplina: Trabalho de Conclusão de Curso II
// Período: 2017.1
// Aluno: Bianor Neto ~ 10.bianor@gmail.com
// Prof. Orientador: José Alberto Nicolau
// Projeto: Estimação da distância entre câmera e objeto de cor conhecida, versão processamento de imagem de uma webcam
#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <stack>
using namespace cv;
using namespace std;

std::stack<clock_t> tictoc_stack;

void tic() {
	tictoc_stack.push(clock());
}

void toc() {
	std::cout << "Time elapsed: "
			<< ((double) (clock() - tictoc_stack.top())) / CLOCKS_PER_SEC
			<< " sec" << std::endl;
	tictoc_stack.pop();
}

// introducao/ trabalhos relacionados/ falar sobre o trabalho /conclusao trabalhos /imgs do feed/vagas nao sao respeitadas/exposicao a luminosidade/ motoristas nao respietam vagas,
int main() {
	//int haar = haarCascade();
	//cout << haar << endl;
	// SETUP STEP: Load an image to use for parking slot definition and set parking spaces
	const int nVagas = 14;
	String slotDatabase = "slotDatabase.txt";
	String pathMap = "images/mapa_vazio.png";
	Vaga posVaga[nVagas]; // posições de cada vaga, centro, largura, altura e inclinação
	Mat vaga_mapa[nVagas], histograma_mapa_h[nVagas];
	Mat vaga[nVagas], histograma_h[nVagas];
	int nContorno[nVagas];
	carregarParametros(slotDatabase, posVaga, nVagas);
	Mat mapa = imread(pathMap, 1);
	Mat src;
	Mat cinza, desenho, hsv;
	vector<Mat> channels;
	Point2f posicao;

	String videoPath = "/home/bianor/Dropbox/gravs/04-Out-11_15.mp4";

	ofstream fileContour; // variavel para auxilio da gravação de dados
	string fullpathContour = (videoPath.c_str() + string("_contour.csv"));
	fileContour.open(fullpathContour.c_str()); // abrindo arquivo onde os dados serão salvos

	ofstream fileHue; // variavel para auxilio da gravação de dados
	string fullpathHue = (videoPath.c_str() + string("_hue.csv"));
	fileHue.open(fullpathHue.c_str()); // abrindo arquivo onde os dados serão salvos

	float limiarUpCorrelation = 0.30;
	float limiarDownCorrelation = 0.4;

	float limiarUpCanny = 5.33;
	float limiarDownCanny = 3.88;

	float limiarUpContornos = 250;
	float limiarDownContornos = 80;

	int inclinacao;
	float correlacao_h[nVagas];
	for (int i = 0; i < nVagas; i++) {
		posicao = posVaga[i].posicao;
		inclinacao = posVaga[i].inclinacao;
		vaga_mapa[i] = pegarImagemCortada(mapa, posicao,
				Size2f(posVaga[i].largura, posVaga[i].altura), inclinacao);
		// histograma de cada vaga apenas do Hue do EV
		cvtColor(vaga_mapa[i], hsv, COLOR_BGR2HSV);
		split(hsv, channels); // separei os canais H, S e V
		histograma_mapa_h[i] = histCalc(channels[0], 180);
	}

	/*
	 If para verificação se existe camera conectada ao sistema
	 */
	VideoCapture capture; //

	capture.open("http://10.7.161.98:10088/?action=stream");
	if (!capture.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}
	//********************LOOP INFINITO PARA PROCESSAMENTO DA IMAGEM**********************

	int aux = 0;

	while (1) {
		if (aux == 0) {
			tic();
		}
		capture.read(src);
		src.copyTo(desenho);
		// loop para processar as 14 vagas
		for (int i = 0; i < nVagas; i++) {
			posicao = posVaga[i].posicao;
			inclinacao = posVaga[i].inclinacao;
			vaga[i] = pegarImagemCortada(src, posicao,
					Size2f(posVaga[i].largura, posVaga[i].altura), inclinacao);
			// histograma de cada vaga apenas do Hue do EV
			Rect a(posicao.x - posVaga[i].largura / 2,
					posicao.y - posVaga[i].altura / 2, posVaga[i].largura,
					posVaga[i].altura);
			cvtColor(vaga[i], hsv, COLOR_BGR2HSV);
			split(hsv, channels); // separei os canais H, S e V
			histograma_h[i] = histCalc(channels[0], 180);
			// comparacao entre histogramas hue
			correlacao_h[i] = compareHist(histograma_mapa_h[i], histograma_h[i],
					HISTCMP_CORREL);

			if (i <= 5) {
				if (correlacao_h[i] > limiarUpCorrelation) {
					fileHue << 0 << " ";
					//rectangle(desenho,a,Scalar(0, 255, 0), 2);
				} else {
					//rectangle(desenho,a,Scalar(0, 0, 255), 2);
					fileHue << 1 << " ";
				}
				nContorno[i] = numeroContornos(vaga[i], limiarUpCanny);
				if (nContorno[i] > limiarUpContornos) {
					fileContour << 0 << " ";
				} else {
					fileContour << 1 << " ";
				}

			} else {
				if (correlacao_h[i] > limiarDownCorrelation) {
					fileHue << 0 << " ";
					//rectangle(desenho,a,Scalar(0, 255, 0), 2);

				} else {
					fileHue << 1 << " ";
					//rectangle(desenho,a,Scalar(0, 0, 255), 2);
				}
				nContorno[i] = numeroContornos(vaga[i], limiarDownCanny);
				if (nContorno[i] > limiarDownContornos) {
					fileContour << 0 << " ";
				} else {
					fileContour << 1 << " ";
				}
			}
		}
		if (aux == 0) {
			toc();
			aux = 1;
		}
		/*namedWindow("Final Result", WINDOW_NORMAL);
		 imshow("Final Result", desenho);
		 waitKey(15);*/
	}
	return 0;
}

