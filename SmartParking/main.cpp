<<<<<<< HEAD
// Universidade Federal do Rio Grande do Norte
// Aluno: Bianor Neto ~ 10.bianor@gmail.com
=======
>>>>>>> 2ab47f2102cc746eccf61e08329a79bb0582f863
#include "opencv2/opencv.hpp"
#include "sup_pdi.h" // Funcoes de suporte de PDI


string int2string ( int Number )
{
	stringstream ss;
	ss << Number;
	return ss.str();
}

int main() {

	Mat vaga_livre, vaga_ocupada, vaga_teste;
	// Pastas com as imagens para treinamento (10 imagens com vagas ocupadas e 10 imagens com vagas livres)
	string pasta_ocupada = "vaga_ocupada/";
	string pasta_livre = "vaga_livre/";
	// Pasta com as imagens para teste (5 imagens, variando entre ocupadas e livres)
	string pasta_teste = "vaga_teste/";

	// Arquivo onde os features cor e contorno para treinamento são salvos
	string trainDataFile = "trainData.csv";
	// Arquivo onde o estado de cada vaga para treinamento é salvo
	string responsesFile = "responses.csv";

	// Arquivo onde os features cor e contorno para teste são salvos
	string testDataFile = "testData.csv";

	ofstream trainData, responses,testeData;
	testeData.open(testDataFile.c_str());
	trainData.open(trainDataFile.c_str());
	responses.open(responsesFile.c_str());

	// Loop para extrair a matiz (hue) e numero de contornos das imagens de treinamento
	for (int i=1;i<=10;i++){

		vaga_ocupada = imread(pasta_ocupada+int2string(i)+".png", 1);
		vaga_livre = imread(pasta_livre+int2string(i)+".png", 1);

		if ((!vaga_ocupada.data)|| (!vaga_livre.data)){
			cout << "Could not open or find the image" << endl;
			return -1;
		}

		trainData << get_hue(vaga_ocupada) << " " << get_contour(vaga_ocupada)<< "\n";
		responses << 1 << "\n";

		trainData << get_hue(vaga_livre) << " " << get_contour(vaga_livre)<< "\n";
		responses << 0 << "\n";
	}

	// Loop para extrair a matiz (hue) e numero de contornos das imagens de teste
	for (int i=1;i<=5;i++){
		vaga_teste = imread(pasta_teste+int2string(i)+".png", 1);

		if (!vaga_teste.data){
			cout << "Could not open or find the image" << endl;
			return -1;
		}

		testeData << get_hue(vaga_teste) << " " << get_contour(vaga_teste)<< "\n";

	}

	testeData.close();
	trainData.close();
	responses.close();

	cout << "Data collection finished :)\nLet's call python for training and classification\n" << endl;

	system("python knn_classif.py");

	return 0;

}
