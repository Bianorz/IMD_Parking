#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	// SETUP STEP: Load an image to use for parking slot definition and set parking spaces
	const int nVagas = 14;
	String slotDatabase = "slotDatabase.txt";
	String pathMap = "images/mapa.png";
	Vaga vaga[nVagas];
	//salvarLocalVagas(pathMap,nVagas,slotDatabase);
	carregarParametros(slotDatabase, vaga, nVagas);
	mostrarMapeamento(pathMap, vaga,nVagas);
}
