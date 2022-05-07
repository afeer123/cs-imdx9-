#include "includes.h"
#include <string_view>
#include <cstring>
#include <Psapi.h>
#define WIN32_LINUX(win32, linux) win32
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;
#include "csgo.hpp"
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	const ImFont* font = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

uint8_t* fakePrimen;

bool tab1 = true;
bool init = false;
bool show_main = true;
int screenx = GetSystemMetrics(SM_CXSCREEN);
int screeny = GetSystemMetrics(SM_CYSCREEN);
bool ESP = false;
bool bhop = false;
bool rcs = false;
bool user = false;
bool triggerbot= false;
bool tBdlay = false;
int triggerCustomDelay = 0;
bool customFov = false;
int fov = 90;
bool thirperson = false;
ImColor EnemyColor;
ImColor TeamColor;
bool fakePrime = false;
bool glow = false;
float rcs_amount = 0;
int leves = 0;
int vector = 0;
int rank = 0;

struct Vec3
{
	float x, y, z;
	Vec3 operator+(Vec3 d)
	{
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator-(Vec3 d)
	{
		return { x - d.x, y - d.y, z - d.z };
	}
	Vec3 operator*(float d)
	{
		return { x * d, y * d, z * d };
	}
	void normailze()
	{
		while (y <- 180)
		{
			y = 360;
		}
		while (y > 180)
		{
			y = -360;
		}
		while (x > 89)
		{
			x = 89;
		}
		while (x < -89)
		{
			x = -89;
		}
	}
};

DWORD clientMod;
DWORD engineMod;
DWORD localPlayer;
int* iShotsFired;
Vec3* viewAngles;
Vec3* aimRecoilPunch;
Vec3 oPunch{ 0,0,0 };
bool menu = false;
static char str1[128] = "123456";
static char str0[128] = "0";

void HackInit()
{
	clientMod = (uintptr_t)GetModuleHandle("client.dll");
	engineMod = (uintptr_t)GetModuleHandle("engine.dll");

	localPlayer = *(uintptr_t*)(clientMod + dwLocalPlayer);

	iShotsFired = (int*)(localPlayer + m_iShotsFired);

	viewAngles = (Vec3*)(*(uintptr_t*)(engineMod + dwClientState) + dwClientState_ViewAngles);

	aimRecoilPunch = (Vec3*)(localPlayer + m_aimPunchAngle);

	TeamColor.Value.w = 1;
	EnemyColor.Value.w = 1;

}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		HackInit();
		InitImGui(pDevice);
		init = true;
	}
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		show_main = !show_main;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show_main)
	{
		ImGui::Begin(u8"注入测试");
		ImGui::InputText(u8"卡密", str0, IM_ARRAYSIZE(str0));
		ImGui::Checkbox(u8"确定", &menu);
		if (menu)
		{
			if (strcmp(str0,str1) != 0)
			{
				ImGui::Text(str1);
				ImGui::Text(str0);
			}
			else
			{
				if (ImGui::BeginTabBar("tab1", tab1))
				{
					misc();
					aimbot();
				}
				ImGui::EndTabBar();

			}
		}		
		ImGui::End();
		
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}
void misc()
{
	if (ImGui::BeginTabItem(u8"杂项"))
	{
		ImGui::Checkbox(u8"透视", &ESP);
		if (ESP)
		{
			ImGui::Checkbox(u8"发光",&glow);
			if (glow)
			{
				ImGui::ColorPicker4(u8"敌人颜色",(float*)&EnemyColor);
				ImGui::ColorPicker4(u8"队友颜色",(float*)&TeamColor);
			}
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text(u8"残疾人选项");
		ImGui::Checkbox(u8"连跳", &bhop);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Checkbox(u8"压枪", &rcs);
		if (rcs)
		{
			ImGui::SliderFloat(u8"压枪强度", &rcs_amount, 0, 1);
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Checkbox(u8"资料修改", &user);
		if(user)
		{
			ImGui::Checkbox(u8"强制优先",&fakePrime);
			ImGui::PushID("leves");
			ImGui::InputInt(u8"等级", &leves);
			ImGui::PopID();
			ImGui::PushID("vector");
			ImGui::InputInt(u8"连胜", &vector);
			ImGui::PopID();
			ImGui::PushID("rank");
			ImGui::InputInt(u8"段位", &rank);
			ImGui::PopID();			
		}
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text(u8"视觉");
		ImGui::Checkbox(u8"视差值",&customFov);
		if (customFov)
		{
			ImGui::SliderInt(u8"FOV", &fov, 30, 179);
		}
		ImGui::Checkbox(u8"第三人称",&thirperson);
		ImGui::EndTabItem();
	}
}
void aimbot()
{
	if (ImGui::BeginTabItem(u8"自瞄"))
	{
		ImGui::Text(u8"自动扳机");
		ImGui::Checkbox(u8"启用", &triggerbot);
		if (triggerbot)
		{
			ImGui::Spacing();
			ImGui::Checkbox(u8"延迟", &tBdlay);
			if (tBdlay)
			{
				ImGui::SliderInt(u8"时间(毫秒)",&triggerCustomDelay,0,250);
			}
		}
		ImGui::EndTabItem();
	}

}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

DWORD WINAPI fakeThread(LPVOID lp)
{
	return 0;
}
DWORD WINAPI GlowThread(LPVOID lp)
{
	DWORD gameMoudle = (uintptr_t)GetModuleHandle("client.dll");
	uintptr_t GlowObj = *(uintptr_t*)(gameMoudle + dwGlowObjectManager);
	while (true)
	{
		DWORD localplayer = *(uintptr_t*)(gameMoudle + dwLocalPlayer);
		if (glow)
		{
			int LocalTeam = *(int*)(dwLocalPlayer + m_iTeamNum);
			for (int i = 0; i < 64; i++)
			{
				uintptr_t entity = *(uintptr_t*)(gameMoudle + dwEntityList + i * 0x10);
				if (entity != NULL)
				{
					int entityteam = *(int*)(entity + m_iTeamNum);
					int GlowIndex  = *(int*)(entity + m_iGlowIndex);
					if (LocalTeam != entityteam)
					{
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x4)) = EnemyColor.Value.x;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x8)) = EnemyColor.Value.y;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0xC)) = EnemyColor.Value.z;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x10)) = EnemyColor.Value.w;

					}
					else
					{
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x4)) = TeamColor.Value.x;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x8)) = TeamColor.Value.y;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0xC)) = TeamColor.Value.z;
						*(float*)(GlowObj + ((GlowIndex * 0x38) + 0x10)) = TeamColor.Value.w;
					}
					*(bool*)(GlowObj + ((GlowIndex * 0x38) + 0x24)) = true;
					*(bool*)(GlowObj + ((GlowIndex * 0x38) + 0x25)) = false;
				}
			}
		}
	}
}
DWORD WINAPI ThridPersonThread(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	DWORD localplayer = NULL;
	if (localplayer == NULL)
	{
		while (localplayer == NULL)
		{
			localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
		}
		Sleep(1);
	}
	while (true)
	{
		if (localplayer != NULL)
		{
			*(int*)(localplayer + m_iFOVStart) = fov;
			*(int*)(localplayer + m_iDefaultFOV) = fov;

		}
		if (localplayer != NULL)
		{
			if (thirperson)
			{
				*(int*)(localplayer + m_iObserverMode) = 1;

			}
			else
			{
				*(int*)(localplayer + m_iObserverMode) = 0;
			}

		}
		Sleep(1);		
	}
}
DWORD WINAPI FovThread(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	DWORD localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
	if (localplayer == NULL)
	{
		while (localplayer == NULL)
		{
			localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
		}
	}
	while (true)
	{
		if (localplayer != NULL)
		{
			if (customFov)
			{
				*(int*)(localPlayer + m_iFOV) = fov;
			}
		}

	}

}
DWORD WINAPI User(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	while (true)
	{
		if (user)
		{
				*(DWORD*)(gameMoudle + dwLeves) = leves;
				*(DWORD*)(gameMoudle + dwUser + 0x10) = vector;
				*(DWORD*)(gameMoudle + dwUser + 0xC) = rank;
		}
		
	}
}
DWORD WINAPI TriggerThread(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	while (true)
	{
		if (triggerbot)
		{
			DWORD localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
			int crosshair = *(int*)(localplayer + m_iCrosshairId);
			int localTeam = *(int*)(localplayer + m_iTeamNum);
			if (crosshair != 0 && crosshair < 64)
			{
				uintptr_t entity = *(uintptr_t*)(gameMoudle + dwEntityList + (crosshair - 1) * 0x10);
				int eTeam = *(int*)(entity + m_iTeamNum);
				int eHealth = *(int*)(entity + m_iHealth);

				if (eTeam != localTeam && eHealth > 0 && eHealth < 101)
				{
						if (tBdlay)
						{
							Sleep(triggerCustomDelay);
							*(int*)(gameMoudle + dwForceAttack) = 5;
							Sleep(20);
							*(int*)(gameMoudle + dwForceAttack) = 4;
						}
						else
						{
							*(int*)(gameMoudle + dwForceAttack) = 5;
							Sleep(20);
							*(int*)(gameMoudle + dwForceAttack) = 4;
						}
				}
			}
		}
		Sleep(2);
	}
}
DWORD WINAPI RcsThread(LPVOID lp)
{
	while (true)
	{
		if (rcs && !GetAsyncKeyState(VK_RBUTTON))
		{
			Vec3 punchAngle = *aimRecoilPunch * (rcs_amount * 2);
			if (*iShotsFired > 1 && GetAsyncKeyState(VK_LBUTTON))
			{
				Vec3 newAngle = *viewAngles + oPunch - punchAngle;
				newAngle.normailze();

				*viewAngles = newAngle;
			}
			oPunch = punchAngle;
		}
	}
}

DWORD WINAPI BhopThread(LPVOID lp)
{
	DWORD gameMoudle = (DWORD)GetModuleHandle("client.dll");
	DWORD localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
	while (localplayer == NULL)
	{
		localplayer = *(DWORD*)(gameMoudle + dwLocalPlayer);
	}
	while (true)
	{
		if (bhop)
		{
			DWORD flag = *(BYTE*)(localplayer + m_fFlags);
			if (GetAsyncKeyState(VK_SPACE) && flag & (1<<0))
			{
				*(DWORD*)(gameMoudle + dwForceJump) = 6;
			}
		}
	}
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, BhopThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, RcsThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, TriggerThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, FovThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, User, hMod, 0, nullptr);
		CreateThread(nullptr, 0, ThridPersonThread, hMod, 0, nullptr);
		CreateThread(nullptr, 0, GlowThread, hMod, 0, nullptr);	
		CreateThread(nullptr, 0, fakeThread, hMod, 0, nullptr);	
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
