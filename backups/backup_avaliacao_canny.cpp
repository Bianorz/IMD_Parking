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
	Mat mapa = imread(pathMap, 1);
	//mostrarMapeamento(mapa, posVaga,nVagas);

	// variavel para salvar os resultados
	ofstream diff; // variavel para auxilio da gravação de dados
	diff.open("diff.csv"); // abrindo arquivo onde os dados serão salvos

	//ofstream hist_hue; // variavel para auxilio da gravação de dados
	//hist_hue.open("h27-Set.csv"); // abrindo arquivo onde os dados serão salvos

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
	int cont = 1;
	VideoCapture capture;
	while (1) {
		capture.open("images/27_Set.mp4");
		if (!capture.isOpened()) {
			cout << "ERRO, VIDEO NAO ENCONTRADO\n";
			getchar();
			return -1;
		}
		while (capture.get(CV_CAP_PROP_POS_FRAMES)
				< capture.get(CV_CAP_PROP_FRAME_COUNT) - 1) {
			capture.read(src);
			src = imread("images/vazia.png", 1);
			Mat src2 = imread("images/ocupada.png", 1);
			if (!src.data) {
				cout << "Could not open or find the image" << std::endl;
				return -1;
			}
			// loop para processar as 14 vagas

			for (int i = 0; i < nVagas; i++) {
				posicao = posVaga[i].posicao;
				inclinacao = posVaga[i].inclinacao;
				vaga[i] = pegarImagemCortada(src, posicao,
						Size2f(posVaga[i].largura, posVaga[i].altura),
						inclinacao);
				Mat ocup = pegarImagemCortada(src2, posicao,
										Size2f(posVaga[i].largura, posVaga[i].altura),
										inclinacao);
				// numero de contornos de cada vaga do estacionamento vazio (EV)
				nContorno[i] = numeroContornos(vaga[i]);
				//contorno << nContorno[i] << " ";
				// histograma de cada vaga apenas do Hue do EV
				cvtColor(vaga[i], hsv, COLOR_BGR2HSV);
				split(hsv, channels); // separei os canais H, S e V
				histograma_h[i] = histCalc(channels[0], 180);
				// comparacao entre histogramas hue
				correlacao_h[i] = compareHist(histograma_mapa_h[i],
						histograma_h[i], HISTCMP_CORREL);
				//hist_hue << correlacao_h[i] << " ";
				int spot = 14;
				if (i == (spot -1)) {
					namedWindow("before canny", 0);
					imshow("before canny", vaga[i]);
					Mat alala,alala2;
					vaga[i].copyTo(alala);
					ocup.copyTo(alala2);
					Mat imagemBorrada,imagemBorrada2, bordas,bordas2;
					vector<vector<Point> > contours,contours2;
					vector<Vec4i> hierarchy,hierarchy2;
					GaussianBlur(vaga[i], imagemBorrada, Size(5, 5), 0, 0);
					GaussianBlur(ocup, imagemBorrada2, Size(5, 5), 0, 0);
					Canny(imagemBorrada, bordas, cont, 3*cont);
					Canny(imagemBorrada2, bordas2, cont, 3*cont);
					findContours(bordas, contours, hierarchy, CV_RETR_CCOMP,
							CV_CHAIN_APPROX_SIMPLE);
					findContours(bordas2, contours2, hierarchy2, CV_RETR_CCOMP,
							CV_CHAIN_APPROX_SIMPLE);
					int numeroBordas = hierarchy.size();
					int numeroBordas2 = hierarchy2.size();
				/*	cout << "borda vazia: "<<numeroBordas<<endl;
					cout << "borda ocupada: "<<numeroBordas2<<endl;
					cout << "contorno funcao 35: " << nContorno[i]<< endl;
					cout << "contador:" << cont<< endl;
					cout << "diferença:" << abs(numeroBordas2-numeroBordas)<< endl;
					*/
					diff << cont << " " << abs(numeroBordas2-numeroBordas) << "\n";
					if (hierarchy.size() > 0) {
						for (int index = 0; index >= 0; index =
								hierarchy[index][0]) {
							drawContours(alala, contours, index,
									Scalar(0, 255, 0), 1, 8, hierarchy, 0,
									Point());
						}
					}

					if (hierarchy2.size() > 0) {
						for (int index2 = 0; index2 >= 0; index2 =
								hierarchy2[index2][0]) {
							drawContours(alala2, contours2, index2,
									Scalar(0, 255, 0), 1, 8, hierarchy2, 0,
									Point());
						}
					}

					/*namedWindow("vazia borda", 0);
					imshow("vazia borda", alala);
					namedWindow("ocupada borda", 0);
					imshow("ocupada borda", alala2);
					*////waitKey(0);
					cont++;
					if (cont > 100){
						diff.close();
						cout << "fim";
						return -1;
					}


				}

				//waitKey(15);
			}

			//contorno << "\n";
			//hist_hue << "\n";
		}
		//contorno.close();
		//hist_hue.close();
		capture.release();
		cout << "PAROU";
		//imshow("vaga", vaga[5]);
		//waitKey(0);

	}

}

