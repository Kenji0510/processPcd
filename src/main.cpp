#include <open3d/Open3D.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>


int voxelProcessOpen3D(std::string pcdFileName) {
    // PCDファイルを読み込み
    auto cloud = open3d::io::CreatePointCloudFromFile(pcdFileName);
    if (cloud == nullptr) {
        std::cerr << "PCDファイルの読み込みに失敗しました " << pcdFileName << std::endl;
        return -1;
    }
    std::cout << "Read " << cloud->points_.size() << " data points from " << pcdFileName << std::endl;

    // ボクセルサイズを設定
    double voxel_size = 0.05;  // PCLの0.05mと同様

    // タイマー開始
    auto start = std::chrono::high_resolution_clock::now();

    // ボクセルダウンサンプリングを実行
    auto voxel_grid = cloud->VoxelDownSample(voxel_size);

    // タイマー終了
    auto finish = std::chrono::high_resolution_clock::now();

    // フィルタリングされたデータを保存
    std::string voxelPcdFileName = "filtered_" + std::to_string(voxel_grid->points_.size()) + ".pcd";
    open3d::io::WritePointCloud(voxelPcdFileName, *voxel_grid, true);
    std::cout << "Saved " << voxel_grid->points_.size() << " data points to " << voxelPcdFileName << std::endl;

    // 時間計算
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    std::cout << "Voxel Process Time: " << elapsed.count() << " ms" << std::endl;

    return 0;
}

std::vector<std::string> getFilesInDirectory(std::string directoryPath) {
    std::vector<std::string> filePaths;

    // Check if directory exists
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        auto filePath = entry.path();

        // Check if file is pcd
        if (filePath.extension() == ".pcd") {
            filePaths.push_back(filePath.string());
        }
    }
    return filePaths;
}

int main(void) {
    // Pcd File Directory Path
    std::string pcdDirectoryPath = "/home/kenji/workspace/resarch/processPcdOnCpp/data";
    // Get Pcd Files
    auto pcdFiles = getFilesInDirectory(pcdDirectoryPath);

    for (const auto& path : pcdFiles) {
        std::cout << "PCD file path: " << path << std::endl; 
        voxelProcessOpen3D(path);
    }

    return 0;
}