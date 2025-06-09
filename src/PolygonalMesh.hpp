#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumCell0Ds;
    unsigned int NumCell1Ds;
    unsigned int NumCell2Ds;
    
    vector<unsigned int> Cell0DsId;
    vector<unsigned int> Cell1DsId;
    vector<unsigned int> Cell2DsId;
    vector< unsigned int > Cell0DsMarker;
    vector< unsigned int > Cell1DsMarker;

    MatrixXd Cell0DsCoordinates;
    MatrixXi Cell1DsExtremes;

    vector<vector<unsigned int>> Cell2DsVertices;
    vector<vector<unsigned int>> Cell2DsEdges;

    map<unsigned int,list <unsigned int>> MarkerCell0Ds;  //mappa marker → lista di nodi (Cell0D)
    map<unsigned int,list <unsigned int>> MarkerCell1Ds;  //mappa marker → lista di segmenti (Cell1D)
    //map<unsigned int,list <unsigned int>> MarkerCell2Ds; 
};
}

//chiave = marker

//valore = lista di ID di segmenti/punti con quel marker.



