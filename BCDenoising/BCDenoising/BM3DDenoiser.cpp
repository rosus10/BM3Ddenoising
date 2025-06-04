#include "BM3DDenoiser.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

BM3DDenoiser::BM3DDenoiser(const std::string &imagePath, int sigma)
    : m_path(imagePath), m_sigma(sigma), m_blockMatcher(nullptr) {}

BM3DDenoiser::~BM3DDenoiser() {
    delete m_blockMatcher;
}

double BM3DDenoiser::PSNR_calculate(const Mat &orig, const Mat &test) {
    double mse = 0.0;
    for (int i = 0; i < orig.rows; ++i) {
        const uchar *o = orig.ptr<uchar>(i);
        const double *t = test.ptr<double>(i);
        for (int j = 0; j < orig.cols; ++j) {
            double diff = o[j] - t[j];
            mse += diff * diff;
        }
    }
    mse /= (orig.cols * orig.rows);
    if (mse <= 0.0)
        return 0.0;
    double psnr = 10.0 * log10(255.0 * 255.0 / mse);
    return psnr;
}

void BM3DDenoiser::run() {
    Mat img = imread(m_path, IMREAD_UNCHANGED);
    if (!img.data || img.channels() != 1) {
        cerr << "Cannot open image or not a grayscale image: " << m_path << endl;
        return;
    }

    if (img.depth() == CV_16U) {
        img.convertTo(m_gray, CV_8U, 1.0 / 256.0);
    } else if (img.depth() == CV_8U) {
        m_gray = img;
    } else {
        cerr << "Unsupported image depth" << endl;
        return;
    }

    Mat noise(m_gray.size(), CV_64FC1);
    randn(noise, Scalar::all(0), Scalar::all(m_sigma));
    m_noisy = Mat::zeros(m_gray.size(), CV_64FC1);
    for (int i = 0; i < m_gray.rows; ++i) {
        const uchar *orig = m_gray.ptr<uchar>(i);
        const double *n = noise.ptr<double>(i);
        double *dst = m_noisy.ptr<double>(i);
        for (int j = 0; j < m_gray.cols; ++j)
            dst[j] = n[j] + orig[j];
    }

    m_blockMatcher = new BlockMatching(m_noisy, m_sigma);
    m_blockMatcher->Initial_hardthreshold();

    Mat est(m_noisy.size(), CV_64FC1);
    Mat out_est(m_noisy.size(), CV_8U);
    Mat out_noisy(m_noisy.size(), CV_8U);
    m_blockMatcher->Output_denoised(est, out_est, out_noisy);

    cout << "PSNR after first step: " << PSNR_calculate(m_gray, est) << endl;

    m_blockMatcher->Second_Wiener();
    m_blockMatcher->Output_denoised_final(est, out_est, out_noisy);
    cout << "PSNR after second step: " << PSNR_calculate(m_gray, est) << endl;

    m_denoised = est;
}
