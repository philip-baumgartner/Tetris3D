//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

// Nodes -- refer to --> Meshes -- refer to --> Materials

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    
}

Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0){
    bool ret = load(ModelFile, FitSize);
    if(!ret)
        throw std::exception();
}

Model::~Model(){
	// Materialien löschen
	delete [] pMaterials;
    deleteNodes(&RootNode);
}

// Rekursives Löschen aller Knoten
void Model::deleteNodes(Node* pNode){
    if(!pNode)
        return;
    for(unsigned int i=0; i<pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if(pNode->ChildCount>0)
        delete [] pNode->Children;
    if(pNode->MeshCount>0)
        delete [] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize){
	// aiScene: Wurzel der importierten Daten, gewährt Zugang zu den restlichen Daten
	// aiImportFile: Liest die Datei und gibt deren Inhalt (read-only) zurück
	// Flag 1: Optimierung für real-time-Rendering, Flag 2: Erstellt neuen UV-Channel
    const aiScene* pScene = aiImportFile( ModelFile,aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords | aiProcess_FlipUVs );
    
    if(pScene==NULL || pScene->mNumMeshes<=0)
        return false;
    
    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if(pos == std::string::npos)
        pos = Filepath.rfind('\\');
    if(pos !=std::string::npos)
        Path.resize(pos+1);
    
    loadMeshes(pScene, FitSize);
    loadMaterials(pScene);
    loadNodes(pScene);
    
    return true;
}

// Meshes besitzen Vertizes (Array mVertices) und Indices (Array mFaces) ( -> Polygone/Dreiecke)
// Lädt aiScene::mMeshes in Model::pMeshes mit Vertex- und Indexbuffern
void Model::loadMeshes(const aiScene* pScene, bool FitSize){
	MeshCount = pScene->mNumMeshes;
	pMeshes = new  Mesh[MeshCount];

	// BoundingBox kalkulieren
	calcBoundingBox(pScene, BoundingBox);

	// VB, die Meshes des Knotens werden durchlaufen, für jedes Mesh werden Normale, Texturkoordinate und zugehöriger Vertex gespeichert
	for (unsigned int i = 0; i < MeshCount; i++) {
		pMeshes[i].VB.begin();
		aiVector3D tmpNormal;
		aiVector3D tmpTex;
		aiVector3D tmpTangent;
		aiVector3D tmpBitangent;
		aiColor4D tmpColor;

		for (int j = 0; j < pScene->mMeshes[i]->mNumVertices; j++) {
			if(pScene->mMeshes[i]->HasNormals())	
				tmpNormal = pScene->mMeshes[i]->mNormals[j];		
			aiVector3D tmpVec = pScene->mMeshes[i]->mVertices[j];	

			// Modell mit Hilfe der BoundingBox auf eine exakte Größe bringen (z.B 5x5x5)
			if (FitSize) {
				Vector lengthXYZ = BoundingBox.Max - BoundingBox.Min;
				float maxLength = 0;

				if		(lengthXYZ.X >= lengthXYZ.Y && lengthXYZ.X >= lengthXYZ.Z)		maxLength = lengthXYZ.X;
				else if (lengthXYZ.Y >= lengthXYZ.X && lengthXYZ.Y >= lengthXYZ.Z)		maxLength = lengthXYZ.Y;
				else																	maxLength = lengthXYZ.Z;

				float skalFact = 5.0 / maxLength;

				tmpNormal.x *= skalFact;	tmpNormal.y *= skalFact;	tmpNormal.z *= skalFact;
				tmpVec.x *= skalFact;		tmpVec.y *= skalFact;		tmpVec.z *= skalFact;
			}

			// Werte speichern
			if (pScene->mMeshes[i]->HasNormals()) {									// Normalen
				pMeshes[i].VB.addNormal(tmpNormal.x, tmpNormal.y, tmpNormal.z);
			}
			if (pScene->mMeshes[i]->HasVertexColors(0)) {							// Farben
				tmpColor = pScene->mMeshes[i]->mColors[0][j];
				pMeshes[i].VB.addColor(Color(tmpColor.r, tmpColor.g, tmpColor.b));
			}
			if (pScene->mMeshes[i]->HasTextureCoords(0)) {							// Texturkoordinaten
				tmpTex = pScene->mMeshes[i]->mTextureCoords[0][j];
				pMeshes[i].VB.addTexcoord0(tmpTex.x, tmpTex.y);
			}
			if (pScene->mMeshes[i]->HasTangentsAndBitangents()) {					// Tangenten & Bitangenten
				tmpTangent = pScene->mMeshes[i]->mTangents[j];
				tmpBitangent = pScene->mMeshes[i]->mBitangents[j];
				pMeshes[i].VB.addTexcoord1(tmpTangent.x, tmpTangent.y, tmpTangent.z);
				pMeshes[i].VB.addTexcoord2(tmpBitangent.x, tmpBitangent.y, tmpBitangent.z);
			}
			pMeshes[i].VB.addVertex(tmpVec.x, tmpVec.y, tmpVec.z);					// Vertices
		}
		pMeshes[i].VB.end();
	}

	// IB, die Meshes des Knotens werden durchlaufen, für jedes Mesh werden die Indices gespeichert
	for (unsigned int i = 0; i < MeshCount; i++) {
		pMeshes[i].IB.begin();

		for (int j = 0; j < pScene->mMeshes[i]->mNumFaces; j++) {
			aiFace tmpFace = pScene->mMeshes[i]->mFaces[j];					// Index
			for (int k = 0; k < tmpFace.mNumIndices; k++) {
				pMeshes[i].IB.addIndex(tmpFace.mIndices[k]);
			}
		}
		pMeshes[i].IB.end();
	}

	// MaterialIdx
	for (unsigned int i = 0; i < MeshCount; i++) {
		pMeshes[i].MaterialIdx = pScene->mMeshes[i]->mMaterialIndex;		// MaterialIndex
	}
}

// Materialien bestehen aus Farben (verschiedene Komponenten) und Texturen
// Lädt aiScene::mMaterials in Model::pMaterials mit sämtlichen Komponenten
void Model::loadMaterials(const aiScene* pScene){

	MaterialCount = pScene->mNumMaterials;
	pMaterials = new Material[MaterialCount];

	for (int i = 0; i < pScene->mNumMaterials; i++) {
		aiMaterial *mat = pScene->mMaterials[i];
		aiColor4D color;
		aiString pathDiff;
		aiString pathNormals;
		std::stringstream ss;
		float exp;

		if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color)) {
			pMaterials[i].DiffColor.R = color.r;
			pMaterials[i].DiffColor.G = color.g;
			pMaterials[i].DiffColor.B = color.b;
		}

		if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color)) {
			pMaterials[i].SpecColor.R = color.r;
			pMaterials[i].SpecColor.G = color.g;
			pMaterials[i].SpecColor.B = color.b;
		}

		if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color)) {
			pMaterials[i].AmbColor.R = color.r;
			pMaterials[i].AmbColor.G = color.g;
			pMaterials[i].AmbColor.B = color.b;
		}

		if (AI_SUCCESS == aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &exp)) {
			pMaterials[i].SpecExp = exp;
		}

		if (AI_SUCCESS == aiGetMaterialTexture(mat, aiTextureType_DIFFUSE, 0, &pathDiff)) {
			ss << Path.c_str() << pathDiff.C_Str();
			pMaterials[i].DiffTex = Texture::LoadShared(ss.str().c_str());
		}

		if (AI_SUCCESS == aiGetMaterialTexture(mat, aiTextureType_NORMALS, 0, &pathNormals)) {	// Keine normal-maps vorhanden?
			//std::cout << pathHeight.C_Str() << std::endl;
			ss << Path.c_str() << pathNormals.C_Str();
			pMaterials[i].NormalTex = Texture::LoadShared(ss.str().c_str());
		}

		/*aiString name;
		aiGetMaterialString(mat, AI_MATKEY_NAME, &name);
		std::cout << name.C_Str() << std::endl;*/
	}
}

