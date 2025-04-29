#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "UCDUtilities.hpp"


using namespace Eigen;

namespace PolygonalLibrary 
{

bool ImportMesh(PolygonalMesh& mesh,const string& nomefile0,const string& nomefile1,const string& nomefile2)
{

    if(!ImportCell0Ds(mesh,nomefile0))
        return false;

    if(!ImportCell1Ds(mesh,nomefile1))
        return false;

    if(!ImportCell2Ds(mesh,nomefile2))
        return false;

    return true;

}

bool ImportCell0Ds(PolygonalMesh& mesh,const string& nomefile)
{
    ifstream file(nomefile);
    

    if(file.fail())
    {
        cerr << "Errore nell'apertura di " << nomefile << endl;
        return false;
    }
        

    list<string> listLines;

    string line;

    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front(); // elimina elt in fronte alla lista

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds); // stabilisce capacita necessaria per il vettore, reserve:spazio riservato ma nessun elemento creato , con resize non potrei piu utilizzare push_back ( la size aumenta ogni pushback) 
    
    mesh.Cell0DsCoordinates = MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines) // per ogni linea in lista
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        string variabile;

        getline(converter, variabile, ';'); 
        id = stoi(variabile);

        getline(converter, variabile, ';');  
        marker = stoi(variabile);

        getline(converter, variabile, ';'); 
        mesh.Cell0DsCoordinates(0, id) = stod(variabile);

        getline(converter, variabile, ';'); 
        mesh.Cell0DsCoordinates(1, id) = stod(variabile);

        mesh.Cell0DsId.push_back(id);
        mesh.Cell0DsMarker.push_back(marker);


        if ( marker != 0 ) // memorizzo solo i marker non nulli nella mappa
        {
            auto it = mesh.MarkerCell0Ds.find(marker); 
            if ( it != mesh.MarkerCell0Ds.end() ) 

             {
                (*it).second.push_back(id);  // se lo trova 
               
             }
            else
            {
                mesh.MarkerCell0Ds.insert({marker, {id}}) ;
            }

        }

    }

    return true;
}



bool ImportCell1Ds(PolygonalMesh& mesh,const string& nomefile)
{
    ifstream file(nomefile);
    

    if(file.fail())
    {
        cerr << "Errore nell'apertura di " << nomefile << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtremes = MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        //Vector2i vertices; non viene utilizzato
        string variabile;
        
        getline(converter, variabile, ';'); 
        id = stoi(variabile);

        getline(converter, variabile, ';');  
        marker = stoi(variabile);

        getline(converter, variabile, ';'); 
        mesh.Cell1DsExtremes(0, id) = stoi(variabile);

        getline(converter, variabile, ';'); 
        mesh.Cell1DsExtremes(1, id) = stoi(variabile);

        mesh.Cell1DsId.push_back(id);
        mesh.Cell1DsMarker.push_back(marker);

        if ( marker != 0 ) // memorizzo solo i marker non nulli nella mappa
        {
            auto it = mesh.MarkerCell1Ds.find(marker); 
            if ( it != mesh.MarkerCell1Ds.end() ) 

             {
                (*it).second.push_back(id);  // se lo trova 
               
             }
            else
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }

        }

    }

    return true;
}




