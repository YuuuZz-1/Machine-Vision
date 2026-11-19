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

int countSmallHoles(cv::Mat& image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 5);

    // Limit the search to the two small hole sizes in gray.png.
    constexpr int minRadius = 18;
    constexpr int maxRadius = 50;
    constexpr int borderMargin = 3;
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, 42,
                     100, 25, minRadius, maxRadius);

    int holeCount = 0;
    for (const cv::Vec3f& circle : circles)
    {
        const cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        const int radius = cvRound(circle[2]);

        // Keep only circles fully inside the image, clear of the dark frame.
        if (center.x - radius < borderMargin || center.y - radius < borderMargin ||
            center.x + radius >= image.cols - borderMargin ||
            center.y + radius >= image.rows - borderMargin)
        {
            continue;
        }

        cv::circle(image, center, radius, markerColor, 2, cv::LINE_AA);
        drawNumber(image, center, ++holeCount);
    }
    return holeCount;
}

}

int main(int argc, char* argv[])
{
    const std::string imagePath = argc > 1 ? argv[1] : R"(D:\vs2023\gray.png)";
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Could not open image: " << imagePath << '\n';
        return 1;
    }

    const int count = countSmallHoles(image);
    std::cout << "Small hole count: " << count << std::endl;
    cv::imshow("Small hole Counting - " + std::to_string(count), image);
    cv::waitKey(0);
    return 0;
}
