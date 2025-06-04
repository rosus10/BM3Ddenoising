#ifndef BM3D_DENOISER_H
#define BM3D_DENOISER_H

#include "Blockmatching.h"
#include <string>

class BM3DDenoiser {
public:
    BM3DDenoiser(const std::string &imagePath, int sigma);
    ~BM3DDenoiser();
    void run();

private:
    double PSNR_calculate(const cv::Mat &orig, const cv::Mat &test);

    std::string m_path;
    int m_sigma;
    cv::Mat m_gray;
    cv::Mat m_noisy;
    cv::Mat m_denoised;
    BlockMatching *m_blockMatcher;
};

#endif // BM3D_DENOISER_H