bool ImportCell2Ds(PolygonalMesh& mesh,const string& nomefile)
{
    ifstream file(nomefile);

    if(file.fail())
    {
        cerr << "Errore nell'apertura di " << nomefile << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds); //devo fare vector di vector , adesso definisco la dimensione del vector che cnterra altri vector
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id, marker, numVertices, numEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;
        
        //replace(listLines.begin(), listLines.end(), ';' ,' ') // sostituisco tutti i ; con spazi in modo da poter usare >>
        //converter >>  id >> marker >> NumVertices; avrei potuto fare cosi se nel file avessi avuto spazi ma ho ;
        
        string variabile;

        getline(converter, variabile, ';'); //legge il primo numero
        id = stoi(variabile);

        getline(converter, variabile, ';'); //legge il secondo numero 
        marker = stoi(variabile);

        getline(converter, variabile, ';'); //legge il terzo numero
        numVertices = stoi(variabile);

        // cicli per i vertici
        for (unsigned int i = 0; i < numVertices; ++i) {
            getline(converter, variabile, ';');
            vertices.push_back(stoi(variabile));
        }

        // leggi il numero di edges
        getline(converter, variabile, ';');
        numEdges = stoi(variabile);

        // cicli con gli edges
        for (unsigned int i = 0; i < numEdges; ++i) {
            getline(converter, variabile, ';'); //qui ho il vettore di vertici
            edges.push_back(stoi(variabile));
        }

        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);// qui ho il vrttore di vettore di vertici
        mesh.Cell2DsEdges.push_back(edges); 

        //INUTILE IN QUESTO CASO IN QUANTO I MARKER SONO TUTTI NULLI 

        /*if ( marker != 0 ) // memorizzo solo i marker non nulli nella mappa
        {
            auto it = mesh.MarkerCell2Ds.find(marker);  // find restituisce end quando non trova il marker 
            if ( it != mesh.MarkerCell2Ds.end() ) 
                {
                    (*it).second.push_back(id);  // se lo trova 
                }
            else
                {
                    mesh.MarkerCell2Ds.insert({marker, {id}}); // se non lo trova ne aggiunge un'altra
                }

        }*/   
    } 
    return true;
}


//dovevo verificare di aver salvato correttamente un vector di vector
void StampaVectorVector(const vector<vector<unsigned int>>& vv)
{
    for (size_t i = 0; i < vv.size(); ++i)
    {
        cout << "Riga " << i << ": ";
        for (size_t j = 0; j < vv[i].size(); ++j)
        {
            cout << vv[i][j] << " ";
        }
        cout << endl;
    }
}



bool Checkmarker(const PolygonalMesh& mesh)
{
    cout<<"PUNTI:"<<endl;
    for (size_t i = 0; i < mesh.Cell0DsId.size(); ++i)
    {
    if ( mesh.Cell0DsMarker[i]!= 0 )  // cosi stampo solo quelli con un marker e non tutti(riesco a controllare meglio)
    cout << "ID Punto: " << mesh.Cell0DsId[i] << ", Marker: " << mesh.Cell0DsMarker[i] << endl;
    }

    cout<<" "<<endl;

    cout<<"SEGMENTI:"<<endl;
    for (size_t i = 0; i < mesh.Cell1DsId.size(); ++i)
    {
        if ( mesh.Cell1DsMarker[i]!= 0 ) 
        cout << "ID Segmento :" << mesh.Cell1DsId[i] << ", Marker: " << mesh.Cell1DsMarker[i] << endl;
    }
    
   
 return true ;
}


// potrei verificarli anche tramite le mappe
bool Checkmarkermap(const PolygonalMesh& mesh )
{   
    cout<<"PUNTI:"<<endl;
    for (const auto& [marker, ids] : mesh.MarkerCell0Ds) //ciclo for sulla map MarkerCell0Ds
    {
        cout<<"marker:"<< marker << "--> Ids: ";

        for (auto id:ids) {cout << id <<" ";}

        cout<<endl;
    } 

    cout<<" "<<endl;
    
    cout<<"SEGMENTI:"<<endl;
    for (const auto& [marker, ids] : mesh.MarkerCell1Ds) //ciclo for sulla map MarkerCell1Ds
    {
        cout<<"marker:"<< marker << "--> Ids: ";

        for (auto id:ids) {cout << id <<" ";}

        cout<<endl;
    } 
    return true;
}
    
    

// bool ImportCell1Ds non serve :i dati della mesh sono già stati caricati prima in main.cpp!

bool Checklati(PolygonalMesh& mesh )
{  
   for( unsigned int id = 0 ; id < mesh.NumCell1Ds; id ++ )

   if (mesh.Cell1DsExtremes(0, id)==mesh.Cell1DsExtremes(1, id)) // se i due estremi sono uguali la lunghezza del lato è 0 
   {
        cerr << "Errore: segmenti lunghezza = 0" << endl;
        return 1;
   }

   return true;
}

