#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include "../MySLQ/vector.h"

using namespace std;
using namespace chrono;

// 定义边界框结构体
struct Rect
{
    int x, y, width, height;
    double confidence;

    Rect() : x(0), y(0), width(0), height(0), confidence(0.0) {}
    Rect(int x, int y, int w, int h, double conf) : x(x), y(y), width(w), height(h), confidence(conf) {}

    int area() const
    {
        return width * height;
    }
};

// 计算两个边界框的交并比(IOU)
double IOU(const Rect& r1, const Rect& r2)
{
    int x1 = std::max(r1.x, r2.x);
    int y1 = std::max(r1.y, r2.y);
    int x2 = std::min(r1.x + r1.width, r2.x + r2.width);
    int y2 = std::min(r1.y + r1.height, r2.y + r2.height);
    int w = std::max(0, (x2 - x1 + 1));
    int h = std::max(0, (y2 - y1 + 1));
    double inter = w * h;
    double o = inter / (r1.area() + r2.area() - inter);
    return (o >= 0) ? o : 0;
}

// 快速排序分区函数
int partition_(Vector<pair<double, int>>& arr, int low, int high)
{
    pair<double, int> pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].first > pivot.first)
        {
            i++;
            pair<double, int> temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    pair<double, int> temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// 快速排序函数
void quickSort_(Vector<pair<double, int>>& arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition_(arr, low, high);
        quickSort_(arr, low, pi - 1);
        quickSort_(arr, pi + 1, high);
    }
}

// NMS算法实现，使用自定义Vector库
void nms(Vector<Rect>& proposals, const double nms_threshold)
{
    Vector<double> scores;
    for (int i = 0; i < proposals.size(); i++)
        scores.insert(proposals[i].confidence);

    Vector<int> index;
    for (int i = 0; i < scores.size(); i++)
    {
        index.insert(i);
    }

    // 根据置信度排序
    Vector<pair<double, int>> score_index_pairs(10, 0, pair<double, int>());
    for (int i = 0; i < index.size(); i++)
    {
        pair<double, int> p;
        p.first = scores[index[i]];
        p.second = index[i];
        score_index_pairs.insert(p);
    }

    // 使用选择排序
    for (int i = 0; i < score_index_pairs.size() - 1; i++)
    {
        int max_idx = i;
        for (int j = i + 1; j < score_index_pairs.size(); j++)
        {
            if (score_index_pairs[j].first > score_index_pairs[max_idx].first)
            {
                max_idx = j;
            }
        }
        if (max_idx != i)
        {
            pair<double, int> temp = score_index_pairs[i];
            score_index_pairs[i] = score_index_pairs[max_idx];
            score_index_pairs[max_idx] = temp;
        }
    }

    for (int i = 0; i < index.size(); i++)
    {
        index[i] = score_index_pairs[i].second;
    }

    Vector<bool> del(10, 0, false);
    for (int i = 0; i < scores.size(); i++)
    {
        del.insert(false);
    }

    for (int i = 0; i < index.size(); i++)
    {
        if (!del[index[i]])
        {
            for (int j = i + 1; j < index.size(); j++)
            {
                if (IOU(proposals[index[i]], proposals[index[j]]) > nms_threshold)
                {
                    del[index[j]] = true;
                }
            }
        }
    }

    Vector<Rect> new_proposals(10, 0, Rect());
    for (int i = 0; i < index.size(); i++)
    {
        if (!del[index[i]])
            new_proposals.insert(proposals[index[i]]);
    }
    proposals = new_proposals;
}

