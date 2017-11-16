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
int main() {

	VideoCapture capture;

	Mat src,crop,mapa_vazio;
	Mat hue,hue_vazio;
	Mat histogramaHue,histogramaVazio;

	float correlacao = 0;

	uint8_t teste = 8;

	mapa_vazio = imread("images/mapa_vazio.png");
	if (!mapa_vazio.data){
		cout << "ERRO, imagem não encontrada\n";
				return -1;
	}
	Mat corte = pegarImagemCortada(mapa_vazio,Point2f(659, 111),Size2f(77, 168),0);
	hue_vazio = get_hue_channel(corte);
	histogramaVazio = histCalc(hue_vazio, 180);


	capture.open("opa.mp4");

	if (!capture.isOpened()) {
		cout << "ERRO, VIDEO NAO ENCONTRADO\n";
		return -1;
	}

	while (capture.get(CV_CAP_PROP_POS_FRAMES)
					< capture.get(CV_CAP_PROP_FRAME_COUNT) - 1) {
		capture.read(src);
		crop = pegarImagemCortada(src,Point2f(659, 111),Size2f(77, 168),0);
		hue = get_hue_channel(crop);
		histogramaHue = histCalc(hue, 180);
		correlacao = compareHist(histogramaVazio,
					histogramaVazio, HISTCMP_CORREL);
	}
	cout << "finish" << endl;

	return 0;
}
