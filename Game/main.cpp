/********************************************************************************* 

  *FileName: main.cpp  
            �R�E  �L�K�N
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/ 


#include "main.h"

#pragma comment(lib, "winmm.lib")

// �ݒ�t�@�C���p�X
#define MAIN_SETTING_FILE   ("Engine/Script/setting.txt")


// �O���[�o���ϐ�
HWND g_hwnd=NULL;
HINSTANCE g_hinstance=NULL;
CScript* g_pScript = nullptr;

// �ݒ�ۑ��p�ϐ�
char g_wClass[MAX_COMMAND_SIZE];// �E�C���h�E�N���X�̖��O
char g_wName[MAX_COMMAND_SIZE]; // �E�C���h�E�N���X�̖��O�i�L���v�V�����ɕ\���j
int g_width = 0;      // �E�B���h�E�̕�
int g_height = 0;   // �E�B���h�E�̍���
bool g_fullScreen = false;     //�S���
int g_multiSample = 0;

// imgui_impl_win32.cpp����̑O���錾���b�Z�[�W�n���h��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MsgProc(HWND hd, UINT msg, WPARAM wp, LPARAM lp)
{
    //���b�Z�[�W��imgui�ɓn��
    if (ImGui_ImplWin32_WndProcHandler(hd, msg, wp, lp))
        return true;

    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
        return 0;
        break;
    }

    return DefWindowProc(hd, msg, wp, lp);// �K��̏�����Ԃ�
}

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev, _In_ LPSTR cmdLine, _In_ int cmdShow)
{
    // �A�v���P�[�V�����E�B���h�E�쐬
    ImGui_ImplWin32_EnableDpiAwareness();
    
    // �ݒ�X�N���v�g�t�@�C����ǂݍ���
    LoadScript(MAIN_SETTING_FILE);

    // �E�C���h�E�N���X�̍\���̂̏�����
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);					// WNDCLASSEX�̃������T�C�Y
    wcex.style = CS_CLASSDC;							// �E�C���h�E�̃X�^�C��
    wcex.lpfnWndProc = MsgProc;						    // �E�C���h�E�v���V�[�W��
    wcex.cbClsExtra = 0;								// �O�ɂ��� (�ʏ�͎g�p���Ȃ�)
    wcex.cbWndExtra = 0;								// �O�ɂ��� (�ʏ�͎g�p���Ȃ�)
    wcex.hInstance = GetModuleHandle(NULL);				// �C���X�^���X�n���h��
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// �^�X�N�o�[�̃A�C�R��
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// �}�E�X�J�[�\��
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// �N���C�A���g�̈�̔w�i�F
    wcex.lpszMenuName = NULL;							// ���j���[�o�[
    wcex.lpszClassName = g_wClass;				// �E�C���h�E�N���X�̖��O
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wcex);


    // �E�C���h�E���쐬
    if (g_fullScreen)
    {
        g_hwnd = CreateWindowEx(
            0,								// �g���E�C���h�E�X�^�C��
            g_wClass,		            // �E�C���h�E�X�^�C���̖��O
            g_wName,		            // �E�C���h�E�̖��O
            WS_POPUP | WS_SYSMENU | WS_VISIBLE,	// �E�C���h�E�X�^�C��
            0,					            // �E�C���h�E�̍���X���W
            0,					            // �@�@�V�@�@�̍���Y���W
            g_width,		                // �@�@�V�@�@�̕�
            g_height,		                // �@�@�V�@�@�̍���
            NULL,							// �e�E�C���h�E�̃n���h��
            NULL,							// ���j���[�n���h���܂��͎q�E�C���h�EID
            instance,				                // �C���X�^���X�n���h��
            NULL);							// �E�C���h�E�쐬�f�[�^

    }
    else
    {
        // �N���C�A���g�̈���w��̃T�C�Y�ɒ���
        RECT rect = { 0, 0, g_width, g_height };		// ��ʃT�C�Y�̍\����
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

        g_hwnd = CreateWindowEx(
            0,								// �g���E�C���h�E�X�^�C��
            g_wClass,		            // �E�C���h�E�X�^�C���̖��O
            g_wName,		            // �E�C���h�E�̖��O
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// �E�C���h�E�X�^�C��
            0,					            // �E�C���h�E�̍���X���W
            0,					            // �@�@�V�@�@�̍���Y���W
            (rect.right - rect.left),		//��
            (rect.bottom - rect.top),		//����
            NULL,							// �e�E�C���h�E�̃n���h��
            NULL,							// ���j���[�n���h���܂��͎q�E�C���h�EID
            instance,				                // �C���X�^���X�n���h��
            NULL);							// �E�C���h�E�쐬�f�[�^

    }

    if (g_hwnd)
    {
        // �E�B���h�E�̕\��
        ShowWindow(g_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(g_hwnd);
    }

    g_hinstance = wcex.hInstance;

    // �G���W���̏�����
    if (InitializeEngine())
    {
        // �Q�[���̏�����
        if (GameInitialize())
        {
            // ���b�Z�[�W���[�v
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));

            // �}�E�X�ʒu������
            SetCursorPos(0, 0);

            // windows�̏���
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    // ���b�Z�[�W�̐ݒ�
                    TranslateMessage(&msg);	// ���z���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
                    DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗐M
                }
                else
                {
                    // �Q�[�����[�v
                    GameLoop();
                }
            }
        }
    }

    // �S�Ẵ��\�[�X�������[�X
    GameShutdown();
    ShutdownEngine();
    g_pScript->Shutdown();

    UnregisterClass(g_wClass, wcex.hInstance);
    return true;
}


//=========================================
// �X�N���v�g���[�h�֐�
//=========================================
bool LoadScript(const char* filename)
{
    g_pScript = new CScript{};

    if (!g_pScript->LoadScriptFile(filename))
        return false;

    char command[MAX_COMMAND_SIZE];

    // ���[�v���āA�w�肳�ꂽ�e�R�}���h�����s���܂��B
    for (int i = 0; i < g_pScript->GetTotalLines(); i++)
    {
        g_pScript->ParseCommand(command);

        if (_stricmp(command, "WINDOW_CLASS") == 0)
        {
            g_pScript->ParseStringParam(g_wClass);
        }
        else if (_stricmp(command, "WINDOW_NAME") == 0)
        {
            g_pScript->ParseStringParam(g_wName);
        }
        else if (_stricmp(command, "WIN_WIDTH") == 0)
        {
            g_width = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "WIN_HEIGHT") == 0)
        {
            g_height = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "FULLSCREEN") == 0)
        {
            g_fullScreen = g_pScript->ParseBoolParam();
        }
        else if (_stricmp(command, "MULTI_SAMPLE") == 0)
        {
            g_multiSample = g_pScript->ParseIntParam();
        }
        else if (_stricmp(command, "#") == 0)
        {
            // ���ɂ����܂���
        }
        else if (_stricmp(command, "\0") == 0)
        {
            // ���ɂ����܂���
        }
        else
        {
            // �����ł́A�����ȃR�}���h�i�\���G���[�j�������ł��܂��B
            char err[64];

            sprintf(err, "Error in script on line %d.",
                g_pScript->GetCurrentLineNum() + 1);
            MessageBox(NULL, err, "Error", MB_OK);

            g_pScript->Shutdown();
            return false;
        }

        // �X�N���v�g�̎��̍s�Ɉړ����܂��B
        g_pScript->MoveToNextLine();
    }

    // ���ׂẴ��\�[�X��������܂��B���̃X�N���v�g�́A���������ɂ̂ݎ��s�����悤�ɐ݌v����Ă��܂��B
    g_pScript->Shutdown();

    return true;
}






