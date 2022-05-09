#pragma once
//
// Created by yuri on 8.5.22.
//

class ObjectLoader {
public:
    ObjectLoader() = default;

    ~ObjectLoader();

    void loadTerrain();

private:
    float * mTerrainMesh = nullptr;

};

