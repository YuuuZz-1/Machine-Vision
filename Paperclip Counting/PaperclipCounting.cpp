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

int countPaperclips(cv::Mat& image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    // Make the dark paperclips white so each can be counted as one component.
    cv::Mat mask;
    cv::threshold(gray, mask, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

    cv::Mat labels, stats, centroids;
    const int componentCount = cv::connectedComponentsWithStats(mask, labels, stats, centroids);
    const double minArea = mask.total() * 0.0005;
    int paperclipCount = 0;

    // Label 0 is the background. Ignore small specks and the dark image border.
    for (int label = 1; label < componentCount; ++label)
    {
        const int* component = stats.ptr<int>(label);
        const cv::Rect bounds(component[cv::CC_STAT_LEFT], component[cv::CC_STAT_TOP],
                              component[cv::CC_STAT_WIDTH], component[cv::CC_STAT_HEIGHT]);
        if (component[cv::CC_STAT_AREA] < minArea ||
            bounds.x == 0 || bounds.y == 0 ||
            bounds.br().x == image.cols || bounds.br().y == image.rows)
        {
            continue;
        }

        ++paperclipCount;
        cv::Mat componentMask = (labels(bounds) == label);
        std::vector<cv::Point> pixels;
        cv::findNonZero(componentMask, pixels);

        // Fit the box to the paperclip's angle, then map it back to the full image.
        cv::RotatedRect box = cv::minAreaRect(pixels);
        box.center += cv::Point2f(bounds.tl());
        cv::Point2f corners[4];
        box.points(corners);
        for (int i = 0; i < 4; ++i)
        {
            cv::line(image, corners[i], corners[(i + 1) % 4], markerColor, 2, cv::LINE_AA);
        }

        drawNumber(image, cv::Point(box.center), paperclipCount);
    }

    return paperclipCount;
}

}

int main(int argc, char* argv[])
{
    const std::string imagePath = argc > 1 ? argv[1] : R"(D:\vs2023\Paperclip.png)";
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty())
    {
        std::cerr << "Could not open image: " << imagePath << '\n';
        return 1;
    }

    const int count = countPaperclips(image);
    std::cout << "Paperclip count: " << count << std::endl;
    cv::imshow("Paperclip Counting - " + std::to_string(count), image);
    cv::waitKey(0);
    return 0;
}
