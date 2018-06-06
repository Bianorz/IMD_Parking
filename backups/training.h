/* These functions are used to generate the data for the KNN classifier, the
 * features will be |time|contour|color|
 *
 * time: Time of the frame in hours. Resolution improvement needed
 * contour: Number of contours detected inside the parking space
 * color: mean of the hue color values inside the parking space
 *
 */

using namespace std;
using namespace cv;

float get_hue(Mat src) {
	Mat hue_channel = get_hue_channel(src);
	Scalar m = mean(hue_channel);
	return m[0];
}

int get_time(string s) {
	char msb = s.at(7);
	char lsb = s.at(8);
	string MSB, LSB;
	MSB = msb;
	LSB = lsb;
	string time_concac = MSB;
	time_concac += LSB;
	int time = atoi(time_concac.c_str());
	return time;
}

int get_contour(Mat imagem) {
	double lowerThresh = 5;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat imagemBorrada, bordas;
	GaussianBlur(imagem, imagemBorrada, Size(5, 5), 0, 0);
	Canny(imagemBorrada, bordas, lowerThresh, 3 * lowerThresh);
	findContours(bordas, contours, hierarchy, CV_RETR_CCOMP,
			CV_CHAIN_APPROX_SIMPLE);
	int numeroBordas = hierarchy.size();
	return numeroBordas;
}

void generate_database(string video_folder, string trainDataFile,
		string responsesFile) {

	int number_of_files = number_of_files_inside_folder(video_folder.c_str());
	string *file_list = get_fileList(video_folder.c_str());
	VideoCapture cap;
	Mat src, slot;
	int frame_number = 0;
	ofstream trainData, responses;
	trainData.open(trainDataFile.c_str());
	responses.open(responsesFile.c_str());
	int video_ok = 0;
	for (int i = 0; i < number_of_files; i++) {

		frame_number = 0;
		string videoPath = video_folder + file_list[i];

		cap.open(videoPath.c_str());

		if (!cap.isOpened()) {
			cout << "ERRO, VIDEO NAO ENCONTRADO\n";
		} else {
			video_ok++;
			while (cap.get(CV_CAP_PROP_POS_FRAMES)
					< cap.get(CV_CAP_PROP_FRAME_COUNT) - 1) {

				cap.read(src);
				slot = get_cropped_image(src, Point2f(659, 111),
						Size2f(77, 168), 0);
				if ((frame_number % 30) == 0) {

					trainData << get_time(file_list[i]) << " "
							<< get_hue(slot) << " " << get_contour(slot)
							<< "\n";
					imshow("slot", slot);
					int state = waitKey(0);
					cout << state << endl;
					if (state == 81) {
						responses << "0" << "\n";
						cout << "vaga vazia" << endl;
					} else if (state == 83) {
						responses << "1" << "\n";
						cout << "vaga ocupada" << endl;
					} else {
						break;
					}
				}
				frame_number++;

			}
		}
	}
	trainData.close();
	responses.close();
	cout << "Database created" << " " <<"\number of analysed videos: "<< video_ok << endl;
}

