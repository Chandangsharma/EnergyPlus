// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SurfaceGeometry_hh_INCLUDED
#define SurfaceGeometry_hh_INCLUDED

// C++ Headers
#include <map>

// ObjexxFCL Headers
#include <ObjexxFCL/Array1S.hh>

// EnergyPlus Headers
#include <EnergyPlus/DataSurfaces.hh>
#include <EnergyPlus/DataVectorTypes.hh>
#include <EnergyPlus/DataViewFactorInformation.hh>
#include <EnergyPlus/EnergyPlus.hh>
#include <EnergyPlus/HeatBalanceKivaManager.hh>

namespace EnergyPlus {

// Forward declarations
struct EnergyPlusData;

namespace SurfaceGeometry {

    extern Array1D_string const BaseSurfCls;
    extern Array1D_string const SubSurfCls;
    extern Array1D_int const BaseSurfIDs;

    extern Array1D_int const SubSurfIDs;

    extern int const UnenteredAdjacentZoneSurface; // allows users to enter one zone surface ("Zone")
    // referencing another in adjacent zone
    extern int const UnreconciledZoneSurface; // interim value between entering surfaces ("Surface") and reconciling
    // surface names in other zones

    enum enclosureType
    {
        RadiantEnclosures,
        SolarEnclosures
    };

    // DERIVED TYPE DEFINITIONS

    // MODULE VARIABLE DECLARATIONS:
    // Following are used only during getting vertices, so are module variables here.
    extern Real64 CosBldgRelNorth;          // Cosine of the building rotation (relative north) (includes appendix G rotation)
    extern Real64 SinBldgRelNorth;          // Sine of the building rotation (relative north)   (includes appendix G rotation)
    extern Real64 CosBldgRotAppGonly;       // Cosine of the building rotation for appendix G only(relative north)
    extern Real64 SinBldgRotAppGonly;       // Sine of the building rotation for appendix G only (relative north)
    extern Array1D<Real64> CosZoneRelNorth; // Cosine of the zone rotation (relative north)
    extern Array1D<Real64> SinZoneRelNorth; // Sine of the zone rotation (relative north)

    extern bool NoGroundTempObjWarning; // This will cause a warning to be issued if surfaces with "Ground"
    // outside environment are used but no ground temperature object was input.
    extern bool NoFCGroundTempObjWarning; // This will cause a warning to be issued if surfaces with "GroundFCfactorMethod"
    // outside environment are used but no FC ground temperatures was input.
    extern bool RectSurfRefWorldCoordSystem; // GlobalGeometryRules=World (true) or Relative (false)
    extern int Warning1Count;                // counts of Modify Window 5/6 windows
    extern int Warning2Count;                // counts of overriding exterior windows with Window 5/6 glazing systems
    extern int Warning3Count;                // counts of overriding interior windows with Window 5/6 glazing systems

    // SUBROUTINE SPECIFICATIONS FOR MODULE SurfaceGeometry

    // Object Data
    extern HeatBalanceKivaManager::KivaManager kivaManager;

    // Functions

    // Clears the global data in HeatBalanceManager.
    // Needed for unit tests, should not be normally called.
    void clear_state();

    void SetupZoneGeometry(EnergyPlusData &state, bool &ErrorsFound);

    void AllocateModuleArrays();

    void AllocateSurfaceWindows(int NumSurfaces);

    void GetSurfaceData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void checkSubSurfAzTiltNorm(DataSurfaces::SurfaceData &baseSurface, // Base surface data (in)
                                DataSurfaces::SurfaceData &subSurface,  // Subsurface data (in)
                                bool &surfaceError        // True if there is subsurface error that requires a fatal
    );

    void GetGeometryParameters(EnergyPlusData &state, bool &ErrorsFound); // set to true if errors found during input

