#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace
{
const cv::Scalar markerColor(30, 220, 30);

void drawNumber(cv::Mat& image, cv::Point center, int index)
{
    const std::string number = std::to_string(index);
    const cv::Size size = cv::getTextSize(number, cv::FONT_HERSHEY_SIMPLEX, 0.7, 2, nullptr);
    const cv::Point origin(center.x - size.width / 2, center.y + size.height / 2);
    cv::putText(image, number, origin, cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(0, 0, 0), 4, cv::LINE_AA);
    cv::putText(image, number, origin, cv::FONT_HERSHEY_SIMPLEX, 0.7,
                markerColor, 2, cv::LINE_AA);
}

int countCoins(cv::Mat& image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 5);

    // Coin.png has two coin sizes, with radii of roughly 55 to 70 pixels.
    constexpr int minRadius = 48;
    constexpr int maxRadius = 78;
    constexpr double minCenterDistance = 90;
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, minCenterDistance,
                     100, 32, minRadius, maxRadius);

    int coinCount = 0;
    for (const cv::Vec3f& circle : circles)
    {
        const cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        cv::circle(image, center, cvRound(circle[2]), markerColor, 2, cv::LINE_AA);
        drawNumber(image, center, ++coinCount);
    }
    return coinCount;
}

}

int main(int argc, char* argv[])
{
    const std::string imagePath = argc > 1 ? argv[1] : R"(D:\vs2023\Coin.png)";
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Could not open image: " << imagePath << '\n';
        return 1;
    }

    const int count = countCoins(image);
    std::cout << "Coin count: " << count << std::endl;
    cv::imshow("Coin Counting - " + std::to_string(count), image);
    cv::waitKey(0);
    return 0;
}