// 使用不同排序算法的NMS实现
void nms_with_sort(Vector<Rect>& proposals, const double nms_threshold, int sort_method)
{
    Vector<double> scores;
    for (int i = 0; i < proposals.size(); i++)
        scores.insert(proposals[i].confidence);

    Vector<int> index;
    for (int i = 0; i < scores.size(); i++)
    {
        index.insert(i);
    }

    // 使用不同的排序方法
    switch (sort_method)
    {
    case 1: // 冒泡排序
    {
        Vector<pair<double, int>> score_index_pairs(10, 0, pair<double, int>());
        for (int i = 0; i < index.size(); i++)
        {
            pair<double, int> p;
            p.first = scores[index[i]];
            p.second = index[i];
            score_index_pairs.insert(p);
        }

        // 使用冒泡排序
        for (int i = 0; i < score_index_pairs.size() - 1; i++)
        {
            bool swapped = false;
            for (int j = 0; j < score_index_pairs.size() - i - 1; j++)
            {
                if (score_index_pairs[j].first < score_index_pairs[j + 1].first)
                {
                    pair<double, int> temp = score_index_pairs[j];
                    score_index_pairs[j] = score_index_pairs[j + 1];
                    score_index_pairs[j + 1] = temp;
                    swapped = true;
                }
            }
            if (!swapped)
                break;
        }

        for (int i = 0; i < index.size(); i++)
        {
            index[i] = score_index_pairs[i].second;
        }
    }
    break;
    case 2: // 选择排序
    {
        Vector<pair<double, int>> score_index_pairs(10, 0, pair<double, int>());
        for (int i = 0; i < index.size(); i++)
        {
            pair<double, int> p;
            p.first = scores[index[i]];
            p.second = index[i];
            score_index_pairs.insert(p);
        }

        // 使用选择排序
        for (int i = 0; i < score_index_pairs.size() - 1; i++)
        {
            int max_idx = i;
            for (int j = i + 1; j < score_index_pairs.size(); j++)
            {
                if (score_index_pairs[j].first > score_index_pairs[max_idx].first)
                {
                    max_idx = j;
                }
            }
            if (max_idx != i)
            {
                pair<double, int> temp = score_index_pairs[i];
                score_index_pairs[i] = score_index_pairs[max_idx];
                score_index_pairs[max_idx] = temp;
            }
        }

        for (int i = 0; i < index.size(); i++)
        {
            index[i] = score_index_pairs[i].second;
        }
    }
    break;
    case 3: // 快速排序
    {
        Vector<pair<double, int>> score_index_pairs(10, 0, pair<double, int>());
        for (int i = 0; i < index.size(); i++)
        {
            pair<double, int> p;
            p.first = scores[index[i]];
            p.second = index[i];
            score_index_pairs.insert(p);
        }

        // 使用快速排序
        quickSort_(score_index_pairs, 0, score_index_pairs.size() - 1);

        for (int i = 0; i < index.size(); i++)
        {
            index[i] = score_index_pairs[i].second;
        }
    }
    break;
    default: // 默认使用简单排序
    {
        Vector<pair<double, int>> score_index_pairs(10, 0, pair<double, int>());
        for (int i = 0; i < index.size(); i++)
        {
            pair<double, int> p;
            p.first = scores[index[i]];
            p.second = index[i];
            score_index_pairs.insert(p);
        }

        // 使用简单的双重循环排序
        for (int i = 0; i < score_index_pairs.size() - 1; i++)
        {
            for (int j = i + 1; j < score_index_pairs.size(); j++)
            {
                if (score_index_pairs[i].first < score_index_pairs[j].first)
                {
                    pair<double, int> temp = score_index_pairs[i];
                    score_index_pairs[i] = score_index_pairs[j];
                    score_index_pairs[j] = temp;
                }
            }
        }

        for (int i = 0; i < index.size(); i++)
        {
            index[i] = score_index_pairs[i].second;
        }
    }
    }

    Vector<bool> del(10, 0, false);
    for (int i = 0; i < scores.size(); i++)
    {
        del.insert(false);
    }

    for (int i = 0; i < index.size(); i++)
    {
        if (!del[index[i]])
        {
            for (int j = i + 1; j < index.size(); j++)
            {
                if (IOU(proposals[index[i]], proposals[index[j]]) > nms_threshold)
                {
                    del[index[j]] = true;
                }
            }
        }
    }

    Vector<Rect> new_proposals(10, 0, Rect());
    for (int i = 0; i < index.size(); i++)
    {
        if (!del[index[i]])
            new_proposals.insert(proposals[index[i]]);
    }
    proposals = new_proposals;
}