    void GetDetShdSurfaceData(EnergyPlusData &state,
                              bool &ErrorsFound,          // Error flag indicator (true if errors found)
                              int &SurfNum,               // Count of Current SurfaceNumber
                              int TotDetachedFixed, // Number of Fixed Detached Shading Surfaces to obtain
                              int TotDetachedBldg   // Number of Building Detached Shading Surfaces to obtain
    );

    void GetRectDetShdSurfaceData(EnergyPlusData &state,
                                  bool &ErrorsFound,              // Error flag indicator (true if errors found)
                                  int &SurfNum,                   // Count of Current SurfaceNumber
                                  int TotRectDetachedFixed, // Number of Fixed Detached Shading Surfaces to obtain
                                  int TotRectDetachedBldg   // Number of Building Detached Shading Surfaces to obtain
    );

    void GetHTSurfaceData(EnergyPlusData &state,
                          bool &ErrorsFound,                // Error flag indicator (true if errors found)
                          int &SurfNum,                     // Count of Current SurfaceNumber
                          int TotHTSurfs,             // Number of Heat Transfer Base Surfaces to obtain
                          int TotDetailedWalls,       // Number of Wall:Detailed items to obtain
                          int TotDetailedRoofs,       // Number of RoofCeiling:Detailed items to obtain
                          int TotDetailedFloors,      // Number of Floor:Detailed items to obtain
                          const Array1D_string &BaseSurfCls, // Valid Classes for Base Surfaces
                          const Array1D_int &BaseSurfIDs,
                          int &NeedToAddSurfaces // Number of surfaces to add, based on unentered IZ surfaces
    );

    void GetRectSurfaces(EnergyPlusData &state,
                         bool &ErrorsFound,             // Error flag indicator (true if errors found)
                         int &SurfNum,                  // Count of Current SurfaceNumber
                         int TotRectExtWalls,     // Number of Exterior Walls to obtain
                         int TotRectIntWalls,     // Number of Adiabatic Walls to obtain
                         int TotRectIZWalls,      // Number of Interzone Walls to obtain
                         int TotRectUGWalls,      // Number of Underground to obtain
                         int TotRectRoofs,        // Number of Roofs to obtain
                         int TotRectCeilings,     // Number of Adiabatic Ceilings to obtain
                         int TotRectIZCeilings,   // Number of Interzone Ceilings to obtain
                         int TotRectGCFloors,     // Number of Floors with Ground Contact to obtain
                         int TotRectIntFloors,    // Number of Adiabatic Walls to obtain
                         int TotRectIZFloors,     // Number of Interzone Floors to obtain
                         const Array1D_int &BaseSurfIDs, // ID Assignments for valid surface classes
                         int &NeedToAddSurfaces         // Number of surfaces to add, based on unentered IZ surfaces
    );

    void MakeRectangularVertices(EnergyPlusData &state,
                                 int SurfNum,
                                 Real64 XCoord,
                                 Real64 YCoord,
                                 Real64 ZCoord,
                                 Real64 Length,
                                 Real64 Height,
                                 bool SurfWorldCoordSystem);

    void GetHTSubSurfaceData(EnergyPlusData &state,
                             bool &ErrorsFound,               // Error flag indicator (true if errors found)
                             int &SurfNum,                    // Count of Current SurfaceNumber
                             int TotHTSubs,             // Number of Heat Transfer SubSurfaces to obtain
                             const Array1D_string &SubSurfCls, // Valid Classes for Sub Surfaces
                             const Array1D_int &SubSurfIDs,    // ID Assignments for valid sub surface classes
                             int &AddedSubSurfaces,           // Subsurfaces added when windows reference Window5
                             int &NeedToAddSurfaces           // Number of surfaces to add, based on unentered IZ surfaces
    );