//Per lavorare con il prodotto vettoriale (cross product),  servono vettori eigen
double area_triangolo(const Vector3d& A, const Vector3d& B, const Vector3d& C)
{
    Vector3d AB = B - A;
    Vector3d AC = C - A;
    Vector3d pv= AB.cross(AC);

    return 0.5 * pv.norm();
}


bool Checkarea (PolygonalMesh& mesh ) 
{  
   // per trovare l'area del poligono scompongo in triangoli il poligono  Area_triangolo = 0.5*​ ∥AB × AC∥
   for (unsigned int i = 0; i < mesh.Cell2DsVertices.size(); i ++) // considero ogni elt (vettore) del vettore
   {  
        double area_poligono = 0.0 ;
        vector<unsigned int> vertici =  mesh.Cell2DsVertices[i]; // vertici del poligono i 
        MatrixXd traspostacoordinate =  mesh.Cell0DsCoordinates.transpose() ;
        //preferisco fare la trasposta e lavorare sulle righe poi row è molto vantaggioso

        for ( unsigned int j = 0; j < vertici.size()-2 ; j ++ ) // -2 poiche all'inizio parto dal triangolo
        {
        //  CONTROLLO di sicurezza: controllo che gli id dei vertici siano nei limiti imposti da Cell0DsCoordinates
        //  mesh.Cell0DsCoordinates.rows() → ti dice quante righe ha la matrice (quanti punti hai)
        if (vertici[0] >= traspostacoordinate.rows() || vertici[j+1] >= traspostacoordinate.rows() || vertici[j+2] >= traspostacoordinate.rows())
        {
            cerr << "Errore: ID fuori dalla matrice di coordinate!" << endl;
            cout << i << endl;
            return false;
        }

        
        Vector3d A = traspostacoordinate.row(vertici[0]); // il primo vertice dev'essere fisso
        Vector3d B = traspostacoordinate.row(vertici[j+1]); //prendo la riga della matrice di coordinate con id pari al numero che trovo nel vertice
        Vector3d C = traspostacoordinate.row(vertici[j+2]);


        area_poligono += area_triangolo(A,B,C);
        }
        // necessario mettere nel for(i) per controllare ogni poligono 
        if (area_poligono == 0.0) 
        {
            cerr << " Poligono " << i << " ha area zero!" << endl;
            return false;
        }
   }
   
   /*if (area_poligono == 0 )
   {
        cerr << "Errore: poligono area = 0" << endl;
        return 1;
   }*/  //se metto qui l'if non ha senso perche sommo tutte le aree dei poligoni

   return true;
}


// DIFFERENZA TRA ROW E COL : MetodO Cosa restituisce
//.row(i)	RowVectorXd (tipo fisso)
//.col(i)	Block (tipo dinamico, no fisso!)
//il metodo .col(i)/.row(i) ti restituisce la colonna/riga numero i della matrice




//funzione che mi permette di vedere i colori su Paraview
//PAIR raggruppo due valori insieme
//DEVE ritornare una coppia di cose : altrimentu il vector che contiene i dati scomparirebbe alla fine della funzione → il puntatore Data diventerebbe invalido! 

//pair<UCDProperty<double>, vector<double>> Colorpunti(PolygonalMesh& mesh)

