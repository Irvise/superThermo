/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  1.7.0                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.com                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    object      blockMeshDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//For m4 preprocessing
changecom(//)changequote([,])
define(calc, [esyscmd(perl -e 'printf ($1)')])
define(roundcalc, [esyscmd(perl -e 'printf ( int( ($1) + 0.5 ) )')])

//Some dimensions here
define(W,  1E-3)             //Channel width
define(H,  0.75E-3)             //Channel height
define(L1, 4E-3)	      //Total length
define(NX, 47)                  
define(NY, 35)                  
define(NZ, 56)               //Number of cells across unheated section (radial direction)
define(ExpRat, 20)           //Expansion ratio in x-direction

convertToMeters 1;

vertices        
(
	//Back face
	(0          0            0)     //00
	(W          0            0)     //01
	(W          H            0)     //02
	(0          H            0)     //03
	//Mid face
	(0          0            L1)     //04
	(W          0            L1)     //05
	(W          H            L1)     //06
	(0          H            L1)     //07

);

blocks          
(
	hex ( 0  1  2  3  4  5  6  7) (calc(2*NX)  calc(2*NY)  NZ) 
	    simpleGrading (
			    1 
	                    ( (0.5 0.5 ExpRat) (0.5 0.5 calc(1/ExpRat)) )
	                    1 
			  )                   
);

edges           
(
);

boundary         
(
	BottWall
	{
		type     wall;
		faces
		(
		    ( 0  4  5  1)
		);
	}

        SideWall1
	{
		type            cyclic;
		neighbourPatch  SideWall2;
		faces
		(
		    ( 1  5  6  2)
		);
	}

        SideWall2
	{
		type            cyclic;
		neighbourPatch  SideWall1;
		faces
		(
		    ( 0  3  7  4)
		);
	}

	TopWall
	{
		type     wall;
		faces
		(
		    ( 2  6  7  3)
		);
	}

	Inlet
	{
		type            cyclic;
		neighbourPatch  Outlet;
	    	faces
		(   ( 0  3  2  1) 	);
	}

	Outlet
	{
		type            cyclic;
		neighbourPatch  Inlet;
		faces
		(	( 4 7 6 5 )	);
	}

);

mergePatchPairs 
();

// ************************************************************************* //
