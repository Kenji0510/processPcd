#include <open3d/Open3D.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>


int voxelProcessOpen3D(std::string pcdFileName, std::string newFileNamePrefix) {
    // PCDファイルを読み込み
    auto cloud = open3d::io::CreatePointCloudFromFile(pcdFileName);
    if (cloud == nullptr) {
        std::cerr << "PCDファイルの読み込みに失敗しました " << pcdFileName << std::endl;
        return -1;
    }
    std::cout << "Read " << cloud->points_.size() << " data points from " << pcdFileName << std::endl;

    // ボクセルサイズを設定
    double voxel_size = 0.1;  // PCLの0.05mと同様

    // タイマー開始
    auto start_to_transfer_gpu = std::chrono::high_resolution_clock::now();

    // Transform to GPU tensor
    auto cloud_tensor = open3d::t::geometry::PointCloud::FromLegacy(*cloud, open3d::core::Dtype::Float32, open3d::core::Device("CUDA:0"));

    // タイマー終了
    auto finish_to_transfer_gpu = std::chrono::high_resolution_clock::now();

    // タイマー開始
    auto start_to_voxel = std::chrono::high_resolution_clock::now();

    // GPUでボクセルダウンサンプリングを実行
    //auto voxel_grid = cloud->VoxelDownSample(voxel_size);
    auto voxel_grid = cloud_tensor.VoxelDownSample(voxel_size);

    // タイマー終了
    auto finish_to_voxel = std::chrono::high_resolution_clock::now();

    // フィルタリングされたデータを保存
    //std::string voxelPcdFileName = "filtered_" + std::to_string(voxel_grid->points_.size()) + ".pcd";
    //open3d::io::WritePointCloud(voxelPcdFileName, *voxel_grid, true);
    //std::cout << "Saved " << voxel_grid->points_.size() << " data points to " << voxelPcdFileName << std::endl;
    //std::string filePath = "/home/kenji/workspace/resarch/pcdFile/aviaExportData/";
    std::string filePath = "/home/kenji/workspace/pcdFile/aviaExportData/";
    std::string voxelPcdFileName = filePath + "filtered_" + newFileNamePrefix + "_" + std::to_string(voxel_size) + "_" + std::to_string(voxel_grid.GetPointPositions().GetLength()) + ".pcd";
    //open3d::io::WritePointCloudToPCD(voxelPcdFileName, voxel_grid.ToLegacy(), true);
    std::cout << "Saved " << voxel_grid.GetPointPositions().GetLength() << " data points to " << voxelPcdFileName << std::endl;

    // 時間計算
    std::chrono::duration<double, std::milli> elapsed01 = finish_to_transfer_gpu - start_to_transfer_gpu;
    std::cout << "Transfer Data To GPU Process Time: " << elapsed01.count() << " ms" << std::endl;

    // 時間計算
    std::chrono::duration<double, std::milli> elapsed02 = finish_to_voxel - start_to_voxel;
    std::cout << "Voxel Process Time: " << elapsed02.count() << " ms" << std::endl;

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

std::string extractNewFileName(const std::string& originalFileName) {
    // 検索する文字列
    std::string searchString1 = "saved_cloud_";
    std::string searchString2 = "_20240430";
    
    // 検索する文字列が見つかった位置を取得
    size_t pos1 = originalFileName.find(searchString1);
    size_t pos2 = originalFileName.find(searchString2);

    // それぞれの文字列が見つかった場合
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        // `_20240430`より前の部分と、`saved_cloud_pub_`より後の部分を結合して新しいファイル名を生成
        std::string extractedName = originalFileName.substr(pos1 + searchString1.length(), pos2 - pos1 - searchString1.length());
        return extractedName;
    } else {
        // いずれかの文字列が見つからない場合はそのままのファイル名を返す
        return originalFileName;
    }
}

int main(void) {
    // Pcd File Directory Path
    //std::string pcdDirectoryPath = "/home/kenji/workspace/resarch/pcdFile/originalData";
    std::string pcdDirectoryPath = "/home/kenji/workspace/pcdFile/originalData";

    // Get Pcd Files
    auto pcdFiles = getFilesInDirectory(pcdDirectoryPath);

    for (const auto& path : pcdFiles) {
        std::cout << "PCD file path: " << path << std::endl; 
        std::string newFileNamePrefix = extractNewFileName(path);
        voxelProcessOpen3D(path, newFileNamePrefix);
    }

    return 0;
}