    void GetRectSubSurfaces(EnergyPlusData &state,
                            bool &ErrorsFound,            // Error flag indicator (true if errors found)
                            int &SurfNum,                 // Count of Current SurfaceNumber
                            int TotWindows,         // Number of Window SubSurfaces to obtain
                            int TotDoors,           // Number of Door SubSurfaces to obtain
                            int TotGlazedDoors,     // Number of Glass Door SubSurfaces to obtain
                            int TotIZWindows,       // Number of Interzone Window SubSurfaces to obtain
                            int TotIZDoors,         // Number of Interzone Door SubSurfaces to obtain
                            int TotIZGlazedDoors,   // Number of Interzone Glass Door SubSurfaces to obtain
                            const Array1D_int &SubSurfIDs, // ID Assignments for valid sub surface classes
                            int &AddedSubSurfaces,        // Subsurfaces added when windows reference Window5
                            int &NeedToAddSubSurfaces     // Number of surfaces to add, based on unentered IZ surfaces
    );

    void CheckWindowShadingControlFrameDivider(EnergyPlusData &state,
                                               std::string const &cRoutineName, // routine name calling this one (for error messages)
                                               bool &ErrorsFound,               // true if errors have been found or are found here
                                               int SurfNum,               // current surface number
                                               int FrameField             // field number for frame/divider
    );

    void CheckSubSurfaceMiscellaneous(EnergyPlusData &state,
                                      std::string const &cRoutineName,           // routine name calling this one (for error messages)
                                      bool &ErrorsFound,                         // true if errors have been found or are found here
                                      int SurfNum,                         // current surface number
                                      std::string const &SubSurfaceName,         // name of the surface
                                      std::string const &SubSurfaceConstruction, // name of the construction
                                      int &AddedSubSurfaces);

    void MakeRelativeRectangularVertices(EnergyPlusData &state,
                                         int BaseSurfNum, // Base surface
                                         int SurfNum,
                                         Real64 XCoord,
                                         Real64 ZCoord,
                                         Real64 Length,
                                         Real64 Height);

    void MakeEquivalentRectangle(int SurfNum, // Surface number
                                 bool &ErrorsFound  // Error flag indicator (true if errors found)
    );

    void GetAttShdSurfaceData(EnergyPlusData &state,
                              bool &ErrorsFound,   // Error flag indicator (true if errors found)
                              int &SurfNum,        // Count of Current SurfaceNumber
                              int TotShdSubs // Number of Attached Shading SubSurfaces to obtain
    );

    void GetSimpleShdSurfaceData(EnergyPlusData &state,
                                 bool &ErrorsFound,                // Error flag indicator (true if errors found)
                                 int &SurfNum,                     // Count of Current SurfaceNumber
                                 int TotOverhangs,           // Number of Overhangs to obtain
                                 int TotOverhangsProjection, // Number of Overhangs (projection) to obtain
                                 int TotFins,                // Number of Fins to obtain
                                 int TotFinsProjection       // Number of Fins (projection) to obtain
    );

    void GetIntMassSurfaceData(EnergyPlusData &state,
                               bool &ErrorsFound, // Error flag indicator (true if errors found)
                               int &SurfNum       // Count of Current SurfaceNumber
    );

    int GetNumIntMassSurfaces(EnergyPlusData &state); // Number of Internal Mass Surfaces to obtain

    void GetShadingSurfReflectanceData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void GetSurfaceSrdSurfsData(EnergyPlusData &state, bool &ErrorsFound); // Error flag indicator (true if errors found)

    void GetSurfaceLocalEnvData(EnergyPlusData &state, bool &ErrorsFound); // Error flag indicator (true if errors found)

    void GetHTSurfExtVentedCavityData(EnergyPlusData &state, bool &ErrorsFound); // Error flag indicator (true if errors found)

    void GetSurfaceHeatTransferAlgorithmOverrides(EnergyPlusData &state, bool &ErrorsFound);

    class ExposedFoundationPerimeter
    {
    public:
        void getData(EnergyPlusData &state, bool &ErrorsFound);
        struct Data
        {
            double exposedFraction = -1; // hush up cppcheck
            std::vector<bool> isExposedPerimeter;
            bool useDetailedExposedPerimeter;
        };
        std::map<int, Data> surfaceMap;
    };

    extern ExposedFoundationPerimeter exposedFoundationPerimeter;

