#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include <map>

#include "types.h"
#include "ui_settings.h"
#include "preset.h"
#include "settings_io.h"
#include <string>
#include "ui_api.h"

// PNG 選択（main.cpp 側で定義されている）
extern void SelectPngForSlot(HWND owner, const std::string& slot);

// TransformSettings（main.cpp 側のグローバル）
extern TransformSettings g_transform;

// スロット辞書（main.cpp 側）
extern std::map<std::string, std::wstring> g_bitmapFiles;


// ------------------------------
// UI ハンドラ
// ------------------------------
void OnPresetSave(HWND hDlg)
{
    OutputDebugString(L"[UI] OnPresetSave\n");
}

void OnPresetLoad(HWND hDlg)
{
    OutputDebugString(L"[UI] OnPresetLoad\n");
}

void OnPresetDelete(HWND hDlg)
{
    OutputDebugString(L"[UI] OnPresetDelete\n");
}

// ------------------------------
// InputName ダイアログ
// ------------------------------
INT_PTR CALLBACK InputNameProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            EndDialog(hDlg, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// ------------------------------
// Settings ダイアログ
// ------------------------------
INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
    {
        // プリセット一覧を読み込む
        HWND hList = GetDlgItem(hDlg, IDC_LIST_PRESETS);

        wchar_t buffer[256];
        const wchar_t* ini = L"presets.ini";

        for (int i = 0; ; i++)
        {
            wchar_t section[256];
            wsprintf(section, L"Preset_%d", i);

            GetPrivateProfileString(section, L"earMoveScale", L"", buffer, 256, ini);
            if (wcslen(buffer) == 0)
                break;

            wchar_t nameOnly[256];
            wsprintf(nameOnly, L"%d", i);

            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)nameOnly);
        }

        // スライダー初期化
        SendDlgItemMessage(hDlg, IDC_SLIDER_EAR_MOVE, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
        SendDlgItemMessage(hDlg, IDC_SLIDER_EAR_MOVE, TBM_SETPOS, TRUE, (int)g_transform.earMoveScale);

        SendDlgItemMessage(hDlg, IDC_SLIDER_BROW_UP, TBM_SETRANGE, TRUE, MAKELONG(-50, 50));
        SendDlgItemMessage(hDlg, IDC_SLIDER_BROW_UP, TBM_SETPOS, TRUE, (int)g_transform.browUpY);

        SendDlgItemMessage(hDlg, IDC_SLIDER_FACE_MOVE, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
        SendDlgItemMessage(hDlg, IDC_SLIDER_FACE_MOVE, TBM_SETPOS, TRUE, (int)g_transform.faceMoveScale);

        // EDITBOX 初期化
        SetDlgItemInt(hDlg, IDC_EDIT_EAR_MOVE, (int)g_transform.earMoveScale, TRUE);
        SetDlgItemInt(hDlg, IDC_EDIT_BROW_UP, (int)g_transform.browUpY, TRUE);
        SetDlgItemInt(hDlg, IDC_EDIT_FACE_MOVE, (int)g_transform.faceMoveScale, TRUE);

        return TRUE;
    }

    case WM_HSCROLL:
    {
        HWND slider = (HWND)lParam;
        int pos = SendMessage(slider, TBM_GETPOS, 0, 0);

        if (slider == GetDlgItem(hDlg, IDC_SLIDER_EAR_MOVE)) {
            g_transform.earMoveScale = pos;
            SetDlgItemInt(hDlg, IDC_EDIT_EAR_MOVE, pos, TRUE);
        }
        else if (slider == GetDlgItem(hDlg, IDC_SLIDER_BROW_UP)) {
            g_transform.browUpY = pos;
            SetDlgItemInt(hDlg, IDC_EDIT_BROW_UP, pos, TRUE);
        }
        else if (slider == GetDlgItem(hDlg, IDC_SLIDER_FACE_MOVE)) {
            g_transform.faceMoveScale = pos;
            SetDlgItemInt(hDlg, IDC_EDIT_FACE_MOVE, pos, TRUE);
        }

        return TRUE;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            // ------------------------------
            // プリセット SAVE
            // ------------------------------
        case IDC_BTN_PRESET_SAVE:
        {
            if (DialogBox(GetModuleHandle(NULL),
                MAKEINTRESOURCE(IDD_INPUT_NAME),
                hDlg, InputNameProc) == IDOK)
            {
                wchar_t name[256];
                GetDlgItemText(hDlg, IDC_EDIT_PRESET_NAME, name, 256);

                SavePreset(name);

                HWND hList = GetDlgItem(hDlg, IDC_LIST_PRESETS);
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
            }

            OnPresetSave(hDlg);
            return TRUE;
        }

        // ------------------------------
        // プリセット LOAD
        // ------------------------------
        case IDC_BTN_PRESET_LOAD:
        {
            HWND hList = GetDlgItem(hDlg, IDC_LIST_PRESETS);
            int idx = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                wchar_t name[256];
                SendMessage(hList, LB_GETTEXT, idx, (LPARAM)name);
                LoadPreset(name);
            }

            OnPresetLoad(hDlg);
            return TRUE;
        }

        // ------------------------------
        // プリセット DELETE
        // ------------------------------
        case IDC_BTN_PRESET_DELETE:
        {
            HWND hList = GetDlgItem(hDlg, IDC_LIST_PRESETS);
            int idx = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
            if (idx != LB_ERR)
            {
                wchar_t name[256];
                SendMessage(hList, LB_GETTEXT, idx, (LPARAM)name);

                DeletePreset(name);
                SendMessage(hList, LB_DELETESTRING, idx, 0);
            }

            OnPresetDelete(hDlg);
            return TRUE;
        }

        // ------------------------------
        // PNG 選択ボタン
        // ------------------------------
        case IDC_BTN_HEADNORMAL:
            SelectPngForSlot(hDlg, "HeadNormal");
            return TRUE;

        case IDC_BTN_EYEL_OPEN:
            SelectPngForSlot(hDlg, "EyeL_Open");
            return TRUE;

        case IDC_BTN_EYER_OPEN:
            SelectPngForSlot(hDlg, "EyeR_Open");
            return TRUE;

        case IDC_BTN_EAR:
            SelectPngForSlot(hDlg, "R_Ear");
            return TRUE;

            // ------------------------------
            // 設定保存
            // ------------------------------
        case IDC_BTN_SAVE:
            SaveSettings();
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }

    return FALSE;
}