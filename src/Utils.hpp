#pragma once

#include<vector>
#include <iostream>
#include "PolygonalMesh.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Gedim;

namespace PolygonalLibrary
{
    bool ImportMesh(PolygonalMesh& mesh,const string& nomefile0,const string& nomefile1,const string& nomefile2);

    bool ImportCell0Ds(PolygonalMesh& mesh,const string& nomefile);

    bool ImportCell1Ds(PolygonalMesh& mesh,const string& nomefile);

    bool ImportCell2Ds(PolygonalMesh& mesh,const string& nomefile);

    bool Checkmarker(const PolygonalMesh& mesh);

    bool Checkmarkermap(const PolygonalMesh& mesh);

    void StampaVectorVector(const vector<vector<unsigned int>>& vv);

    bool Checklati(PolygonalMesh& mesh );

    double area_triangolo (const Vector3d& A, const Vector3d& B, const Vector3d& C);

    bool Checkarea (PolygonalMesh& mesh );

    void Colorpunti(const PolygonalMesh& mesh);

    void Colorsegmenti(const PolygonalMesh& mesh);

}
