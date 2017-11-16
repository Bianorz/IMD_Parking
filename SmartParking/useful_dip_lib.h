// Fun��es para o estacionamento inteligente
#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>   
#include <fstream>

using namespace cv;
using namespace std;

void draw_locations(Mat & img, const vector<Rect> & locations,
		const Scalar & color) {
	if (!locations.empty()) {
		vector<Rect>::const_iterator loc = locations.begin();
		vector<Rect>::const_iterator end = locations.end();
		for (; loc != end; ++loc) {
			rectangle(img, *loc, color, 2);
		}
	}
}

int haarCascade() {
	CascadeClassifier classifier;
	vector<Rect> vFound;
	classifier.load("cars3.xml");
	Mat src,gray;
	VideoCapture capture;
	while (1) {
		capture.open("/home/bianor/Dropbox/gravs/28-Set-16_15.mp4");
		if (!capture.isOpened()) {
			cout << "ERRO, VIDEO NAO ENCONTRADO\n";
			getchar();
			return -1;
		}
		while (capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(
				CV_CAP_PROP_FRAME_COUNT) - 1) {
			capture.read(src);
			cvtColor(src, gray, COLOR_BGR2GRAY);
			equalizeHist(gray, gray);
			classifier.detectMultiScale(gray, vFound,1.1, 1, 0, Size(0,0),gray.size());
			draw_locations(src, vFound, Scalar(0, 255, 0));
			namedWindow("teste haar",CV_WINDOW_NORMAL);
			imshow("teste haar",src);
			waitKey(20);
		}
		capture.release();
	}
	return 0;
}

Mat pegarImagemCortada(Mat src, Point2f centro, Size2f tamanho, int angulo) {
	Mat imagemCortada;
	Size tamImagemOriginal = src.size();
	RotatedRect rRect = RotatedRect(centro, tamanho, angulo);
	Point2f vertices[4];
	rRect.points(vertices);
	vector<Point> tri2CroppedInt;
	for (int i = 0; i < 4; ++i) {
		tri2CroppedInt.push_back(vertices[i]);
	}
	if (src.type() != 0) {
		Mat preta(tamImagemOriginal, CV_8UC3, Scalar(0, 0, 0));
		fillConvexPoly(preta, tri2CroppedInt, Scalar(255, 255, 255));
		bitwise_and(src, preta, imagemCortada);
	} else {
		Mat preta(tamImagemOriginal, CV_8U, Scalar(0));
		fillConvexPoly(preta, tri2CroppedInt, Scalar(255));
		bitwise_and(src, preta, imagemCortada);

	}

	Rect brect = rRect.boundingRect();
	imagemCortada = imagemCortada(brect);
	return imagemCortada;
}

void apenasParaCompletarParametros(int, void*) {
	// � chamado quando se usa os sliders, necess�rio passar uma fun��o como par�metro
}

void salvarLocalVagas(String nomeImagem, int nVagas, String nomeArqTxt) {
	int centro_x = 1;
	int centro_y = 1;
	int largura = 1;
	int altura = 1;
	int inclinacao = 0;
	int fim = 0;
	const string nomeJanela = "Parking Spot Positioning";
	namedWindow(nomeJanela, 0);

	Mat imagem = imread(nomeImagem, 1);
	createTrackbar("x", nomeJanela, &centro_x, imagem.cols,
			apenasParaCompletarParametros);
	createTrackbar("y", nomeJanela, &centro_y, imagem.rows,
			apenasParaCompletarParametros);
	createTrackbar("height", nomeJanela, &largura, imagem.cols,
			apenasParaCompletarParametros);
	createTrackbar("width", nomeJanela, &altura, imagem.rows,
			apenasParaCompletarParametros);
	createTrackbar("tilt", nomeJanela, &inclinacao, 360,
			apenasParaCompletarParametros);
	createTrackbar("end", nomeJanela, &fim, 1, apenasParaCompletarParametros);
	if (!imagem.data) {
		cout << "ERRO - ARQUIVO NAO ENCONTRADO" << endl;
		system("pause");
	}
	ofstream myfile;
	myfile.open(nomeArqTxt.c_str());
	printf(
			"Use o slider para realizar a marcacao, mude 'fim' para '1'quando acabar\n");
	for (int i = 1; i <= nVagas; i++) {
		while (fim == 0) {
			imagem = imread(nomeImagem, 1);
			RotatedRect rRect = RotatedRect(Point2f(centro_x, centro_y),
					Size2f(largura, altura), inclinacao);
			Point2f vertices[4];
			rRect.points(vertices);
			for (int i = 0; i < 4; i++)
				line(imagem, vertices[i], vertices[(i + 1) % 4],
						Scalar(0, 255, 0),3);

			imshow("rectangles", imagem);
			waitKey(15);
		}
		fim = 0;
		if (myfile.is_open()) {
			myfile << centro_x << " " << centro_y << " " << largura << " "
					<< altura << " " << inclinacao << "\n";
			cout << "Vaga " << i << " salva\n";
		} else {
			cout << "ERRO DE ACESSO AO ARQUIVO TXT \n";
			system("pause");
		}
	}
	destroyAllWindows();
	// Checo se arquivo est� aberto e escrevo os valores HSV
	if (myfile.is_open()) {
		myfile << "\ncentro_x centro_y largura altura inclinacao";
		myfile.close();
	} else {
		cout << "ERRO DE ACESSO AO ARQUIVO TXT \n";
		system("pause");
	}
	cout << "Marca��o concluida com sucesso, valores determinados salvos em "
			<< nomeArqTxt << endl;
	imshow("rectangles", imagem);
	waitKey(0);
	system("pause");

}