    void GetVertices(EnergyPlusData &state,
                     int SurfNum,             // Current surface number
                     int NSides,              // Number of sides to figure
                     Array1S<Real64> Vertices // Vertices, in specified order
    );

    void ReverseAndRecalculate(EnergyPlusData &state,
                               int SurfNum,   // Surface number for the surface
                               int NSides,    // number of sides to surface
                               Real64 &SurfAzimuth, // Surface Facing angle (will be 0 for roofs/floors)
                               Real64 &SurfTilt     // Surface tilt (
    );

    void MakeMirrorSurface(EnergyPlusData &state, int &SurfNum); // In=>Surface to Mirror, Out=>new Surface index

    void GetWindowShadingControlData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void InitialAssociateWindowShadingControlFenestration(EnergyPlusData &state, bool &ErrorsFound, int &SurfNum);

    void FinalAssociateWindowShadingControlFenestration(bool &ErrorsFound);

    void CheckWindowShadingControlSimilarForWindow(bool& ErrorsFound);

    bool isWindowShadingControlSimilar(int a, int b);

    void GetStormWindowData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void GetWindowGapAirflowControlData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void GetOSCData(EnergyPlusData &state, bool &ErrorsFound);

    void GetOSCMData(EnergyPlusData &state, bool &ErrorsFound);

    void GetFoundationData(EnergyPlusData &state, bool &ErrorsFound);

    void GetMovableInsulationData(EnergyPlusData &state, bool &ErrorsFound); // If errors found in input

    void CalculateZoneVolume(EnergyPlusData &state, const Array1D_bool &CeilingHeightEntered);

    struct EdgeOfSurf
    {
        int surfNum;
        DataVectorTypes::Vector start;
        DataVectorTypes::Vector end;
        EdgeOfSurf() : surfNum(0), start(DataVectorTypes::Vector(0., 0., 0.)), end(DataVectorTypes::Vector(0., 0., 0.))
        {
        }
    };

    bool isEnclosedVolume(DataVectorTypes::Polyhedron const &zonePoly, std::vector<EdgeOfSurf> &edgeNot2);

    std::vector<EdgeOfSurf> edgesInBoth(std::vector<EdgeOfSurf> edges1, std::vector<EdgeOfSurf> edges2);

    bool edgesEqualOnSameSurface(EdgeOfSurf a, EdgeOfSurf b);

    std::vector<EdgeOfSurf> edgesNotTwoForEnclosedVolumeTest(DataVectorTypes::Polyhedron const &zonePoly, std::vector<DataVectorTypes::Vector> const &uniqueVertices);

    void makeListOfUniqueVertices(DataVectorTypes::Polyhedron const &zonePoly, std::vector<DataVectorTypes::Vector> &uniqVertices);

    DataVectorTypes::Polyhedron updateZonePolygonsForMissingColinearPoints(DataVectorTypes::Polyhedron const &zonePoly,
                                                                           std::vector<DataVectorTypes::Vector> const &uniqVertices);

    void insertVertexOnFace(DataVectorTypes::Face &face, int const &indexBefore, DataVectorTypes::Vector const &vertexToInsert);

    bool areFloorAndCeilingSame(DataVectorTypes::Polyhedron const &zonePoly);

    bool areWallHeightSame(DataVectorTypes::Polyhedron const &zonePoly);

    std::tuple<bool, bool, bool> areSurfaceHorizAndVert(DataVectorTypes::Polyhedron const &zonePoly);

    bool areOppositeWallsSame(DataVectorTypes::Polyhedron const &zonePoly, Real64 &oppositeWallArea, Real64 &distanceBetweenOppositeWalls);

    std::vector<int> listOfFacesFacingAzimuth(DataVectorTypes::Polyhedron const &zonePoly, Real64 const &azimuth);

    int findPossibleOppositeFace(DataVectorTypes::Polyhedron const &zonePoly, int const &faceIndex);

