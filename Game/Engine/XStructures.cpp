/*
   Demo Name:  Game Project 12
      Author:  Allen Sherrod
     Chapter:  Chapter 13
*/


#include "XStructures.h"
#include "defines.h"

HRESULT CAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
    UINT cbLength;

    if (Name != nullptr)
    {
        cbLength = (UINT)strlen(Name) + 1;
        *pNewName = new CHAR[cbLength];
        if (*pNewName == nullptr)
            return E_OUTOFMEMORY;
        memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
    }
    else
    {
        *pNewName = nullptr;
    }

    return S_OK;
}

// ------------------------------------------------ --------------------------------------
// �X�L�����b�V�������[�h����Ƃ��A�܂��̓��\�b�h��ύX����Ƃ���CreateMeshContainer�ɂ���ČĂяo����܂��B 
// ���̊֐��́A���b�V���R���e�i��pSkinInfo���g�p���āA�ړI�̕`��\�ȃ��b�V���ƃ{�[���̑g�ݍ��킹�e�[�u���𐶐����܂��B
// ------------------------------------------------ --------------------------------------
HRESULT CAllocateHierarchy::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, stD3DContainerDerived* pMeshContainer)
{
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps(&d3dCaps);

    if (pMeshContainer->pSkinInfo == nullptr)
        return S_OK;

    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
    SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

    // �{�[���̍œK��
    if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
        pMeshContainer->pOrigMesh,// ���̓��b�V��
        D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
        pMeshContainer->pAdjacency,// ���b�V���̗אڃf�[�^(����)
        NULL,// ���b�V���̗אڃf�[�^(�o��)
        NULL,// �|���S���̐V�K�C���f�b�N�X�̃o�b�t�@
        NULL,// ���_�̐V�K�C���f�b�N�X�̃o�b�t�@
        &pMeshContainer->NumInfl,// �P�̒��_�ɉe�����y�ڂ��d�݂̐�
        &pMeshContainer->NumAttributeGroups,// �{�[���̐�
        &pMeshContainer->pBoneCombinationBuf,// �{�[���f�[�^���i�[���ꂽ�o�b�t�@
        &pMeshContainer->MeshData.pMesh)))// �ϊ���̃��b�V��
        return E_FAIL;
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: CD3DAllocate::CreateFrame()
// Desc: Frame���쐬���āC����������
//----------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{

    HRESULT hr = S_OK;
    stD3DFrameDerived* pFrame;

    *ppNewFrame = nullptr;

    pFrame = new stD3DFrameDerived;
    if (pFrame == nullptr)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    hr = AllocateName(Name, &pFrame->Name);
    if (FAILED(hr))
        goto e_Exit;

    // frame�̃����o�[�����������܂�
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->finalMatrix);

    pFrame->pMeshContainer = nullptr;
    pFrame->pFrameSibling = nullptr;
    pFrame->pFrameFirstChild = nullptr;

    *ppNewFrame = pFrame;
    pFrame = nullptr;

e_Exit:
    delete pFrame;
    return hr;

}