// Box, die das Objekt umschließt. Kanten sind parallel zu den Achsen (achsenausgerichtet)
// Wird mit Hilfe zweier Vektoren beschrieben (min/max)
void Model::calcBoundingBox(const aiScene* pScene, AABB& Box){
	Box.Min = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
	Box.Max = Vector(FLT_MIN, FLT_MIN, FLT_MIN);

	// Vertices der Szene durchlaufen, kleinste/größte Werte für x,y,z finden und speichern
	for (int i = 0; i < pScene->mNumMeshes; i++) {
		for (int j = 0; j < pScene->mMeshes[i]->mNumVertices; j++) {
			aiVector3D tmpVec = pScene->mMeshes[i]->mVertices[j];		// Vertex

			if (Box.Min.X > tmpVec.x)	Box.Min.X = tmpVec.x;
			if (Box.Min.Y > tmpVec.y)	Box.Min.Y = tmpVec.y;
			if (Box.Min.Z > tmpVec.z)	Box.Min.Z = tmpVec.z;

			if (Box.Max.X < tmpVec.x)	Box.Max.X = tmpVec.x;
			if (Box.Max.Y < tmpVec.y)	Box.Max.Y = tmpVec.y;
			if (Box.Max.Z < tmpVec.z)	Box.Max.Z = tmpVec.z;
		}
	}
}

void Model::loadNodes(const aiScene* pScene){
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

// Kopieren der Knoten (mit sämtlichen Eigenschaften) in Membervarianble RootNode
void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode){
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);
    
    if(paiNode->mNumMeshes > 0)
    {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for(unsigned int i=0; i<pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int)paiNode->mMeshes[i];
    }
    
    if(paiNode->mNumChildren <=0)
        return;
    
    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for(unsigned int i=0; i<paiNode->mNumChildren; ++i)
    {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

// Speichert das Material
void Model::applyMaterial( unsigned int index){
    if(index>=MaterialCount)
        return;
    
    PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
    if(!pPhong) {
        return;
    }
    
    Material* pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
	pPhong->normalTexture(pMat->NormalTex);
}

// Zeichnen des Modells (ohne BoundingBox)
void Model::draw(const BaseCamera& Cam){
    if(!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());
    
    std::list<Node*> DrawNodes;
    DrawNodes.push_back(&RootNode);
    
    while(!DrawNodes.empty())
    {
        Node* pNode = DrawNodes.front();
        Matrix GlobalTransform;
        
        if(pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;
        
        pShader->modelTransform(pNode->GlobalTrans);
    
        for(unsigned int i = 0; i<pNode->MeshCount; ++i )
        {
            Mesh& mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for(unsigned int i = 0; i<pNode->ChildCount; ++i )
            DrawNodes.push_back(&(pNode->Children[i]));
        
        DrawNodes.pop_front();
    }
}

Matrix Model::convert(const aiMatrix4x4& m){
    return Matrix(m.a1, m.a2, m.a3, m.a4,
                  m.b1, m.b2, m.b3, m.b4,
                  m.c1, m.c2, m.c3, m.c4,
                  m.d1, m.d2, m.d3, m.d4);
}