    bool areCornersEquidistant(DataVectorTypes::Polyhedron const &zonePoly, int const &faceIndex, int const &opFaceIndex, Real64 &distanceBetween);

    bool isAlmostEqual3dPt(DataVectorTypes::Vector v1, DataVectorTypes::Vector v2);

    bool isAlmostEqual2dPt(DataVectorTypes::Vector_2d v1, DataVectorTypes::Vector_2d v2);

    int findIndexOfVertex(DataVectorTypes::Vector vertexToFind, std::vector<DataVectorTypes::Vector> listOfVertices);

    Real64 distance(DataVectorTypes::Vector v1, DataVectorTypes::Vector v2);

    bool isPointOnLineBetweenPoints(DataVectorTypes::Vector start, DataVectorTypes::Vector end, DataVectorTypes::Vector test);

    void ProcessSurfaceVertices(EnergyPlusData &state, int ThisSurf, bool &ErrorsFound);

    void CalcCoordinateTransformation(EnergyPlusData &state,
                                      int SurfNum,            // Surface Number
                                      DataVectorTypes::Vector &CompCoordTranslVector // Coordinate Translation Vector
    );

    void CreateShadedWindowConstruction(EnergyPlusData &state,
                                        int SurfNum, // Surface number
                                        int WSCPtr,  // Pointer to WindowShadingControl for SurfNum
                                        int ShDevNum, // Shading device material number for WSCptr
                                        int shadeControlIndex // index to the Surface().windowShadingControlList,
                                              // Surface().shadedConstructionList, and Surface().shadedStormWinConstructionList
    );

    void CreateStormWindowConstructions(EnergyPlusData &state);

    int createAirMaterialFromDistance(Real64 distance, std::string namePrefix); //return new material number

    // create a new construction with storm based on an old construction and storm and gap materials
    int createConstructionWithStorm(EnergyPlusData &state, int oldConstruction, std::string name, int stormMaterial, int gapMaterial);

    void ModifyWindow(EnergyPlusData &state,
                      int SurfNum,    // SurfNum has construction of glazing system from Window5 Data File;
                      bool &ErrorsFound,    // Set to true if errors found
                      int &AddedSubSurfaces // Subsurfaces added when window references a
    );

    void AddWindow(EnergyPlusData &state,
                   int SurfNum,    // SurfNum has construction of glazing system from Window5 Data File;
                   bool &ErrorsFound,    // Set to true if errors found
                   int &AddedSubSurfaces // Subsurfaces added when window references a
    );

    void TransformVertsByAspect(EnergyPlusData &state,
                                int SurfNum, // Current surface number
                                int NSides   // Number of sides to figure
    );

    void CalcSurfaceCentroid();

    void SetupShadeSurfacesForSolarCalcs(EnergyPlusData &state);

    void SetupEnclosuresAndAirBoundaries(EnergyPlusData &state,
                                         Array1D<DataViewFactorInformation::ZoneViewFactorInformation> &Enclosures, // Radiant or Solar Enclosures
                                         SurfaceGeometry::enclosureType const &EnclosureType,                       // Radiant or Solar
                                         bool &ErrorsFound);                                                        // Set to true if errors found

    void CheckConvexity(EnergyPlusData &state,
                        int SurfNum, // Current surface number
                        int NSides   // Number of sides to figure
    );

    bool isRectangle(int ThisSurf // Current surface number
    );

    void CheckForReversedLayers(EnergyPlusData &state,
                                bool &RevLayerDiffs,    // true when differences are discovered in interzone constructions
                                int ConstrNum,    // construction index
                                int ConstrNumRev, // construction index for reversed construction
                                int TotalLayers   // total layers for construction definition
    );

} // namespace SurfaceGeometry

struct SurfaceGeometryData : BaseGlobalStruct {

    Array1D<DataSurfaces::SurfaceData> SurfaceTmp;

    void clear_state() override
    {
        this->SurfaceTmp.deallocate();
    }
};

} // namespace EnergyPlus

#endif