//--------------------------------------------------------------------------------------
// ���b�V���ǂݍ���ŁA�K�v�ȃf�[�^��ۑ�����
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(
    LPCSTR Name,
    CONST D3DXMESHDATA* pMeshData,
    CONST D3DXMATERIAL* pMaterials,
    CONST D3DXEFFECTINSTANCE* pEffectInstances,
    DWORD NumMaterials,
    CONST DWORD* pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* ppNewMeshContainer)
{    
    HRESULT hr;
    stD3DContainerDerived* pMeshContainer = nullptr;
    UINT NumFaces;
    UINT iMaterial; //�}�e���A���J�E���g
    UINT countBone, numBones;
    LPDIRECT3DDEVICE9 pd3dDevice = nullptr;

    LPD3DXMESH pMesh = nullptr;

   if(pMeshData->Type != D3DXMESHTYPE_MESH)
      {
       //D3DXMESHTYPE_PMESH�̓v���O���b�V�u���b�V���ł��B
       //D3DXMESHTYPE_PATCHMESH�̓p�b�`���b�V���ł��B
       //���̃R�[�h�ł́A�ʏ�̃��b�V���݂̂��������܂��B
       hr = E_FAIL;
       goto e_Exit;
      }

   // ���b�V���f�[�^�\������pMesh�C���^�[�t�F�C�X�|�C���^���擾���܂�
   pMesh = pMeshData->pMesh;

   // FVF������Ȃ�����I���܂�
   if (pMesh->GetFVF() == 0)
   {
       hr = E_FAIL;
       goto e_Exit;
   }
   
   //�I�[�o�[���[�h���ꂽ�\�������蓖�Ă�D3DXMESHCONTAINER�Ƃ��ĕԂ��܂�
   pMeshContainer = new stD3DContainerDerived;
   if (pMeshContainer == NULL)
   {
       hr = E_OUTOFMEMORY;
       goto e_Exit;
   }
   memset(pMeshContainer, 0, sizeof(stD3DContainerDerived));

   // �l�[�����R�s�[���܂��B
   hr = AllocateName(Name, &pMeshContainer->Name);
   if (FAILED(hr))
       goto e_Exit;

    // D3D �f�o�C�X�擾.
    pMesh->GetDevice(&pd3dDevice);

    // �ʐ���ۑ�
    NumFaces = pMesh->GetNumFaces();

    // ���_�@���Ȃ��ꍇ�͒ǉ����܂��B
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // ���b�V�����N���[������FVF�ɖ@����ǉ����܂�
        hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
            pMesh->GetFVF() | D3DFVF_NORMAL,
            pd3dDevice, &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        // �g�p���郁�b�V���R���e�i����V����pMesh�|�C���^���擾���܂�
        // ���FpMesh�ւ̎Q�Ƃ��܂��Ȃ����߁ApMesh�������[�X���܂���
        pMesh = pMeshContainer->MeshData.pMesh;

        // �@�����쐬���܂�
        D3DXComputeNormals(pMesh, NULL);
    }
    else 
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }

    // �}�e���A�����ۑ��p�̃����������蓖�Ă܂��B
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    // �אڏ��ۑ����܂�
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);

    // �������N���A
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // �}�e���A����񂪒񋟂���Ă���ꍇ�́A�������R�s�[���܂�
    if (NumMaterials > 0)
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != nullptr)
            {
                if (FAILED(D3DXCreateTextureFromFileA(pd3dDevice, 
                    pMeshContainer->pMaterials[iMaterial].pTextureFilename,
                    &pMeshContainer->ppTextures[iMaterial])))
                    pMeshContainer->ppTextures[iMaterial] = nullptr;//�e�N�X�`���Ȃ��ꍇ��nullptr

                // ���[�h��e�N�X�`���l�[����nullptr�ɂ��܂��B
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = nullptr;
            }
        }
    }
    else //�񋟂���Ă��Ȃ��ꍇ�́A�f�t�H���g���g�p���܂�
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // �X�L�����b�V����񂪂���ꍇ�́A�K�v�ȃf�[�^��ۑ����Ă���AHW�X�L���p�ɃZ�b�g�A�b�v���܂�
    if (pSkinInfo != NULL)
    {
        // �ŏ���SkinInfo�ƃI���W�i�����b�V���f�[�^��ۑ����܂�
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // ���_��}�`��Ԃ���Bones�̋�ԂɈړ�����ɂ́Aoffset�s��̔z�񂪕K�v�ɂȂ�܂�
        numBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[numBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // �e�{�[��offset�s����擾���܂�
        for (countBone = 0; countBone < numBones; countBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[countBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(countBone));
        }

        // GenerateSkinnedMesh�֐��́A��ʓI�ȃX�L�������擾���AHW�Ή��o�[�W�����ɕϊ����܂�
        hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
        if (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = nullptr;

 
e_Exit:
   SAFE_RELEASE(pd3dDevice);

   // �������N���A 
   if (pMeshContainer != NULL)
   {
       DestroyMeshContainer(pMeshContainer);
   }

   return hr;
}


HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pInFrame) 
{
    SAFE_DELETE_ARRAY(pInFrame->Name);
    SAFE_DELETE(pInFrame);
    return S_OK;
}


HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT countMaterial;

    stD3DContainerDerived* pMeshContainer = (stD3DContainerDerived*)pMeshContainerBase;

    SAFE_DELETE_ARRAY(pMeshContainer->Name);
    SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
    SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
    SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

    // ���蓖�Ă�ꂽ���ׂẴe�N�X�`����������܂�
    if (pMeshContainer->ppTextures != NULL)
    {
        for (countMaterial = 0; countMaterial < pMeshContainer->NumMaterials; countMaterial++)
        {
            SAFE_RELEASE(pMeshContainer->ppTextures[countMaterial]);
        }
    }
    SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
    SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
    SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
    SAFE_RELEASE(pMeshContainer->pSkinInfo);
    SAFE_RELEASE(pMeshContainer->pOrigMesh);
    SAFE_DELETE(pMeshContainer);
    return S_OK;

}