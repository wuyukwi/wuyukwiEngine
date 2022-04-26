/*********************************************************************************

  *FileName: XStructures.h
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/15

**********************************************************************************/


#ifndef _X_ALLOCATE_
#define _X_ALLOCATE_

#include<d3d9.h>
#include<d3dx9.h>


struct stD3DFrameDerived: public D3DXFRAME
{
   D3DXMATRIX finalMatrix;  //�s��ϊ���̃}�g���b�N�X
};

//--------------------------------------------------------------------------------------
// D3DXMESHCONTAINER����h�������\���A�e���b�V���ɕۑ����ꂽ�ŗL�̏���ǉ��ł��܂�
//--------------------------------------------------------------------------------------
struct stD3DContainerDerived: public D3DXMESHCONTAINER
{
   //LPDIRECT3DTEXTURE9 *textures;
   //D3DMATERIAL9 *mat;
   //LPD3DXMESH originalMesh;
   //D3DXMATRIX **boneMatrices;

   //�e�N�X�`��
   LPDIRECT3DTEXTURE9* ppTextures;

   //���b�V��         
   LPD3DXMESH pOrigMesh;					// �I���W�i�����b�V��
   LPD3DXATTRIBUTERANGE pAttributeTable;	// �����e�[�u���G���g��
   DWORD NumAttributeGroups;				// �{�[���̐�
   DWORD NumInfl;							// �P�̒��_�ɉe�����y�ڂ��d�݂̐�
   LPD3DXBUFFER pBoneCombinationBuf;		// �{�[���f�[�^���i�[���ꂽ�o�b�t�@
   D3DXMATRIX** ppBoneMatrixPtrs;			// �{�[���}�g���N�X
   D3DXMATRIX* pBoneOffsetMatrices;		// �{�[�������}�g���N�X
   DWORD NumPaletteEntries;				// �{�[���ő�l
   bool UseSoftwareVP;						// �\�t�g�E�F�A�g�����ǂ���
   DWORD iAttributeSW;						// �C���f�b�N�X�t������Ă��Ȃ��X�L�j���O�ɕK�v�ȏꍇ�́ASW��HW�̊Ԃ̕������������߂Ɏg�p����܂�
};

//--------------------------------------------------------------------------------------
// x�t�@�C�����烁�b�V���f�[�^�ǂݍ��ݗp
//--------------------------------------------------------------------------------------
class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
    HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);
    HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, stD3DContainerDerived* pMeshContainer);

   public:	
	   STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	   STDMETHOD(CreateMeshContainer)(THIS_
		   LPCSTR Name,
		   CONST D3DXMESHDATA* pMeshData,
		   CONST D3DXMATERIAL* pMaterials,
		   CONST D3DXEFFECTINSTANCE* pEffectInstances,
		   DWORD NumMaterials,
		   CONST DWORD* pAdjacency,
		   LPD3DXSKININFO pSkinInfo,
		   LPD3DXMESHCONTAINER* ppNewMeshContainer);
	   STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pInFrame);
	   STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	   CAllocateHierarchy() {}

};

#endif