struct Vaga {
	Point2f posicao;
	int largura;
	int altura;
	int inclinacao;
};

void carregarParametros(String nomeArqTxt, Vaga vaga[], int nVagas) {
	ifstream file;
	file.open(nomeArqTxt.c_str());
	for (int i = 0; i < nVagas; i++) {
		file >> vaga[i].posicao.x >> vaga[i].posicao.y >> vaga[i].largura
				>> vaga[i].altura >> vaga[i].inclinacao;
	}
	file.close();
}

void mostrarMapeamento(Mat imagem, Vaga vaga[], int nVagas) {
	//Mat imagem = imread(nomeImagem, 1);
	if (!imagem.data) {
		cout << "ERRO - ARQUIVO NAO ENCONTRADO" << endl;
		system("pause");
	}
	for (int i = 0; i < nVagas; i++) {
		RotatedRect rRect = RotatedRect(
				Point2f(vaga[i].posicao.x, vaga[i].posicao.y),
				Size2f(vaga[i].largura, vaga[i].altura), vaga[i].inclinacao);
		Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(imagem, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0),3);
	}

	imshow("mapa", imagem);
	waitKey(0);
}

int numeroContornos(Mat imagem, double lowerThresh) {
	vector<vector<Point> > contours;
	//int lowerThresh = 5;
	vector<Vec4i> hierarchy;
	Mat imagemBorrada, bordas;
	GaussianBlur(imagem, imagemBorrada, Size(5, 5), 0, 0);
	Canny(imagemBorrada, bordas, lowerThresh, 3*lowerThresh);
	findContours(bordas, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE);
	int numeroBordas = hierarchy.size();
	return numeroBordas;
}

float mediaArray(float vetor[], int buff) {
	float sum = 0, media = 0;
	for (int i = 0; i < buff; i++) {
		sum = sum + vetor[i];
	}
	media = ((float) sum) / buff;
	return media;
}

Mat histCalc(Mat image, int hsize) {
	Mat hist;
	int histSize = hsize; // número de cores do meu histograma
	float range[] = { 0, hsize }; // range de cores
	const float* histRange = { range }; // variavel do histograma
	calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, &histRange); // cálculo o histograma de cada vaga e salvo em backHist
	return hist;
}

void plotHist(Mat hist, int histSize) {
	int hist_w = 640;
	int hist_h = 480;
	int bin_w = cvRound((double) hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++) {
		line(
				histImage,
				Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float> (i - 1))),
				Point(bin_w * (i), hist_h - cvRound(hist.at<float> (i))),
				Scalar(255, 255, 255), 2, 8, 0);
	}
	namedWindow("Histograma", CV_WINDOW_NORMAL);
	imshow("Histograma", histImage);
	waitKey(0);
}


Mat get_hue_channel(Mat src){
	Mat hsv;
	vector<Mat> channels;
	cvtColor(src, hsv, COLOR_BGR2HSV);
	split(hsv, channels); // separei os canais H, S e V
	return channels[0];
}

int get_contour(Mat src){

}