// 生成分散的随机边界框
void generate_scattered_boxes(Vector<Rect>& boxes, int count)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> x_dist(0, 1000);
    uniform_int_distribution<> y_dist(0, 1000);
    uniform_int_distribution<> size_dist(10, 100);
    uniform_real_distribution<> conf_dist(0.0, 1.0);

    for (int i = 0; i < count; i++)
    {
        int x = x_dist(gen);
        int y = y_dist(gen);
        int width = size_dist(gen);
        int height = size_dist(gen);
        double confidence = conf_dist(gen);
        Rect r(x, y, width, height, confidence);
        boxes.insert(r);
    }
}

// 生成聚集的随机边界框
void generate_clustered_boxes(Vector<Rect>& boxes, int count)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> cluster_dist(1, 5); // 生成1-5个聚类中心
    uniform_int_distribution<> size_dist(10, 100);
    uniform_real_distribution<> conf_dist(0.0, 1.0);

    int clusters = cluster_dist(gen);

    // 生成聚类中心
    vector<pair<int, int>> centers(clusters);
    uniform_int_distribution<> x_dist(0, 1000);
    uniform_int_distribution<> y_dist(0, 1000);

    for (int i = 0; i < clusters; i++)
    {
        centers[i] = make_pair(x_dist(gen), y_dist(gen));
    }

    // 围绕聚类中心生成边界框
    uniform_int_distribution<> offset_dist(-100, 100);
    for (int i = 0; i < count; i++)
    {
        int cluster_idx = i % clusters;
        int center_x = centers[cluster_idx].first;
        int center_y = centers[cluster_idx].second;

        int x = max(0, min(1000, center_x + offset_dist(gen)));
        int y = max(0, min(1000, center_y + offset_dist(gen)));
        int width = size_dist(gen);
        int height = size_dist(gen);
        double confidence = conf_dist(gen);
        Rect r(x, y, width, height, confidence);
        boxes.insert(r);
    }
}

// 测试NMS性能
void test_nms_performance()
{
    Vector<int> test_sizes(10, 0, int());
    test_sizes.insert(100);
    test_sizes.insert(1000);
    test_sizes.insert(10000);

    const double nms_threshold = 0.1;

    cout << "测试NMS在不同排序算法下的性能表现（分散数据）：" << endl;
    printf("%-10s\t%-12s\t%-10s\t%-10s\n", "框数量", "排序算法", "耗时(ms)", "剩余框数");

    for (int i = 0; i < test_sizes.size(); i++)
    {
        int size = test_sizes[i];

        cout << "数据规模: " << size << endl;

        // 测试默认排序方法
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_scattered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms(proposals, nms_threshold);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "默认排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试冒泡排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_scattered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 1);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "冒泡排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试选择排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_scattered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 2);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "选择排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试快速排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_scattered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 3);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "快速排序", duration.count() / 1000.0, proposals.size());
        }

        cout << endl;
    }

    cout << "测试NMS在不同排序算法下的性能表现（聚集数据）：" << endl;
    printf("%-10s\t%-12s\t%-10s\t%-10s\n", "框数量", "排序算法", "耗时(ms)", "剩余框数");

    for (int i = 0; i < test_sizes.size(); i++)
    {
        int size = test_sizes[i];

        cout << "数据规模: " << size << endl;

        // 测试默认排序方法
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_clustered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms(proposals, nms_threshold);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "默认排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试冒泡排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_clustered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 1);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "冒泡排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试选择排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_clustered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 2);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "选择排序", duration.count() / 1000.0, proposals.size());
        }

        // 测试快速排序
        {
            Vector<Rect> proposals(10, 0, Rect());
            generate_clustered_boxes(proposals, size);

            auto start = high_resolution_clock::now();
            nms_with_sort(proposals, nms_threshold, 3);
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);
            printf("%-10s\t%-12s\t%-10.3f\t%-10d\n", "", "快速排序", duration.count() / 1000.0, proposals.size());
        }

        cout << endl;
    }
}

int main()
{
    test_nms_performance();
    return 0;
}