void Colorpunti(const PolygonalMesh& mesh)
{
    //(vector<UCDProperty<double> >& points_properties) questo è l'elt presente in UCDUtilities
    //PROCEDIAMO CON IL UCDProperty<douoble>

    UCDProperty<double> marker_punti; // struttura dati (struct) che si trova definita dentro UCDUtilities.hpp del tipo template <typename T>,
    // <double>->la proprieetà ha valori di tipo double

    //richiamo delle parti della struct di marker_punti
    marker_punti.Label = "marker_punti";
    marker_punti.UnitLabel = "-";  //"-" -> "nessuna unità", è un colore

    marker_punti.NumComponents = 1; // scalare

    // memorizziamo i dati veri da far puntare
    vector<double> dati_marker_punti(mesh.Cell0DsId.size(), 0.0); //lungo mesh.Cell0DsId.size(), e inizializza ogni elemento a 0.0

    for (const auto& [marker, ids] : mesh.MarkerCell0Ds) { //Scorro tutta la mappa mesh.MarkerCell0Ds. n-esimo giro ho il marker n-esimo

        /*marker	unsigned int (la chiave)	    il valore del marker (es: 1, 2, 3...)
          ids	    list<unsigned int> (il valore)	la lista degli ID dei punti associati */

        for (auto id : ids) { //scorro tutti gli id del marker n-esimo
            if (id < mesh.Cell0DsId.size()) //necessariamente un id (identificativo del punto) dev'essere minore della grandezza del vector di punti. se fosse piu grande --> segmentation fault!!
                dati_marker_punti[id] = static_cast<double>(marker);
        
                //Accedi alla posizione id nel vettore dati_marker_punti
                //Assegno il valore del marker convertito in double ( con static_cast sono sicura della conversione )
                //"Se il punto esiste davvero nella mesh, gli assegno il suo marker come numero double, per poterlo colorare in ParaView."
            }
    }

    marker_punti.Data = dati_marker_punti.data(); //Il campo Data ( PUNTATORE)della struct (const double*) ora punta al primo valore dell'array dei valori che ho appena creato.
  //.data() ti dà l'indirizzo di quell'array. Il tipo di ritorno di .data() è double*.

   vector<UCDProperty<double>> points_properties = { marker_punti };

    UCDUtilities utilities;
    utilities.ExportPoints("./MeshCompleta.inp", // cosi riesco a visualizzzare tutto nello stesso file
                        mesh.Cell0DsCoordinates, points_properties);


    // Ritorno sia la proprietà che il vettore
    //return {marker_punti, dati_marker_punti};
}



void Colorsegmenti(const PolygonalMesh& mesh) //passo come const che non devo esportarlo
{
    UCDProperty<double> marker_segmenti; 

    marker_segmenti.Label = "marker_segmenti";
    marker_segmenti.UnitLabel = "-";
    marker_segmenti.NumComponents = 1;  

    vector<double> dati_marker_segmenti(mesh.Cell1DsId.size(), 0.0);

    for( const auto& [marker, ids] : mesh.MarkerCell1Ds) 
    {
        for(unsigned int id : ids)
        {  
            auto it = find(mesh.Cell1DsId.begin(), mesh.Cell1DsId.end(), id); // find: Serve per cercare un valore specifico(id) dentro un intervallo(vettore mesh.Cell1DsId.) Quando lo trova, ti restituisce un iteratore che punta all'elemento trovato.

            //Se non trova il valore, ti restituisce un iteratore alla fine (.end()).
            
            
            if (it != mesh.Cell1DsId.end()) //se lho trovato:
            {
                size_t posizione = distance(mesh.Cell1DsId.begin(), it); //Calcola l'indice numerico (tipo int) della posizione trovata.

                //distance(begin, it) ti dice: "Quante posizioni devi spostarti da begin() per arrivare a it."

                dati_marker_segmenti[posizione] = static_cast<double>(marker);
                cout<< dati_marker_segmenti[posizione]<<endl; //sembra giusto
            }
        }
    // Collego il vettore al puntatore Data della struct ( .data() restituisce l'indirizzo )
    }
    marker_segmenti.Data = dati_marker_segmenti.data(); 

    vector<UCDProperty<double>> segments_properties = { marker_segmenti };  //segments_properties.push_back(marker_segmenti);

    UCDUtilities utilities;
    utilities.ExportSegments("./MeshCompleta.inp",
        mesh.Cell0DsCoordinates,
        mesh.Cell1DsExtremes,{}, segments_properties);

    // Ritorno sia la proprietà che il vettore
    //return {marker_segmenti, dati_marker_segmenti};

}
//ritornano: ( HO CAMBIATO LA FUNZIONE INSERENDO EXPORTSEGMENTS NEL UTILS.CPP)

//Un UCDProperty<double> che contiene i dati puntati (la "descrizione" della proprietà)

//E il vector<double> che mantiene viva la memoria. 

}