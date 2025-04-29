#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp" // per disegno

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
using namespace Gedim;


int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh,"./Cell0Ds.csv","./Cell1Ds.csv","./Cell2Ds.csv"))
    {
        cerr << "file not found" << endl;
        return 1;
    }

    //devo ricavare il vettore di proprietà da passare alla funzione ExportPoints().

    //  inizializzo marker dei punti
    //auto [marker_punti, dati_punti] = Colorpunti(mesh);
    // Poi metti marker_punti dentro un vettore
    
                        
    // visualizzo i marker esportandoli come proprieta scalari(associate a un solo numero) nel file.inp

    // --- Colora i segmenti ---
    // auto [marker_segmenti, dati_segmenti] = Colorsegmenti(mesh); ( SE AVESSI LASCIATO IL PAIR)
    // Crea il vettore di proprietà dei segmenti
    

    //utilities.ExportPolygons( "./Cell2Ds_export.inp",mesh.Cell0DsCoordinates,mesh.Cell1DsExtremes)

    //cout<<mesh.Cell0DsCoordinates.transpose()<<endl;

    //StampaVectorVector(mesh.Cell2DsVertices);

    Checkmarker(mesh);

    cout << " " << endl;

    Checkmarkermap(mesh);

    cout << " " << endl;
    
    if (Checklati(mesh)) 
    cout << "check lati andato a buon fine"<< endl;
    
    cout << " "<< endl;

    if (Checkarea(mesh)) 
    cout<< "check area andato a buon fine"<< endl;

    Colorpunti(mesh);
    Colorsegmenti(mesh); 


    return 0;
}


