/* Copyright (c) 2011-2016, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * This file is part of BRayns
 */

#include "ProteinLoader.h"

#include <brayns/common/log.h>
#include <brayns/common/types.h>
#include <brayns/common/geometry/Sphere.h>

#include <assert.h>
#include <fstream>

namespace brayns
{

/** Structure containing the positions of the proteins in space
 */
struct PDBCellPositions
{
    int id;
    Vector3f position;
    Vector3f unknown;
};

/** Structure defining an atom as it is stored in a PDB file
 */
struct Atom
{
    int    processed;
    int    id;
    int    index;
    Vector3f  position;
    float  radius;
    int    materialId;
    int    chainId;
    int    residue;
};

/** Structure defining an atom radius in microns
 */
struct AtomicRadius
{
    std::string Symbol;
    float radius;
    int index;
};

/** Structure defining the color of atoms according to the JMol Scheme
 */
struct ColorMap
{
    std::string symbol;
    short R,G,B;
};

const size_t colorMapSize = 119;
static ColorMap colorMap[colorMapSize] = {
    { "H", 0xDF,0xDF,0xDF} ,
    { "He",0xD9,0xFF,0xFF} ,
    { "Li",0xCC,0x80,0xFF} ,
    { "Be",0xC2,0xFF,0x00} ,
    { "B", 0xFF,0xB5,0xB5} ,
    { "C", 0x90,0x90,0x90} ,
    { "N", 0x30,0x50,0xF8} ,
    { "O", 0xFF,0x0D,0x0D} ,
    { "F", 0x9E,0x05,0x1} ,
    { "Ne",0xB3,0xE3,0xF5} ,
    { "Na",0xAB,0x5C,0xF2} ,
    { "Mg",0x8A,0xFF,0x00} ,
    { "Al",0xBF,0xA6,0xA6} ,
    { "Si",0xF0,0xC8,0xA0} ,
    { "P", 0xFF,0x80,0x00} ,
    { "S", 0xFF,0xFF,0x30} ,
    { "Cl",0x1F,0xF0,0x1F} ,
    { "Ar",0x80,0xD1,0xE3} ,
    { "K", 0x8F,0x40,0xD4} ,
    { "Ca",0x3D,0xFF,0x00} ,
    { "Sc",0xE6,0xE6,0xE6} ,
    { "Ti",0xBF,0xC2,0xC7} ,
    { "V", 0xA6,0xA6,0xAB} ,
    { "Cr",0x8A,0x99,0xC7} ,
    { "Mn",0x9C,0x7A,0xC7} ,
    { "Fe",0xE0,0x66,0x33} ,
    { "Co",0xF0,0x90,0xA0} ,
    { "Ni",0x50,0xD0,0x50} ,
    { "Cu",0xC8,0x80,0x33} ,
    { "Zn",0x7D,0x80,0xB0} ,
    { "Ga",0xC2,0x8F,0x8F} ,
    { "Ge",0x66,0x8F,0x8F} ,
    { "As",0xBD,0x80,0xE3} ,
    { "Se",0xFF,0xA1,0x00} ,
    { "Br",0xA6,0x29,0x29} ,
    { "Kr",0x5C,0xB8,0xD1} ,
    { "Rb",0x70,0x2E,0xB0} ,
    { "Sr",0x00,0xFF,0x00} ,
    { "Y" ,0x94,0xFF,0xFF} ,
    { "Zr",0x94,0xE0,0xE0} ,
    { "Nb",0x73,0xC2,0xC9} ,
    { "Mo",0x54,0xB5,0xB5} ,
    { "Tc",0x3B,0x9E,0x9E} ,
    { "Ru",0x24,0x8F,0x8F} ,
    { "Rh",0x0A,0x7D,0x8C} ,
    { "Pd",0x69,0x85,0x00} ,
    { "Ag",0xC0,0xC0,0xC0} ,
    { "Cd",0xFF,0xD9,0x8F} ,
    { "In",0xA6,0x75,0x73} ,
    { "Sn",0x66,0x80,0x80} ,
    { "Sb",0x9E,0x63,0xB5} ,
    { "Te",0xD4,0x7A,0x00} ,
    { "I" ,0x94,0x00,0x94} ,
    { "Xe",0x42,0x9E,0xB0} ,
    { "Cs",0x57,0x17,0x8F} ,
    { "Ba",0x00,0xC9,0x00} ,
    { "La",0x70,0xD4,0xFF} ,
    { "Ce",0xFF,0xFF,0xC7} ,
    { "Pr",0xD9,0xFF,0xC7} ,
    { "Nd",0xC7,0xFF,0xC7} ,
    { "Pm",0xA3,0xFF,0xC7} ,
    { "Sm",0x8F,0xFF,0xC7} ,
    { "Eu",0x61,0xFF,0xC7} ,
    { "Gd",0x45,0xFF,0xC7} ,
    { "Tb",0x30,0xFF,0xC7} ,
    { "Dy",0x1F,0xFF,0xC7} ,
    { "Ho",0x00,0xFF,0x9C} ,
    { "Er",0x00,0xE6,0x75} ,
    { "Tm",0x00,0xD4,0x52} ,
    { "Yb",0x00,0xBF,0x38} ,
    { "Lu",0x00,0xAB,0x24} ,
    { "Hf",0x4D,0xC2,0xFF} ,
    { "Ta",0x4D,0xA6,0xFF} ,
    { "W", 0x21,0x94,0xD6} ,
    { "Re",0x26,0x7D,0xAB} ,
    { "Os",0x26,0x66,0x96} ,
    { "Ir",0x17,0x54,0x87} ,
    { "Pt",0xD0,0xD0,0xE0} ,
    { "Au",0xFF,0xD1,0x23} ,
    { "Hg",0xB8,0xB8,0xD0} ,
    { "Tl",0xA6,0x54,0x4D} ,
    { "Pb",0x57,0x59,0x61} ,
    { "Bi",0x9E,0x4F,0xB5} ,
    { "Po",0xAB,0x5C,0x00} ,
    { "At",0x75,0x4F,0x45} ,
    { "Rn",0x42,0x82,0x96} ,
    { "Fr",0x42,0x00,0x66} ,
    { "Ra",0x00,0x7D,0x00} ,
    { "Ac",0x70,0xAB,0xFA} ,
    { "Th",0x00,0xBA,0xFF} ,
    { "Pa",0x00,0xA1,0xFF} ,
    { "U", 0x00,0x8F,0xFF} ,
    { "Np",0x00,0x80,0xFF} ,
    { "Pu",0x00,0x6B,0xFF} ,
    { "Am",0x54,0x5C,0xF2} ,
    { "Cm",0x78,0x5C,0xE3} ,
    { "Bk",0x8A,0x4F,0xE3} ,
    { "Cf",0xA1,0x36,0xD4} ,
    { "Es",0xB3,0x1F,0xD4} ,
    { "Fm",0xB3,0x1F,0xBA} ,
    { "Md",0xB3,0x0D,0xA6} ,
    { "No",0xBD,0x0D,0x87} ,
    { "Lr",0xC7,0x00,0x66} ,
    { "Rf",0xCC,0x00,0x59} ,
    { "Db",0xD1,0x00,0x4F} ,
    { "Sg",0xD9,0x00,0x45} ,
    { "Bh",0xE0,0x00,0x38} ,
    { "Hs",0xE6,0x00,0x2E} ,
    { "Mt",0xEB,0x00,0x26}
};

static AtomicRadius atomic_radii[colorMapSize] = // atomic radii in microns
{
    { "C"  , 67.f,  1 },
    { "N"  , 56.f,  2 },
    { "O"  , 48.f,  3 },
    { "H"  , 53.f,  4 },
    { "B"  , 87.f,  5 },
    { "F"  , 42.f,  6 },
    { "P"  , 98.f,  7 },
    { "S"  , 88.f,  8 },
    { "V"  ,171.f,  9 },
    { "K"  ,243.f, 10 },
    { "HE" , 31.f, 11 },
    { "LI" ,167.f, 12 },
    { "BE" ,112.f, 13 },
    { "NE" , 38.f, 14 },
    { "NA" ,190.f, 15 },
    { "MG" ,145.f, 16 },
    { "AL" ,118.f, 17 },
    { "SI" ,111.f, 18 },
    { "CL" , 79.f, 19 },
    { "AR" , 71.f, 20 },
    { "CA" ,194.f, 21 },
    { "SC" ,184.f, 22 },
    { "TI" ,176.f, 23 },
    { "CR" ,166.f, 24 },
    { "MN" ,161.f, 25 },
    { "FE" ,156.f, 26 },
    { "CO" ,152.f, 27 },
    { "NI" ,149.f, 28 },
    { "CU" ,145.f, 29 },
    { "ZN" ,142.f, 30 },
    { "GA" ,136.f, 31 },
    { "GE" ,125.f, 32 },
    { "AS" ,114.f, 33 },
    { "SE" ,103.f, 34 },
    { "BR" , 94.f, 35 },
    { "KR" , 88.f, 36 },

    // TODO
    { "OD1" , 25.f, 37 },
    { "OD2" , 25.f, 38 },
    { "CG1" , 25.f, 39 },
    { "CG2" , 25.f, 40 },
    { "CD1" , 25.f, 41 },
    { "CB"  , 25.f, 42 },
    { "CG"  , 25.f, 43 },
    { "CD"  , 25.f, 44 },
    { "OE1" , 25.f, 45 },
    { "NE2" , 25.f, 46 },
    { "CZ"  , 25.f, 47 },
    { "NH1" , 25.f, 48 },
    { "NH2" , 25.f, 49 },
    { "CD2" , 25.f, 50 },
    { "CE1" , 25.f, 51 },
    { "CE2" , 25.f, 52 },
    { "CE"  , 25.f, 53 },
    { "NZ"  , 25.f, 54 },
    { "OH"  , 25.f, 55 },
    { "CE"  , 25.f, 56 },
    { "ND1" , 25.f, 57 },
    { "ND2" , 25.f, 58 },
    { "OXT" , 25.f, 59 },
    { "OG1" , 25.f, 60 },
    { "NE1" , 25.f, 61 },
    { "CE3" , 25.f, 62 },
    { "CZ2" , 25.f, 63 },
    { "CZ3" , 25.f, 64 },
    { "CH2" , 25.f, 65 },
    { "OE2" , 25.f, 66 },
    { "OG"  , 25.f, 67 },
    { "OE2" , 25.f, 68 },
    { "SD"  , 25.f, 69 },
    { "SG"  , 25.f, 70 },
    { "C1*" , 25.f, 71 },
    { "C2" ,  25.f, 72 },
    { "C2*" , 25.f, 73 },
    { "C3*" , 25.f, 74 },
    { "C4" ,  25.f, 75 },
    { "C4*" , 25.f, 76 },
    { "C5" ,  25.f, 77 },
    { "C5*" , 25.f, 78 },
    { "C5M" , 25.f, 79 },
    { "C6" ,  25.f, 80 },
    { "C8" ,  25.f, 81 },
    { "H1" ,  25.f, 82 },
    { "H1*" , 25.f, 83 },
    { "H2" ,  25.f, 84 },
    { "H2*" , 25.f, 85 },
    { "H3" ,  25.f, 86 },
    { "H3*" , 25.f, 87 },
    { "H3P" , 25.f, 88 },
    { "H4" ,  25.f, 89 },
    { "H4*" , 25.f, 90 },
    { "H5" ,  25.f, 91 },
    { "H5*" , 25.f, 92 },
    { "H5M" , 25.f, 93 },
    { "H6" ,  25.f, 94 },
    { "H8" ,  25.f, 95 },
    { "N1" ,  25.f, 96 },
    { "N2" ,  25.f, 97 },
    { "N3" ,  25.f, 98 },
    { "N4" ,  25.f, 99 },
    { "N6" ,  25.f, 100 },
    { "N7" ,  25.f, 101 },
    { "N9" ,  25.f, 102 },
    { "O1P" , 25.f, 103 },
    { "O2" ,  25.f, 104 },
    { "O2P" , 25.f, 105 },
    { "O3*" , 25.f, 106 },
    { "O3P" , 25.f, 107 },
    { "O4" ,  25.f, 108 },
    { "O4*" , 25.f, 109 },
    { "O5*" , 25.f, 110 },
    { "O6" ,  25.f, 111 },
    { "OXT" , 25.f, 112 },
    { "P" ,   25.f, 113 }
};

ProteinLoader::ProteinLoader(
    const GeometryParameters& geometryParameters )
    : _geometryParameters(geometryParameters)
{
}

bool ProteinLoader::importPDBFolder(
    const int material,
    const Materials& materials,
    const int positionsOnly,
    PrimitivesMap& primitives,
    Boxf& bounds)
{
    BRAYNS_INFO << "Folder   : " << _geometryParameters.getPDBFolder() << std::endl;
    BRAYNS_INFO << "Cells    : " << _geometryParameters.getPDBCells() << std::endl;
    BRAYNS_INFO << "Positions: " << _geometryParameters.getPDBPositions() << std::endl;

    size_t totalNbProteins = 0;
    std::map<int,std::string> proteins;
    std::ifstream cellsFile(_geometryParameters.getPDBCells());
    if(!cellsFile.is_open())
        return false;
    else
    {
        while(cellsFile.good())
        {
            std::string line;
            std::getline(cellsFile, line);
            std::istringstream iss(line);
            size_t id;
            std::string code;
            size_t instances;
            iss >> code >> id >> instances;

            totalNbProteins += instances;

            proteins[id] = code;
            if(!positionsOnly)
            {
                const std::string pdbFilename(_geometryParameters.getPDBFolder() + '/' + code + ".pdb");
                std::ifstream pdbFile(pdbFilename);
                if(pdbFile.is_open())
                {
                    // File is in the cache
                    pdbFile.close();
                }
                else
                {
                    // Download missing file
                    std::string command;
                    command = "wget http://www.rcsb.org/pdb/files/";
                    command += code;
                    command += ".pdb";
                    command += " -P ";
                    command += _geometryParameters.getPDBFolder();
                    size_t status = system(command.c_str());
                    BRAYNS_INFO << command << ": " << status << std::endl;
                }
            }
        }
        cellsFile.close();
    }

    BRAYNS_INFO << "Proteins : " << totalNbProteins << std::endl;
    size_t proteinCount = 0;
    std::ifstream positionsFile(_geometryParameters.getPDBPositions());
    if(!positionsFile.is_open())
        return false;
    else
    {
        while(positionsFile.good())
        {
            std::string line;
            std::getline(positionsFile, line);
            std::istringstream iss(line);

            size_t id;
            PDBCellPositions cellPositions;
            iss >> id
                >> cellPositions.position[0]
                >> cellPositions.position[1]
                >> cellPositions.position[2]
                >> cellPositions.unknown[0]
                >> cellPositions.unknown[1]
                >> cellPositions.unknown[2];

            std::map<int,std::string>::const_iterator it = proteins.find(id);
            if(it != proteins.end())
            {
                if(positionsOnly)
                {
                    const size_t defaultMaterial = id%materials.size();
                    SpherePtr sphere(
                        new Sphere(defaultMaterial, cellPositions.position, 0.05f, 0.f, 0.f));
                    primitives[defaultMaterial].push_back(sphere);
                    bounds.merge(sphere->getCenter());
                }
                else
                {
                    const std::string pdbFilename(
                        _geometryParameters.getPDBFolder() + '/' + proteins[id] + ".pdb");
                    if(!importPDBFile(pdbFilename, cellPositions.position,
                        material, materials, primitives, bounds))
                        BRAYNS_WARN << "Failed to import " << pdbFilename << std::endl;

                    const size_t step = totalNbProteins/10;
                    if(proteinCount%step==0 && proteinCount)
                        BRAYNS_INFO << "Loaded " << proteinCount << " proteins (" <<
                            proteinCount/step << "%)" << std::endl;
                }
                ++proteinCount;
            }
        }
        positionsFile.close();
    }

    BRAYNS_INFO << "Total number of different proteins: " <<
                   proteins.size() << std::endl;
    BRAYNS_INFO << "Total number of proteins          : " <<
                   proteinCount << std::endl;
    return true;
}

bool ProteinLoader::importPDBFile(
    const std::string &filename,
    const Vector3f& position,
    const int material,
    const Materials& materials,
    PrimitivesMap& primitives,
    Boxf& bounds)
{
    Vector3f inViewPos(0,0,0);
    float inViewRadius = 1.0;

    int index(0);
    std::ifstream file(filename.c_str());
    if(!file.is_open())
    {
        BRAYNS_ERROR << "Could not open " << filename << std::endl;
        return false;
    }
    else
    {
        while( file.good() )
        {
            std::string line;
            std::string value;
            std::getline( file, line );
            if( line.find("ATOM") == 0 )
            {
                // Atom
                Atom atom;
                atom.chainId = 0;
                atom.residue = 0;
                atom.index = index;
                index++;
                std::string atomName;
                std::string atomCode;
                size_t i=0;
                while( i<line.length() )
                {
                    switch(i)
                    {
                    case 6: //ID
                    case 12:
                    case 76: // Atom name
                    case 22: // ChainID
                    case 30: // x
                    case 38: // y
                    case 46: // z
                        value = "";
                        break;
                    case 21:
                        atom.chainId = (int)line.at(i)-64;
                        break;
                    case 11:
                        atom.id = static_cast<int>(atoi(value.c_str()));
                        break;
                    case 17:
                        atomCode = value;
                        break;
                    case 79:
                        atomName = value;
                        break;
                    case 26:
                        atom.residue = static_cast<int>(atoi(value.c_str()));
                        break;
                    case 37:
                        atom.position[0] =
                                static_cast<float>(atof(value.c_str()));
                        break;
                    case 45:
                        atom.position[1] =
                                static_cast<float>(atof(value.c_str()));
                        break;
                    case 53:
                        atom.position[2] =
                                static_cast<float>(atof(value.c_str()));
                        break;
                    default:
                        if( line.at(i) != ' ' )
                            value += line.at(i);
                        break;
                    }
                    i++;
                }

                // Material
                atom.materialId = 0;
                i=0;
                bool found(false);
                const ColorScheme colorScheme =
                    _geometryParameters.getColorScheme( );
                if( colorScheme == CS_PROTEIN_BACKBONE )
                    atom.materialId = material;
                else
                {
                    while(!found && i<colorMapSize)
                    {
                        if( atomName == colorMap[i].symbol )
                        {
                            found = true;
                            switch( colorScheme )
                            {
                            case CS_PROTEIN_CHAINS:
                                atom.materialId =
                                        abs(atom.chainId) % materials.size();
                                break;
                            case CS_PROTEIN_RESIDUES:
                                atom.materialId =
                                        abs(atom.residue) % materials.size();
                                break;
                            case CS_PROTEIN_BACKBONE:
                                atom.materialId = 0;
                                break;
                            default:
                                atom.materialId = static_cast<int>(i);
                                break;
                            }
                        }
                        ++i;
                    }
                }
                // Radius
                atom.radius = 25.f;
                i=0;
                found = false;
                while( !found && i<colorMapSize )
                {
                    if( atomName == atomic_radii[i].Symbol )
                    {
                        atom.radius = atomic_radii[i].radius;
                        found = true;
                    }
                    ++i;
                }

                SpherePtr sphere(new Sphere(
                    0, Vector3f(
                        position[0] + 0.01f*atom.position[0],
                        position[1] + 0.01f*atom.position[1],
                        position[2] + 0.01f*atom.position[2]),
                    0.0001f * atom.radius * _geometryParameters.getRadius(),
                    0.f, 0.f));

                if( colorScheme == CS_PROTEIN_BACKBONE )
                {
                    Vector3f inView( sphere->getCenter() - inViewPos );
                    float dist = inView.length();
                    if( dist > inViewRadius )
                    {
                        atom.materialId = ( dist < (inViewRadius+0.1) ) ? 1 : 0;
                        primitives[atom.materialId].push_back(sphere);
                    }
                }
                else
                    primitives[atom.materialId].push_back(sphere);
                bounds.merge(sphere->getCenter());
            }
        }
        file.close();
    }

    return true;
}

void ProteinLoader::getMaterialKd(
        const size_t index,
        float& r,
        float& g,
        float& b)
{
    r = colorMap[index].R/255.f;
    g = colorMap[index].G/255.f;
    b = colorMap[index].B/255.f;
}

}
