#include "stdafx.h"
#include "BM3DDenoiser.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: BCDenoising.exe img_file sigma" << std::endl;
        return 0;
    }

    std::string image = argv[1];
    int sigma = std::atoi(argv[2]);
    BM3DDenoiser denoiser(image, sigma);
    denoiser.run();
    return 0;
}
