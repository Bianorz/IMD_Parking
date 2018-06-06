using namespace cv;
using namespace std;

Mat get_hue_channel(Mat src){
	Mat hsv;
	vector<Mat> channels;
	cvtColor(src, hsv, COLOR_BGR2HSV);
	split(hsv, channels); // separei os canais H, S e V
	return channels[0];
}

float get_hue(Mat src) {
	Mat hue_channel = get_hue_channel(src);
	Scalar m = mean(hue_channel);
	return m[0];
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


